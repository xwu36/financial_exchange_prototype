load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//srcs:__subpackages__"])

cc_library(
    name = "price4",
    srcs = ["price4.cc"],
    hdrs = ["price4.h"],
    deps = [
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

cc_library(
    name = "trade_message",
    hdrs = ["trade_message.h"],
    srcs = ["trade_message.cc"],
    deps = [
        ":price4",
        "@com_github_nlohmann_json//:json",
        "@com_google_absl//absl/strings",
        "@glog",
    ],
)

cc_test(
    name = "trade_message_test",
    srcs = ["trade_message_test.cc"],
    deps = [
        ":trade_message",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "tick_size_rule",
    hdrs = ["tick_size_rule.h"],
    srcs = ["tick_size_rule.cc"],
    deps = [
        ":price4",
        "@com_github_nlohmann_json//:json",
    ],
)

cc_test(
    name = "tick_size_rule_test",
    srcs = ["tick_size_rule_test.cc"],
    deps = [
        ":tick_size_rule",
        "@com_google_googletest//:gtest_main",
    ],
)
