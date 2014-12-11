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

#ifndef RANGES_V3_LINES_RANGE_HPP
#define RANGES_V3_LINES_RANGE_HPP

#include <istream>
#include <string>
#include <range/v3/range_fwd.hpp>
#include <range/v3/range_facade.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-core
        /// @{
        struct lines_range
          : range_facade<lines_range>
        {
        private:
            friend range_access;
            std::istream *sin_;
            std::string line_;
            struct cursor
            {
            private:
                lines_range *rng_;
            public:
                cursor() = default;
                explicit cursor(lines_range &rng)
                  : rng_(&rng)
                {}
                void next()
                {
                    std::getline(*rng_->sin_, rng_->line_);
                }
                std::string const &current() const
                {
                    return rng_->line_;
                }
                bool done() const
                {
                    return !*rng_->sin_;
                }
            };
            cursor begin_cursor()
            {
                return cursor{*this};
            }
        public:
            lines_range() = default;
            lines_range(std::istream &sin)
              : sin_(&sin), line_{}
            {
                std::getline(*sin_, line_); // prime the pump
            }
            std::string & cached()
            {
                return line_;
            }
        };

        struct lines_fn
        {
            lines_range operator()(std::istream & sin) const
            {
                return lines_range{sin};
            }
        };

      /// \ingroup group-core
      /// \sa `lines_fn`
        constexpr lines_fn lines{};

        /// @}
    }
}

#endif
