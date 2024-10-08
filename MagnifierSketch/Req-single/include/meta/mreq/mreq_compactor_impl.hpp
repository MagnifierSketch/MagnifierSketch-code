#pragma once
#include "mreq_compactor.hpp"
#include <algorithm>
#include "../../common/vec_ops.hpp"

namespace sketch {
    mReqCmtor::mReqCmtor(u64 lg_w_, u64 cap_)
        : lg_w(lg_w_), cap(cap_) {
        items.reserve(cap);
    }

    bool mReqCmtor::full() const {
        return size() >= cap;
    }

    u64 mReqCmtor::size() const {
        return items.size();
    }

    u64 mReqCmtor::capacity() const {
        return cap;
    }

    u64 mReqCmtor::lgWeight() const {
        return lg_w;
    }

    u64 mReqCmtor::weight() const {
        return 1U << lgWeight();
    }

    u64 mReqCmtor::memory() const {
        return sizeof(u32) * capacity();
    }

    u64_const_iter mReqCmtor::begin() const {
        return items.begin();
    }

    u64_const_iter mReqCmtor::end() const {
        return items.end();
    }

    u64_iter mReqCmtor::begin() {
        return items.begin();
    }

    u64_iter mReqCmtor::end() {
        return items.end();
    }

    void mReqCmtor::append(u64 item) {
        if (full()) {
            throw std::logic_error("append to a full compactor");
        }
        vec_insert_ordered(items, item);
    }

    void mReqCmtor::compact(mReqCmtor& next) {
        if (!full()) {
            throw std::logic_error("compact a non-full compactor");
        }

        // output coin, coin+2, coin+4, ... into next compactor
        bool coin = rand_bit();
        for (u32 i = coin; i < size(); i += 2) {
            vec_insert_ordered(next.items, items[i]);
        }

        // clear this compactor
        items.clear();
    }

    u64 mReqCmtor::rank(u64 item, bool inclusive) const {
        return vec_rank(items, item, inclusive);
    }

    u64 mReqCmtor::weightedRank(u64 item, bool inclusive) const {
        return rank(item, inclusive) * weight();
    }
} // namespace sketch