#pragma once

#include "common.h"

template <typename T> void unstable_erase(std::vector<T> &v, size_t index) {
    std::swap(v[index], v.back());
    v.pop_back();
}
