#pragma once

#include "../common.h"
#include "../util.h"

#include "../job.h"
#include "JobQueue.h"
#include "scheduler.h"

class FirstInFirstOutQueue : public Scheduler {
    JobQueue jobs{};
    std::shared_ptr<Job> currentJob{};
    int tick{0};

  public:
    /// Adds job to end of queue, ignored if present.
    void addJob(const std::shared_ptr<Job> &job) override;
    /// Removes job from queue, ignored if missing.
    void remJob(const std::shared_ptr<Job> &job) override;
    /// Set new tick for queue to use as present.
    void update(int tick) override;
    /// Return job at front of queue.
    std::shared_ptr<Job> chooseJob() override;
    /// Return string representation of FIFO object.
    std::string toString() const override;

    FirstInFirstOutQueue() = default;
    ~FirstInFirstOutQueue() override = default;
};
