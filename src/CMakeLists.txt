add_library(period INTERFACE)
target_include_directories(period INTERFACE period)

add_executable(app app.c)
target_link_libraries(app emailfilter_static)
