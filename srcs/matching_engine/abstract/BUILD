load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//srcs:__subpackages__"])

cc_library(
    name = "matching_engine",
    hdrs = ["matching_engine.h"],
    deps = [
        "//lib:price4",
        "//lib:tick_size_rule",
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
