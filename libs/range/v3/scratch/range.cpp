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

#include <chrono>
#include <iostream>
#include <range/v3/all.hpp>

using namespace ranges;

class timer
{
private:
    std::chrono::high_resolution_clock::time_point start_;
public:
    timer()
    {
        reset();
    }
    void reset()
    {
        start_ = std::chrono::high_resolution_clock::now();
    }
    std::chrono::milliseconds elapsed() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start_);
    }
    friend std::ostream &operator<<(std::ostream &sout, timer const &t)
    {
        return sout << t.elapsed().count() << "ms";
    }
};

void benchmark()
{
    // Define an infinite range containing all the Pythagorean triples:
    auto all_triples =
        view::for_each(view::ints(1), [](int z)
        {
            return view::for_each(view::ints(1, z), [=](int x)
            {
                return view::for_each(view::ints(x, z), [=](int y)
                {
                    return yield_if(x*x + y*y == z*z, std::make_tuple(x, y, z));
                });
            });
        });

    static constexpr int max_triples = 3000;
    auto triples = view::take(all_triples, max_triples);

    timer t;
    int result = 0;
    for_each(triples, [&](std::tuple<int, int, int> triple){
        int i, j, k;
        std::tie(i, j, k) = triple;
        result += (i + j + k);
    });
    std::cout << t << '\n';
    std::cout << result << '\n';

    result = 0;
    int found = 0;
    t.reset();
    for(int z = 1;; ++z)
    {
        for(int x = 1; x <= z; ++x)
        {
            for(int y = x; y <= z; ++y)
            {
                if(x*x + y*y == z*z)
                {
                    result += (x + y + z);
                    ++found;
                    if(found == max_triples)
                        goto done;
                }
            }
        }
    }
done:
    std::cout << t << '\n';
    std::cout << result << '\n';
}

int main()
{
    // Define an infinite range containing all the Pythagorean triples:
    auto triples =
        view::for_each(view::ints(1), [](int z)
        {
            return view::for_each(view::ints(1, z), [=](int x)
            {
                return view::for_each(view::ints(x, z), [=](int y)
                {
                    return yield_if(x*x + y*y == z*z,
                        std::make_tuple(x, y, z));
                });
            });
        });

    // Display the first 100 triples
    RANGES_FOR(auto triple, triples | view::take(100))
    {
        std::cout << '('
            << std::get<0>(triple) << ','
            << std::get<1>(triple) << ','
            << std::get<2>(triple) << ')' << '\n';
    }
}
