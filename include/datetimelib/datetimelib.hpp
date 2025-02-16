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
}  // namespace datetimelib
