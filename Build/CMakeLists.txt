cmake_minimum_required(VERSION 3.20)

project(Streamrecorder)

find_package(wxWidgets CONFIG REQUIRED)
find_package(CURL REQUIRED)

set(SOURCES "../Source/Main.cpp"
            "../Source/Streamrecorder.rc"
            "../Dependencies/SQLite/sqlite3.c")

add_executable(Streamrecorder WIN32 ${SOURCES})

target_link_libraries(Streamrecorder wx::core wx::base)
target_link_libraries(Streamrecorder CURL::libcurl)

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