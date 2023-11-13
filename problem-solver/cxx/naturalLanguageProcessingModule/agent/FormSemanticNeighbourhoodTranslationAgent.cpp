#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/TranslationKeynodes.hpp"

#include "manager/FormSemanticNeighbourhoodTranslationManager.hpp"

#include "FormSemanticNeighbourhoodTranslationAgent.hpp"

namespace naturalLanguageProcessingModule
{
SC_AGENT_IMPLEMENTATION(FormSemanticNeighbourhoodTranslationAgent)
{
  ScAddr const & actionNode = otherAddr;
  try
  {
    if (checkActionClass(actionNode) == SC_FALSE)
      return SC_RESULT_OK;
    SC_LOG_DEBUG("FormSemanticNeighbourhoodTranslationAgent started");

    ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
    ScAddr setAddr = utils::IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);

    if (setAddr.IsValid() == SC_FALSE)
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "FormSemanticNeighbourhoodTranslationAgent: setAddr is not valid");

    auto summaryIterator = ms_context->Iterator3(setAddr, ScType::EdgeAccessConstPosPerm, ScType::Node);

    ScAddrVector nodeSummaries;
    auto manager = make_unique<FormSemanticNeighbourhoodTranslationManager>(&m_memoryCtx);
    while(summaryIterator->Next())
    { 
      nodeSummaries.push_back(manager->manage({summaryIterator->Get(2)}));

    }
    if (nodeSummaries.empty())
      SC_THROW_EXCEPTION(utils::ScException, "FormSemanticNeighbourhoodTranslationAgent: answer is empty");

    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, nodeSummaries, true);
    SC_LOG_DEBUG("FormSemanticNeighbourhoodTranslationAgent finished");
    return SC_RESULT_OK;
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, false);
    SC_LOG_DEBUG("FormSemanticNeighbourhoodTranslationAgent finished with error");
    return SC_RESULT_ERROR;
  }
}

bool FormSemanticNeighbourhoodTranslationAgent::checkActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      TranslationKeynodes::action_form_semantic_neighbourhood_translation, actionNode, ScType::EdgeAccessConstPosPerm);
}

}  // namespace naturalLanguageProcessingModule
