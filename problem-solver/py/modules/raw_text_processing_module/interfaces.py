from abc import ABC, abstractmethod


class IGetCleanText(ABC):

    @abstractmethod
    def _get_clean_text(raw_text: str, language: str) -> str:
        pass
