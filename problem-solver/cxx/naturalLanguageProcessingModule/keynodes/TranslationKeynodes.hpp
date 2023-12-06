#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_object.hpp"

#include "TranslationKeynodes.generated.hpp"

namespace naturalLanguageProcessingModule
{

class TranslationKeynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
  SC_PROPERTY(Keynode("action_form_semantic_neighbourhood_translation"), ForceCreate(ScType::NodeConstClass))
  static ScAddr action_form_semantic_neighbourhood_translation;

  SC_PROPERTY(Keynode("lang_en"), ForceCreate(ScType::NodeConstClass))
  static ScAddr lang_en;

  SC_PROPERTY(Keynode("lang_ru"), ForceCreate(ScType::NodeConstClass))
  static ScAddr lang_ru;

  SC_PROPERTY(Keynode("parameter"), ForceCreate(ScType::NodeConstClass))
  static ScAddr parameter;

  SC_PROPERTY(Keynode("translation_ignored_keynodes"), ForceCreate(ScType::NodeConstClass))
  static ScAddr translation_ignored_keynodes;

  SC_PROPERTY(Keynode("nrel_rough_summary"), ForceCreate(ScType::NodeConstClass))
  static ScAddr nrel_rough_summary;

  SC_PROPERTY(Keynode("action_get_clean_text_using_non_official_api"), ForceCreate(ScType::NodeConstClass))
  static ScAddr action_get_clean_text_using_non_official_api;

  SC_PROPERTY(Keynode("action_example_action"), ForceCreate(ScType::NodeConstClass))
  static ScAddr action_example_action;

  SC_PROPERTY(Keynode("nrel_answer"), ForceCreate(ScType::NodeConstClass))
  static ScAddr nrel_answer;
};

} // naturalLanguageProcessingModule
