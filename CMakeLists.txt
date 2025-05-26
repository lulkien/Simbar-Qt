cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(
  SimbarQt
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  string(REPLACE "-mno-direct-extern-access" "" CMAKE_CXX_FLAGS
                 "${CMAKE_CXX_FLAGS}")
  string(REPLACE "-mno-direct-extern-access" "" CMAKE_C_FLAGS
                 "${CMAKE_C_FLAGS}")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml)
find_package(LayerShellQt REQUIRED)

qt_standard_project_setup()

set(QT_QML_GENERATE_QMLLS_INI ON)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(
  simbar src/main.cpp src/engine/engine.cpp src/bluetooth/controller.cpp
  src/bluetooth/model.cpp src/ui/flexrectangle.cpp)

qt_add_qml_module(
  simbar
  URI
  Simbar
  VERSION
  1.0
  SOURCES
  src/bluetooth/common.h
  src/ui/flexrectangle.h
  src/ui/flexrectangle.cpp
  QML_FILES
  ui/Main.qml
  ui/components/Widget.qml)

target_include_directories(
  simbar
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
          ${CMAKE_CURRENT_SOURCE_DIR}/src/engine
          ${CMAKE_CURRENT_SOURCE_DIR}/src/bluetooth
          ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)

target_link_libraries(simbar PRIVATE LayerShellQtInterface Qt6::Core Qt6::Qml
                                     Qt6::Quick)
