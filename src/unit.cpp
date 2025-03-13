//
// 2024-12-24 12:51:24 dpw
//

// #include <spdlog/spdlog.h>

#include <cassert>
#include <datetimelib/datetimelib.hpp>
#include <test/testlib.hpp>
#include <vendor/ansi_colors.hpp>

using namespace dtlib_test;
using namespace colors;

Results test_version() {
    Results r = {.name = "Version test"};

    auto vers = datetimelib::get_version();
    r.equals(vers == datetimelib::VERSION, "should be at version 0.5");
    r.equals(vers.starts_with("0.6."), "should be at the correct major/minor version");

    return r;
}

// Function that returns a static point in time, 2024-02-15T04:26:40-0800
std::chrono::system_clock::time_point get_static_time() {
    using namespace std::chrono;
    const system_clock::time_point fixed_time = system_clock::from_time_t(1708000000);
    return fixed_time;
}

Results test_wait_for_next_mark() {
    using namespace std::chrono;
    Results r = {.name = "Wait for next Mark"};

    auto tsnow = datetimelib::timestamp_seconds() - 1;
    r.equals(tsnow < datetimelib::timestamp_seconds(), "should be one second behind");

    //
    // TODO: need unit tests with alternate MarkProviders
    //

    return r;
}

Results test_ts_to_local() {
    Results r = {.name = "Unix ts to Local"};

    std::time_t ts = 1740369686;
    auto dt = datetimelib::ts_to_local_isodate(ts);
    // spdlog::debug("ts {} local {}", ts, dt);
    r.equals(dt == "2025-02-23T20:01:26-0800", "ts to local");

    ts = 2840369686;
    dt = datetimelib::ts_to_local_isodate(ts);
    // spdlog::debug("ts {} local {}", ts, dt);
    r.equals(dt == "2060-01-03T07:34:46-0800", "future ts to local");

    return r;
}

Results test_ts_to_utc() {
    Results r = {.name = "Unix ts to UTC/Zulu"};
    // spdlog::set_level(spdlog::level::debug);

    std::time_t ts = 1740369686;
    auto dt = datetimelib::ts_to_utc_isodate(ts);
    // spdlog::debug("ts {} local {}", ts, dt);
    r.equals(dt == "2025-02-24T04:01:26Z", "ts to UTC/zulu");

    ts = 2840369686;
    dt = datetimelib::ts_to_utc_isodate(ts);
    // spdlog::debug("ts {} local {}", ts, dt);
    r.equals(dt == "2060-01-03T15:34:46Z", "future ts to UTC/zulu");

    // test for the Y2038 bug
    ts = 2'147'483'647;  // max 32 bit int
    dt = datetimelib::ts_to_utc_isodate(ts);
    // spdlog::debug("ts {} local {}", ts, dt);
    r.equals(dt == "2038-01-19T03:14:07Z", "Y2038 future ts to UTC/zulu");
    ts += 1;  // when it rolls over
    dt = datetimelib::ts_to_utc_isodate(ts);
    // spdlog::debug("ts {} local {}", ts, dt);
    r.equals(dt == "2038-01-19T03:14:08Z", "Y2038 future ts to UTC/zulu");

    return r;
}

Results test_time_t_size() {
    Results r = {.name = "Test time_t size"};

    r.equals(sizeof(std::time_t) == 8, "should be 64 bits, 8 bytes");
    r.equals(sizeof(time_t) == 8, "should be 64 bits, 8 bytes");

    return r;
}

int main() {
    // spdlog::set_level(spdlog::level::info);

    const auto vers = datetimelib::VERSION;
    // spdlog::info("{}Starting Unit Tests: Version: {}{}", cyan, vers, reset);

    Results summary = {.name = "Unit Test Summary"};

    // lambda to run a test and add its result to the summary
    auto run_test = [&summary](auto test_func) {
        auto result = test_func();
        // spdlog::info("{}", result.to_string());
        summary.add(result);
    };

    try {
        run_test(test_version);
        run_test(test_time_t_size);
        run_test(test_ts_to_local);
        run_test(test_ts_to_utc);
        run_test(test_wait_for_next_mark);

        // spdlog::info("{}", summary.to_string());
        auto msg = (summary.failed == 0) ? green + "Ok" : "\n" + red + "Tests failed!";
        // spdlog::info("Unit Test Results: {}{}{}", cyan, msg, reset);

        return summary.failed;
    } catch (const std::exception &e) {
        // spdlog::error("Test failed: {}", e.what());
        return 1;
    }
}
