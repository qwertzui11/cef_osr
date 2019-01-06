# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#
# Target executable names.
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CEF_TARGET "ogre_cef")
project(${CEF_TARGET})


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    link_directories(${CMAKE_SOURCE_DIR}/lib/Debug)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    link_directories(${CMAKE_SOURCE_DIR}/lib/Release)
else()
     set(CMAKE_BUILD_TYPE "Debug")
    link_directories(${CMAKE_SOURCE_DIR}/lib/Debug)
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    link_directories(${CMAKE_SOURCE_DIR}/lib/Debug)
    set(build_type "_d")
    set(build_type2 "d")
else()
    set(build_type "")
    set(build_type2 "")
endif()

message("XYXYXYXYXYX ${CMAKE_BUILD_TYPE} mode XYXYXYXYX")

set(CEF_ROOT D:/Programming/cef)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")
find_package(CEF REQUIRED)

find_package(OGRE REQUIRED)
include_directories(
${OGRE_INCLUDE_DIRS} 
${OGRE_INCLUDE_DIRS}/Bites
${OGRE_INCLUDE_DIRS}/RTShaderSystem)
# cefsimple sources.
set(CEFSIMPLE_SRCS
  osr.cpp
  )



#
# Shared configuration.
#




#
# Windows configuration.
#

  # Executable target.
  add_subdirectory(${CEF_ROOT}/libcef_dll libcef_dll_wrapper)

  add_executable(${CEF_TARGET} WIN32 ${CEFSIMPLE_SRCS})
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  ADD_LOGICAL_TARGET(libcef_lib ${CEF_LIB_DEBUG} ${CEF_LIB_RELEASE})
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  #include_directories(D:/Programming/cef)
  target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS} ${OGRE_LIBRARIES} )

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${CEF_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS} )
  endif()

  # Add the custom manifest files to the executable.
 # ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
  COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
  
  
  file(GLOB OGRE_CFGS ${OGRE_CONFIG_DIR}/*.cfg)
  set(DLLFiles  
  #${OGRE_FFMPEG}/build/${CMAKE_BUILD_TYPE}/OgreVideoPlugin.dll
  ${OGRE_CFGS}
  ${OGRE_CONFIG_DIR}/RenderSystem_Direct3D11${build_type}.dll 
  ${OGRE_CONFIG_DIR}/RenderSystem_Direct3D9${build_type}.dll 
  ${OGRE_CONFIG_DIR}/OgreMain${build_type}.dll 
  ${OGRE_CONFIG_DIR}/OgreOverlay${build_type}.dll  
  ${OGRE_CONFIG_DIR}/OgreBites${build_type}.dll 
  ${OGRE_CONFIG_DIR}/OgreRTShaderSystem${build_type}.dll
  ${OGRE_CONFIG_DIR}/SDL2.dll
  ${OGRE_CONFIG_DIR}/zlib.dll
  )
  
  foreach(DLLFile ${DLLFiles})
           add_custom_command(TARGET ${CEF_TARGET} POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E
           copy_if_different  ${DLLFile} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE} 
           )
  endforeach() 

  PRINT_CEF_CONFIG()