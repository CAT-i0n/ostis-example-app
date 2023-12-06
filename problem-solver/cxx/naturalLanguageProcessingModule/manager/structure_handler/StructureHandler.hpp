#pragma once
#include "manager/language_handler/LanguageHandler.hpp"

namespace naturalLanguageProcessingModule
{
    class StructureHandler
    {
    public:
        StructureHandler(LanguageHandler* languageHandler);
        std::string construct(std::map<std::string, std::vector<std::vector<std::string>>> inTransl,
                            std::map<std::string, std::vector<std::vector<std::string>>> fromTransl);
        std::string constructInTranslations(std::map<std::string, std::vector<std::vector<std::string>>> inTransl, std::string mainIdtf);
        std::string constructFromTranslations(std::map<std::string, std::vector<std::vector<std::string>>> fromTransl, std::string mainIdtf);
        std::string createSentanceForm(std::vector<std::string> strVector, bool isLast = true);
        bool noQuasyBinaryRelations(std::vector<std::vector<std::string>> vec);
        std::string deleteTags(std::string text);
    private: 
        LanguageHandler* _languageHandler;
    };

}  
