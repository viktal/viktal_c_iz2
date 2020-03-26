add_library(period INTERFACE)
target_include_directories(period INTERFACE period)

add_executable(app_static app.c)
target_link_libraries(app_static emailfilter_static)

add_executable(app_shared app.c)
target_link_libraries(app_shared emailfilter_shared)
