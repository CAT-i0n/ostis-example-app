#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc_addr.hpp"
#include <sc-memory/kpm/sc_agent.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/TranslationKeynodes.hpp"

namespace naturalLanguageProcessingModule
{
class LanguageHandler
{
public:
    ScMemoryContext* context;

    LanguageHandler(ScMemoryContext * context);
    std::string getContent(ScAddr const & linkNode) const;
    ScIterator5Ptr getNrelMainIdtfIterator(ScAddr const & node) const;
    std::string getMainIdtf(ScAddr const & node) const;
    virtual std::string findSynonyms(std::string idtf, bool isInTransl, std::string mainIdtf, std::string translations) = 0;
    virtual bool isInIgnoredIdtfs(std::string idtf) = 0;
    virtual std::string getEntityName(std::map<std::string, std::vector<std::vector<std::string>>> inTransl) = 0;
    virtual std::string getConjuctionWord() = 0;
    virtual ~LanguageHandler();
    virtual bool isInAppropriateLanguage(ScAddr const & node) const = 0;
};

class EnglishLanguageHandler: public LanguageHandler
{
public:
    EnglishLanguageHandler(ScMemoryContext * context);
    std::string findSynonyms(std::string idtf, bool isInTransl, std::string mainIdtf, std::string translations) override;
    bool isInIgnoredIdtfs(std::string idtf) override;
    std::string getEntityName(std::map<std::string, std::vector<std::vector<std::string>>> inTransl) override;
    std::string getConjuctionWord() override;
    bool isInAppropriateLanguage(ScAddr const & node) const override;
};

class RussianLanguageHandler: public LanguageHandler
{
public:
    RussianLanguageHandler(ScMemoryContext * context);
    std::string findSynonyms(std::string idtf, bool isInTransl, std::string mainIdtf, std::string translations) override;
    bool isInIgnoredIdtfs(std::string idtf) override;
    std::string getEntityName(std::map<std::string, std::vector<std::vector<std::string>>> inTransl) override;
    std::string getConjuctionWord() override;
    bool isInAppropriateLanguage(ScAddr const & node) const override;
};


}  // namespace naturalLanguageProcessingModule
