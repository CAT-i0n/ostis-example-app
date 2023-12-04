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
  bool isEnglish = true;
  auto translations = semanticNeighbourhoodTranslatorSet->getSemanticNeighbourhoods(
      node, TranslationConstants::MAX_TRANSLATIONS_FROM_EACH_TRANSLATOR, inTransl, fromTransl, isEnglish);
  std::shuffle(translations.begin(), translations.end(), std::mt19937(std::random_device()()));
  if (translations.size() > TranslationConstants::MAX_TRANSLATIONS_FROM_ALL_TRANSLATORS)
    translations.resize(TranslationConstants::MAX_TRANSLATIONS_FROM_ALL_TRANSLATORS);

  std::string const & translation =
      join(translations.cbegin(), translations.cend(), TranslationConstants::TRANSLATIONS_DELIMITER);
  
  SC_LOG_INFO(translation);
  SC_LOG_INFO("\n");
  auto res = construct(inTransl, fromTransl, isEnglish);
  SC_LOG_INFO(res);

  ScAddr const & translationLink = context->CreateLink();
  if (translationLink.IsValid() == SC_FALSE)
    SC_THROW_EXCEPTION(utils::ScException, "FormSemanticNeighbourhoodTranslationManager: cannot create answer link");
  if (context->SetLinkContent(translationLink, translation) == SC_FALSE)
    SC_THROW_EXCEPTION(utils::ScException, "FormSemanticNeighbourhoodTranslationManager: cannot set link content");
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

std::string FormSemanticNeighbourhoodTranslationManager::construct(std::map<std::string, std::vector<std::vector<std::string>>> inTransl, 
                                            std::map<std::string, std::vector<std::vector<std::string>>> fromTransl, bool isEnglish)
{

  std::string mainIdtf = isEnglish ? inTransl["main identifier*"][0][0]: inTransl["основной идентификатор*"][0][0];
  std::string result = mainIdtf + "\n";
  
  for(auto tr: fromTransl)
  {
    if (isInIgnoredIdtfs(tr.first))
      continue;
    std::string synonyms = isEnglish? findSynonymsEn(tr.first, false): findSynonymsRu(tr.first, false);
    result += mainIdtf + " " + synonyms + " ";
    if(noQuasyBinaryRelations(tr.second))
    {
      std::vector<std::string> totalStrings;
      for(auto vec: tr.second)
      {
        if(vec.size() > 0)
          totalStrings.push_back(vec[0]);
      }
      result += createRightForm(totalStrings, isEnglish);
    }
    else
    {
      for(auto vec: tr.second)
      {
        result+=createRightForm(vec, isEnglish);
      }
    }
    result += "\n";
  }

  for(auto tr: inTransl)
  {
    if (isInIgnoredIdtfs(tr.first))
      continue;
    std::string synonyms = isEnglish? findSynonymsEn(tr.first, true): findSynonymsRu(tr.first, true);
    result += mainIdtf + " " + synonyms + " ";
    if(noQuasyBinaryRelations(tr.second))
    {
      std::vector<std::string> totalStrings;
      for(auto vec: tr.second)
      {
        if(vec.size() > 0)
          totalStrings.push_back(vec[0]);
      }
      result += createRightForm(totalStrings, isEnglish);
    }
    else
    {
      for(auto vec: tr.second)
      {
        result+=createRightForm(vec, isEnglish);
      }
    }
  }
  return result;
}

std::string FormSemanticNeighbourhoodTranslationManager::createRightForm(std::vector<std::string> strVector, bool isEnglish)
{
    std::string rightForm = "";
    if(strVector.size() == 0)
      return "";
    else if(strVector.size() == 1)
        return strVector[0] + ". ";
    else if (strVector.size() == 2)
        return strVector[0] + (isEnglish? " and " : " и ") + strVector[1] + ". ";
    else{
        rightForm = strVector[0];
        for(long unsigned int i = 1; i < strVector.size()-1; i++)
        {
            rightForm += ", " + strVector[i];
        }
        rightForm += (isEnglish? " and " : " и ") + strVector[strVector.size()-1] + ". ";
        return rightForm;
    }
}

bool FormSemanticNeighbourhoodTranslationManager::noQuasyBinaryRelations(std::vector<std::vector<std::string>> vec)
{
  for(auto v: vec)
  {
    if(v.size() > 1)
      return false;
  }
  return true;
}

std::string FormSemanticNeighbourhoodTranslationManager::findSynonymsEn(std::string idtf, bool isInTransl)
{
  std::map<std::string, std::string> inSynonyms;
  std::map<std::string, std::string> fromSynonyms;

  fromSynonyms["subdividing*"] = "is subdivided into";
  fromSynonyms["included in"] = "is";
  fromSynonyms["strict inclusion*"] = "strictly included in";
  inSynonyms["subdividing*"] = "is subdividing of";
  inSynonyms["inclusion*"] = "includes" ;
  inSynonyms["identifier*"] = "also known as";
  inSynonyms["strict inclusion*"] = "strictly includes";
  if(isInTransl)
  {
    if(inSynonyms[idtf].size()>0)
      return inSynonyms[idtf];
    else 
    {
      if(idtf.find('*') != std::string::npos)
        idtf = idtf.erase(idtf.find('*'), 1);
      return idtf + " -";
    }
  }
  else
  {
    if(fromSynonyms[idtf].size()>0)
      return fromSynonyms[idtf];
    else
    {
      if(idtf.find('*') != std::string::npos)
        idtf = idtf.erase(idtf.find('*'), 1);
      return idtf + " of";
    }
  }
} 

std::string FormSemanticNeighbourhoodTranslationManager::findSynonymsRu(std::string idtf, bool isInTransl)
{
  std::map<std::string, std::string> inSynonyms;
  std::map<std::string, std::string> fromSynonyms;

  fromSynonyms["разбиение*"] = "разбивается на";
  fromSynonyms["included in"] = "является";
  fromSynonyms["строгое включение*"] = "строго включается в ";
  inSynonyms["разбиение*"] = "является элементом разбиения";
  inSynonyms["включение*"] = "включает в себя" ;
  inSynonyms["идентификатор*"] = "также известен, как";
  inSynonyms["строгое включение*"] = "строго включает в себя";
  if(isInTransl)
  {
    if(inSynonyms[idtf].size()>0)
      return inSynonyms[idtf];
    else 
    {
      if(idtf.find('*') != std::string::npos)
        idtf = idtf.erase(idtf.find('*'), 1);
      return idtf + " -";
    }
  }
  else
  {
    if(fromSynonyms[idtf].size()>0)
      return fromSynonyms[idtf];
    else
    {
      if(idtf.find('*') != std::string::npos)
        idtf = idtf.erase(idtf.find('*'), 1);
      return idtf + " ";
    }
  }
} 


bool FormSemanticNeighbourhoodTranslationManager::isInIgnoredIdtfs(std::string idtf)
{
  std::vector<std::string> ignoredIdfts = {"main identifier*", "definitional domain*", "second domain*", "first domain*",
                                              "using constants*", "основной идентификатор*", "второй домен*", "первый домен*"};

  if(std::count(ignoredIdfts.begin(), ignoredIdfts.end(), idtf))
    return true;
  return false;
} 

}