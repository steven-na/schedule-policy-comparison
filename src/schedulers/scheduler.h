#pragma once

#include "../common.h"
#include "../util.h"

#include "../job.h"

class Scheduler {
  public:
    virtual void addJob(const std::shared_ptr<Job> &job) = 0;
    virtual void remJob(const std::shared_ptr<Job> &job) = 0;
    virtual void update(int tick) = 0;
    virtual std::shared_ptr<Job> chooseJob() = 0;
    virtual std::string toString() const = 0;

    virtual ~Scheduler() = default;

    enum class SchedulerType { FIFO, FIFOQUEUE, SJF, STCF, RR };
};
