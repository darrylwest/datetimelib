//
// 2025-02-07 00:08:09 dpw
//

#pragma once

#include <chrono>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace datetimelib {

    using Str = std::string;
    using StrView = std::string_view;
    template <typename T> using Vec = std::vector<T>;
    template <typename T> using Func = std::function<T>;
    template <typename K, typename V> using HashMap = std::unordered_map<K, V>;

    // add wait delay
    std::chrono::system_clock::time_point get_current_time();

    constexpr StrView VERSION = "0.5.3-125";

    // return the lib's version
    StrView get_version();

    // unix timestamp
    unsigned int timestamp_seconds();

    // the time in milliseconds
    unsigned long timestamp_millis();

    // returns the local datetime in iso8601 format
    Str local_iso_datetime(const std::time_t now_seconds = 0);

    // parse the datetimme string (iso8601) to a 12 character yyyymmddhhmm
    const Str parse_datetime_to_minutes(const Str& datetime);

    // truncate an iso date to the nearest n minutes, defaulting to 5 minutes
    const Str truncate_to_minutes(const Str& isodate, const int minute = 5);

    void sleep_seconds(const int seconds, const bool verbose = false);

    // Type alias for a function that provides the current time
    struct MarkProvider {
        Func<int()> get_seconds = timestamp_seconds;
        int mark_minute = 5;
        int tolerance = 20; // seconds
        Func<void(int, bool)> delay_seconds = sleep_seconds;
    };

    // Function to wait for the next 5-minute interval with a tolerance of +20 seconds
    void wait_for_next_mark(const MarkProvider& provider = MarkProvider{}, const bool verbose = false);
    
}  // namespace datetimelib
