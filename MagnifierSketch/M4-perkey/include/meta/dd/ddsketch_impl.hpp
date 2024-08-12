#pragma once
#include "ddsketch.hpp"
#include <stdexcept>
#include <cmath>
#include <algorithm>

namespace sketch {
    DDSketch::DDSketch(u32 cap_, f64 alpha_)
        : cap(cap_), alpha(alpha_),
          gamma((1.0 + alpha) / (1.0 - alpha)) {
        if (alpha <= 0.0 || alpha >= 1.0) {
            throw std::invalid_argument("alpha must be in (0, 1)");
        }

        u32 num = std::ceil(std::log2(1e9) / std::log2(gamma)) + 1;
        counters = vec_u32(num, 0);
    }

    u64 DDSketch::size() const {
        return totalSize;
    }

    u64 DDSketch::capacity() const {
        return cap;
    }

    bool DDSketch::empty() const {
        return size() == 0;
    }

    bool DDSketch::full() const {
        return maxCnt >= cap;
    }

    u64 DDSketch::memory() const {
        u64 counter_bits = std::ceil(std::log2(static_cast<f64>(cap) + 1));
        return (counter_bits * counters.size() + 7) / 8;
    }

    u64 DDSketch::pos(u64 item) const {
        return std::ceil(std::log2(item) / std::log2(gamma));
    }

    void DDSketch::append(u64 item) {
        u64 idx = pos(item);
        if (counters[idx] >= cap) {
            throw std::runtime_error("append to a full DDSketch");
        }
        append(item, idx);
    }

    void DDSketch::append(u64 item, u64 pos) {
        ++counters[pos];
        ++totalSize;
        maxCnt = std::max(maxCnt, counters[pos]);
    }

    u64 DDSketch::quantile(f64 nom_rank) const {
        if (nom_rank < 0.0 || nom_rank > 1.0) {
            throw std::invalid_argument("normalized rank out of range");
        }

        u64 rank = nom_rank * (totalSize - 1);
        u64 idx = 0;

        for (u32 sum = counters[0]; sum <= rank; sum += counters[++idx]);

        f64 res = idx == 0 ? 1 : 2 * std::pow(gamma, idx) / (gamma + 1);
        return std::lrint(res);
    }

    DDSketch::operator Histogram() const {
        u64 sz = counters.size();
        vec_f64 split = vec_f64(sz + 1, 0);
        vec_u64 height = vec_u64(sz, 0);

        for (u32 i = 0; i < counters.size(); ++i) {
            split[i + 1] = std::pow(gamma, i);
            height[i] = counters[i];
        }
        return Histogram(split, height);
    }
}   // namespace sketch
