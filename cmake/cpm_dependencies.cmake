if(NOT UNIX)
    CPMAddPackage("gh:wxWidgets/wxWidgets@3.2.6")
    CPMAddPackage("gh:opencv/opencv#4.10.0")
endif()

CPMAddPackage(
        NAME csv-parser
        GITHUB_REPOSITORY vincentlaucsb/csv-parser
        GIT_TAG 2.3.0
        DOWNLOAD_ONLY YES
)