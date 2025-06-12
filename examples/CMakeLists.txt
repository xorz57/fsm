add_executable(example example.cpp)
target_compile_features(example PRIVATE cxx_std_11)
target_link_libraries(example PRIVATE ${PROJECT_NAME})

include(StrictWarnings)

enable_strict_warnings(example)