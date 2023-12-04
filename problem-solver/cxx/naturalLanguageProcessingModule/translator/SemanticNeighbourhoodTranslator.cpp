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
