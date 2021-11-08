load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//srcs:__subpackages__"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
      "//lib:tick_size_rule",
      "//lib:trade_message",
      "//srcs/order:order",
      "//srcs/order:order_utils",
      "//srcs/matching_engine:matching_engine_factory",
      "//srcs/market_data_publisher:market_data_publisher_factory",
      "@com_google_absl//absl/status:statusor",
      "@com_google_absl//absl/flags:flag",
      "@com_google_absl//absl/flags:parse",
      "@com_google_absl//absl/flags:usage",
      "@com_github_gflags_gflags//:gflags",
      "@com_github_nlohmann_json//:json",
      "@glog",
    ],
    data = ["data/orders.jsonl", "data/tick_size_rule.json"]
)