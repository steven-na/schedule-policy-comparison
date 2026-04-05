#include "RR.h"

RoundRobin::RoundRobin(const int maxQuantumLength)
    : maxQuantumLength{maxQuantumLength} {};

void RoundRobin::addJob(const std::shared_ptr<Job> &job) {
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

void RoundRobin::remJob(const std::shared_ptr<Job> &job) {
    if (currentJob->id == job->id) {
        currentJob = nullptr;
        return;
    }
    jobs.dequeue(job);
}

void RoundRobin::update(int tick) { this->tick = tick; }

std::shared_ptr<Job> RoundRobin::chooseJob() {
    if ((tick - currentQuantumStartTick) >= maxQuantumLength) {
        if (currentJob) {
            jobs.enqueue(currentJob);
        }
        currentJob = jobs.poll();
        currentQuantumStartTick = tick;
    } else if (!currentJob) {
        currentJob = jobs.poll();
        currentQuantumStartTick = tick;
    }
    return currentJob;
}

std::string RoundRobin::toString() const {
    if (!currentJob) {
        return std::format("RR<{}>()", maxQuantumLength);
    } else {
        return std::format("RR<{}>(Job {} with {} remaining ticks)",
                           maxQuantumLength, currentJob->id,
                           currentJob->time_remaining);
    }
}
