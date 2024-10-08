#pragma once
#include "sketch_utils.hpp"
#include "histogram.hpp"

namespace sketch {
    class SortedView {
    public:
        /// @brief Weighted item.
        struct witem {
            u64 value;      ///< Value.
            u64 weight;     ///< Weight.

            /// @brief Compare by value.
            static bool value_less(const witem& a, const witem& b) {
                return a.value < b.value;
            }

            /// @brief Compare by weight.
            static bool weight_less(const witem& a, const witem& b) {
                return a.weight < b.weight;
            }
        };

        using witem_iter = vector<witem>::iterator;
        using witem_const_iter = vector<witem>::const_iterator;
        using vec_witem = vector<witem>;

    public:
        /// @brief Constructor.
        /// @param num Number of items in the sketch.
        SortedView(u64 num);

        /// @brief Deleted default constructor.
        SortedView() = delete;

        /// @brief Insert items in [first, last) into the sorted view.
        /// @param first First iterator.
        /// @param last Last iterator.
        /// @param weight Weight of each inserted item.
        void insert(u64_const_iter first, u64_const_iter last, 
                           u64 weight);

        /// @brief Insert items in [first, last) into the sorted view.
        /// @param first First iterator.
        /// @param last Last iterator.       
        void insert(witem_const_iter first, witem_const_iter last);

        /// @brief Insert a single item into the sorted view.
        /// @param value Item value.
        /// @param weight Item weight.
        void insert(u64 value, u64 weight);

        /// @brief Convert the sorted view to a cumulative view.
        void convertToCumulative();

        /// @brief Estimate absolute rank of a given item.
        /// @param item Item to be ranked. 
        /// @param inclusive If the item is included in the rank.
        u64 rank(u64 item, bool inclusive = true) const;
        
        /// @brief Estimate normalized rank of a given item.
        /// @param item Item to be ranked.
        /// @param inclusive If the item is included in the rank.
        f64 nomRank(u64 item, bool inclusive = true) const;

        /// @brief Estimate the quantile value of a given normalized rank.
        /// @param nom_rank Normalized rank.
        /// @param inclusive If the given rank is included.
        u64 quantile(f64 nom_rank, bool inclusive = true) const;

        /// @brief Convert the sorted view to a histogram.
        operator Histogram() const;

    private:
        vector<witem> view;     ///< Sorted view.
        u64 totalWeight;       ///< Total weight.
    };
} // namespace sketch

#include "sorted_view_impl.hpp"