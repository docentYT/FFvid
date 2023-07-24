cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(FFvid)

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "FFvid")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fast and easy-to-use video editor based on FFmpeg.")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_EXECUTABLES "FFvid" "FFvid")
include(CPack)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
if (NOT WIN32)
    find_package(fmt)
endif()

include(${wxWidgets_USE_FILE})

set(SOURCES App.cpp MainFrame.cpp Modules/Join/Join.cpp Modules/Trim/Trim.cpp Modules/Watermark/Watermark.cpp Controls/FilePathCtrl.cpp Controls/FilesPathsOrderedListView.cpp Controls/ProcessBar.cpp Format.h)

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

if (WIN32)
    target_link_libraries(FFvid PRIVATE ${wxWidgets_LIBRARIES})
else()
    target_link_libraries(FFvid PRIVATE ${wxWidgets_LIBRARIES} fmt::fmt)
endif()

install(TARGETS FFvid RUNTIME COMPONENT applications DESTINATION "bin")
install(FILES "${PROJECT_SOURCE_DIR}/Assets/upArrow.bmp" "${PROJECT_SOURCE_DIR}/Assets/downArrow.bmp" DESTINATION "bin")