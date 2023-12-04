#pragma once

#include "structure_handler/StructureHandler.hpp"
#include "translator/SemanticNeighbourhoodTranslatorSet.hpp"
#include "sc-memory/sc_object.hpp"

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


};

}  // namespace naturalLanguageProcessingModule
