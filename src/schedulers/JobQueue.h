#pragma once

#include "../common.h"

#include "../job.h"

class JobQueue {
    struct QueueItem {
        std::shared_ptr<Job> job{};
        std::shared_ptr<QueueItem> next{nullptr};
        std::shared_ptr<QueueItem> prev{nullptr};
    };
    std::shared_ptr<QueueItem> head{nullptr};
    std::shared_ptr<QueueItem> tail{nullptr};

  public:
    void enqueue(const std::shared_ptr<Job> &job);
    void dequeue(const std::shared_ptr<Job> &job);
    /// Enqueue job in queue, starting at tail and placing at the first spot
    /// that test_func returns false
    void enqueueWeighted(const std::shared_ptr<Job> &job,
                         std::function<bool(std::shared_ptr<Job> &)> test_func);
    bool itemExists(const std::shared_ptr<Job> &job);
    std::shared_ptr<Job> poll();
    std::shared_ptr<Job> peek();

    std::string toString() const;
};
