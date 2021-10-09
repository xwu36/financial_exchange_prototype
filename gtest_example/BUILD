load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "hello_time",
    srcs = ["hello_time.cc"],
    hdrs = ["hello_time.h"],
    deps = ["@com_google_absl//absl/strings"],
    visibility = ["//json_example:__pkg__"],
)

cc_test(
    name = "hello_time_test",
    srcs = ["hello_time_test.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "hello_time",
    ],
)