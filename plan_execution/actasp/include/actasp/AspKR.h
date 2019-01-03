#pragma once

#include <actasp/AspAtom.h>
#include <actasp/AnswerSet.h>
#include <actasp/MultiPlanner.h>

#include <vector>
#include <list>

namespace actasp {

class Action;

struct AspKR : public actasp::MultiPlanner {

    //actions available in this state
    virtual ActionSet availableActions() const noexcept = 0;

    virtual AnswerSet currentStateQuery(const std::vector<actasp::AspFluentRule> &query) const noexcept = 0;

    //most general query, doesn't try to parse the result into fluents
    virtual std::list<actasp::AnswerSet> query(const std::vector<actasp::AspFluentRule> &query, unsigned int timestep) const noexcept = 0;

    virtual bool isPlanValid(const AnswerSet &plan, const std::vector<actasp::AspFluentRule> &goal) const noexcept = 0;

  ~AspKR() override = default;
};

}

