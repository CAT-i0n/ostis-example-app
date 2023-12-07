#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

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

    //Generate clean text by language model
    ScAddr nextActionNode = ms_context->CreateNode(ScType::NodeConst);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, TranslationKeynodes::action_get_clean_text_using_non_official_api, nextActionNode);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question, nextActionNode);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, TranslationKeynodes::lang_ru, answerElements[0]);
    ScAddr rrelEdge = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, nextActionNode, answerElements[0]);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::rrel_1, rrelEdge);
    SC_LOG_INFO(utils::AgentUtils::applyAction(&m_memoryCtx, nextActionNode, 20000));

    ScAddr structure = utils::IteratorUtils::getFirstByOutRelation(&m_memoryCtx, nextActionNode, TranslationKeynodes::nrel_answer);
    ScAddr cleanText = utils::IteratorUtils::getAnyFromSet(&m_memoryCtx, structure);


    //Create node to show in interface
    ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, nodeAddr);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, TranslationKeynodes::nrel_summary);

    ScAddr const & summary_edge = ms_context->CreateEdge(ScType::EdgeDCommon, nodeAddr, cleanText);
    ScAddr const & summary_nrel_edge = ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, TranslationKeynodes::nrel_summary, summary_edge);

    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, cleanText);
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
