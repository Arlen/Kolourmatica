cmake_minimum_required(VERSION 3.26)
project(Kolourmatica)

set(CMAKE_CXX_STANDARD 23)


find_package(Eigen3 REQUIRED NO_MODULE)
find_package(Qt6 COMPONENTS REQUIRED Widgets Gui Core)

add_executable(kolourmatica "src/gui/Kolourmatica.cpp")


# headers
target_include_directories(kolourmatica PUBLIC ".")
target_include_directories(kolourmatica PUBLIC "src")
target_include_directories(kolourmatica PUBLIC "src/core")
target_include_directories(kolourmatica PUBLIC "src/gui")

# sources
file(GLOB CORE_SRC_FILES "src/core/*.cpp")
file(GLOB GUI_SRC_FILES "src/gui/*.cpp")

target_sources(kolourmatica PUBLIC ${CORE_SRC_FILES})
target_sources(kolourmatica PUBLIC ${GUI_SRC_FILES})

# deps
target_link_libraries(kolourmatica PUBLIC Eigen3::Eigen Qt6::Widgets Qt6::Core Qt6::Gui)

set_property(TARGET kolourmatica PROPERTY AUTOMOC ON)
