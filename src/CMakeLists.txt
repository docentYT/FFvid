cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(FFvid)

find_package(wxWidgets REQUIRED COMPONENTS net core base)

include(${wxWidgets_USE_FILE})

set(SOURCES App.cpp MainFrame.cpp Modules/Join/Join.cpp Modules/Trim/Trim.cpp Modules/Watermark/Watermark.cpp Controls/FilePathCtrl.cpp Controls/FilesPathsOrderedListView.cpp Controls/ProcessBar.cpp)

if (WIN32)
    add_executable(FFvid WIN32 ${SOURCES} main.manifest resource.rc resource.h resource.aps)
else()
    add_executable(FFvid ${SOURCES})
endif()

add_custom_command(TARGET FFvid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/Assets/upArrow.bmp"
        $<TARGET_FILE_DIR:FFvid>)
        
add_custom_command(TARGET FFvid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/Assets/downArrow.bmp"
        $<TARGET_FILE_DIR:FFvid>)

target_link_libraries(FFvid PRIVATE ${wxWidgets_LIBRARIES})