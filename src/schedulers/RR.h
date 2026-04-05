#pragma once

#include "../common.h"
#include "../util.h"

#include "../JobQueue.h"
#include "../job.h"
#include "scheduler.h"

class RoundRobin : public Scheduler {
    JobQueue jobs{};
    std::shared_ptr<Job> currentJob{};
    int tick{0};

    const int maxQuantumLength{};
    int currentQuantumStartTick{};

  public:
    /// Adds job to end of queue, ignored if present.
    void addJob(const std::shared_ptr<Job> &job) override;
    /// Removes job from queue, ignored if missing.
    void remJob(const std::shared_ptr<Job> &job) override;
    /// Set new tick for queue to use as present.
    void update(int tick) override;
    /// Return job at front of queue.
    std::shared_ptr<Job> chooseJob() override;
    /// Return string representation of RR object.
    std::string toString() const override;

    explicit RoundRobin(int maxQuantumLength);
    ~RoundRobin() override = default;
};
