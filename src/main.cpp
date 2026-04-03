#include "job.h"
#include "schedulers/scheduler.h"
#include "simulator.h"

int main() {
    std::vector<std::shared_ptr<Job>> workload;
    size_t job_count = 10;
    int avg_job_length = 50;
    workload.reserve(job_count);
    for (int i = 0; i < job_count; i++) {
        int len{(rand() % (avg_job_length * 2)) + 1};
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
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::FIFOQUEUE};
        s.setup();
        s.run();
        s.print();
        s.printMetrics();
    }
}
