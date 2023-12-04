#include "NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator::NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
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
  auto const & tupleIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE ||
        isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;
    std::string const & nrelMainIdtf = languageHandler->getMainIdtf(nrelNode);
    if (nrelMainIdtf.empty())
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(2);
    auto const & tupleNodeIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
    std::vector<std::string> tupleNodesIdtf;
    while (tupleNodeIterator->Next() && translations.size() < maxTranslations)
    {
      if (isInStructure(tupleNodeIterator->Get(1), structure) == SC_FALSE)
        continue;
      if (anyIsInStructure(
              {tupleIterator->Get(2), tupleIterator->Get(4), tupleNodeIterator->Get(2)},
              atLeastOneNodeFromConstruction) == SC_FALSE)
        continue;
      ScAddr const & tupleNodeNode = tupleNodeIterator->Get(2);
      if (isInIgnoredKeynodes(tupleNodeNode))
        continue;
      std::string const & tupleNodeMainIdtf = languageHandler->getMainIdtf(tupleNodeNode);
      if (tupleNodeMainIdtf.empty())
        continue;
      tupleNodesIdtf.push_back(tupleNodeMainIdtf);
      translations.push_back(nrelMainIdtf + " " + tupleNodeMainIdtf);
      inTr[nrelMainIdtf].push_back(tupleNodesIdtf);
    }
  }
  return translations;
}

std::list<ScAddrVector> NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;
  auto const & tupleIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next())
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE ||
        isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(2);
    auto const & tupleNodeIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

    while (tupleNodeIterator->Next())
    {
      if (isInStructure(tupleNodeIterator->Get(1), structure) == SC_FALSE)
        continue;
      ScAddr const & tupleNodeNode = tupleNodeIterator->Get(2);
      if (isInIgnoredKeynodes(tupleNodeNode))
        continue;

      answer.push_back(
          {tupleIterator->Get(1),
           tupleIterator->Get(2),
           tupleIterator->Get(3),
           tupleIterator->Get(4),
           tupleNodeIterator->Get(1),
           tupleNodeIterator->Get(2)});
    }
  }
  return answer;
}
}  // namespace naturalLanguageProcessingModule
