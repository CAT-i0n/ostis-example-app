#pragma once

#include "SemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
/**
 * node _=> _nrel_relation:: {_quasybinary_node};;
 */
class NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator : public SemanticNeighbourhoodTranslator
{
public:
  explicit NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator(ScMemoryContext * context);

  std::vector<std::string> getSemanticNeighbourhoods(
      ScAddr const & node,
      size_t const & maxTranslations,
      ScAddrSet const & structure,
      ScAddrSet const & atLeastOneNodeFromConstruction,
      std::map<std::string, std::vector<std::vector<std::string>>> & inTr,
      std::map<std::string, std::vector<std::vector<std::string>>> & fromTr,
      LanguageHandler* languageHandler) const override;

  std::list<ScAddrVector> getSemanticNeighbourhoodsElements(ScAddr const & node, ScAddrSet const & structure) const override;

private:
};

}  // namespace naturalLanguageProcessingModule
