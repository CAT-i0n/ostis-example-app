#pragma once


#include "translator/SemanticNeighbourhoodTranslatorSet.hpp"

namespace naturalLanguageProcessingModule
{
class FormSemanticNeighbourhoodTranslationManager
{
public:
  explicit FormSemanticNeighbourhoodTranslationManager(ScMemoryContext * context);

  ScAddrVector manage(ScAddrVector const & processParameters) const;

private:
  ScMemoryContext * context;
  std::unique_ptr<SemanticNeighbourhoodTranslatorSet> semanticNeighbourhoodTranslatorSet;

  static std::string join(
      std::vector<std::string>::const_iterator const & cbegin,
      std::vector<std::string>::const_iterator const & cend,
      const std::string & delimiter = "");
  static std::string construct(std::map<std::string, std::vector<std::vector<std::string>>> inTransl, 
                              std::map<std::string, std::vector<std::vector<std::string>>> fromTransl, bool isEnglish);

  static std::string createRightForm(std::vector<std::string> strVector, bool isEnglish);
  static bool noQuasyBinaryRelations(std::vector<std::vector<std::string>> vec);
  static std::string findSynonymsEn(std::string idtf, bool isInTransl);
  static std::string findSynonymsRu(std::string idtf, bool isInTransl);
  static bool isInIgnoredIdtfs(std::string idtf);
};

}  // namespace naturalLanguageProcessingModule
