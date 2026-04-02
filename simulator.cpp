#include "simulator.h"

Simulator::Simulator(const std::vector<std::shared_ptr<Job>> &jobs,
                     Scheduler::SchedulerType stype)
    : mAllJobs{jobs} {
    switch (stype) {
    case Scheduler::SchedulerType::FIFO:
        mScheduler = std::make_unique<FirstInFirstOut>();
        break;
    default:
        throw std::invalid_argument("Unknown Scheduler Type");
    }
}

void Simulator::addJob(const std::shared_ptr<Job> &j) { mScheduler->addJob(j); }

void Simulator::remJob(const std::shared_ptr<Job> &j) { mScheduler->remJob(j); }

void Simulator::buildInitialState() {
    for (size_t i = 0; i < mAllJobs.size(); i++) {
        if (mAllJobs[i]->time_arrival == 0) {
            mActiveJobs.emplace_back(mAllJobs[i]);
        } else {
            mNotArrivedJobs.emplace_back(mAllJobs[i]);
        }
    }
}

void Simulator::loadInitialJobs() {
    for (size_t i = 0; i < mActiveJobs.size(); i++) {
        addJob(mActiveJobs[i]);
    }
}

void Simulator::updateJobsIO() {
    for (size_t i = 0; i < mActiveJobs.size();) {
        auto &job = mActiveJobs[i];
        if (!job->io_max_runtime.has_value() ||
            !job->io_min_downtime.has_value()) {
            i++;
            continue;
        }
        if (job->io_current_runtime.value() >= job->io_max_runtime.value()) {
            job->io_current_runtime = 0;
            job->io_current_downtime = 0;
            remJob(job);
            mIOJobs.emplace_back(job);

            // Dont use &job anymore
            unstable_erase(mActiveJobs, i);
        } else {
            i++;
        }
    }

    for (size_t i = 0; i < mIOJobs.size();) {
        auto &job = mIOJobs[i];
        if (job->io_current_downtime.value() >= job->io_min_downtime.value()) {
            job->io_current_downtime.value() = 0;
            job->io_current_runtime.value() = 0;
            addJob(job);
            mActiveJobs.emplace_back(job);

            // Dont use &job anymore
            unstable_erase(mIOJobs, i);
        } else {
            job->io_current_downtime.value()++;
            i++;
        }
    }
}

void Simulator::updateJobsArrived() {
    if (mNotArrivedJobs.empty()) {
        return;
    }
    for (size_t i = 0; i < mNotArrivedJobs.size();) {
        auto &job = mNotArrivedJobs[i];
        if (tick >= job->time_arrival) {
            addJob(job);
            mActiveJobs.emplace_back(job);

            // Dont use &job anymore
            unstable_erase(mNotArrivedJobs, i);
        } else {
            i++;
        }
    }
}

void Simulator::updateCompleteJob(std::shared_ptr<Job> &j) {
    if (j->time_remaining <= 0) {
        remJob(j);
        mCompletedJobs.emplace_back(j);

        size_t i;
        bool found = false;
        for (i = 0; i < mActiveJobs.size(); i++) {
            if (mActiveJobs[i]->id == j->id) {
                found = true;
                break;
            }
        }
        if (found) {
            unstable_erase(mActiveJobs, i);
        }
    }
}

void Simulator::mainLoop() {
    while (mAllJobs.size() != mCompletedJobs.size()) {
        mScheduler->update(tick);
        updateJobsArrived();
        updateJobsIO();

        auto chosen_job = mScheduler->chooseJob();
        if (chosen_job) {
            chosen_job->time_remaining--;
            updateCompleteJob(chosen_job);
        }

        tick++;

#ifdef DEBUG
        if (chosen_job) {
            std::println("Chose job {} on tick {}. Remaining {} -> {}",
                         chosen_job->id, tick, chosen_job->time_remaining + 1,
                         chosen_job->time_remaining);
        } else {
            std::println("Chose no job");
        }
#endif
    }
#ifdef DEBUG
    std::println("Simulation complete!");
#endif
}

void Simulator::setup() {
    tick = 0;
    buildInitialState();
    loadInitialJobs();
    mScheduler->update(tick);
}

void Simulator::run() { mainLoop(); }

std::string Simulator::toString() const {
    return std::format(
        "Simulator on tick {} using:\n\t{}\n\tWith {} jobs:\n\t\tCompleted: "
        "{}\n\t\tActive: {}\n\t\tNot Arrived: {}\n\t\tSleeping: {}",
        tick, mScheduler->toString(), mAllJobs.size(), mCompletedJobs.size(),
        mActiveJobs.size(), mNotArrivedJobs.size(), mIOJobs.size());
}

void Simulator::print() const { std::cout << toString() << '\n'; }
