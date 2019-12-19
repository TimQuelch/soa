#include <benchmark/benchmark.h>
#include <limits>
#include <random>

#include "soa.h"

constexpr auto min = -100;
constexpr auto max = 100;

constexpr auto minRange = 1 << 3;
constexpr auto maxRange = 1 << 15;

struct S {
    int one;
    int two;
    int three;
    int four;
    int five;
};

static void aos_accumulate(benchmark::State& state) {
    auto rng = std::mt19937{42};

    auto v = std::vector<S>{};
    auto d = std::uniform_int_distribution{min, max};
    std::generate_n(std::back_inserter(v), state.range(0), [&d, &rng]() {
        return S{d(rng), d(rng), d(rng), d(rng), d(rng)};
    });

    for (auto _ : state) {
        auto r = std::accumulate(
            v.begin(), v.end(), 0, [](const auto& a, const auto& b) { return a + b.one; });
        benchmark::DoNotOptimize(r);
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(int));
}
BENCHMARK(aos_accumulate)->Range(minRange, maxRange);

static void soa_accumulate(benchmark::State& state) {
    auto rng = std::mt19937{42};

    auto s = soa::soa<int, int, int, int, int>{};

    auto d = std::uniform_int_distribution{min, max};
    std::generate_n(std::back_inserter(s), state.range(0), [&d, &rng]() {
        return std::tuple{d(rng), d(rng), d(rng), d(rng), d(rng)};
    });

    for (auto _ : state) {
        auto r = std::accumulate(
            s.begin(), s.end(), 0, [](const auto& a, const auto& b) { return a + std::get<0>(b); });
        benchmark::DoNotOptimize(r);
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(int));
}
BENCHMARK(soa_accumulate)->Range(minRange, maxRange);

static void aos_sort(benchmark::State& state) {
    auto rng = std::mt19937{42};

    auto v = std::vector<S>{};
    auto d = std::uniform_int_distribution{min, max};
    std::generate_n(std::back_inserter(v), state.range(0), [&d, &rng]() {
        return S{d(rng), d(rng), d(rng), d(rng), d(rng)};
    });

    for (auto _ : state) {
        std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.one < b.one; });
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(int));
}
BENCHMARK(aos_sort)->Range(minRange, maxRange);

static void soa_sort(benchmark::State& state) {
    auto rng = std::mt19937{42};

    auto s = soa::soa<int, int, int, int, int>{};

    auto d = std::uniform_int_distribution{min, max};
    std::generate_n(std::back_inserter(s), state.range(0), [&d, &rng]() {
        return std::tuple{d(rng), d(rng), d(rng), d(rng), d(rng)};
    });

    for (auto _ : state) {
        std::sort(s.begin(), s.end(), [](const auto& a, const auto& b) {
            return std::get<0>(a) < std::get<0>(b);
        });
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(int));
}
BENCHMARK(soa_sort)->Range(minRange, maxRange);

BENCHMARK_MAIN();
