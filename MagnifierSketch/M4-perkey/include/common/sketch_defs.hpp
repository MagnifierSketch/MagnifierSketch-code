#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdint>

namespace sketch {
    using std::cout, std::cerr, std::endl;
    using std::vector, std::string;

    using namespace std::chrono;

    using u8 = uint8_t;
    using i32 = int32_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    using f64 = double;
    using vec_f64 = vector<f64>;
    using vec_u32 = vector<u32>;
    using vec_u64 = vector<u64>;
    using u64_const_iter = vec_u64::const_iterator;
    using u64_iter = vec_u64::iterator;

#define UNUSED(x) (void(x))

    enum FlowType {
        TINY = 1,
        MID = 2,
        HUGE = 4,
    };


    struct FlowItem {
        u64 id;
        u64 value;
    };

    // SketchModel is defined as enum not enum class
    // to allow for implicit conversion from/to u64.
    enum SketchModel {
        ANDOR,
        DLEFT,
        CUCKOO,
        NUM_MODELS,
    };

    enum MetaModel {
        DD,
        MREQ,
        TD,
        NUM_METAS,
    };
}   // namespace sketch