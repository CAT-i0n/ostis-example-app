
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
            std::string translationText = "";
            if (_languageHandler->isInIgnoredIdtfs(tr.first))
                continue;

            if(noQuasyBinaryRelations(tr.second))
            {
                std::vector<std::string> totalStrings;
                for(auto vec: tr.second)
                {
                    if(vec.size() > 0)
                        totalStrings.push_back(vec[0]);
                }
                translationText += createSentanceForm(totalStrings, false);
            }
            else
            {
                for(int i  = 0; i < tr.second.size()-1; i++)
                {
                    translationText += createSentanceForm(tr.second[i], false);
                }
                translationText += createSentanceForm(tr.second[tr.second.size()-1], false);
            }
            if(translationText.empty())
                continue;
            std::string synonyms = _languageHandler->findSynonyms(tr.first, true, mainIdtf, translationText); 
            if (synonyms.empty())
                continue;
            inTranslText += synonyms;
        }
        return inTranslText;
    }

    std::string StructureHandler::constructFromTranslations(std::map<std::string, std::vector<std::vector<std::string>>> fromTransl, std::string mainIdtf)
    {
        std::string fromTranslText = "";
        for(auto tr: fromTransl)
        {
            std::string translationText = "";
            if (_languageHandler->isInIgnoredIdtfs(tr.first))
                continue;
            if(noQuasyBinaryRelations(tr.second))
            {
                std::vector<std::string> totalStrings;
                for(auto vec: tr.second)
                {
                    if(vec.size() > 0)
                        totalStrings.push_back(vec[0]);
                }
                
                translationText += createSentanceForm(totalStrings);
                if(translationText == "")
                    continue;
            }
            else
            {
                for(int i  = 0; i < tr.second.size()-1; i++)
                {
                    std::string sentance = createSentanceForm(tr.second[i], false);
                    if(sentance == "")
                        break;
                    translationText += sentance;
                }
                translationText += createSentanceForm(tr.second[tr.second.size()-1]);
                if(translationText == "")
                    continue;
            }
            std::string synonyms = _languageHandler->findSynonyms(tr.first, false, mainIdtf, translationText);
            if (synonyms.empty())
                continue;
            fromTranslText += synonyms;
        }
        return fromTranslText;
    }

    std::string StructureHandler::createSentanceForm(std::vector<std::string> strVector, bool isLast)
    {
        std::string rightForm = "";
        std::string conjuctWord = _languageHandler->getConjuctionWord();
        std::string separator = isLast? "; " : ", ";

        for(int i = 0; i < strVector.size(); i++)
        {
            strVector[i] = deleteTags(strVector[i]);
            if(strVector[i].empty())
                return "";
        }
        
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