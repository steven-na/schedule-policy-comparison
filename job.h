#pragma once

#include "common.h"

struct Job {
    int id{0};
    int time_remaining{0};
    std::optional<int> io_current_runtime{};
    std::optional<int> io_current_downtime{};
    const int time_arrival{0};
    const std::optional<int> io_max_runtime{};
    const std::optional<int> io_min_downtime{};
    const std::optional<int> ticket_count{};
    const std::optional<int> stride_length{};

    std::string toString() const {
        return std::format("Job{}(life:{}, arrival:{})", id, time_remaining,
                           time_arrival);
    }
};
