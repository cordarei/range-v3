// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_UTILITY_INVOKABLE_HPP
#define RANGES_V3_UTILITY_INVOKABLE_HPP

#include <utility>
#include <functional>
#include <type_traits>
#include <range/v3/range_fwd.hpp>
#include <range/v3/utility/concepts.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/semiregular.hpp>

namespace ranges
{
    inline namespace v3
    {
        struct make_invokable_fn
        {
            template<typename R, typename T>
            auto operator()(R T::* p) const -> decltype(std::mem_fn(p))
            {
                return std::mem_fn(p);
            }

            template<typename T, typename U = detail::decay_t<T>>
            auto operator()(T && t) const -> enable_if_t<!std::is_member_pointer<U>::value, T>
            {
                return std::forward<T>(t);
            }
        };

        // RANGES_CONSTEXPR make_invokable_fn invokable {};
        namespace
        {
          constexpr auto const & invokable = odr_detail::static_const<make_invokable_fn>::value;
        }

        template<typename T>
        using invokable_t = decltype(invokable(std::declval<T>()));

        template<typename Fun>
        using semiregular_invokable_t =
            detail::conditional_t<
                (bool) SemiRegular<invokable_t<Fun>>(),
                invokable_t<Fun>,
                semiregular<invokable_t<Fun>>>;

        template<typename Fun>
        using semiregular_invokable_ref_t =
            detail::conditional_t<
                (bool) SemiRegular<invokable_t<Fun>>(),
                invokable_t<Fun>,
                reference_wrapper<semiregular<invokable_t<Fun>> const>>;

        namespace concepts
        {
            struct Invokable
            {
                template<typename Fun, typename...Args>
                using result_t = Function::result_t<invokable_t<Fun>, Args...>;

                template<typename Fun, typename...Args>
                auto requires_(Fun &&fun, Args &&...args) -> decltype(
                    concepts::valid_expr(
                        concepts::model_of<Function>(invokable((Fun &&) fun), (Args &&) args...)
                    ));
            };

            struct RegularInvokable
              : refines<Invokable>
            {};

            struct InvokablePredicate
              : refines<RegularInvokable>
            {
                template<typename Fun, typename...Args>
                auto requires_(Fun &&fun, Args &&...args) -> decltype(
                    concepts::valid_expr(
                        concepts::model_of<Predicate>(invokable((Fun &&) fun), (Args &&) args...)
                    ));
            };

            struct InvokableRelation
              : refines<InvokablePredicate>
            {
                template<typename Fun, typename T, typename U>
                auto requires_(Fun &&fun, T && t, U && u) -> decltype(
                    concepts::valid_expr(
                        concepts::model_of<Relation>(invokable((Fun &&) fun), (T &&) t, (U &&) u)
                    ));
            };

            struct InvokableTransform
              : refines<RegularInvokable>
            {
                template<typename Fun, typename T>
                auto requires_(Fun &&fun, T && t) -> decltype(
                    concepts::valid_expr(
                        concepts::model_of<Transform>(invokable((Fun &&) fun), (T &&) t)
                    ));
            };
        }

        template<typename Fun, typename...Args>
        using Invokable = concepts::models<concepts::Invokable, Fun, Args...>;

        template<typename Fun, typename...Args>
        using RegularInvokable = concepts::models<concepts::RegularInvokable, Fun, Args...>;

        template<typename Fun, typename...Args>
        using InvokablePredicate = concepts::models<concepts::InvokablePredicate, Fun, Args...>;

        template<typename Fun, typename T, typename U = T>
        using InvokableRelation = concepts::models<concepts::InvokableRelation, Fun, T, U>;

        template<typename F, typename T>
        using InvokableTransform = concepts::models<concepts::InvokableTransform, F, T>;
    }
}

#endif // RANGES_V3_UTILITY_INVOKABLE_HPP
