//
// 2025-02-07 00:08:09 dpw
//

#include <datetimelib/datetimelib.hpp>
#include <chrono>
#include <iostream>
#include <iomanip>      // for put_time, get_time
#include <sstream>      // for stringstream
#include <ctime>
#include <thread>
#include <functional>

namespace datetimelib {
    // return the version
    StrView get_version() {
        return VERSION;
    }

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

    // Default function to get the current system time
    std::chrono::system_clock::time_point get_current_time() {
        return std::chrono::system_clock::now();
    }

    // Function to wait for the next 5-minute interval with a tolerance of +20 seconds
    void wait_for_next_mark(const MarkProvider& provider) {
        using namespace std::chrono;

        std::cout << "provider minutes_past:  " << provider.minutes_past << "\n";
        std::cout << "provider tolerance   :  " << provider.tolerance << "\n";

        auto now = provider.get_now();
        std::time_t now_c = system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_c);

        int current_minute = local_tm.tm_min;
        int current_second = local_tm.tm_sec;

        int minutes_past = current_minute % provider.minutes_past;
        int seconds_until_next = (provider.minutes_past * 60) - (provider.minutes_past * 60) - current_second;

        if (seconds_until_next > ((provider.minutes_past * 60) - provider.tolerance)) {
            return;
        }

        std::cout << "Waiting for " << seconds_until_next << " seconds...\n";
        std::this_thread::sleep_for(seconds(seconds_until_next));
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
