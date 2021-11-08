load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//srcs:__subpackages__"])

cc_library(
    name = "order",
    hdrs = ["order.h"],
    srcs = ["order.cc"],
    deps = [
        "//lib:price4",
        "//lib:tick_size_rule",
        "//srcs/stock:instrument_symbol",
        "@com_github_nlohmann_json//:json",
        "@com_google_absl//absl/status",
    ],
)

cc_test(
    name = "order_test",
    srcs = ["order_test.cc"],
    deps = [
        ":order",
        "@com_google_googletest//:gtest_main",
    ],
    data = ["test_data/orders.jsonl"]

)

cc_test(
    name = "order_utils_test",
    srcs = ["order_utils_test.cc"],
    deps = [
        ":order",
        ":order_utils",
        "//lib:price4",
        "@com_google_googletest//:gtest_main",
    ],
    data = ["test_data/orders.jsonl"]
)

cc_library(
    name = "order_utils",
    hdrs = ["order_utils.h"],
    srcs = ["order_utils.cc"],
    deps = [
        ":order",
        "@com_github_nlohmann_json//:json",
    ],
)

cc_library(
    name = "order_book",
    hdrs = ["order_book.h"],
    deps = [
        ":order_book_entry",
    ],
)

cc_test(
    name = "order_book_test",
    srcs = ["order_book_test.cc"],
    deps = [
        "//lib:price4",
        ":order_book",
        "@com_google_googletest//:gtest_main",
    ]
)

cc_library(
    name = "order_book_entry",
    hdrs = ["order_book_entry.h"],
    deps = [
        "//lib:price4",
    ],
)

cc_library(
    name = "order_pool",
    hdrs = ["order_pool.h"],
    srcs = ["order_pool.cc"],
    deps = [
        "//lib:price4",
        "//srcs/stock:instrument_symbol",
        ":order",
    ]
)

cc_test(
    name = "order_pool_test",
    srcs = ["order_pool_test.cc"],
    deps = [
        ":order_pool",
        "@com_google_googletest//:gtest_main",
        "@com_github_nlohmann_json//:json",
    ]
)