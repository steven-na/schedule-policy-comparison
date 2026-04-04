#include "job.h"
#include "simulator.h"
#include "timer.h"

#include "schedulers/scheduler.h"

int main() {
    std::vector<std::shared_ptr<Job>> workload;
    size_t job_count = 1000;
    int avg_job_length = 500;
    int avg_job_arrival = 50000;
    workload.reserve(job_count);
    for (size_t i = 0; i < job_count; i++) {
        int len{(rand() % (avg_job_length * 2)) + 1};
        int arrival{rand() % (avg_job_arrival * 2)};
        // int arrival{0};
        workload.emplace_back(
            std::make_shared<Job>(Job{.id{static_cast<int>(i) + 1},
                                      .time_remaining{len},
                                      .time_arrival{arrival}}));
    }

    auto getOwnWorkload = [&workload]() {
        std::vector<std::shared_ptr<Job>> copy;
        copy.reserve(workload.size());
        for (const auto &j : workload) {
            copy.push_back(std::make_shared<Job>(*j));
        }
        return copy;
    };

    Timer t{};

    {
        t.reset();
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::FIFO};
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
    std::print("---------------------------------------------\n");
    {
        t.reset();
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::FIFOQUEUE};
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
    std::print("---------------------------------------------\n");
    {
        t.reset();
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::SJF};
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
    std::print("---------------------------------------------\n");
    {
        t.reset();
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::STCF};
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
}
