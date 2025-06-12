macro(enable_strict_warnings TARGET)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${TARGET} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wconversion
            -Wsign-conversion
        )
    else()
        message(WARNING "enable_strict_warnings: unsupported compiler '${CMAKE_CXX_COMPILER_ID}' — skipping warning flags")
    endif()
endmacro()