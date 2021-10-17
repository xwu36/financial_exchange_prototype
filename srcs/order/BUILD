load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "order",
    hdrs = ["order.h"],
    deps = [
        "//lib:price4",
        "//srcs/stock:instrument_symbol",
    ],
)

cc_test(
    name = "order_test",
    srcs = ["order_test.cc"],
    deps = [
        ":order",
        ":order_utils",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "order_utils",
    hdrs = ["order_utils.h"],
    deps = [
        ":order",
        "//lib:price4",
        "//srcs/stock:instrument_symbol",
        "@com_github_nlohmann_json//:json"
    ],
)