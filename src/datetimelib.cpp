//
// 2025-02-07 00:08:09 dpw
//

#include <chrono>
#include <ctime>
#include <datetimelib/datetimelib.hpp>
#include <functional>
#include <iomanip>  // for put_time, get_time
#include <iostream>
#include <sstream>  // for stringstream
#include <thread>

namespace datetimelib {
    // return the version
    StrView get_version() { return VERSION; }

    // unix timestamp
    std::time_t timestamp_seconds() {
        using namespace std::chrono;
        auto now = system_clock::now();

        return duration_cast<seconds>(now.time_since_epoch()).count();
    }

    // get the timestamp in millis
    std::time_t timestamp_millis() {
        using namespace std::chrono;
        auto now = system_clock::now();
        return duration_cast<milliseconds>(now.time_since_epoch()).count();
    }

    // convert the unix timestamp to the local iso date
    Str ts_to_local_isodate(const std::time_t unix_timestamp) {
        // Convert time_t to std::tm (local time)
        std::tm local_tm = *std::localtime(&unix_timestamp);

        // Format the local time as ISO8601
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%FT%T%z");

        return oss.str();
    }

    // convert the unix timestamp to the utc/zulu iso date
    Str ts_to_utc_isodate(const std::time_t timestamp) {
        using std::chrono::system_clock;
        system_clock::time_point tp = system_clock::from_time_t(timestamp);

        // Convert to time_t for formatting
        std::time_t tt = system_clock::to_time_t(tp);

        // Convert to zulu time
        std::tm utc_tm = *std::gmtime(&tt);

        // Format as ISO 8601
        std::ostringstream oss;
        oss << std::put_time(&utc_tm, "%Y-%m-%dT%H:%M:%SZ");

        return oss.str();
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
    const Str parse_datetime_to_minutes(const Str &datetime) {
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
    const Str truncate_to_minutes(const Str &isodate, const int minute) {
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
        std::tm *truncatedTm = std::localtime(&truncatedTime);

        // Format the result back to ISO 8601 without seconds
        std::ostringstream result;
        result << std::put_time(truncatedTm, "%Y-%m-%dT%H:%M");
        return result.str();
    }

    // sleep for the number of seconds; if verbose, then show progress
    void sleep_seconds(const int seconds, const bool verbose) {
        if (verbose) {
            std::cout << "countdown: " << std::endl;

            auto show_countdown = [](const int seconds) {
                std::cout << "\r\033[K";  // Move to start of line and clear it
                std::cout << "\033[1;33m" << "seconds remaining: " << seconds << "\033[0m";
                std::cout.flush();
            };

            show_countdown(seconds);
            for (int sec = seconds; sec > 0; --sec) {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                if (sec < 10) {
                    show_countdown(sec);
                } else if (sec % 10 == 0) {
                    show_countdown(sec);
                } else {
                    std::cout << "." << std::flush;
                }
            }

            // Final message
            show_countdown(0);
            std::cout << " : \033[1;32mdone!\033[0m" << std::endl;

        } else {
            std::this_thread::sleep_for(std::chrono::seconds(seconds));
        }
    }

    // configure the MarkProbider or use the default
    void wait_for_next_mark(const MarkProvider &provider, const bool verbose) {
        using namespace std::chrono;

        // mod for second of hour
        auto now = provider.get_seconds() % 3600;

        const int mark_second = provider.mark_minute * 60;

        const int delay = mark_second - now % mark_second;

        const int tol = provider.tolerance;

        // if the delay would wait beyond the mark_second - tolerance, then skip delay
        const int max_delay = mark_second - provider.tolerance;
        if (delay > max_delay) {
            if (verbose) {
                std::cout << "SKIP the wait for " << delay << " seconds, returning now...\n";
            }

            return;
        }

        if (verbose) {
            std::cout << "Waiting for " << delay << " seconds...\n";
        }
        provider.delay_seconds(delay, verbose);
    }

}  // namespace datetimelib
