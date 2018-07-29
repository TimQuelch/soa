#define BOOST_TEST_MODULE soa tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>

#include "soa.h"

using tuple_log1 = std::tuple<int&, double&>;
using tuple_log2 = std::tuple<int, double>;
using soa_it_log1 = soa::soa<int, double>::iterator;
using soa_it_log2 = soa::soa<int, double>::const_iterator;
BOOST_TEST_DONT_PRINT_LOG_VALUE(tuple_log1);
BOOST_TEST_DONT_PRINT_LOG_VALUE(tuple_log2);
BOOST_TEST_DONT_PRINT_LOG_VALUE(soa_it_log1);
BOOST_TEST_DONT_PRINT_LOG_VALUE(soa_it_log2);

constexpr auto numValues = 5;

auto create_soa(int numValues) {
    auto s = soa::soa<int, double>{};
    for (auto i = 0; i < numValues; i++) {
        s.push_back({static_cast<int>(i), static_cast<double>(i)});
    }
    return s;
}

BOOST_AUTO_TEST_CASE(default_constructor) {
    auto i = soa::soa<int>{};
    auto d = soa::soa<double>{};
    auto id = soa::soa<int, double>{};
    auto ids = soa::soa<int, double, std::string>{};

    BOOST_TEST(i.size() == 0);
    BOOST_TEST(d.size() == 0);
    BOOST_TEST(id.size() == 0);
    BOOST_TEST(ids.size() == 0);
    BOOST_TEST(i.empty());
    BOOST_TEST(d.empty());
    BOOST_TEST(id.empty());
    BOOST_TEST(ids.empty());
}

BOOST_AUTO_TEST_CASE(const_default_constructor) {
    const auto ic = soa::soa<int>{};
    const auto dc = soa::soa<double>{};
    const auto idc = soa::soa<int, double>{};
    const auto idsc = soa::soa<int, double, std::string>{};

    BOOST_TEST(ic.size() == 0);
    BOOST_TEST(dc.size() == 0);
    BOOST_TEST(idc.size() == 0);
    BOOST_TEST(idsc.size() == 0);
    BOOST_TEST(ic.empty());
    BOOST_TEST(dc.empty());
    BOOST_TEST(idc.empty());
    BOOST_TEST(idsc.empty());
}

BOOST_AUTO_TEST_CASE(size_of) {
    auto i = soa::soa<int>{};
    auto d = soa::soa<double>{};
    auto id = soa::soa<int, double>{};

    BOOST_TEST(sizeof(i) == sizeof(std::vector<int>));
    BOOST_TEST(sizeof(d) == sizeof(std::vector<double>));
    BOOST_TEST(sizeof(id) == sizeof(std::vector<double>) + sizeof(std::vector<int>));
}

BOOST_AUTO_TEST_CASE(get) {
    auto s = soa::soa<int, double>{};
    const auto cs = soa::soa<int, double>{};
    auto v1 = s.get<0>();
    auto v2 = s.get<1>();

    static_assert(std::is_same_v<decltype(s.get<0>()), std::vector<int>&>);
    static_assert(std::is_same_v<decltype(s.get<1>()), std::vector<double>&>);
    static_assert(std::is_same_v<decltype(cs.get<0>()), const std::vector<int>&>);
    static_assert(std::is_same_v<decltype(cs.get<1>()), const std::vector<double>&>);

    BOOST_TEST(s.size() == 0);
    BOOST_TEST(v1.size() == 0);
    BOOST_TEST(v2.size() == 0);

    s.push_back({1, 1.2});
    s.push_back({3, 4.5});

    auto v3 = s.get<0>();
    auto v4 = s.get<1>();

    BOOST_TEST(s.size() == 2);
    BOOST_TEST(v3.size() == 2);
    BOOST_TEST(v4.size() == 2);

    BOOST_TEST(v3[0] == 1);
    BOOST_TEST(v3[1] == 3);
    BOOST_TEST(v4[0] == 1.2);
    BOOST_TEST(v4[1] == 4.5);
}

BOOST_AUTO_TEST_CASE(push_back) {
    auto s = soa::soa<int, double>{};

    BOOST_TEST(s.size() == 0);
    BOOST_TEST(s.empty());
    s.push_back({1, 7.0});
    BOOST_TEST(s.size() == 1);
    BOOST_TEST(!s.empty());
    s.push_back({8, 7.8});
    BOOST_TEST(s.size() == 2);
    BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(array_access) {
    auto s = soa::soa<int, double>{};

    s.push_back({1, 2.3});
    s.push_back({3, 4.5});
    s.push_back({6, 7.8});

    BOOST_TEST(s[0] == std::tuple(1, 2.3));
    BOOST_TEST(s[1] == std::tuple(3, 4.5));
    BOOST_TEST(s[2] == std::tuple(6, 7.8));
}

BOOST_AUTO_TEST_CASE(basic_iterators) {
    auto s = soa::soa<int, double>{};

    auto b = s.begin();
    auto e = s.end();

    static_assert(std::is_same_v<decltype(s.begin()), soa::soa<int, double>::iterator>);
    static_assert(std::is_same_v<decltype(s.end()), soa::soa<int, double>::iterator>);

    BOOST_TEST(b == e);
    BOOST_TEST(b == s.begin());
    BOOST_TEST(e == s.end());
}

BOOST_AUTO_TEST_CASE(const_basic_iterators) {
    auto s = soa::soa<int, double>{};
    auto b = s.begin();
    auto e = s.end();

    static_assert(std::is_same_v<decltype(s.cbegin()), soa::soa<int, double>::const_iterator>);
    static_assert(std::is_same_v<decltype(s.cend()), soa::soa<int, double>::const_iterator>);

    const auto c = s;
    auto cb = c.begin();
    auto ce = c.end();

    static_assert(std::is_same_v<decltype(c.begin()), soa::soa<int, double>::const_iterator>);
    static_assert(std::is_same_v<decltype(c.end()), soa::soa<int, double>::const_iterator>);
    static_assert(std::is_same_v<decltype(c.cbegin()), soa::soa<int, double>::const_iterator>);
    static_assert(std::is_same_v<decltype(c.cend()), soa::soa<int, double>::const_iterator>);

    BOOST_TEST(b == cb);
    BOOST_TEST(e == ce);
}

BOOST_AUTO_TEST_CASE(iterator_comparison) {
    auto s = create_soa(numValues);

    BOOST_TEST(s.begin() != s.end());
    BOOST_TEST(s.begin() < s.end());
    BOOST_TEST(s.begin() <= s.end());
    BOOST_TEST(!(s.begin() > s.end()));
    BOOST_TEST(!(s.begin() >= s.end()));
    BOOST_TEST(s.begin() <= s.begin());
    BOOST_TEST(s.begin() >= s.begin());

    BOOST_TEST(s.begin() - s.end() == -5);
    BOOST_TEST(s.end() - s.begin() == 5);
}

BOOST_AUTO_TEST_CASE(iterator_increment) {
    auto s = create_soa(numValues);

    auto it = s.begin();
    for (auto i = 0; i < (numValues - 1); i++) {
        BOOST_TEST(++it < s.end());
    }
    BOOST_TEST(++it == s.end());
}

BOOST_AUTO_TEST_CASE(iterator_decrement) {
    auto s = create_soa(numValues);

    auto it = s.end();
    for (auto i = 0; i < (numValues - 1); i++) {
        BOOST_TEST(--it > s.begin());
    }
    BOOST_TEST(--it == s.begin());
}

BOOST_AUTO_TEST_CASE(iterator_advance) {
    auto s = create_soa(numValues);
    BOOST_TEST(s.begin() + numValues == s.end());
    BOOST_TEST(s.end() - numValues == s.begin());
}

BOOST_AUTO_TEST_CASE(iterator_dereference) {
    auto s = create_soa(numValues);

    auto it = s.begin();
    for (auto i = 0; i < (numValues - 1); i++) {
        BOOST_TEST(*(it++) == s[i]);
    }

    it = s.begin() + 2;
    static_assert(std::is_same_v<decltype(*it), std::tuple<int&, double&>>);
    *it = std::tuple{10, 10.0};

    BOOST_TEST(s[2] == std::tuple(10, 10.0));
}

BOOST_AUTO_TEST_CASE(range_for) {
    auto s = create_soa(numValues);

    auto i = 0;
    for (auto val : s) {
        BOOST_TEST(val == std::tuple(static_cast<int>(i), static_cast<double>(i)));
        i++;
    }
    i = 0;
    for (const auto& val : s) {
        BOOST_TEST(val == std::tuple(static_cast<int>(i), static_cast<double>(i)));
        i++;
    }
}

BOOST_AUTO_TEST_CASE(transform) {
    auto s = create_soa(numValues);
    auto v = std::vector<int>{};

    std::transform(s.begin(), s.end(), std::back_inserter(v), [](const auto& val) {
        return std::get<0>(val);
    });

    BOOST_TEST(v == s.get<0>());
}

BOOST_AUTO_TEST_CASE(accumulate) {
    auto s = create_soa(numValues);

    auto r =
        std::accumulate(s.begin(), s.end(), std::tuple{0, 0.0}, [](const auto& p, const auto& n) {
            return std::tuple{std::get<0>(p) + std::get<0>(n), std::get<1>(p) + std::get<1>(n)};
        });

    const auto& v0 = s.get<0>();
    const auto& v1 = s.get<1>();
    BOOST_TEST(std::get<0>(r) == std::accumulate(v0.begin(), v0.end(), 0));
    BOOST_TEST(std::get<1>(r) == std::accumulate(v1.begin(), v1.end(), 0));
}
