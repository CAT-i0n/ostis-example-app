#include <random>
#include <websocketpp/random/random_device.hpp>
#include <iterator>
#include "constants/TranslationConstants.hpp"


#include "FormSemanticNeighbourhoodTranslationManager.hpp"

namespace naturalLanguageProcessingModule
{
FormSemanticNeighbourhoodTranslationManager::FormSemanticNeighbourhoodTranslationManager(ScMemoryContext * context)
  : context(context)
{
  this->semanticNeighbourhoodTranslatorSet = std::make_unique<SemanticNeighbourhoodTranslatorSet>(context);
}

ScAddrVector FormSemanticNeighbourhoodTranslationManager::manage(ScAddrVector const & processParameters) const
{
  ScAddr const & node = processParameters[0];
  std::map<std::string, std::vector<std::vector<std::string>>> inTransl;
  std::map<std::string, std::vector<std::vector<std::string>>> fromTransl;
  //bool isEnglish = true;
  std::string interfaceLanguage = "ru";

  LanguageHandler* languageHandler;
  if(interfaceLanguage == "en")
    languageHandler = new EnglishLanguageHandler(context);
  else if(interfaceLanguage == "ru")
    languageHandler = new RussianLanguageHandler(context);
  else
    languageHandler = new EnglishLanguageHandler(context);
  
  auto translations = semanticNeighbourhoodTranslatorSet->getSemanticNeighbourhoods(
      node, TranslationConstants::MAX_TRANSLATIONS_FROM_EACH_TRANSLATOR, inTransl, fromTransl, languageHandler);
  std::shuffle(translations.begin(), translations.end(), std::mt19937(std::random_device()()));
  if (translations.size() > TranslationConstants::MAX_TRANSLATIONS_FROM_ALL_TRANSLATORS)
    translations.resize(TranslationConstants::MAX_TRANSLATIONS_FROM_ALL_TRANSLATORS);

  std::string const & translation =
      join(translations.cbegin(), translations.cend(), TranslationConstants::TRANSLATIONS_DELIMITER);
  
  SC_LOG_INFO(translation);
  SC_LOG_INFO("\n");

  StructureHandler structureHandler(languageHandler);
  auto roughtText = structureHandler.construct(inTransl, fromTransl);

  SC_LOG_INFO(roughtText);

  ScAddr const & translationLink = context->CreateLink();
  if (translationLink.IsValid() == SC_FALSE)
    SC_THROW_EXCEPTION(utils::ScException, "FormSemanticNeighbourhoodTranslationManager: cannot create answer link");
  if (context->SetLinkContent(translationLink, roughtText) == SC_FALSE)
    SC_THROW_EXCEPTION(utils::ScException, "FormSemanticNeighbourhoodTranslationManager: cannot set link content");

  delete languageHandler;
  return {translationLink};
}

std::string FormSemanticNeighbourhoodTranslationManager::join(
    std::vector<std::string>::const_iterator const & cbegin,
    std::vector<std::string>::const_iterator const & cend,
    std::string const & delimiter)
{
  std::ostringstream os;
  std::copy(cbegin, cend, std::ostream_iterator<std::string>(os, delimiter.c_str()));
  std::string joined = os.str();
  if (joined.size() > delimiter.size())
    joined.erase(joined.size() - delimiter.size());
  return joined;
}

}