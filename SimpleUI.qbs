import qbs

CppApplication {
    consoleApplication: true
    cpp.cxxFlags: "-std=c++11"
    files: [
        "basetype.h",
        "main.cpp",
        "suidebug.cpp",
        "suidebug.h",
        "suiobject.cpp",
        "suiobject.h",
        "suisurface.cpp",
        "suisurface.h",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
