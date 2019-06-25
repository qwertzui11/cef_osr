cmake_minimum_required (VERSION 3.10 FATAL_ERROR)
project (cef_ogre)
set (CMAKE_CXX_STANDARD 11)

# Chromium Embedded Framework (CEF) 
# homepage: https://bitbucket.org/chromiumembedded/cef/src/master
# builds: http://opensource.spotify.com/cefbuilds/index.html
set(CEF_ROOT "" CACHE PATH "a path that points to the root directory of the cef build")
set(CEF_LIBCEF_DLL_WRAPPER_DEBUG "" CACHE FILEPATH "a path that points to the debug libcef_dll_wrapper.a")
set(CEF_LIBCEF_DLL_WRAPPER_RELEASE "" CACHE FILEPATH "a path that points to the release libcef_dll_wrapper.a")
if(NOT CEF_ROOT OR NOT CEF_LIBCEF_DLL_WRAPPER_DEBUG OR NOT CEF_LIBCEF_DLL_WRAPPER_RELEASE)
  message(FATAL_ERROR "please ensure CEF_ROOT, CEF_LIBCEF_DLL_WRAPPER_DEBUG and CEF_LIBCEF_DLL_WRAPPER_RELEASE are set")
endif()
set(CEF_INCLUDE_DIRS "${CEF_ROOT}/include" "${CEF_ROOT}")
add_library(Cef::cef3 INTERFACE IMPORTED)
target_include_directories(Cef::cef3 INTERFACE ${CEF_INCLUDE_DIRS})
find_library(CEF_LIBRARY_DEBUG
  NAMES cef
  PATHS ${CEF_ROOT}
  PATH_SUFFIXES Debug
  NO_DEFAULT_PATH
  )
find_library(CEF_LIBRARY_RELEASE
  NAMES cef
  PATHS ${CEF_ROOT}
  PATH_SUFFIXES Release
  NO_DEFAULT_PATH
  )
target_link_libraries(Cef::cef3 INTERFACE
  debug ${CEF_LIBRARY_DEBUG}
  optimized ${CEF_LIBRARY_RELEASE}
  debug ${CEF_LIBCEF_DLL_WRAPPER_DEBUG}
  optimized ${CEF_LIBCEF_DLL_WRAPPER_RELEASE}
  )

# Ogre3d - https://www.ogre3d.org
find_package(OGRE REQUIRED)
add_library(Ogre3d::Ogre3d INTERFACE IMPORTED)
target_include_directories(Ogre3d::Ogre3d INTERFACE ${OGRE_INCLUDE_DIRS})
target_link_libraries(Ogre3d::Ogre3d INTERFACE ${OGRE_LIBRARIES})

add_executable(osr osr.cpp)
target_link_libraries(osr PRIVATE Ogre3d::Ogre3d Cef::cef3)
