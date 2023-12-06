"""This is agent which uses official API
of ChatGPT. You can choose used model in configs
by changing OPENAI_MODEL parameter. Maybe some more
LLM's support will be added in future."""


from openai import OpenAI
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
from requests import HTTPError
import logging


logging.basicConfig(
    level=logging.INFO, format="%(asctime)s | %(levelname)s | %(name)s | %(message)s", datefmt="[%d-%b-%y %H:%M:%S]"
)


class OfficialAPITextProcessor(ScAgentClassic, IGetCleanText):
    def __init__(self) -> None:                     
        super().__init__(cf.OFFICIAL_API_AGENT_ACTION)               

    def on_event(self, event_element: ScAddr, event_edge: ScAddr, action_element: ScAddr) -> ScResult:                       
        self.logger.info('Official API raw text processor began to run...')

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
            self.logger.error(f'Error: {err}.\nThis error is on official API\'s side.')
            return ScResult.ERROR        

        # Creating answer and finishing agent work
        answer_link = create_link(clean_text, ScLinkContentType.STRING)        
        create_action_answer(action_element, answer_link)
        self.logger.info('Successfully processed the text using official API! Wery well!')
        finish_action_with_status(action_element, True)
        return ScResult.OK
    
    def _get_clean_text(self, raw_text: str, language: str) -> str:
        """Method applies raw text describing structure from KB and language, which is string
        containing text like lang_YOUR_LANGUAGE_CODE (which is different representation for languages in
        OSTIS KBs). For example, english language will be lang_en. For text processing official
        LLM api is used. You can configure used model in raw_text_processing_configs file"""

        # Request for clean text from Open-AI
        client = OpenAI(
            api_key=cf.OPENAI_TOKEN
        )
        messages = [{'role': 'user', 'content': cf.PROMPTS[language].format(raw_text)}]
        response = client.chat.completions.create(
            model=cf.OPENAI_DEFAULT_MODEL,
            messages=messages,
            temperature=0
        )
        clean_text = response.choices[0].message.content
        self.logger.info(f'Successfully cleaned text for you\n: {clean_text}')
        return clean_text      
