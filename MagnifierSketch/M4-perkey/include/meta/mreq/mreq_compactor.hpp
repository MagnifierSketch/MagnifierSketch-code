#pragma once
#include "../../common/sketch_utils.hpp"

namespace sketch {
    class mReqCmtor {
    public:
        /// @brief Constructor.
        /// @param lg_w_ log2 of weight of the compactor.
        /// @param cap_ Capacity of the compactor.
        mReqCmtor(u64 lg_w_, u64 cap_);
        
        /// @brief Deleted default constructor.
        mReqCmtor() = delete;

        /// @brief Return if the compactor is full.
        bool full() const;
        /// @brief Return size of the compactor.
        u64 size() const;
        /// @brief Return capacity of the compactor.
        u64 capacity() const;
        /// @brief Return log2 of weight of the compactor.
        u64 lgWeight() const;
        /// @brief Return weight of the compactor.
        u64 weight() const;
        /// @brief Return number of bytes the compactor uses.
        u64 memory() const;

        /// @brief Return begin iterator.
        u64_const_iter begin() const;
        /// @brief Return end iterator.
        u64_const_iter end() const;
        /// @brief Return begin iterator.
        u64_iter begin();
        /// @brief Return end iterator.
        u64_iter end();

        /// @brief Append a given item into the compactor.
        /// @param item Item to be appended.
        void append(u64 item);

        /// @brief Compact the compactor into another compactor.
        /// @param next The next compactor which receives those
        ///             compacted items.
        void compact(mReqCmtor& next);

        /// @brief Estimate absolute rank of a given item.
        /// @param item Item to be ranked.
        /// @param inclusive If the given item is included in the rank.
        u64 rank(u64 item, bool inclusive = true) const;

        /// @brief Estimate weighted rank of a given item.
        /// @param item Item to be ranked.
        /// @param inclusive If the given item is included in the rank.
        u64 weightedRank(u64 item, bool inclusive = true) const;


    private:
        u64     lg_w;      ///< Log2 of the weight of the compactor.
        u64     cap;       ///< Capacity of the compactor.
        vec_u64 items;     ///< Items in the compactor.
    };
} // namespace sketch

#include "mreq_compactor_impl.hpp"