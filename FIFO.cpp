#include "FIFO.h"

void FirstInFirstOut::addJob(const std::shared_ptr<Job> &job) {
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

void FirstInFirstOut::remJob(const std::shared_ptr<Job> &job) {
    for (size_t i = 0; i < jobs.size(); i++) {
        if (jobs[i]->id == job->id) {
            jobs.erase(jobs.begin() + static_cast<long int>(i));
            return;
        }
    }
}

void FirstInFirstOut::sort() {
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

void FirstInFirstOut::update(int tick) { this->tick = tick; }

std::shared_ptr<Job> FirstInFirstOut::chooseJob() const {
    if (jobs.empty()) {
        return nullptr;
    }
    return jobs[0];
}

std::string FirstInFirstOut::toString() const {
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
