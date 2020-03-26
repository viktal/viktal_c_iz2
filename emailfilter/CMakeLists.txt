set(SOURCES
        src/date.c
        src/email.c
        src/filter.c
        src/utils.c
)
set(INCLUDE_DIRS interface)

# static version
add_library(emailfilter_static STATIC ${SOURCES} src/filter_sequential.c)
target_include_directories(emailfilter_static PUBLIC ${INCLUDE_DIRS})

# dynamic version
add_library(emailfilter_shared SHARED ${SOURCES} src/filter_pthread.c)
target_include_directories(emailfilter_shared PUBLIC ${INCLUDE_DIRS})
target_link_libraries(emailfilter_shared PRIVATE pthread)
