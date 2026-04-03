#include "JobQueue.h"

void JobQueue::enqueue(const std::shared_ptr<Job> &job) {
    std::shared_ptr<QueueItem> new_item = std::make_shared<QueueItem>();
    new_item->job = job;

    if (!tail) {
        head = new_item;
        tail = new_item;
        return;
    }

    new_item->prev = tail;
    tail->next = new_item;
    tail = new_item;
}

void JobQueue::dequeue(const std::shared_ptr<Job> &job) {
    if (!tail) {
        return;
    }

    std::shared_ptr<QueueItem> test_against = tail;
    while (test_against) {
        if (test_against->job->id == job->id) {
            test_against->next->prev = test_against->prev;
            test_against->prev->next = test_against->next;
            return;
        }
        test_against = test_against->prev;
    }
}

void JobQueue::enqueueWeighted(
    const std::shared_ptr<Job> &job,
    std::function<bool(std::shared_ptr<Job> &)> test_func) {

    if (!tail) {
        enqueue(job);
        return;
    }

    std::shared_ptr<QueueItem> new_item = std::make_shared<QueueItem>();
    new_item->job = job;

    std::shared_ptr<QueueItem> test_against = tail;
    while (test_against && test_func(test_against->job)) {
        test_against = test_against->prev;
    }
    if (!test_against) { // Head insert
        new_item->next = head;
        head->prev = new_item;
        head = new_item;
    } else if (test_against == tail) { // Tail insert
        new_item->prev = tail;
        tail->next = new_item;
        tail = new_item;
    } else { // Middle insert
        new_item->prev = test_against->prev;
        new_item->next = test_against;
        test_against->prev->next = new_item;
        test_against->prev = new_item;
    }
}

bool JobQueue::itemExists(const std::shared_ptr<Job> &job) {
    if (!tail) {
        return false;
    }

    std::shared_ptr<QueueItem> test_against = tail;
    while (test_against) {
        if (test_against->job->id == job->id) {
            return true;
        }
        test_against = test_against->prev;
    }
    return false;
}

std::shared_ptr<Job> JobQueue::poll() {
    if (!head) {
        return nullptr;
    }
    std::shared_ptr<Job> out{head->job};
    std::shared_ptr<QueueItem> next{head->next};

    head = next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    return out;
}

std::shared_ptr<Job> JobQueue::peek() {
    std::shared_ptr<Job> out{};
    if (head) {
        out = head->job;
    }
    return out;
}

std::string JobQueue::toString() const {
    std::string out{"JobQueue("};
    if (head) {
        out += std::format("Head: id {}, {}/", head->job->id,
                           head->job->time_remaining);
    } else {
        out += "No head/";
    }
    if (tail) {
        out += std::format("Tail: id {}, {})", tail->job->id,
                           tail->job->time_remaining);
    } else {
        out += "No tail)";
    }
    return out;
}
