#include "NrelInNodeSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
NrelInNodeSemanticNeighbourhoodTranslator::NrelInNodeSemanticNeighbourhoodTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> NrelInNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction,
    std::map<std::string, std::vector<std::vector<std::string>>> & inTr,
    std::map<std::string, std::vector<std::vector<std::string>>> & fromTr,
    LanguageHandler* languageHandler) const
{
  std::vector<std::string> translations;
  translations.reserve(maxTranslations);
  auto const & nrelIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (nrelIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(nrelIterator->Get(1), structure) == SC_FALSE ||
        isInStructure(nrelIterator->Get(3), structure) == SC_FALSE)
      continue;
    if (anyIsInStructure({nrelIterator->Get(2), nrelIterator->Get(4)}, atLeastOneNodeFromConstruction) == SC_FALSE)
      continue;
    ScAddr const & nrelTargetNode = nrelIterator->Get(2);
    if (isInIgnoredKeynodes(nrelTargetNode))
      continue;
    std::string const & nrelTargetMainIdtf = languageHandler->getMainIdtf(nrelTargetNode);
    if (nrelTargetMainIdtf.empty())
      continue;

    ScAddr const & nrelNode = nrelIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;
    std::string nrelMainIdtf = languageHandler->getMainIdtf(nrelNode);
    if (nrelMainIdtf.empty())
      continue;

    inTr[nrelMainIdtf].push_back({nrelTargetMainIdtf});
    translations.push_back(nrelMainIdtf.append(" ").append(nrelTargetMainIdtf));
  }
  return translations;
}

std::list<ScAddrVector> NrelInNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;
  auto const & nrelIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (nrelIterator->Next())
  {
    if (isInStructure(nrelIterator->Get(1), structure) == SC_FALSE ||
        isInStructure(nrelIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelTargetNode = nrelIterator->Get(2);
    if (isInIgnoredKeynodes(nrelTargetNode))
      continue;

    ScAddr const & nrelNode = nrelIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;

    answer.push_back({nrelIterator->Get(1), nrelIterator->Get(2), nrelIterator->Get(3), nrelIterator->Get(4)});
  }
  return answer;
}
}  // namespace naturalLanguageProcessingModule
