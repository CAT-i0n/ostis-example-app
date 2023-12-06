#include <memory>

#include "sc-memory/sc_memory.hpp"


#include "sc-memory/sc_event.hpp"




#define TranslationKeynodes_hpp_14_init  bool _InitInternal(ScAddr const & outputStructure = ScAddr::Empty) override \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "TranslationKeynodes::_InitInternal"); \
    ScSystemIdentifierFiver fiver; \
    bool result = true; \
    return result; \
}


#define TranslationKeynodes_hpp_14_initStatic static bool _InitStaticInternal(ScAddr const & outputStructure = ScAddr::Empty)  \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "TranslationKeynodes::_InitStaticInternal"); \
    ScSystemIdentifierFiver fiver; \
    bool result = true; \
	ctx.HelperResolveSystemIdtf("action_form_semantic_neighbourhood_translation", ScType::NodeConstClass, fiver);action_form_semantic_neighbourhood_translation = fiver.addr1; result = result && action_form_semantic_neighbourhood_translation.IsValid(); if (outputStructure.IsValid()) {ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr1);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr2);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr3);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr4);}; \
	ctx.HelperResolveSystemIdtf("lang_en", ScType::NodeConstClass, fiver);lang_en = fiver.addr1; result = result && lang_en.IsValid(); if (outputStructure.IsValid()) {ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr1);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr2);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr3);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr4);}; \
	ctx.HelperResolveSystemIdtf("lang_ru", ScType::NodeConstClass, fiver);lang_ru = fiver.addr1; result = result && lang_ru.IsValid(); if (outputStructure.IsValid()) {ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr1);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr2);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr3);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr4);}; \
	ctx.HelperResolveSystemIdtf("parameter", ScType::NodeConstClass, fiver);parameter = fiver.addr1; result = result && parameter.IsValid(); if (outputStructure.IsValid()) {ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr1);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr2);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr3);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr4);}; \
	ctx.HelperResolveSystemIdtf("translation_ignored_keynodes", ScType::NodeConstClass, fiver);translation_ignored_keynodes = fiver.addr1; result = result && translation_ignored_keynodes.IsValid(); if (outputStructure.IsValid()) {ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr1);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr2);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr3);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr4);}; \
	ctx.HelperResolveSystemIdtf("nrel_rough_summary", ScType::NodeConstClass, fiver);nrel_rough_summary = fiver.addr1; result = result && nrel_rough_summary.IsValid(); if (outputStructure.IsValid()) {ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr1);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr2);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr3);ctx.CreateEdge(ScType::EdgeAccessConstPosPerm, outputStructure, fiver.addr4);}; \
    return result; \
}


#define TranslationKeynodes_hpp_14_decl 

#define TranslationKeynodes_hpp_TranslationKeynodes_impl 

#undef ScFileID
#define ScFileID TranslationKeynodes_hpp

