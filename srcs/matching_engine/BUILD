load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//srcs:__subpackages__"])

cc_library(
    name = "matching_engine",
    srcs = ["matching_engine.cc"],
    hdrs = ["matching_engine.h"],
    deps = [
        "//srcs/order:order_book",
        "//srcs/order:order_pool",
        "//srcs/order:order",
        "@com_google_absl//absl/strings",
        "@glog",
    ],
)

cc_test(
    name = "price4_test",
    srcs = ["price4_test.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "price4",
    ],
)