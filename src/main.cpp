#include "job.h"
#include "schedulers/scheduler.h"
#include "simulator.h"

int main() {
    std::vector<std::shared_ptr<Job>> workload;
    workload.reserve(1000);
    for (int i = 0; i < 1000; i++) {
        int len{(rand() % 2000) + 1};
        workload.emplace_back(std::make_shared<Job>(
            Job{.id{i + 1}, .time_remaining{len}, .time_arrival{0}}));
    }

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
