#include "FIFOQueue.h"

void FirstInFirstOutQueue::addJob(const std::shared_ptr<Job> &job) {
    if (jobs.itemExists(job)) {
        return;
    }

    if (tick == 0) {
        jobs.enqueueWeighted(job, [job](const std::shared_ptr<Job> &other_job) {
            return job->id < other_job->id;
        });
        return;
    }
    jobs.enqueue(job);
}

void FirstInFirstOutQueue::remJob(const std::shared_ptr<Job> &job) {
    if (currentJob->id == job->id) {
        currentJob = nullptr;
        return;
    }
    jobs.dequeue(job);
}

void FirstInFirstOutQueue::update(int tick) { this->tick = tick; }

std::shared_ptr<Job> FirstInFirstOutQueue::chooseJob() {
    if (!currentJob) {
        currentJob = jobs.poll();
    }
    if (!currentJob) {
        exit(1);
    }
    return currentJob;
}

std::string FirstInFirstOutQueue::toString() const {
    if (!currentJob) {
        return std::string{"FIFOQueue()"};
    } else {
        return std::format("FIFOQueue({} with {} remaining ticks)",
                           currentJob->id, currentJob->time_remaining);
    }
}
