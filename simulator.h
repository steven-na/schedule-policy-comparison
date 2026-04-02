#pragma once

#include "common.h"
#include "util.h"

#include "job.h"
#include "scheduler.h"

#include "FIFO.h"

class Simulator {
    std::vector<std::shared_ptr<Job>> mAllJobs{};
    std::vector<std::shared_ptr<Job>> mActiveJobs{};
    std::vector<std::shared_ptr<Job>> mNotArrivedJobs{};
    std::vector<std::shared_ptr<Job>> mIOJobs{};
    std::vector<std::shared_ptr<Job>> mCompletedJobs{};
    std::unique_ptr<Scheduler> mScheduler{};

    int tick = 0;

    void addJob(const std::shared_ptr<Job> &j);
    void remJob(const std::shared_ptr<Job> &j);
    void buildInitialState();
    void loadInitialJobs();
    void updateJobsIO();
    void updateJobsArrived();
    void updateCompleteJob(std::shared_ptr<Job> &j);
    void mainLoop();

    std::string toString() const;

  public:
    Simulator(const std::vector<std::shared_ptr<Job>> &jobs,
              Scheduler::SchedulerType stype);
    ~Simulator() = default;

    void setup();
    void run();
    void print() const;
};
