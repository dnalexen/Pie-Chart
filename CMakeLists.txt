cmake_minimum_required(VERSION 3.5)

project(pie_chart LANGUAGES C)
find_package(gd)
find_package(m)

add_executable(pie main.c)
target_link_libraries (pie gd m)
install(TARGETS pie
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
