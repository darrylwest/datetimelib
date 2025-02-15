//
// 2025-02-06 19:35:55 dpw
//

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace datetimelib {

    using Str = std::string;

    using StrView = std::string_view;

    template <typename T> using Vec = std::vector<T>;

    template <typename T> using Func = std::function<T>;

    template <typename K, typename V> using HashMap = std::unordered_map<K, V>;
}
