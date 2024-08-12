#pragma once
#include <unordered_map>
#include "sketch_defs.hpp"

namespace sketch {
    class real_dist {
    public:
        /// @brief Append a given item.
        /// @param id Item ID.
        /// @param value Item value.
        void append(u64 id, u64 value);

        /// @brief Return absolute rank of a given item.
        /// @param id Item ID.
        /// @param value Item value.
        /// @param inclusive If the item is included in the rank.
        u64 rank(u64 id, u64 value, bool inclusive = true) const;

        /// @brief Return normalized rank of a given item.
        /// @param id Item ID.
        /// @param value Item value.
        /// @param inclusive If the item is included in the rank.
        f64 nomRank(u64 id, u64 value, bool inclusive = true) const;

        /// @brief Return the quantile value of a given normalized rank.
        /// @param id Item ID.
        /// @param nom_rank Normalized rank.
        /// @param inclusive If the item is included in the rank.
        u64 quantile(u64 id, f64 nom_rank, bool inclusive = true) const;

        /// @brief Return the size of a given flow.
        /// @param id Flow ID.
        u64 size(u64 id) const;

        /// @brief Return the type of a given flow.
        /// @param id Flow ID.
        FlowType type(u64 id) const;

    private:
        std::unordered_map<u64, vec_u64> container;  ///< Container.
    };
}   // namespace sketch

#include "real_dist_impl.hpp"