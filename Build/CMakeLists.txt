cmake_minimum_required(VERSION 2.8.9)

project(Streamrecorder)

add_definitions(-DCURL_STATICLIB)

set(CMAKE_PREFIX_PATH ../Dependencies)

set(CMAKE_LIBRARY_PATH ../Dependencies/libcurl/lib)

find_package(wxWidgets COMPONENTS core base REQUIRED)

find_library(CURL_LIBRARY_DEBUG NAMES libcurl_a_debug libcurl curl PATH_SUFFIXES debug)
find_library(CURL_LIBRARY_RELEASE NAMES libcurl_a libcurl curl PATH_SUFFIXES release)

set(CURL_LIBRARY
    debug ${CURL_LIBRARY_DEBUG}
    optimized ${CURL_LIBRARY_RELEASE})
	
include("${wxWidgets_USE_FILE}")
include_directories("../Dependencies/SQLite/include")
include_directories("../Dependencies/libcurl/include")

set(SOURCES "../Source/Main.cpp"
            "../Source/Streamrecorder.rc"
            "../Dependencies/SQLite/sqlite3.c")

add_executable(Streamrecorder WIN32 ${SOURCES})

target_link_libraries(Streamrecorder ${wxWidgets_LIBRARIES})
target_link_libraries(Streamrecorder ${CURL_LIBRARY})

if(WIN32)
    target_link_libraries(Streamrecorder ws2_32)
    target_link_libraries(Streamrecorder wldap32)
endif()

add_custom_command(TARGET Streamrecorder
                   POST_BUILD 
                   COMMAND "${CMAKE_COMMAND}" -E copy 
                           "${CMAKE_CURRENT_LIST_DIR}/../Data/resourcestrings.ini"
                           "$<TARGET_FILE_DIR:Streamrecorder>")

add_custom_command(TARGET Streamrecorder
                   POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E copy 
                           "${CMAKE_CURRENT_LIST_DIR}/../Data/config.ini"
                           "$<TARGET_FILE_DIR:Streamrecorder>")

add_custom_command(TARGET Streamrecorder
                   POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E copy 
                           "${CMAKE_CURRENT_LIST_DIR}/../Data/stations.db"
                           "$<TARGET_FILE_DIR:Streamrecorder>")