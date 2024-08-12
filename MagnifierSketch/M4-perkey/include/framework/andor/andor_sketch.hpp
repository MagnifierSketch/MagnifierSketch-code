#pragma once
#include "../../common/BOBHash32.h"
#include "../../common/tiny_counter.hpp"
#include "../../common/histogram.hpp"
#include "../framework.hpp"

namespace sketch {
    template <typename META>
    class SketchSingleTest;

    template <typename META>
    class AndorSketch : public Framework {
        using vec_tiny = std::vector<TinyCnter>;
        using vec_meta = std::vector<META>;

    public:
        /// @brief Constructor.
        /// @param mem_limit Memory limit in bytes.
        /// @param hash_num Number of hash functions per level, by default 2.
        /// @param seed Seed for generating hash functions, by default 0.
        AndorSketch(u32 mem_limit, u32 hash_num = 2, u32 seed = 0);

        void append(u64 id, u64 value) override;
        u64 size(u64 id) const override;
        u64 memory() const override;
        u64 quantile(u64 id, f64 nom_rank) const override;
        FlowType type(u64 id) const override;

    private:
        // MetaModel metaType; ///< Meta sketch type.
        static constexpr u64 LEVELS = 4;      ///< Number of levels.

        static constexpr u32 cap[4] = {3, UINT8_MAX, UINT16_MAX, UINT32_MAX};
        static constexpr f64 alpha[4] = {0, 0.5, 0.5, 0.3};
        static constexpr u32 cmtor_cap[4] = {0, 2, 2, 4};
        static constexpr u32 td_cap[4] = {0, 4, 8, 16};
        static constexpr f64 mem_div[4] = {0.03, 0.60, 0.35, 0.02};

        vec_tiny lv0;   ///< Level 0.
        vec_meta lv1;   ///< Level 1.
        vec_meta lv2;   ///< Level 2.
        vec_meta lv3;   ///< Level 3.
        std::vector<BOBHash32> hash[LEVELS];    ///< Hash functions.
        mutable vec_u64 hashVal[LEVELS];        ///< Hash values.

        /// @brief Calculate hash values for a given item.
        void calcHash(u64 id) const;

        // Level granularity functions.

        /// @brief Append a given item into lv0 (tiny counter level).
        void appendTiny(u64 id, u64 value);
        /// @brief Append a given item into lv1, 2, or 3 (dd level).
        void appendMETA(u64 level, u64 id, u64 value);

        /// @brief Estimate absolute rank in a given dd level.
        u64 rank(u64 level, u64 id, u64 value, bool inclusive) const;

        /// @brief Calculate the appending level of a given flow.
        u64 calcAppendLevel(u64 id) const;
        /// @brief Calculate the query level of a given flow.
        u64 calcQueryLevel(u64 id) const;

        Histogram doAND(u64 level, u64 id) const;
        Histogram doOR(u64 id) const;

        // Little helper functions.

        /// @brief Get container by level.
        vec_meta& getVecMETA(u64 level);
        /// @brief Get container by level.
        const vec_meta& getVecMETA(u64 level) const;

        /// @brief Check if buckets of a given flow are all full
        ///        in a given level.
        bool isAllFull(u64 level, u64 id) const;

        bool hasAnyFull(u64 level, u64 id) const;

        /// @brief Check if any bucket of a given flow is empty
        ///        in a given level.
        bool hasAnyEmpty(u64 level, u64 id) const;
    };
}   // namespace sketch

#include "andor_sketch_impl.hpp"