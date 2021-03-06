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
#ifndef RANGES_V3_ALGORITHM_PARTIAL_SORT_HPP
#define RANGES_V3_ALGORITHM_PARTIAL_SORT_HPP

#include <functional>
#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/invokable.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/algorithm/heap_algorithm.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct partial_sort_fn
        {
            template<typename I, typename S, typename C = ordered_less, typename P = ident,
                CONCEPT_REQUIRES_(Sortable<I, C, P>() && RandomAccessIterator<I>() && IteratorRange<I, S>())>
            I operator()(I begin, I middle, S end, C pred_ = C{}, P proj_ = P{}) const
            {
                auto && pred = invokable(pred_);
                auto && proj = invokable(proj_);

                make_heap(begin, middle, std::ref(pred), std::ref(proj));
                auto const len = middle - begin;
                I i = middle;
                for(; i != end; ++i)
                {
                    if(pred(proj(*i), proj(*begin)))
                    {
                        iter_swap(i, begin);
                        detail::sift_down_n(begin, len, begin, std::ref(pred), std::ref(proj));
                    }
                }
                sort_heap(begin, middle, std::ref(pred), std::ref(proj));
                return i;
            }

            template<typename Rng, typename C = ordered_less, typename P = ident,
                typename I = range_iterator_t<Rng>,
                CONCEPT_REQUIRES_(Sortable<I, C, P>() && RandomAccessIterable<Rng &>())>
            I operator()(Rng & rng, I middle, C pred = C{}, P proj = P{}) const
            {
                return (*this)(begin(rng), std::move(middle), end(rng), std::move(pred), std::move(proj));
            }
        };

        /// \sa `partial_sort_fn`
        /// \ingroup group-algorithms
        constexpr partial_sort_fn partial_sort {};

        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
