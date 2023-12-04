
#include "StructureHandler.hpp"

namespace naturalLanguageProcessingModule
{

    StructureHandler::StructureHandler(LanguageHandler * languageHandler)
    {
        _languageHandler = languageHandler;
    }

    std::string StructureHandler::construct(std::map<std::string, std::vector<std::vector<std::string>>> inTransl,
                                            std::map<std::string, std::vector<std::vector<std::string>>> fromTransl)
    {
        std::string mainIdtf = _languageHandler->getEntityName(inTransl);
        std::string roughtText = mainIdtf + "\n";
        roughtText += this->constructInTranslations(inTransl, mainIdtf);
        roughtText += this->constructFromTranslations(fromTransl, mainIdtf);
        return roughtText;
    }

    std::string StructureHandler::constructInTranslations(std::map<std::string, std::vector<std::vector<std::string>>> inTransl, std::string mainIdtf)
    {
        std::string inTranslText = "";
        for(auto tr: inTransl)
        {
            if (_languageHandler->isInIgnoredIdtfs(tr.first))
                continue;
            std::string synonyms = _languageHandler->findSynonyms(tr.first, true);
            inTranslText += mainIdtf + " " + synonyms + " ";
            if(noQuasyBinaryRelations(tr.second))
            {
            std::vector<std::string> totalStrings;
            for(auto vec: tr.second)
            {
                if(vec.size() > 0)
                totalStrings.push_back(vec[0]);
            }
            inTranslText += createSentanceForm(totalStrings);
            }
            else
            {
                for(auto vec: tr.second)
                {
                    inTranslText += createSentanceForm(vec);
                }
            }
        }
        return inTranslText;
    }

    std::string StructureHandler::constructFromTranslations(std::map<std::string, std::vector<std::vector<std::string>>> fromTransl, std::string mainIdtf)
    {
        std::string fromTranslText = "";
        for(auto tr: fromTransl)
        {
            if (_languageHandler->isInIgnoredIdtfs(tr.first))
                continue;
            std::string synonyms = _languageHandler->findSynonyms(tr.first, false);
            fromTranslText += mainIdtf + " " + synonyms + " ";
            if(noQuasyBinaryRelations(tr.second))
            {
                std::vector<std::string> totalStrings;
                for(auto vec: tr.second)
                {
                    if(vec.size() > 0)
                        totalStrings.push_back(vec[0]);
                }
                fromTranslText += createSentanceForm(totalStrings);
            }
            else
            {
                for(auto vec: tr.second)
                {
                    fromTranslText+= createSentanceForm(vec);
                }
            }
        }
        return fromTranslText;
    }

    std::string StructureHandler::createSentanceForm(std::vector<std::string> strVector)
    {
        std::string rightForm = "";
        std::string conjuctWord = _languageHandler->getConjuctionWord();
        if(strVector.size() == 0)
        return "";
        else if(strVector.size() == 1)
            return strVector[0] + ". ";
        else if (strVector.size() == 2)
            return strVector[0] + conjuctWord + strVector[1] + ". ";
        else{
            rightForm = strVector[0];
            for(long unsigned int i = 1; i < strVector.size()-1; i++)
            {
                rightForm += ", " + strVector[i];
            }
            rightForm += conjuctWord + strVector[strVector.size()-1] + ". ";
            return rightForm;
        }
    }

    bool StructureHandler::noQuasyBinaryRelations(std::vector<std::vector<std::string>> vec)
    {
        for(auto v: vec)
        {
            if(v.size() > 1)
            return false;
        }
        return true;
    }

}