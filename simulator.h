#pragma once

#include "common.h"
#include "util.h"

#include "job.h"
#include "scheduler.h"

#include "FIFO.h"
#include "SJF.h"

class Simulator {
    // workload and scheduler data
    std::vector<std::shared_ptr<Job>> mAllJobs{};
    std::vector<std::shared_ptr<Job>> mActiveJobs{};
    std::vector<std::shared_ptr<Job>> mNotArrivedJobs{};
    std::vector<std::shared_ptr<Job>> mIOJobs{};
    std::vector<std::shared_ptr<Job>> mCompletedJobs{};
    std::unique_ptr<Scheduler> mScheduler{};
    int tick = 0;

    // simulation member functions
    void addJob(const std::shared_ptr<Job> &j);
    void remJob(const std::shared_ptr<Job> &j);
    void buildInitialState();
    void loadInitialJobs();
    void updateJobsIO();
    void updateJobsArrived();
    void updateCompleteJob(std::shared_ptr<Job> &j);
    void mainLoop();

    // performance logging members
    std::unordered_map<int, int> jobCompletions{};
    std::unordered_map<int, int> jobFirstRunTimes{};
    std::vector<int> jobPreviouslyRun{};
    double avgTimeTurnaround() const;
    double avgTimeResponse() const;

  public:
    Simulator(const std::vector<std::shared_ptr<Job>> &jobs,
              Scheduler::SchedulerType stype);
    ~Simulator() = default;

    // simulation member functions
    void setup();
    void run();

    // data output member functions
    std::string toString() const;
    void print() const;
    void printMetrics() const;
};
