cmake_minimum_required(VERSION 3.15)
project(multi_threaded_multi_file_breakpoint_resume)

set(CMAKE_CXX_STANDARD 20)


set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Quick qml sql Network Widgets WebEngine WebChannel Multimedia LinguistTools QuickControls2 WebSockets REQUIRED)



add_executable(multi_threaded_multi_file_breakpoint_resume
    src/main.cpp
    src/DowmloadFileTaskModel.cpp
    src/DowmloadFileTaskModel.h
    src/DownloadFileManagerModel.cpp
    src/DownloadFileManagerModel.h src/DownloadClient.cpp src/DownloadClient.h src/FileMerge.cpp src/FileMerge.h src/DownloadProxy.cpp src/DownloadProxy.h src/DOWNLOAD_CONSTANT.h)



target_compile_definitions(${PROJECT_NAME}
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt5::Core Qt5::Quick Qt5::Sql Qt5::Network Qt5::Widgets Qt5::WebEngine Qt5::WebChannel Qt5::Multimedia Qt5::WebSockets)
