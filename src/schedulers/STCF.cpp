#include "STCF.h"

void ShortestTimeCompletionFirst::addJob(const std::shared_ptr<Job> &job) {
    if (jobs.itemExists(job)) {
        return;
    }

    jobs.enqueueWeighted(job, [job](const std::shared_ptr<Job> &other_job) {
        if (job->time_remaining == other_job->time_remaining) {
            return job->id < other_job->id;
        }
        return job->time_remaining < other_job->time_remaining;
    });
}

void ShortestTimeCompletionFirst::remJob(const std::shared_ptr<Job> &job) {
    jobs.dequeue(job);
}

void ShortestTimeCompletionFirst::update(int tick) { (void)tick; }

std::shared_ptr<Job> ShortestTimeCompletionFirst::chooseJob() {
    return jobs.peek();
}

std::string ShortestTimeCompletionFirst::toString() const {
    if (!jobs.peek()) {
        return std::string{"STCF()"};
    }
    auto job = jobs.peek();
    return std::format("STCF(Job {} with {} remaining ticks)", job->id,
                       job->time_remaining);
}
