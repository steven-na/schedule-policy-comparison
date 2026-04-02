#include "job.h"
#include "scheduler.h"
#include "simulator.h"

int main() {
    std::vector<std::shared_ptr<Job>> workload;
    workload.reserve(20);
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 1, .time_remaining = 11, .time_arrival = 12}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 2, .time_remaining = 12, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 3, .time_remaining = 13, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 4, .time_remaining = 140, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 5, .time_remaining = 15, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 6, .time_remaining = 16, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 7, .time_remaining = 17, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 8, .time_remaining = 18, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 9, .time_remaining = 19, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 10, .time_remaining = 20, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 11, .time_remaining = 2, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 12, .time_remaining = 22, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 13, .time_remaining = 23, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 14, .time_remaining = 24, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 15, .time_remaining = 25, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 16, .time_remaining = 26, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 17, .time_remaining = 27, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 18, .time_remaining = 28, .time_arrival = 15}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 19, .time_remaining = 29, .time_arrival = 150}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 20, .time_remaining = 30, .time_arrival = 0}));

    auto getOwnWorkload = [&workload]() {
        std::vector<std::shared_ptr<Job>> copy;
        copy.reserve(workload.size());
        for (const auto &j : workload) {
            copy.push_back(std::make_shared<Job>(*j));
        }
        return copy;
    };

    {
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::FIFO};
        s.setup();
        s.run();
        s.print();
        s.printMetrics();
    }
    {
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::SJF};
        s.setup();
        s.run();
        s.print();
        s.printMetrics();
    }
}
