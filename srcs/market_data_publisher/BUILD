load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//srcs:__subpackages__"])

cc_library(
    name = "market_data_publisher_impl",
    hdrs = ["market_data_publisher_impl.h"],
    deps = [
        "//srcs/market_data_publisher/abstract:market_data_publisher",
        "@glog",
    ],
)

cc_library(
    name = "market_data_publisher_factory",
    hdrs = ["market_data_publisher_factory.h"],
    deps = [
        "//lib:tick_size_rule",
        ":market_data_publisher_impl",
    ],
)