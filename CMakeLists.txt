cmake_minimum_required(VERSION 3.26)
project(ImageViewer VERSION 0.1 LANGUAGES CXX)

# 保证 .rc 文件可以被正确编译
set(CMAKE_RC_COMPILER windres)
set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 启用Qt的自动生成功能
#set(CMAKE_AUTOUIC ON) 这个是把 .ui 文件转化为 .h 文件的设置, 这里我不使用 .ui 文件, 故不开启这个功能
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 查找Qt6的Widgets模块
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# 查找OpenCV
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# 设置项目源文件
set(PROJECT_SOURCES
        CPP/main.cpp
        # 在这里添加其他源文件和头文件
        CPP/mainwindow.cpp
        HPP/mainwindow.h
        CPP/decode.cpp
        HPP/decode.h
        CPP/Login.cpp
        HPP/Login.h
        HPP/ui_mainwindow.h
)

# 创建可执行文件
qt_add_executable(ImageViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        # 在这里添加资源文件
        res.qrc
        res.rc
)

# 链接Qt的Widgets模块
target_link_libraries(ImageViewer PRIVATE ${OpenCV_LIBS} Qt6::Widgets)

# 设置Windows平台特定属性
set_target_properties(ImageViewer PROPERTIES
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ImageViewer
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(ImageViewer)
