
add_library(fennec data_structures/dynamic_array.c
                   data_structures/hashtable.c
                   utilities/file.c
                   utilities/path.c
                   utilities/string.c)
if (LINUX)
    target_link_libraries(fennec m)
endif()