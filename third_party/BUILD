# third_party/usr_local.BUILD
# Demos how to use third party include/lib files
cc_library(
  name = "glog",
  hdrs = glob(["include/glog/**"]),
  srcs = [
    "lib/libglog.a",
  ],
  includes = [
    "include/glog",
    "include/gflags",
    "include",
  ],
  visibility = ["//visibility:public"],
)