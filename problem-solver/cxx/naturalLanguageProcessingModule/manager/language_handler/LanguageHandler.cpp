#include "LanguageHandler.hpp"

namespace naturalLanguageProcessingModule
{
    LanguageHandler::LanguageHandler(ScMemoryContext* context): context(context){}
    std::string LanguageHandler::getContent(ScAddr const & linkNode) const
    {
        std::string content;
        if (isInAppropriateLanguage(linkNode))
            content = utils::CommonUtils::readString(context, linkNode);
        return content;
    }

    ScIterator5Ptr LanguageHandler::getNrelMainIdtfIterator(ScAddr const & node) const
    {
        return context->Iterator5(
            node,
            ScType::EdgeDCommonConst,
            ScType::LinkConst,
            ScType::EdgeAccessConstPosPerm,
            scAgentsCommon::CoreKeynodes::nrel_main_idtf);
    }

    std::string LanguageHandler::getMainIdtf(ScAddr const & node) const
    {
        std::string mainIdtf;
        auto const & iterator = getNrelMainIdtfIterator(node);
        while (iterator->Next())
        {
            mainIdtf = getContent(iterator->Get(2)) ;
            if (mainIdtf.empty() == SC_FALSE)
                break;
        }
        return mainIdtf;
    }

    LanguageHandler:: ~LanguageHandler()
    {
    }

    EnglishLanguageHandler::EnglishLanguageHandler(ScMemoryContext* context): LanguageHandler(context) {}

    bool EnglishLanguageHandler::isInAppropriateLanguage(ScAddr const & node) const 
    {
        return context->HelperCheckEdge(TranslationKeynodes::lang_en, node, ScType::EdgeAccessConstPosPerm);
    }

    std::string EnglishLanguageHandler::findSynonyms(std::string idtf, bool isInTransl, 
                                                    std::string mainIdtf, std::string translations) 
    {
        std::map<std::string, std::string> inSynonyms;
        std::map<std::string, std::string> fromSynonyms;

        fromSynonyms["subdividing*"] = "is subdivided into";
        fromSynonyms["included in"] = "is";
        fromSynonyms["strict inclusion*"] = "strictly included in";
        inSynonyms["subdividing*"] = "is subdividing of";
        inSynonyms["inclusion*"] = "includes" ;
        inSynonyms["identifier*"] = "also known as";
        inSynonyms["strict inclusion*"] = "strictly includes";
        inSynonyms["text"] = ":";
        if(isInTransl)
        {
            if(inSynonyms[idtf].size()>0) 
                return mainIdtf + " " + inSynonyms[idtf] + " " + translations;
            else 
            {
                if(idtf.find('*') != std::string::npos)
                    idtf = idtf.erase(idtf.find('*'), 1);
                return idtf == " "? "" : translations + " - " + idtf + " of " + mainIdtf;
            }
        }
        else
        {
            if(fromSynonyms[idtf].size()>0) 
                return mainIdtf + " " + fromSynonyms[idtf] + " " + translations;
            else
            {
                if(idtf.find('*') != std::string::npos)
                    idtf = idtf.erase(idtf.find('*'), 1);
                return idtf == " "? "" :  mainIdtf + " - " + idtf + " of " + translations;
            }
        }
    } 

    bool EnglishLanguageHandler::isInIgnoredIdtfs(std::string idtf) 
    {
        std::vector<std::string> ignoredIdfts = {"main identifier*", "definitional domain*", "second domain*", "first domain*",
                                                    "using constants*"};

        if(std::count(ignoredIdfts.begin(), ignoredIdfts.end(), idtf))
            return true;
        return false;
    } 

    std::string EnglishLanguageHandler::getEntityName(std::map<std::string, std::vector<std::vector<std::string>>> inTransl)
    {
        return inTransl["main identifier*"][0][0];
    }

    std::string EnglishLanguageHandler::getConjuctionWord()
    {
        return " and ";
    }

    RussianLanguageHandler::RussianLanguageHandler(ScMemoryContext* context): LanguageHandler(context) {}

    bool RussianLanguageHandler::isInAppropriateLanguage(ScAddr const & node) const 
    {
        return context->HelperCheckEdge(TranslationKeynodes::lang_ru, node, ScType::EdgeAccessConstPosPerm);
    }

    std::string RussianLanguageHandler::findSynonyms(std::string idtf, bool isInTransl, 
                                                    std::string mainIdtf, std::string translations) 
    {
        std::map<std::string, std::string> inSynonyms;
        std::map<std::string, std::string> fromSynonyms;

        fromSynonyms["разбиение*"] = "разбивается на";
        fromSynonyms["included in"] = "является";
        fromSynonyms["строгое включение*"] = "строго включается в ";
        inSynonyms["разбиение*"] = "является элементом разбиения";
        inSynonyms["включение*"] = "включает в себя" ;
        inSynonyms["идентификатор*"] = "также известен, как";
        inSynonyms["строгое включение*"] = "строго включает в себя";
        inSynonyms["text"] = ":";
        if(isInTransl)
        {
            translations.pop_back();
            translations.pop_back();
            if(inSynonyms[idtf].size()>0) 
                return mainIdtf + " " + inSynonyms[idtf] + " " + translations + "; ";
            else 
            {
                if(idtf.find('*') != std::string::npos)
                    idtf = idtf.erase(idtf.find('*'), 1);
                return idtf == " "? "" : translations + " - " + idtf + " " + mainIdtf+ "; ";
            }
        }
        else
        {
            if(fromSynonyms[idtf].size()>0) 
                return mainIdtf + " " + fromSynonyms[idtf] + " " + translations;
            else
            {
                if(idtf.find('*') != std::string::npos)
                    idtf = idtf.erase(idtf.find('*'), 1);
                return idtf == " "? "" :  mainIdtf + " - " + idtf + " " + translations;
            }
        }
    }

    bool RussianLanguageHandler::isInIgnoredIdtfs(std::string idtf) 
    {
        std::vector<std::string> ignoredIdfts = {"основной идентификатор*", "второй домен*", "первый домен*"};

        if(std::count(ignoredIdfts.begin(), ignoredIdfts.end(), idtf))
            return true;
        return false;
    }  

    std::string RussianLanguageHandler::getEntityName(std::map<std::string, std::vector<std::vector<std::string>>> inTransl)
    {
        return inTransl["основной идентификатор*"][0][0];
    }

    std::string RussianLanguageHandler::getConjuctionWord()
    {
        return " и ";
    }
}