cmake_minimum_required (VERSION 2.8)

project (cefosr)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 ${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OGRE REQUIRED)
find_package(Cef3 REQUIRED)

set (INCLUDES ${INCLUDES} ${OGRE_INCLUDE_DIRS} ${CEF3_INCLUDE_DIRS} )
set (LIBS ${LIBS} ${OGRE_LIBRARIES} ${CEF3_LIBRARIES} )

include_directories(${INCLUDES})
add_executable(osr osr.cpp)
target_link_libraries(osr ${LIBS})

