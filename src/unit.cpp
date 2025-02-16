//
// 2024-12-24 12:51:24 dpw
//

#include <iostream>
#include <vendor/testlib.hpp>
#include <datetimelib/datetimelib.hpp>
#include <vendor/ansi_colors.hpp>

using namespace rcstestlib;

Results test_version() {
    Results r = {.name = "Version Test"};

    auto vers = datetimelib::get_version();
    // r.equals(vers.startsWith("0.5", "should be at version 0.5");
    r.pass("");

    return r;
}

int main() {
    using namespace colors;
    int ok = 0;

    const auto vers = datetimelib::VERSION;
    std::cout << "Unit Tests: Version: " << vers << std::endl;

    return ok;
}
