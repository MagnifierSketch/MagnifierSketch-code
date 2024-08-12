#pragma once
#include "mreq_compactor.hpp"
#include "../../common/sorted_view.hpp"
#include "../../common/histogram.hpp"

namespace sketch {
    class mReqSketch {
        using vec_cmtor = vector<mReqCmtor>;
    public:
        /// @brief Constructor.
        /// @param sketch_cap_ Capacity of the sketch. 
        /// @param cmtor_cap_ Capacity of each compactor.
        mReqSketch(u32 sketch_cap_, u32 cmtor_cap_);

        /// @brief Destructor.
        ~mReqSketch();

        /// @brief Default constructor.
        /// @warning Members are potential uninitialized after construction.
        ///          Make sure you know what you are doing.
        mReqSketch() = default;
        
        /// @brief Return size of the sketch.
        u32 size() const;
        /// @brief Return if the sketch is empty.
        bool empty() const;
        /// @brief Return if the sketch is full.
        bool full() const;
        /// @brief Return number of bytes the sketch uses.
        u32 memory() const;

        /// @brief Append a given item into the sketch.
        /// @param item Appended item.
        void append(u64 id,u64 item);

        /// @brief Estimate absolute rank of a given item.
        /// @param item Item to be ranked.
        /// @param inclusive If the item is included in the rank.
        u64 rank(u64 item, bool inclusive) const;
        
        /// @brief Estimate normalized rank of a given item.
        /// @param item Item to be ranked.
        /// @param inclusive If the item is included in the rank.
        f64 nomRank(u64 item, bool inclusive) const;

        /// @brief Estimate the quantile value of a given normalized rank.
        /// @param nom_rank Normalized rank.
        /// @param inclusive If the given rank is included.
        u64 quantile(f64 nom_rank, u64 id) const;

        /// @brief Convert the sketch into a histogram.
        operator Histogram() const;

    private:
        u32 itemNum;               ///< Number of items in the sketch.
        u32 sketchCap;             ///< Capacity of the sketch.
        vec_cmtor cmtors;          ///< Compactors.
        u64 minItem = UINT64_MAX;  ///< Minimum item in the sketch.
        u64 maxItem = 0;           ///< Maximum item in the sketch.
        
        SortedView setupSortedView() const;
    };
} // namespace sketch

#include "mreq_sketch_impl.hpp"