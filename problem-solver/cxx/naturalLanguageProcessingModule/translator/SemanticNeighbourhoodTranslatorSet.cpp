#include "SemanticNeighbourhoodTranslatorSet.hpp"

#include "NrelInNodeSemanticNeighbourhoodTranslator.hpp"
#include "NrelInLinkSemanticNeighbourhoodTranslator.hpp"
#include "NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"
#include "NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"
#include "NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"
#include "FromParameterSemanticNeighbourhoodTranslator.hpp"
#include "FromConceptSemanticNeighbourhoodTranslator.hpp"
#include "NrelFromNodeSemanticNeighbourhoodTranslator.hpp"
#include "NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"
#include "TestTranslatorForSubdividingElement.hpp"
#include "InConceptSemanticNeighbourhoodTranslator.hpp"
#include "TestTakeExplanationTranslator.hpp"


namespace naturalLanguageProcessingModule
{
SemanticNeighbourhoodTranslatorSet::SemanticNeighbourhoodTranslatorSet(ScMemoryContext * context)
  : handlers(
        {new NrelInNodeSemanticNeighbourhoodTranslator(context),
         new NrelInLinkSemanticNeighbourhoodTranslator(context),
         new NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator(context),
         new NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator(context),
         new NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator(context),
         new FromParameterSemanticNeighbourhoodTranslator(context),
         new FromConceptSemanticNeighbourhoodTranslator(context),
         new NrelFromNodeSemanticNeighbourhoodTranslator(context),
         new NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator(context),
         new TestTranslatorForSubdividingElement(context),
         new InConceptSemanticNeighbourhoodTranslator(context),
         new TestTakeExplanationTranslator(context)})
{
}

SemanticNeighbourhoodTranslatorSet::~SemanticNeighbourhoodTranslatorSet()
{
  for (const auto & handler : handlers)
    delete handler;
}

    std::vector<std::string> SemanticNeighbourhoodTranslatorSet::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslationsFromEachHandler,
    std::map<std::string, std::vector<std::vector<std::string>>> & inTr,
    std::map<std::string, std::vector<std::vector<std::string>>> & fromTr,
    bool isEnglish,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction) const
{
  std::vector<std::string> answer;
  for (const auto & handler : handlers)
  {
    auto translations =
        handler->getSemanticNeighbourhoods(node, maxTranslationsFromEachHandler, structure, atLeastOneNodeFromConstruction, inTr, fromTr, isEnglish);
    answer.insert(answer.cend(), translations.cbegin(), translations.cend());
  }
  return answer;
}

std::list<ScAddrVector> SemanticNeighbourhoodTranslatorSet::getSemanticNeighbourhoodsElements(ScAddr const & node, ScAddrSet const & structure)
    const
{
  std::list<ScAddrVector> answerList;
  for (const auto & handler : handlers)
  {
    std::list<ScAddrVector> const & addrs = handler->getSemanticNeighbourhoodsElements(node, structure);
    answerList.insert(answerList.cend(), addrs.cbegin(), addrs.cend());
  }
  return answerList;
}
}  // namespace naturalLanguageProcessingModule
