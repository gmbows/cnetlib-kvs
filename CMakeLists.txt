cmake_minimum_required(VERSION 3.5)

project(cnetlib-kvs VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CMAKE_CXX_FLAGS "-D_WIN32_WINNT=0x0601 -DASIO_STANDALONE -L /lib -O2")

set(CLIENT_SOURCES
		client/main.cpp
		client/mainwindow.cpp
		client/mainwindow.h
		client/mainwindow.ui
)

set(SERVER_SOURCES
		server/main.cpp
		server/mainwindow.cpp
		server/mainwindow.h
		server/mainwindow.ui
		server/kvtable.cpp
		server/kvtable.h
)

set(CNETLIB_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src/cnetlib.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src/cnetlib.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src/cnet_test.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src/cnet_utility.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src/cnet_utility.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src/serializer.h
	${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src/serializer.cpp
)

add_executable(cnetlib-kvs-client
	${CLIENT_SOURCES}
	${CNETLIB_SOURCES}
)

add_executable(cnetlib-kvs-server
	${SERVER_SOURCES}
	${CNETLIB_SOURCES}
)

target_link_libraries(cnetlib-kvs-client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(cnetlib-kvs-server PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(cnetlib-kvs-server PRIVATE mingw32 wsock32 ws2_32 stdc++fs)
target_link_libraries(cnetlib-kvs-client PRIVATE mingw32 wsock32 ws2_32 stdc++fs)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../cnetlib3/src)

set_target_properties(cnetlib-kvs-server PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set_target_properties(cnetlib-kvs-client PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS cnetlib-kvs-client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS cnetlib-kvs-server
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(cnetlib-kvs-client)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(cnetlib-kvs-server)
endif()

