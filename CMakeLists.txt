cmake_minimum_required (VERSION 3.7)
project (fennec)
enable_testing()

set (FENNEC_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${FENNEC_INCLUDE_DIRECTORIES})

if (FENNEC_SANITIZE)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif()

add_subdirectory(src)
add_subdirectory(tests)

target_include_directories(fennec PUBLIC ${FENNEC_INCLUDE_DIRECTORIES})
set_property(TARGET fennec PROPERTY C_STANDARD 11)