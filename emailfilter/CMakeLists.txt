set(SOURCES
        src/date.c
        src/email.c
        src/filter.c
        src/utils.c
)
set(INCLUDE_DIR include)

# static version
add_library(emailfilter_static STATIC ${SOURCES})
target_include_directories(emailfilter_static PUBLIC ${INCLUDE_DIR})
target_link_libraries(emailfilter_static PRIVATE pthread)

# dynamic version
add_library(emailfilter_shared SHARED ${SOURCES})
target_include_directories(emailfilter_shared PUBLIC ${INCLUDE_DIR})
target_link_libraries(emailfilter_shared PRIVATE pthread)
