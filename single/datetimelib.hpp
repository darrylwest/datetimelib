// Merged Header-Only Library
#include <chrono>
#include <datetimelib/datetimelib.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// Header Definitions
//
// 2025-02-07 00:08:09 dpw
//

#pragma once


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

// Inline Implementation
//
// 2025-02-07 00:08:09 dpw
//


namespace datetimelib {
    // unix timestamp
    unsigned int timestamp_seconds() {
        using namespace std::chrono;
        auto now = system_clock::now();

        return duration_cast<seconds>(now.time_since_epoch()).count();
    }

    // get the timestamp in millis
    unsigned long timestamp_millis() {
        using namespace std::chrono;
        auto now = system_clock::now();
        return duration_cast<milliseconds>(now.time_since_epoch()).count();
    }

    // returns the local datetime in iso8601 format
    Str local_iso_datetime(const std::time_t now_seconds) {
        using namespace std::chrono;

        // const auto ts = timestamp_seconds();
        const time_t ts = (now_seconds == 0) ? timestamp_seconds() : now_seconds;

        system_clock::time_point tp = system_clock::from_time_t(ts);
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);

        // convert to local time
        std::tm local_tm = *std::localtime(&tt);

        // format as iso8601
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%Y-%m-%dT%H:%M:%S%z");

        return oss.str();
    }

    // parse the datetimme string (iso8601) to a 12 character yyyymmddhhmm
    const Str parse_datetime_to_minutes(const Str& datetime) {
        Str result;
        result.reserve(12);  // Reserve space for "YYYYMMDDHHMM"

        // Copy only digits
        std::copy_if(datetime.begin(), datetime.end(), std::back_inserter(result), ::isdigit);

        // Truncate to first 12 characters (YYYYMMDDHHMM)
        if (result.size() > 12) {
            result.resize(12);
        }

        return result;
    }

    // truncate the iso8601 date to the nearest minute, default 5 minute mark
    const Str truncate_to_minutes(const Str& isodate, const int minute) {
        using namespace std::chrono;
        // Parse the ISO 8601 datetime string
        std::tm tm = {};
        std::istringstream ss(isodate);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

        // Convert to time_point
        auto timePoint = system_clock::from_time_t(std::mktime(&tm));

        // Truncate to nearest 5 minutes
        auto mins = duration_cast<minutes>(timePoint.time_since_epoch()) % minute;
        timePoint -= mins;

        // Convert back to time_t
        std::time_t truncatedTime = system_clock::to_time_t(timePoint);
        std::tm* truncatedTm = std::localtime(&truncatedTime);

        // Format the result back to ISO 8601 without seconds
        std::ostringstream result;
        result << std::put_time(truncatedTm, "%Y-%m-%dT%H:%M");
        return result.str();
    }

}  // namespace datetimelib
