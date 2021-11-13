load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//srcs:__subpackages__"])

cc_library(
    name = "matching_engine_impl",
    srcs = ["matching_engine_impl.cc"],
    hdrs = ["matching_engine_impl.h"],
    deps = [
        "//lib:price4",
        "//lib:trade_message",
        "//srcs/matching_engine/abstract:matching_engine",
        "//srcs/order:order_book",
        "//srcs/order:order_pool",
        "//srcs/order:order_utils",
        "//srcs/order:order",
        "//srcs/stock:instrument_symbol",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@glog",
    ],
)

cc_test(
    name = "matching_engine_impl_test",
    srcs = ["matching_engine_impl_test.cc"],
    deps = [
        ":matching_engine_impl",
        "//lib:tick_size_rule",
        "//srcs/order:order",
        "//srcs/order:order_book_entry",
        "//srcs/order:order_utils",
        "@com_google_googletest//:gtest_main",
        "@com_github_nlohmann_json//:json",
    ]
)

cc_library(
    name = "matching_engine_factory",
    hdrs = ["matching_engine_factory.h"],
    deps = [
        "//lib:tick_size_rule",
        ":matching_engine_impl",
    ],
)