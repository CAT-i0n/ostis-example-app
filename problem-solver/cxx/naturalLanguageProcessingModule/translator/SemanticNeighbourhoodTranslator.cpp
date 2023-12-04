#include "sc-agents-common/utils/CommonUtils.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/TranslationKeynodes.hpp"

#include "SemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
size_t SemanticNeighbourhoodTranslator::hashCode() const
{
  return std::hash<std::string>()(typeid(*this).name());
}

bool SemanticNeighbourhoodTranslator::operator<(SemanticNeighbourhoodTranslator const & other) const
{
  return this->hashCode() < other.hashCode();
}

std::string SemanticNeighbourhoodTranslator::getEnglishContent(ScAddr const & linkNode) const
{
  std::string content;
  if (isInEnglish(linkNode))
    content = utils::CommonUtils::readString(context, linkNode);
  return content;
}

std::string SemanticNeighbourhoodTranslator::getRussianContent(ScAddr const & linkNode) const
{
  std::string content;
  if (isInRussian(linkNode))
    content = utils::CommonUtils::readString(context, linkNode);
  return content;
}

ScIterator5Ptr SemanticNeighbourhoodTranslator::getNrelMainIdtfIterator(ScAddr const & node) const
{
  return context->Iterator5(
      node,
      ScType::EdgeDCommonConst,
      ScType::LinkConst,
      ScType::EdgeAccessConstPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_main_idtf);
}

bool SemanticNeighbourhoodTranslator::isInEnglish(ScAddr const & node) const
{
  return context->HelperCheckEdge(TranslationKeynodes::lang_en, node, ScType::EdgeAccessConstPosPerm);
}

bool SemanticNeighbourhoodTranslator::isInRussian(ScAddr const & node) const
{
  return context->HelperCheckEdge(TranslationKeynodes::lang_ru, node, ScType::EdgeAccessConstPosPerm);
}

std::string SemanticNeighbourhoodTranslator::getMainIdtf(ScAddr const & node, bool isEnglish) const
{
  std::string mainIdtf;
  auto const & iterator = getNrelMainIdtfIterator(node);
  while (iterator->Next())
  {
    mainIdtf = isEnglish ? getEnglishContent(iterator->Get(2)) : getRussianContent(iterator->Get(2)) ;
    if (mainIdtf.empty() == SC_FALSE)
      break;
  }
  return mainIdtf;
}

bool SemanticNeighbourhoodTranslator::isInIgnoredKeynodes(ScAddr const & node) const
{
  return context->HelperCheckEdge(
      TranslationKeynodes::translation_ignored_keynodes, node, ScType::EdgeAccessConstPosPerm);
}

bool SemanticNeighbourhoodTranslator::isInStructure(ScAddr const & elementAddr, ScAddrSet const & structure)
{
  return structure.empty() || (structure.find(elementAddr) != structure.end());
}

bool SemanticNeighbourhoodTranslator::anyIsInStructure(ScAddrVector const & elements, ScAddrSet const & structure)
{
  if (structure.empty())
    return true;
  for (const auto & element : elements)
  {
    if (structure.find(element) != structure.end())
      return true;
  }
  return false;
}
}  // namespace naturalLanguageProcessingModule
