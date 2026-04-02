#include "FIFOQueue.h"

void JobQueue::enqueue(std::shared_ptr<Job> &job) {}

void JobQueue::enqueueWeighted(std::shared_ptr<Job> &job) {}

std::optional<std::shared_ptr<Job>> JobQueue::poll() {
    QueueItem *prev{head->prev};
    std::optional<std::shared_ptr<Job>> out{};
    if (head) {
        out = head->job;
    }
    head = prev;
    return out;
}

std::optional<std::shared_ptr<Job>> JobQueue::peek() {
    std::optional<std::shared_ptr<Job>> out{};
    if (head) {
        out = head->job;
    }
    return out;
}

void FirstInFirstOutQueue::addJob(const std::shared_ptr<Job> &job) {
    // Make this use the queue
    if (std::any_of(jobs.cbegin(), jobs.cend(), // Check if job in jobs
                    [job](const std::shared_ptr<Job> &test_job) {
                        return test_job->id == job->id;
                    })) {
        return;
    }
    jobs.emplace_back(job);
    arrivals[job->id] = tick;
    sort();
}

void FirstInFirstOutQueue::remJob(const std::shared_ptr<Job> &job) {
    for (size_t i = 0; i < jobs.size(); i++) {
        if (jobs[i]->id == job->id) {
            jobs.erase(jobs.begin() + static_cast<long int>(i));
            return;
        }
    }
}

void FirstInFirstOutQueue::sort() {
    std::sort(
        jobs.begin(), jobs.end(),
        [this](const std::shared_ptr<Job> &a, const std::shared_ptr<Job> &b) {
            int aarr = arrivals[a->id];
            int barr = arrivals[b->id];
            if (aarr == barr) {
                return a->id < b->id;
            }
            return aarr < barr;
        });
}

void FirstInFirstOutQueue::update(int tick) { this->tick = tick; }

std::shared_ptr<Job> FirstInFirstOutQueue::chooseJob() {
    if (jobs.empty()) {
        return nullptr;
    }
    return jobs[0];
}

std::string FirstInFirstOutQueue::toString() const {
    std::string out_string{"FIFO("};
    for (size_t i = 0; i < jobs.size(); i++) {
        auto &j = jobs[i];
        out_string += std::format("{}[{}]", j->id, j->time_remaining);
        if (i != jobs.size() - 1) {
            out_string += " ";
        }
    }
    out_string += ")";
    return out_string;
}
