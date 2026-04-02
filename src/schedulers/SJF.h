#pragma once

#include "../common.h"
#include "../util.h"

#include "../job.h"
#include "scheduler.h"

class ShortestJobFirst : public Scheduler {
    std::vector<std::shared_ptr<Job>> jobs{};
    std::shared_ptr<Job> currentJob{nullptr};

    void sort();

  public:
    /// Adds job to list, ignored if present.
    void addJob(const std::shared_ptr<Job> &job) override;
    /// Removes job from list, ignored if missing.
    void remJob(const std::shared_ptr<Job> &job) override;
    /// Set new tick for queue to use as present.
    void update(int tick) override;
    /// Return job at front of queue.
    std::shared_ptr<Job> chooseJob() override;
    /// Return string representation of SJF object.
    std::string toString() const override;

    ShortestJobFirst() = default;
    ~ShortestJobFirst() override = default;
};
