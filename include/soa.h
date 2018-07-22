#ifndef SOA_H
#define SOA_H

#include <tuple>
#include <utility>
#include <vector>

#include <boost/iterator/iterator_facade.hpp>

namespace soa {
    namespace {
        template <typename Func, std::size_t... Is>
        constexpr auto tuple_apply_impl(Func&& f, std::index_sequence<Is...>) {
            return f(std::integral_constant<std::size_t, Is>{}...);
        }
    } // namespace

    template <typename... Ts>
    class soa {
    private:
        template <bool Const>
        class iterator_impl;

    public:
        static_assert(sizeof...(Ts) > 0);

        using value_type = std::tuple<Ts...>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = std::tuple<Ts&...>;
        using const_reference = std::tuple<const Ts&...>;
        using iterator = iterator_impl<false>;
        using const_iterator = iterator_impl<true>;

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

        auto begin() {
            return tuple_apply(
                [this](auto... Is) { return iterator{std::get<Is>(vecs_).begin()...}; });
        }

        auto begin() const { return cbegin(); }

        auto cbegin() const {
            return tuple_apply(
                [this](auto... Is) { return const_iterator{std::get<Is>(vecs_).begin()...}; });
        }

        auto end() {
            return tuple_apply(
                [this](auto... Is) { return iterator{std::get<Is>(vecs_).end()...}; });
        }

        auto end() const { return cend(); }

        auto cend() const {
            return tuple_apply(
                [this](auto... Is) { return const_iterator{std::get<Is>(vecs_).end()...}; });
        }

    private:
        std::tuple<std::vector<Ts>...> vecs_;

        template <typename Func>
        static constexpr auto tuple_apply(Func&& f) {
            return tuple_apply_impl(std::forward<Func>(f), std::index_sequence_for<Ts...>{});
        }

        template <bool Const>
        class iterator_impl
            : public boost::iterator_facade<iterator_impl<Const>,
                                            value_type,
                                            boost::random_access_traversal_tag,
                                            std::conditional_t<Const, const_reference, reference>,
                                            difference_type> {
        public:
            using vec_iterators =
                std::conditional_t<Const,
                                   std::tuple<typename std::vector<Ts>::const_iterator...>,
                                   std::tuple<typename std::vector<Ts>::iterator...>>;

            using reference =
                std::conditional_t<Const, soa<Ts...>::const_reference, soa<Ts...>::reference>;

            template <bool Const_ = Const, typename = std::enable_if_t<Const_>>
            iterator_impl(typename std::vector<Ts>::const_iterator... its)
                : its_{std::move(its)...} {}

            template <bool Const_ = Const, typename = std::enable_if_t<!Const_>>
            iterator_impl(typename std::vector<Ts>::iterator... its)
                : its_{std::move(its)...} {}

            template <bool Const_>
            iterator_impl(iterator_impl<Const_> other)
                : its_{std::move(other.its_)} {}

            iterator_impl() = default;

        private:
            reference dereference() const {
                return soa<Ts...>::tuple_apply(
                    [this](auto... Is) { return reference{*std::get<Is>(its_)...}; });
            }

            void increment() {
                soa<Ts...>::tuple_apply([this](auto... Is) { (++std::get<Is>(its_), ...); });
            }

            void decrement() {
                soa<Ts...>::tuple_apply([this](auto... Is) { (--std::get<Is>(its_), ...); });
            }

            void advance(difference_type n) {
                soa<Ts...>::tuple_apply(
                    [this, n](auto... Is) { (static_cast<void>(std::get<Is>(its_) += n), ...); });
            }

            template <bool Const_>
            difference_type distance_to(const iterator_impl<Const_>& other) const {
                return std::get<0>(other.its_) - std::get<0>(its_);
            }

            template <bool Const_>
            bool equal(const iterator_impl<Const_>& other) const {
                return std::get<0>(its_) == std::get<0>(other.its_);
            }

            vec_iterators its_ = {};
        };
    };
} // namespace soa

#endif
