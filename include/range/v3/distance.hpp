// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_DISTANCE_HPP
#define RANGES_V3_DISTANCE_HPP

#include <utility>
#include <range/v3/range_fwd.hpp>
#include <range/v3/size.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>

namespace ranges
{
    inline namespace v3
    {
        struct enumerate_fn : iter_enumerate_fn
        {
        private:
            template<typename Rng, typename D, typename I = range_iterator_t<Rng>>
            std::pair<D, I> impl_r(Rng &rng, D d, concepts::Iterable*, concepts::Iterable*) const
            {
                return iter_enumerate(begin(rng), end(rng), d);
            }
            template<typename Rng, typename D, typename I = range_iterator_t<Rng>>
            std::pair<D, I> impl_r(Rng &rng, D d, concepts::BoundedIterable*, concepts::SizedIterable*) const
            {
                return {static_cast<D>(size(rng)) + d, end(rng)};
            }
        public:
            using iter_enumerate_fn::operator();

            template<typename Rng, typename D = range_difference_t<Rng>,
                typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(Integral<D>() && Iterable<Rng>())>
            std::pair<D, I> operator()(Rng &&rng, D d = 0) const
            {
                static_assert(!is_infinite<Rng>::value,
                    "Trying to compute the length of an infinite range!");
                return this->impl_r(rng, d, bounded_iterable_concept<Rng>(),
                    sized_iterable_concept<Rng>());
            }
        };

        RANGES_CONSTEXPR enumerate_fn enumerate{};

        struct distance_fn : iter_distance_fn
        {
        private:
            template<typename Rng, typename D>
            D impl_r(Rng &rng, D d, concepts::Iterable*) const
            {
                return enumerate(rng, d).first;
            }
            template<typename Rng, typename D>
            D impl_r(Rng &rng, D d, concepts::SizedIterable*) const
            {
                return static_cast<D>(size(rng)) + d;
            }
        public:
            using iter_distance_fn::operator();

            template<typename Rng, typename D = range_difference_t<Rng>,
                CONCEPT_REQUIRES_(Integral<D>() && Iterable<Rng>())>
            D operator()(Rng &&rng, D d = 0) const
            {
                static_assert(!is_infinite<Rng>::value,
                    "Trying to compute the length of an infinite range!");
                return this->impl_r(rng, d, sized_iterable_concept<Rng>());
            }
        };

        // RANGES_CONSTEXPR distance_fn distance {};
        namespace
        {
          constexpr auto const & distance = odr_detail::static_const<distance_fn>::value;
        }
    }
}

#endif
