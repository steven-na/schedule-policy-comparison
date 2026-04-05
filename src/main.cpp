#include "job.h"
#include "simulator.h"
#include "timer.h"

#include "schedulers/scheduler.h"

int main() {
    std::vector<std::shared_ptr<Job>> workload;
    int job_count = 250;
    int avg_job_length = 250;
    int avg_job_arrival = 25000;
    workload.reserve(static_cast<size_t>(job_count));
    for (int i = 0; i < job_count; i++) {
        int len{(rand() % (avg_job_length * 2)) + 1};
        int arrival{rand() % (avg_job_arrival * 2)};
        workload.emplace_back(std::make_shared<Job>(
            Job{.id{i + 1}, .time_remaining{len}, .time_arrival{arrival}}));
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
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::FIFO};
        t.reset();
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
    std::print("---------------------------------------------\n");
    {
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::FIFOQUEUE};
        t.reset();
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
    std::print("---------------------------------------------\n");
    {
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::SJF};
        t.reset();
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
    std::print("---------------------------------------------\n");
    {
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::STCF};
        t.reset();
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
    std::print("---------------------------------------------\n");
    {
        Simulator s{getOwnWorkload(), Scheduler::SchedulerType::RR, 10};
        t.reset();
        s.setup();
        s.run();
        double duration = t.elapsed();
        s.print();
        s.printMetrics();
        std::print("Run took {} seconds\n", duration);
    }
}
