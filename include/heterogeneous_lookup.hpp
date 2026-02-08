#pragma once

#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    constexpr bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }
};

struct TransparentStringEqual {
    using is_transparent = void;
    constexpr bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }
};

struct TransparentStringHash {
    using is_transparent = void;
    auto operator()(std::string_view s) const { return std::hash<std::string_view>{}(s); }
};

}  // namespace bookdb
