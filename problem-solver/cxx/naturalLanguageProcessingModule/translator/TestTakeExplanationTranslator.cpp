#include "TestTakeExplanationTranslator.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>

namespace naturalLanguageProcessingModule
{
TestTakeExplanationTranslator::TestTakeExplanationTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}
std::vector<std::string> TestTakeExplanationTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction,
    std::map<std::string, std::vector<std::vector<std::string>>> & inTr,
    std::map<std::string, std::vector<std::vector<std::string>>> & fromTr,
    bool isEnglish) const

{
  std::vector<std::string> translations;
  std::string linkContents = "";
  translations.reserve(maxTranslations);
  auto const & it5 = context->Iterator5(
      ScType::NodeConst, ScType::EdgeAccessConstPosPerm, node, ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::rrel_key_sc_element);
  while (it5->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(it5->Get(1), structure) == SC_FALSE)
      continue;

    ScAddr const & emptyNode = it5->Get(0);
    auto const & emptyNodeIterator = context->Iterator5(ScType::NodeConst, ScType::EdgeDCommonConst, emptyNode, ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::nrel_sc_text_translation);
    while (emptyNodeIterator->Next() && translations.size() < maxTranslations)
    {
        
      if (isInStructure(emptyNodeIterator->Get(1), structure) == SC_FALSE)
        continue;
      if (anyIsInStructure({it5->Get(0), emptyNodeIterator->Get(4), emptyNodeIterator->Get(2)},
              atLeastOneNodeFromConstruction) == SC_FALSE)
        continue;
        
        ScAddr const & translationNode = emptyNodeIterator->Get(0);
        auto const & linkIterator = context->Iterator5(translationNode, ScType::EdgeAccessConstPosPerm, ScType::LinkConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConstRole);
        while (linkIterator->Next() && translations.size() < maxTranslations)
        {
            if (isInStructure(linkIterator->Get(1), structure) == SC_FALSE)
                continue;
            ScAddr const & linkNode = linkIterator->Get(2);
            std::string const & linkContent = isEnglish? getEnglishContent(linkNode): getRussianContent(linkNode);
            if (linkContent.empty())
                continue;
            linkContents += linkContent;
        }
    }
  }
    inTr[" "].push_back({linkContents});
  return translations;
}

std::list<ScAddrVector> TestTakeExplanationTranslator::getSemanticNeighbourhoodsElements(
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