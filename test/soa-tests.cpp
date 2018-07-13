#define BOOST_TEST_MODULE soa tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <type_traits>

#include "soa.h"

template <typename Func, std::size_t... Is>
constexpr auto tuple_apply(Func&& f, std::index_sequence<Is...>) {
    return f(std::integral_constant<std::size_t, Is>{}...);
}

template <typename... Ts>
std::ostream& operator<<(std::ostream os, std::tuple<Ts...> val) {
    os << "{ ";
    tuple_apply([&val](auto... Is) { ((std::cout << std::get<Is> << " "), ...); },
                std::index_sequence_for<Ts...>{});
    os << "}";
    return os;
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

    static_assert(std::is_same_v<decltype(s.get<0>()), std::vector<int>>);
    static_assert(std::is_same_v<decltype(s.get<1>()), std::vector<double>>);
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
