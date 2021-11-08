load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//srcs:__subpackages__"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
      "//srcs/matching_engine:matching_engine_factory",
      "//srcs/market_data_publisher:market_data_publisher_factory",
    ],
)