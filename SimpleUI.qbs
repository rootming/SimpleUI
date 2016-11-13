import qbs

CppApplication {
    consoleApplication: true
    cpp.cxxFlags: ["-std=c++11", "-Wall"]
    //cpp.runtimeLibrary: "static"
    files: [
        "font.cpp",
        "font.h",
        "main.cpp",
        "sui.cpp",
        "sui.h",
        "suibasedraw.cpp",
        "suibasedraw.h",
        "suiconfig.h",
        "suidebug.cpp",
        "suidebug.h",
        "suiimage.cpp",
        "suiimage.h",
        "suiobject.cpp",
        "suiobject.h",
        "suisurface.cpp",
        "suisurface.h",
        "suitype.h",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
