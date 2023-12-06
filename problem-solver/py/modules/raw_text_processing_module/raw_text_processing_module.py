"""
This module contains agents, that process raw text
with KB structure (knowledge graph) description using different ways.
On current moment you can get clean and human-readable
text using local language model (LLAMA-2 7B by default),
using official API of other more powerful LLM like ChatGPT
(don't forget to get API Token for that) or using non-official
API of powerful LLMs (be careful, it's not quite legal and
LLMs creators try to get rid of such ways of using their LLMs. That means
agents' creators are not responsible for stable working of such methods).
The best way is to use official API for LLMs because such LLMs are <<smarter>> than
ones you can download, faster as LLMs providers have access to fastest GPUs.
"""

from sc_kpm import ScModule
from .non_official_api_agent import NonOfficialAPITextProcessor
from .official_api_agent import OfficialAPITextProcessor


class RawTextProcessingModule(ScModule):
    def __init__(self) -> None:
        super().__init__(NonOfficialAPITextProcessor())
