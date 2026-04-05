#include "JobQueue.h"

void JobQueue::enqueue(const std::shared_ptr<Job> &job) {
    assert(job && "Can't pass 'nullptr' into JobQueue::enqueue");
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
    assert(job && "Can't pass 'nullptr' into JobQueue::dequeue");
    if (!tail) {
        return;
    }

    std::shared_ptr<QueueItem> test_against = tail;
    while (test_against) {
        if (test_against->job->id == job->id) {
            bool eq_head = test_against == head;
            bool eq_tail = test_against == tail;
            if (eq_head && eq_tail) {
                head = nullptr;
                tail = nullptr;
                return;
            }
            if (eq_tail) {
                tail = test_against->prev;
                tail->next = nullptr;
            } else if (eq_head) {
                head = test_against->next;
                head->prev = nullptr;
            } else {
                test_against->prev->next = test_against->next;
                test_against->next->prev = test_against->prev;
            }
            return;
        }
        test_against = test_against->prev;
    }
}

void JobQueue::enqueueWeighted(
    const std::shared_ptr<Job> &job,
    std::function<bool(std::shared_ptr<Job> &)> test_func) {
    assert(job && "Can't pass 'nullptr' into JobQueue::enqueueWeighted");

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
        new_item->prev = test_against;
        new_item->next = test_against->next;
        test_against->next->prev = new_item;
        test_against->next = new_item;
    }
}

bool JobQueue::itemExists(const std::shared_ptr<Job> &job) {
    assert(job && "Can't pass 'nullptr' into JobQueue::itemExists");
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

std::shared_ptr<Job> JobQueue::peek() const {
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
