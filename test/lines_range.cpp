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

#include <vector>
#include <string>
#include <sstream>
#include <range/v3/core.hpp>
#include <range/v3/lines_range.hpp>
#include <range/v3/view/bounded.hpp>
#include "./simple_test.hpp"
#include "./test_utils.hpp"

int main()
{
    using namespace ranges;

    std::string str{"line1\nline2\nline3\n"};
    std::stringstream sin{str};

    auto && rng = lines(sin);
    has_type<std::string const &>(*begin(rng));
    models<concepts::Range>(rng);
    models_not<concepts::SizedRange>(rng);
    models_not<concepts::BoundedRange>(rng);
    models<concepts::InputIterator>(begin(rng));
    models_not<concepts::ForwardIterator>(begin(rng));


    auto && tmp = rng | view::bounded;
    has_type<std::string const &>(*begin(tmp));
    models<concepts::BoundedRange>(tmp);
    models_not<concepts::SizedRange>(tmp);
    models<concepts::InputIterator>(begin(tmp));
    models_not<concepts::ForwardIterator>(begin(tmp));
    std::vector<std::string> actual{begin(tmp), end(tmp)};
    ::check_equal(actual, {"line1", "line2", "line3"});

    std::stringstream sin2{};
    auto && rng2 = lines(sin2);
    CHECK(empty(rng2));

    return ::test_result();
}
