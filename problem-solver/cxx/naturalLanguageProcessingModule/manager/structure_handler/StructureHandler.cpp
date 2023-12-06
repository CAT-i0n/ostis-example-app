
#include "StructureHandler.hpp"
#include <regex>
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
        std::string roughtText = mainIdtf + ";\n";
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
            if (synonyms.empty())
                continue;
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
                for(int i  = 0; i < tr.second.size()-1; i++)
                {
                    inTranslText += createSentanceForm(tr.second[i], false);
                }
                inTranslText += createSentanceForm(tr.second[tr.second.size()-1]);
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
            if (synonyms.empty())
                continue;
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
                for(int i  = 0; i < tr.second.size()-1; i++)
                {
                    fromTranslText+= createSentanceForm(tr.second[i], false);
                }
                fromTranslText += createSentanceForm(tr.second[tr.second.size()-1]);
            }
        }
        return fromTranslText;
    }

    std::string StructureHandler::createSentanceForm(std::vector<std::string> strVector, bool isLast)
    {
        std::string rightForm = "";
        std::string conjuctWord = _languageHandler->getConjuctionWord();
        std::string separator = isLast? "; " : ", ";

        for(int i = 0; i < strVector.size(); i++)
            strVector[i] = deleteTags(strVector[i]);
        if(strVector.size() == 0)
        return "";
        else if(strVector.size() == 1)
            return strVector[0] + separator;
        else if (strVector.size() == 2)
            return strVector[0] + conjuctWord + strVector[1] + separator;
        else{
            rightForm = strVector[0];
            for(long unsigned int i = 1; i < strVector.size()-1; i++)
            {
                rightForm += ", " + strVector[i];
            }
            rightForm += conjuctWord + strVector[strVector.size()-1] + separator;
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

    std::string StructureHandler::deleteTags(std::string text)
    {
        std::regex htmlTag("<[^>]*>");

        return std::regex_replace(text, htmlTag, "");
    }
}