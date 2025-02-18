//
// 2024-12-24 12:51:24 dpw
//

#include <datetimelib/datetimelib.hpp>
#include <test/testlib.hpp>
#include <vendor/ansi_colors.hpp>
#include <spdlog/spdlog.h>
#include <cassert>

using namespace dtlib_test;
using namespace colors;

Results test_version() {
    Results r = {.name = "Version test"};

    auto vers = datetimelib::get_version();
    r.equals(vers == datetimelib::VERSION, "should be at version 0.5");

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

Results test_local_iso_datetime() {
    Results r = {.name = "Local ISO Datetime"};

    time_t now = time(nullptr);
    auto result = datetimelib::local_iso_datetime(now);
    r.equals(!result.empty(), "result should not be empty");

    return r;
}

Results test_truncate_to_minutes() {
    Results r = {.name = "Truncate to Minutes"};

    std::string test_date = "2023-12-25T14:30:45";
    auto result = datetimelib::truncate_to_minutes(test_date, 15);
    r.equals(result == "2023-12-25T14:30", "should truncate correctly");

    return r;
}

int main() {
    spdlog::set_level(spdlog::level::debug);

    const auto vers = datetimelib::VERSION;
    spdlog::info("{}Starting Unit Tests: Version: {}{}", cyan, vers, reset);

    Results summary = {.name = "Unit Test Summary"};

    // lambda to run a test and add its result to the summary
    auto run_test = [&summary](auto test_func) {
        auto result = test_func();
        spdlog::info("{}", result.to_string());
        summary.add(result);
    };

    try {
        run_test(test_version);
        run_test(test_local_iso_datetime);
        run_test(test_truncate_to_minutes);
        run_test(test_wait_for_next_mark);
        
        spdlog::info("{}", summary.to_string());
        auto msg = (summary.failed == 0) ? green + "Ok" : "\n" + red + "Tests failed!";
        spdlog::info("Unit Test Results: {}{}{}", cyan, msg, reset);

        return summary.failed;
    } catch (const std::exception& e) {
        spdlog::error("Test failed: {}", e.what());
        return 1;
    }
}
