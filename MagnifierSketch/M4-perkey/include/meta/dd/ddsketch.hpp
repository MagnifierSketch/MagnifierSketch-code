#pragma once
#include "../../common/sketch_defs.hpp"
#include "../../common/histogram.hpp"

namespace sketch {
        template <typename META>
        class AndorSketch;
    class DDSketch {
        friend class AndorSketch<DDSketch>;
    public:
        /// @brief Default constructor.
        /// @warning Members are potential uninitialized after construction.
        ///          Make sure you know what you are doing.
        DDSketch() = default;

        /// @brief Constructor.
        /// @param cap_ Capacity, i.e. maximum number of items that
        ///             can be held in the DDSketch.
        /// @param alpha_ Argument for interval division.
        DDSketch(u32 cap_, f64 alpha_);

        /// @brief Destructor.
        ~DDSketch() = default;

        /// @brief Return the number of items in the DDSketch.
        u64 size() const;
        /// @brief Return the capacity of the DDSketch.
        u64 capacity() const;

        /// @brief Return whether the DDSketch is empty.
        bool empty() const;
        /// @brief Return whether the DDSketch is full.
        bool full() const;

        /// @brief Return the number of bytes the DDSketch uses.
        u64 memory() const;

        /// @brief Append an item to the DDSketch.
        /// @param item The item to append.
        void append(u64 item);

        /// @brief Estimate the quantile value of a given normalized rank.
        u64 quantile(f64 nom_rank) const;

        /// @brief Convert the DDSketch to a histogram.
        operator Histogram() const;

    private:
        vec_u64 counters;       ///< Counters.
        u64 totalSize = 0;      ///< Total number of items.
        u64 maxCnt = 0;         ///< Maximum counter.
        u32 cap;                ///< Capacity.
        f64 alpha;              ///< Argument for interval division.
        f64 gamma;              ///< gamma = (1 + alpha) / (1 - alpha).

        /// @brief Return the index of an item in @c counters.
        u64 pos(u64 item) const;

        /// @brief Append an item to given position.
        void append(u64 item, u64 pos);
    };
}   // namespace sketch

#include "ddsketch_impl.hpp"