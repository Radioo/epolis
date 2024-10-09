#if(NOT SKIP_CPM_WX)
#    CPMAddPackage("gh:wxWidgets/wxWidgets@3.2.6")
#endif()
#CPMAddPackage("gh:opencv/opencv#4.10.0")

CPMAddPackage(
        NAME csv-parser
        GITHUB_REPOSITORY vincentlaucsb/csv-parser
        GIT_TAG 2.3.0
        DOWNLOAD_ONLY YES
)