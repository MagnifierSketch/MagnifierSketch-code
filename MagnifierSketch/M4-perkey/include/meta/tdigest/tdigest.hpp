#pragma once
#include "centroid.hpp"
#include <utility>
#include "../../common/histogram.hpp"

namespace sketch {
    class TDigest {
    public:
        /// @brief Default constructor.
        /// @warning Members are potential uninitialized after construction.
        ///          Make sure you know what you are doing.
        TDigest() = default;

        /// @brief Constructor.
        /// @param cap_ Capacity, i.e. maximum number of items that
        ///             can be held in the t-digest.
        /// @param delta_ Argument for compression.
        TDigest(u32 cap_, u32 delta_);

        /// @brief Destructor.
        ~TDigest() = default;

        /// @brief Return the number of items in the t-digest.
        u64 size() const;

        /// @brief Return whether the t-digest is empty.
        bool empty() const;

        /// @brief Return whether the t-digest is full.
        bool full() const;

        /// @brief Return the number of bytes the t-digest uses.
        u64 memory() const;

        /// @brief Append an item to the t-digest.
        /// @param item The item to append.
        void append(u64 item);

        /// @brief Estimate the quantile value of a normalized rank.
        /// @param nom_rank Normalized rank.
        u64 quantile(f64 nom_rank) const;

        /// @brief Convert the t-digest to a histogram.
        operator Histogram() const;
        
    private:
        vector<Centroid> centroids; ///< Centroids in the t-digest.
        u64 totalWeight;            ///< Total weight.
        u32 cap;                    ///< Capacity.
        u32 DELTA;                  ///< Argument delta, logically const.

        u64 min_item = UINT64_MAX;  ///< Minimum item value.
        u64 max_item = 0;           ///< Maximum item value.
        u64 max_weight = 0;

        /// @brief Calculate scale function of a percentage.
        /// @param p Percentage.
        f64 scale(f64 p) const;

        /// @brief Calculate the quantile bounds of a centroid.
        /// @param c The centroid, must be an element in @c centroids.
        /// @return A @c std::pair<f64, f64>, whose first key is the
        ///         centroid's q_{left} and second key is its q_{right}.
        std::pair<f64, f64> qBound(const Centroid& c) const;

        /// @brief Calculate the k-size of a centroid.
        /// @param c The centroid, must be an element, in @c centroids.
        f64 kSize(const Centroid& c) const;
    
        void compressNearest();

        /// @brief Find the appending position of a new item.
        /// @param item The item to append.
        /// @return Pointer to a centroid if found appropriate position,
        ///         @c nullptr otherwise.
        Centroid* findAppendPos(u64 item);

        /// @brief Check whether a centroid can be appended to, i.e.
        ///        its k-size still no more than 1 after the appending.
        /// @param c The centroid to be appended to, 
        ///          must be an element in @c centroids.
        bool appendable(const Centroid& c) const;
    };
}   // namespace sketch

#include "tdigest_impl.hpp"