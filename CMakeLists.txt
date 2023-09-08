cmake_minimum_required(VERSION 3.10)

project(WebSocketServer)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(websocketpp REQUIRED)
include_directories(${WEBSOCKETPP_INCLUDE_DIRS})


#源文件
set (SRC_PATH
    ${CMAKE_CURRENT_LIST_DIR}/src
)
foreach(item ${SRC_PATH})
    aux_source_directory (${item} SRC_LIST)
endforeach()
#头文件
set (STREAM_INCLUDE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
foreach(item ${STREAM_INCLUDE})
    include_directories ( ${item} ) 
endforeach()

add_compile_options(-Wall)

add_executable(WebSocketServer src/main.cpp)

target_link_libraries(WebSocketServer ${OpenCV_LIBS} ${WEBSOCKETPP_LIBRARIES} pthread)
