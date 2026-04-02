#pragma once

#include "common.h"
#include "util.h"

#include "job.h"
#include "scheduler.h"

class FirstInFirstOut : public Scheduler {
    std::vector<std::shared_ptr<Job>> jobs{};
    std::unordered_map<int, int> arrivals{};
    int tick{0};

    void sort();

  public:
    void addJob(const std::shared_ptr<Job> &job) override;
    void remJob(const std::shared_ptr<Job> &job) override;
    void update(int tick) override;
    std::shared_ptr<Job> chooseJob() const override;
    std::string toString() const override;

    FirstInFirstOut() = default;
    ~FirstInFirstOut() override = default;
};
