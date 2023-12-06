"""That file contains configurations for raw text processing
agents."""
# Common configs for all agents
AVAILABLE_LANGUAGES = ('lang_ru', 'lang_en')

# Prompts
PROMPTS = {'lang_en': 'I have sentences divided by semicolons. Connect those sentences in one human-readable text.'
                      'Connect some sentences in one if possible. Drop repeatable information.'
                      'Do not add information which is not represented in sentences. Give only answer. Here are sentences: "{!s}"',
           'lang_ru': 'У меня есть предложения, разделённые точкой с запятой. Соедини эти предложения в читаемый текст.'
                      'Объедини некоторые предложения в одно, если возможно. Убери повторяющуюся информацию.'
                      'Не добавляй информацию, которой нет в предложениях. Дай только ответ. Вот предложения: "{!s}"'
           }

# Configs for non official API agent
NON_OFFICIAL_API_DEFAULT_MODEL = 'gpt-3.5-turbo'
NON_OFFICIAL_API_AGENT_ACTION = 'action_get_clean_text_using_non_official_api'

# Configs for local language model agent
LOCAL_MODEL_PATH = 'PUT_YOR_INTEL_LLM_PATH_HERES'
LOCAL_MODEL_AGENT_ACTION = 'action_get_clean_text_using_local_model'

# Configs for official API agent
OPENAI_TOKEN = 'PUT_YOUR_OPENAI_TOKEN_HERE'
OPENAI_DEFAULT_MODEL = 'gpt-3.5-turbo'
OFFICIAL_API_AGENT_ACTION = 'action_get_clean_text_using_official_api'
