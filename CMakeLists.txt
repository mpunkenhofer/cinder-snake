cmake_minimum_required(VERSION 3.7)
project(CinderSnake)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)

get_filename_component(CINDER_PATH "~/Cinder/" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

set( SRC_FILES
        ${APP_PATH}/src/SnakeApp.cpp
        ${APP_PATH}/src/Snake.cpp
        ${APP_PATH}/src/TextDisplay.cpp
)

ci_make_app(
        APP_NAME    "Snake"
        SOURCES     ${SRC_FILES}
        INCLUDES    ${APP_PATH}/include
        CINDER_PATH ${CINDER_PATH}
)
