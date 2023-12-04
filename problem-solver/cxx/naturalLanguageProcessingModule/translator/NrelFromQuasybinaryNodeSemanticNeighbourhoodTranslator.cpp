#include "NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator::NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator(
    ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction,
    std::map<std::string, std::vector<std::vector<std::string>>> & inTr,
    std::map<std::string, std::vector<std::vector<std::string>>> & fromTr,
    bool isEnglish) const
{
  std::vector<std::string> translations;
  translations.reserve(maxTranslations);
  auto const & tupleIterator = context->Iterator5(
      ScType::NodeConstTuple, ScType::EdgeDCommonConst, node, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE ||
        isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;
    std::string const & nrelMainIdtf = getMainIdtf(nrelNode, isEnglish);
    if (nrelMainIdtf.empty())
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(0);
    std::vector<std::string> tupleNodesIdtf;
    auto const & tupleNodeIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

    while (tupleNodeIterator->Next() && translations.size() < maxTranslations)
    {
      if (isInStructure(tupleNodeIterator->Get(1), structure) == SC_FALSE)
        continue;
      if (anyIsInStructure(
              {tupleIterator->Get(0), tupleIterator->Get(4), tupleNodeIterator->Get(2)},
              atLeastOneNodeFromConstruction) == SC_FALSE)
        continue;
      ScAddr const & tupleNodeNode = tupleNodeIterator->Get(2);
      if (isInIgnoredKeynodes(tupleNodeNode))
        continue;
      std::string const & tupleNodeMainIdtf = getMainIdtf(tupleNodeNode, isEnglish);
      if (tupleNodeMainIdtf.empty())
        continue;

      translations.push_back(nrelMainIdtf + " " + tupleNodeMainIdtf);
      tupleNodesIdtf.push_back(tupleNodeMainIdtf);
    }
    fromTr[nrelMainIdtf].push_back(tupleNodesIdtf);
  }
  return translations;
}

std::list<ScAddrVector> NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;

  auto const & tupleIterator = context->Iterator5(
      ScType::NodeConstTuple, ScType::EdgeDCommonConst, node, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next())
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE ||
        isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(0);
    auto const & tupleNodeIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

    while (tupleNodeIterator->Next())
    {
      if (isInStructure(tupleNodeIterator->Get(1), structure) == SC_FALSE)
        continue;
      ScAddr const & tupleNodeNode = tupleNodeIterator->Get(2);
      if (isInIgnoredKeynodes(tupleNodeNode))
        continue;

      answer.push_back(
          {tupleIterator->Get(0),
           tupleIterator->Get(1),
           tupleIterator->Get(3),
           tupleIterator->Get(4),
           tupleNodeIterator->Get(1),
           tupleNodeIterator->Get(2)});
    }
  }
  return answer;
}
}  // namespace naturalLanguageProcessingModule
