#include "SJF.h"

void ShortestJobFirst::addJob(const std::shared_ptr<Job> &job) {
    if (std::any_of(jobs.cbegin(), jobs.cend(), // Check if job in jobs
                    [job](const std::shared_ptr<Job> &test_job) {
                        return test_job->id == job->id;
                    })) {
        return;
    }
    jobs.emplace_back(job);
}

void ShortestJobFirst::remJob(const std::shared_ptr<Job> &job) {
    if (job->id == currentJob->id) {
        currentJob = nullptr;
        return;
    }
    for (size_t i = 0; i < jobs.size(); i++) {
        if (jobs[i]->id == job->id) {
            jobs.erase(jobs.begin() + static_cast<long int>(i));
            return;
        }
    }
}

void ShortestJobFirst::update(int tick) { (void)tick; }

std::shared_ptr<Job> ShortestJobFirst::chooseJob() {
    if (!currentJob && !jobs.empty()) {
        int shortest_job_index{-1};
        std::shared_ptr<Job> shortest_job = nullptr;
        for (size_t i = 0; i < jobs.size(); i++) {
            if (shortest_job == nullptr ||
                jobs[i]->time_remaining < shortest_job->time_remaining) {
                shortest_job = jobs[i];
                shortest_job_index = static_cast<int>(i);
            }
        }
        if (shortest_job_index >= 0) {
            jobs.erase(jobs.begin() + static_cast<size_t>(shortest_job_index));
            currentJob = shortest_job;
        }
    }
    if (!currentJob && jobs.empty()) {
        return nullptr;
    }
    return currentJob;
}

std::string ShortestJobFirst::toString() const {
    if (!currentJob) {
        return std::format("SJF()");
    } else {
        return std::format("SJF({} with {} remaining ticks)", currentJob->id,
                           currentJob->time_remaining);
    }
}
