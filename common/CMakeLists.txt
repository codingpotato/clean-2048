add_library(common INTERFACE)
target_include_directories(common INTERFACE include)

add_subdirectory(tests)
