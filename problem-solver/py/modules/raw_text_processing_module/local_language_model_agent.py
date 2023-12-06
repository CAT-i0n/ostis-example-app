"""This is agent which uses local LLM which is located on your computer/server.
You can choose used model in configs by changing LOCAL_MODEL_PATH parameter.
Better make server which will contain and maintain that model as you need powerful
GPU to run it locally."""


from sc_client.constants import sc_types
from sc_client.models import ScAddr, ScLinkContentType, ScTemplate
from sc_client.client import template_search

from sc_kpm.sc_keynodes import Idtf
from sc_kpm.sc_result import ScResult
from sc_kpm.utils.action_utils import (
    get_action_arguments,
    finish_action_with_status,
    create_action_answer
)
from sc_kpm.utils import (
    get_link_content_data,
    create_link,
    get_system_idtf,
)
from sc_kpm import ScAgentClassic

from . import raw_text_processing_configs as cf
from .interfaces import IGetCleanText
import logging

import transformers


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(levelname)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class LocalLLMTextProcessor(ScAgentClassic, IGetCleanText):
    def __init__(self) -> None:                     
        super().__init__(cf.LOCAL_MODEL_AGENT_ACTION)

        # Initiating tokenizer and model
        self._tokenizer = transformers.AutoTokenizer.from_pretrained(cf.LOCAL_MODEL_PATH)
        self._model = transformers.AutoModelForCausalLM.from_pretrained(
            pretrained_model_name_or_path=cf.LOCAL_MODEL_PATH,
            device_map='auto',
            torch_dtype='auto'
        )                 

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:                       
        self.logger.info('Local LLM raw text processor began to run...')

        # Get sc-link with raw text        
        raw_text_node = get_action_arguments(action_element, 1)[0]
        if not raw_text_node:
            self.logger.error('Error: could not find raw text sc-link to process')
            return ScResult.ERROR_INVALID_PARAMS
        
        #Get language of raw text sc-link
        language_template = ScTemplate()
        language_template.triple(
            sc_types.NODE_CLASS,
            sc_types.EDGE_ACCESS_VAR_POS_PERM,
            raw_text_node
        )
        search_result = template_search(language_template)
        if len(search_result) != 1:
            self.logger.error('Error: You have passed no language or too many arguments.')
            return ScResult.ERROR_INVALID_PARAMS
        language_node = search_result[0][0]
        language = get_system_idtf(language_node)
        if not language in cf.AVAILABLE_LANGUAGES:
            self.logger.error(f'Error: you have not passed available language as argument. You passed: {language}')
            return ScResult.ERROR_INVALID_PARAMS
        
        # Get raw text string
        raw_text = get_link_content_data(raw_text_node)        
        if not isinstance(raw_text, str):
            self.logger.error(f'Error: your raw text link must be string type, but text of yours is {type(raw_text)}')
            return ScResult.ERROR_INVALID_TYPE

        # Trying to get clean text        
        try:
            clean_text = self._get_clean_text(raw_text, language)            
        except Exception as err:
            self.logger.error(f'Error: {err}.\nThis error is on local LLM\'s side.')
            return ScResult.ERROR
        
        # Check text for emptiness. If processed text is empty, that means that model does not work
        if clean_text is None or clean_text == '':
            self.logger.error(f'Error: local LLM does not work. Try to change it or use other processing method.')
            return ScResult.ERROR

        # Creating answer and finishing agent work
        answer_link = create_link(clean_text, ScLinkContentType.STRING)        
        create_action_answer(action_element, answer_link)
        self.logger.info('Successfully processed the text using local LLM.')
        finish_action_with_status(action_element, True)
        return ScResult.OK
    
    def _get_clean_text(self, raw_text: str, language: str) -> str:
        """Method applies raw text describing structure from KB and language, which is string
        containing text like lang_YOUR_LANGUAGE_CODE (which is different representation for languages in
        OSTIS KBs). For example, english language will be lang_en. For text processing local LLM
        is used. You can configure used model in raw_text_processing_configs file"""
        
        system_input = cf.PROMPTS[language].format('').replace("\'", '')
        prompt = f"### System:\n{system_input}\n### User:\n{raw_text}\n### Assistant:\n"

        # Tokenize and encode the prompt
        inputs = self._tokenizer.encode(prompt, return_tensors="pt", add_special_tokens=False).to('cuda')
        outputs = self._model.generate(inputs, max_length=1000, num_return_sequences=1, eos_token_id=self._tokenizer.eos_token_id,
                                do_sample=True,top_k=10)
        response = self._tokenizer.decode(outputs[0], skip_special_tokens=True)

        # Extract only the assistant's response
        return response.split("### Assistant:\n")[-1]
