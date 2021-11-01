load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//srcs:__subpackages__"])

cc_library(
    name = "matching_engine",
    srcs = ["matching_engine.cc"],
    hdrs = ["matching_engine.h"],
    deps = [
        "//lib:price4",
        "//lib:trade_message",
        "//srcs/order:order_book",
        "//srcs/order:order_pool",
        "//srcs/order:order",
        "//srcs/stock:instrument_symbol",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@glog",
    ],
)

cc_test(
    name = "matching_engine_test",
    srcs = ["matching_engine_test.cc"],
    deps = [
        ":matching_engine",
        "//srcs/order:order",
        "//srcs/order:order_book_entry",
        "@com_google_googletest//:gtest_main",
        "@com_github_nlohmann_json//:json",
    ]
)