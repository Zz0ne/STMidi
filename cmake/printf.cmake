include(FetchContent)

FetchContent_Declare(
    printf_lib
    GIT_REPOSITORY https://github.com/mpaland/printf.git
    GIT_TAG master  # Use a specific version if needed
)

FetchContent_MakeAvailable(printf_lib)

# Add the printf source file to your project (this ensures it gets compiled)
add_library(printf STATIC ${printf_lib_SOURCE_DIR}/printf.c)
target_include_directories(printf PUBLIC ${printf_lib_SOURCE_DIR})

