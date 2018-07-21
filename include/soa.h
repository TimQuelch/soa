#ifndef SOA_H
#define SOA_H

#include <tuple>
#include <utility>
#include <vector>

namespace soa {
    namespace {
        template <typename Func, std::size_t... Is>
        constexpr auto tuple_apply_impl(Func&& f, std::index_sequence<Is...>) {
            return f(std::integral_constant<std::size_t, Is>{}...);
        }
    } // namespace

    template <typename... Ts>
    class soa {
    public:
        static_assert(sizeof...(Ts) > 0);

        using value_type = std::tuple<Ts...>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = std::tuple<Ts&...>;
        using const_reference = std::tuple<const Ts&...>;
        using pointer = std::tuple<Ts*...>;
        using const_pointer = std::tuple<const Ts*...>;

        template <size_type I>
        auto& get() {
            return std::get<I>(vecs_);
        }

        template <size_type I>
        const auto& get() const {
            return std::get<I>(vecs_);
        }

        reference operator[](size_type i) {
            return tuple_apply(
                [this, i](auto... Is) { return std::tie(std::get<Is>(vecs_)[i]...); });
        }

        const_reference operator[](size_type i) const {
            return tuple_apply(
                [this, i](auto... Is) { return std::tie(std::get<Is>(vecs_)[i]...); });
        }

        void push_back(value_type val) {
            tuple_apply([this, &val](auto... Is) {
                (std::get<Is>(vecs_).push_back(std::get<Is>(val)), ...);
            });
        }

        size_type size() const { return std::get<0>(vecs_).size(); }
        bool empty() const { return size() == 0; }

        void reserve(size_type size) {
            tuple_apply([this, size](auto... Is) { (std::get<Is>(vecs_).reserve(size), ...); });
        }

        void clear() {
            tuple_apply([this](auto... Is) { (std::get<Is>(vecs_).clear(), ...); });
        }

        void pop_back() {
            tuple_apply([this](auto... Is) { (std::get<Is>(vecs_).pop_back(), ...); });
        }

    private:
        std::tuple<std::vector<Ts>...> vecs_;

        template <typename Func>
        static constexpr auto tuple_apply(Func&& f) {
            return tuple_apply_impl(std::forward<Func>(f), std::index_sequence_for<Ts...>{});
        }
    };
} // namespace soa

#endif
