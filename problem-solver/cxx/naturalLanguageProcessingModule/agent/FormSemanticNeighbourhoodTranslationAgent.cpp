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
  ScAddr nodeAddr = utils::IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);

    if (nodeAddr.IsValid() == SC_FALSE)
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "FormSemanticNeighbourhoodTranslationAgent: nodeAddr is not valid");

    auto manager = make_unique<FormSemanticNeighbourhoodTranslationManager>(&m_memoryCtx);
    ScAddrVector const & answerElements = manager->manage({nodeAddr});

    if (answerElements.empty())
      SC_THROW_EXCEPTION(utils::ScException, "FormSemanticNeighbourhoodTranslationAgent: answer is empty");


    ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);


    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeAddr);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, TranslationKeynodes::nrel_rough_summary);

    ScAddr const & summary_edge = ms_context->CreateEdge(ScType::EdgeDCommon, nodeAddr, answerElements[0]);
    ScAddr const & summary_nrel_edge = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, TranslationKeynodes::nrel_rough_summary, summary_edge);

    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, answerElements[0]);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, summary_edge);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, summary_nrel_edge);

    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionNode, answer, true);
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
