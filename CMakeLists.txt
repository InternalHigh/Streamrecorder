cmake_minimum_required(VERSION 4.2.0)

project(Streamrecorder)

find_package(wxWidgets CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

set(SOURCES "Source/Main.cpp"
            "Source/Streamrecorder.rc")

add_executable(Streamrecorder WIN32 ${SOURCES})

target_link_libraries(Streamrecorder wx::core wx::base)
target_link_libraries(Streamrecorder CURL::libcurl)
target_link_libraries(Streamrecorder unofficial::sqlite3::sqlite3)

add_custom_command(TARGET Streamrecorder
                   POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E copy_directory 
                           "${CMAKE_CURRENT_LIST_DIR}/Data"
                           "$<TARGET_FILE_DIR:Streamrecorder>")
