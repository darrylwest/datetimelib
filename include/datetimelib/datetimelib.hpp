//
// 2025-02-07 00:08:09 dpw
//

#pragma once

#include <chrono>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace datetimelib {

    using Str = std::string;
    using StrView = std::string_view;
    using TimePoint = std::chrono::system_clock::time_point;
    template <typename T> using Vec = std::vector<T>;
    template <typename T> using Func = std::function<T>;
    template <typename K, typename V> using HashMap = std::unordered_map<K, V>;

    // the lib version
    constexpr StrView VERSION = "0.6.1-130";

    // return the lib's version
    StrView get_version();

    // add wait delay
    TimePoint get_current_time();

    // unix timestamp
    std::time_t timestamp_seconds();

    // the timestamp in milliseconds
    std::time_t timestamp_millis();

    // convert the unix timestamp to the local iso date
    Str ts_to_local_isodate(std::time_t timestamp, const char *format = "%FT%T%z");

    // convert the unix timestamp to the utc/zulu iso date
    Str ts_to_utc_isodate(std::time_t timestamp, const char *format = "%FT%TZ");

    // parse the datetimme string (iso8601) to a 12 character yyyymmddhhmm
    const Str parse_datetime_to_minutes(const Str &datetime);

    void sleep_seconds(int seconds, bool verbose = false);

    // Type alias for a function that provides the current time
    struct MarkProvider {
        Func<int()> get_seconds = timestamp_seconds;
        int mark_minute = 5;
        int tolerance = 20;  // seconds
        Func<void(int, bool)> delay_seconds = sleep_seconds;
    };

    // Function to wait for the next 5-minute interval with a tolerance of +20
    // seconds
    void wait_for_next_mark(const MarkProvider &provider = MarkProvider{},
                            bool verbose = false);

}  // namespace datetimelib
