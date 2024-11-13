cmake_minimum_required(VERSION 3.15)
project(ConanTest)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan/build/Release/generators/conan_toolchain.cmake")
include(${CMAKE_TOOLCHAIN_FILE})

set(SOURCES main.cpp)

find_package(nlohmann_json REQUIRED)
find_package(PahoMqttCpp  REQUIRED)
add_executable(ConanTest ${SOURCES})

target_include_directories(ConanTest PRIVATE ${CONAN_INCLUDE_DIRS})
target_link_libraries(ConanTest nlohmann_json::nlohmann_json)
target_link_libraries(ConanTest PahoMqttCpp::paho-mqttpp3-static)

add_custom_target(run
    COMMAND ConanTest
    DEPENDS ConanTest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
