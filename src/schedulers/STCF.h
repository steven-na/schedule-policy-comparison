#pragma once

#include "../common.h"
#include "../util.h"

#include "../JobQueue.h"
#include "../job.h"
#include "scheduler.h"

class ShortestTimeCompletionFirst : public Scheduler {
    JobQueue jobs{};

  public:
    /// Adds job to queue, sorts it based on time_remaining
    void addJob(const std::shared_ptr<Job> &job) override;
    /// Removes job from queue, ignored if missing.
    void remJob(const std::shared_ptr<Job> &job) override;
    /// Does nothing in STCF.
    void update(int tick) override;
    /// Return job at front of queue.
    std::shared_ptr<Job> chooseJob() override;
    /// Return string representation of STCF object.
    std::string toString() const override;

    ShortestTimeCompletionFirst() = default;
    ~ShortestTimeCompletionFirst() override = default;
};
