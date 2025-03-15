load("@rules_qt//:qt.bzl", "qt_cc_binary")

qt_cc_binary(
    name = "typing_test",
    srcs = [
        "main.cpp",
        "typing_test.cpp",
        "typing_test.h",
    ],
    deps = [
        "@rules_qt//:qt_core",
        "@rules_qt//:qt_gui",
        "@rules_qt//:qt_widgets",
    ],
)
