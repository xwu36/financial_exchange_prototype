load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

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