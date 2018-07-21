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
        using pointer = std::tuple<Ts*...>;
        using const_pointer = std::tuple<const Ts*...>;
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
                [this](auto... Is) { return iterator{pointer{vecs_[Is].begin()...}}; });
        }

        auto begin() const { return cbegin(); }

        auto cbegin() const {
            return tuple_apply(
                [this](auto... Is) { return const_iterator{pointer{vecs_[Is].begin()...}}; });
        }

        auto end() {
            return tuple_apply(
                [this](auto... Is) { return iterator{pointer{vecs_[Is].end()...}}; });
        }

        auto end() const { return cend(); }

        auto cend() const {
            return tuple_apply(
                [this](auto... Is) { return const_iterator{pointer{vecs_[Is].end()...}}; });
        }

    private:
        std::tuple<std::vector<Ts>...> vecs_;

        template <typename Func>
        static constexpr auto tuple_apply(Func&& f) {
            return tuple_apply_impl(std::forward<Func>(f), std::index_sequence_for<Ts...>{});
        }

        template <bool Const>
        class iterator_impl {
        public:
            using value_type = value_type;
            using difference_type = difference_type;
            using reference = typename std::conditional_t<Const, const_reference, reference>;
            using pointer = typename std::conditional_t<Const, const_pointer, pointer>;
            using iterator_category = std::random_access_iterator_tag;

            iterator_impl(pointer val) { ptrs_ = std::move(val); }

            template <typename std::enable_if_t<Const>>
            reference operator*() const {
                return soa<Ts...>::tuple_apply(
                    [this](auto... Is) { return reference{*std::get<Is>(ptrs_)...}; });
            }

            template <typename std::enable_if_t<!Const>>
            reference operator*() {
                return soa<Ts...>::tuple_apply(
                    [this](auto... Is) { return reference{*std::get<Is>(ptrs_)...}; });
            }

            iterator_impl<Const>& operator++() {
                soa<Ts...>::tuple_apply([this](auto... Is) { (++std::get<Is>(ptrs_), ...); });
                return *this;
            }

            iterator_impl<Const> operator++(int) {
                auto tmp = *this;
                operator++();
                return tmp;
            }

            iterator_impl<Const>& operator--() {
                soa<Ts...>::tuple_apply([this](auto... Is) { (--std::get<Is>(ptrs_), ...); });
                return *this;
            }

            iterator_impl<Const> operator--(int) {
                auto tmp = *this;
                operator--();
                return tmp;
            }

            iterator_impl<Const>& operator+=(difference_type n) {
                soa<Ts...>::tuple_apply(
                    [this, n](auto... Is) { ((void)(std::get<Is>(ptrs_) += n), ...); });
                return *this;
            }

            friend iterator_impl<Const> operator+(iterator_impl<Const> a, difference_type b) {
                return a += b;
            }

            iterator_impl<Const>& operator-=(difference_type n) {
                soa<Ts...>::tuple_apply(
                    [this, n](auto... Is) { ((void)(std::get<Is>(ptrs_) -= n), ...); });
                return *this;
            }

            friend iterator_impl<Const> operator-(iterator_impl<Const> a, difference_type b) {
                return a -= b;
            }

            friend difference_type operator-(iterator_impl<Const> a, iterator_impl<Const> b) {
                return std::get<0>(a.ptrs_) - std::get<0>(b.ptrs_);
            }

            friend bool operator==(iterator_impl<Const> a, iterator_impl<Const> b) {
                return std::get<0>(a.ptrs_) == std::get<0>(b.ptrs_);
            }

            friend bool operator!=(iterator_impl<Const> a, iterator_impl<Const> b) {
                return !(a == b);
            }

            friend bool operator<(iterator_impl<Const> a, iterator_impl<Const> b) {
                return std::get<0>(a.ptrs_) < std::get<0>(b.ptrs_);
            }

            friend bool operator<(iterator_impl<Const> a, iterator_impl<Const> b) {
                return std::get<0>(a.ptrs_) > std::get<0>(b.ptrs_);
            }

            friend bool operator<=(iterator_impl<Const> a, iterator_impl<Const> b) {
                return !(a > b);
            }

            friend bool operator>=(iterator_impl<Const> a, iterator_impl<Const> b) {
                return !(a < b);
            }

        private:
            pointer ptrs_ = {};
        };
    };
} // namespace soa

#endif
