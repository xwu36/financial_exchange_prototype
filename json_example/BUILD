load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_library(
    name = "hello_greet",
    srcs = ["hello_greet.cc"],
    hdrs = ["hello_greet.h"],
)

cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cc"],
    deps = [
        ":hello_greet",
        "//gtest_example:hello_time",
        "@com_github_nlohmann_json//:json"
    ],
    data = ["hello_world_message.json"]
)

cc_test(
    name = "hello_greet_test",
    srcs = ["hello_greet_test.cc"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "hello_greet",
    ],
)