// Copyright (C) 2020-2021 Sami Väisänen
// Copyright (C) 2020-2021 Ensisoft http://www.ensisoft.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

// this file should only be included in unit test files.

#include "warnpush.h"
#  include <glm/vec2.hpp>
#include "warnpop.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <functional>

#include "base/test_float.h"
#include "base/test_minimal.h"
#include "base/color4f.h"
#include "base/types.h"
#include "base/utility.h"

#undef MIN
#undef MAX
#undef min
#undef max

namespace base {

static bool operator==(const Color4f& lhs, const Color4f& rhs)
{
    return real::equals(lhs.Red(), rhs.Red()) &&
           real::equals(lhs.Green(), rhs.Green()) &&
           real::equals(lhs.Blue(), rhs.Blue()) &&
           real::equals(lhs.Alpha(), rhs.Alpha());
}
static bool operator!=(const Color4f& lhs, const Color4f& rhs)
{ return !(lhs == rhs); }

static bool operator==(const FRect& lhs, const FRect& rhs)
{
    return real::equals(lhs.GetX(), rhs.GetX()) &&
           real::equals(lhs.GetY(), rhs.GetY()) &&
           real::equals(lhs.GetWidth(), rhs.GetWidth()) &&
           real::equals(lhs.GetHeight(), rhs.GetHeight());
}
static bool operator!=(const FRect& lhs, const FRect& rhs)
{ return !(lhs == rhs); }

static bool operator==(const FSize& lhs, const FSize& rhs)
{
    return real::equals(lhs.GetWidth(), rhs.GetWidth()) &&
           real::equals(lhs.GetHeight(), rhs.GetHeight());
}
static bool operator!=(const FSize& lhs, const FSize& rhs)
{ return !(lhs == rhs); }

static bool operator==(const FPoint& lhs, const FPoint& rhs)
{
    return real::equals(lhs.GetX(), rhs.GetX()) &&
           real::equals(lhs.GetY(), rhs.GetY());
}
static bool operator!=(const FPoint& lhs, const FPoint& rhs)
{ return !(lhs == rhs); }

struct TestTimes {
    unsigned iterations = 0;
    double average = 0.0f;
    double minimum = 0.0f;
    double maximum = 0.0f;
    double median  = 0.0f;
    double total   = 0.0f;
};

static TestTimes TimedTest(unsigned iterations, std::function<void()> function)
{
    std::vector<double> times;
    times.resize(iterations);
    for (unsigned i=0; i<iterations; ++i)
    {
        base::ElapsedTimer timer;
        timer.Start();
        function();
        times[i] = timer.SinceStart();
    }
    auto min_time = std::numeric_limits<double>::max();
    auto max_time = std::numeric_limits<double>::min();
    auto sum_time = 0.0f;
    for (auto time : times)
    {
        min_time = std::min(min_time, time);
        max_time = std::max(max_time, time);
        sum_time += time;
    }
    std::sort(times.begin(), times.end());
    TestTimes ret;
    ret.iterations = iterations;
    ret.average    = sum_time / times.size();
    ret.minimum    = min_time;
    ret.maximum    = max_time;
    ret.total      = sum_time;

    const auto index = iterations/2;

    if (iterations % 2)
        ret.median = times[index];
    else ret.median = (times[index-1] + times[index]) / 2.0;
    return ret;
}

static void PrintTestTimes(const char* name, const TestTimes& times)
{
    std::printf("\nTest='%s'\n", name);
    std::printf("==============================\n");
    std::printf("total  = %.6f s %6u ms\n", times.total,   unsigned(times.total * 1000u));
    std::printf("min    = %.6f s %6u ms\n", times.minimum, unsigned(times.minimum * 1000u));
    std::printf("max    = %.6f s %6u ms\n", times.maximum, unsigned(times.maximum * 1000u));
    std::printf("avg    = %.6f s %6u ms\n", times.average, unsigned(times.average * 1000u));
    std::printf("median = %.6f s %6u ms\n", times.median,  unsigned(times.median * 1000u));
}

} // base

namespace glm {
static bool operator==(const glm::vec2& lhs, const glm::vec2& rhs)
{ return real::equals(lhs.x, rhs.x) && real::equals(lhs.y, rhs.y); }
static bool operator!=(const glm::vec2& lhs, const glm::vec2& rhs)
{ return !(lhs == rhs); }

} // glm

