#include "keynodes/TranslationKeynodes.hpp"

#include "InConceptSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
InConceptSemanticNeighbourhoodTranslator::InConceptSemanticNeighbourhoodTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> InConceptSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(

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
  auto const & classIterator = context->Iterator3(node, ScType::EdgeAccessConstPosPerm, ScType::Node);
  while (classIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(classIterator->Get(1), structure) == SC_FALSE)
      continue;
    if (anyIsInStructure({classIterator->Get(0)}, atLeastOneNodeFromConstruction) == SC_FALSE)
      continue;
    ScAddr const & classNode = classIterator->Get(0);
    ScAddr const & includedNode = classIterator->Get(2);
    if (isInIgnoredKeynodes(classNode)||isInIgnoredKeynodes(includedNode))
      continue;
    if (isParameter(classNode))
      continue;
    std::string const & nodeIdtf = getMainIdtf(includedNode, isEnglish);
    if (nodeIdtf.empty())
      continue;

    translations.push_back("includes " + nodeIdtf);
    inTr["includes"].push_back({nodeIdtf});
  }
  return translations;
}

bool InConceptSemanticNeighbourhoodTranslator::isParameter(ScAddr const & node) const
{
  std::string const & NODE_ALIAS = "_node;";
  ScTemplate scTemplate;
  scTemplate.Triple(TranslationKeynodes::parameter, ScType::EdgeAccessVarPosPerm, ScType::NodeVarClass >> NODE_ALIAS);
  scTemplate.Triple(NODE_ALIAS, ScType::EdgeAccessVarPosPerm, node);
  ScTemplateSearchResult searchResult;
  context->HelperSearchTemplate(scTemplate, searchResult);
  return searchResult.IsEmpty() == SC_FALSE;
}

std::list<ScAddrVector> InConceptSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;

  auto const & classIterator = context->Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, node);
  while (classIterator->Next())
  {
    if (isInStructure(classIterator->Get(1), structure) == SC_FALSE)
      continue;
    ScAddr const & classNode = classIterator->Get(0);
    if (isInIgnoredKeynodes(classNode))
      continue;
    if (isParameter(classNode))
      continue;
    answer.push_back({classIterator->Get(0), classIterator->Get(1)});
  }

  return answer;
}
}  // namespace naturalLanguageProcessingModule
