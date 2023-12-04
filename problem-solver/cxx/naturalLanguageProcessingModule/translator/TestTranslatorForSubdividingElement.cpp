#include "TestTranslatorForSubdividingElement.hpp"

namespace naturalLanguageProcessingModule
{
TestTranslatorForSubdividingElement::TestTranslatorForSubdividingElement(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}
std::vector<std::string> TestTranslatorForSubdividingElement::getSemanticNeighbourhoods(
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
  auto const & tupleIterator = context->Iterator3(
      ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, node);
  while (tupleIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE)
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(0);
    auto const & tupleNodeIterator = context->Iterator5(tupleNode, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);

    while (tupleNodeIterator->Next() && translations.size() < maxTranslations)
    {
      if (isInStructure(tupleNodeIterator->Get(1), structure) == SC_FALSE)
        continue;
      if (anyIsInStructure(
              {tupleIterator->Get(0), tupleNodeIterator->Get(4), tupleNodeIterator->Get(2)},
              atLeastOneNodeFromConstruction) == SC_FALSE)
        continue;
      ScAddr const & tupleNodeNode = tupleNodeIterator->Get(2);
      ScAddr const & nrelNode = tupleNodeIterator->Get(4);
      if (isInIgnoredKeynodes(tupleNodeNode)||isInIgnoredKeynodes(nrelNode))
        continue;
      std::string const & tupleNodeMainIdtf = languageHandler->getMainIdtf(tupleNodeNode);
      std::string const & nrelMainIdtf = languageHandler->getMainIdtf(nrelNode);
      if (tupleNodeMainIdtf.empty()||nrelMainIdtf.empty())
        continue;
      inTr[nrelMainIdtf].push_back({tupleNodeMainIdtf});
      translations.push_back("Is " + nrelMainIdtf + " of " + tupleNodeMainIdtf);
    }
  }
  return translations;
}

std::list<ScAddrVector> TestTranslatorForSubdividingElement::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
    std::list<ScAddrVector> answer;

    auto const & tupleIterator = context->Iterator3(
      ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, node);
    while (tupleIterator->Next())
    {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE)
        continue;

    ScAddr const & tupleNode = tupleIterator->Get(0);
    auto const & tupleNodeIterator = context->Iterator5(tupleNode, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);


    while (tupleNodeIterator->Next())
    {
      if (isInStructure(tupleNodeIterator->Get(1), structure) == SC_FALSE)
        continue;
      ScAddr const & tupleNodeNode = tupleNodeIterator->Get(2);
      ScAddr const & nrelNode = tupleNodeIterator->Get(4);
      if (isInIgnoredKeynodes(tupleNodeNode)||isInIgnoredKeynodes(nrelNode))
        continue;

      answer.push_back(
          {tupleIterator->Get(0),
           tupleIterator->Get(1),
           tupleNodeIterator->Get(1),
           tupleNodeIterator->Get(2),
           tupleNodeIterator->Get(3),
           tupleNodeIterator->Get(4)});
    }
  }
  return answer;
}
}