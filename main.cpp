#include "job.h"
#include "scheduler.h"
#include "simulator.h"

int main() {
    std::vector<std::shared_ptr<Job>> workload;
    workload.reserve(6);
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 1, .time_remaining = 1000, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 2, .time_remaining = 1000, .time_arrival = 0}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 3, .time_remaining = 1000, .time_arrival = 5}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 4, .time_remaining = 1000, .time_arrival = 10}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 5, .time_remaining = 1000, .time_arrival = 7}));
    workload.emplace_back(std::make_shared<Job>(
        Job{.id = 6, .time_remaining = 1000, .time_arrival = 55}));

    size_t a{0};

    Simulator s{workload, Scheduler::SchedulerType::FIFO};
    s.setup();
    s.print();
    s.run();
    s.print();
    std::print("Exiting...\n");
}
