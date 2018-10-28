cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(component)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (os "linux")
    if(USE_32BIT)
        set (file_prefix "32")
        set (with_64bit FALSE)
    else()
        set (file_prefix "64")
        set (with_64bit TRUE)
    endif()
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set (os "windows")
    set (VS_WIN32 "VC/bin/cl.exe")
    set (VS_WIN64 "VC/bin/x86_amd64/cl.exe")
    if (DEFINED ENV{VS100COMNTOOLS})
        string (REPLACE "\\" "/" VS100COMNTOOLS $ENV{VS100COMNTOOLS})
        string (REPLACE "/Common7/Tools/" "" VS2010_COMPILER ${VS100COMNTOOLS})
    endif ()
    if (DEFINED ENV{VS140COMNTOOLS})
        string (REPLACE "\\" "/" VS140COMNTOOLS $ENV{VS140COMNTOOLS})
        string (REPLACE "/Common7/Tools/" "" VS2015_COMPILER ${VS140COMNTOOLS})
    endif ()
else()
endif()

if (${os} STREQUAL "linux") 
    if (${PLATFORM_TYPE} MATCHES "arm")
        set (OUTPUT_PATH ${PLATFORM_TYPE}_${PLATFORM_NAME})
    else ()
        if (with_64bit)
            set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
            set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
        else ()
            #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") 
            #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")  
            #SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")     
        endif ()
        set (OUTPUT_PATH gcc_${OUTPUT_PATH_SUFFIX})
    endif ()
          
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -g -Wall -fPIC -Wl,--whole-archive -lpthread  -Wl,--no-whole-archive -lc -ldl")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++0x -fPIC -Wl,--whole-archive -lpthread  -Wl,--no-whole-archive -lc -ldl")
        set (OUTPUT_SUB_PATH "debug")   
    else ()
        set (OUTPUT_SUB_PATH "release")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -O3 -Os -fPIC -Wl,--whole-archive -lpthread  -Wl,--no-whole-archive -lc -ldl")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2 -Os -fPIC -Wl,--whole-archive -lpthread  -Wl,--no-whole-archive -lc -ldl")
    endif () 
        
elseif (${os} STREQUAL "windows")
    if (${CMAKE_C_COMPILER} STREQUAL ${VS2010_COMPILER}/${VS_WIN32})
        set (OUTPUT_PATH "vs2010_win32")
        set (file_prefix "")
        set (WITH_64BIT FALSE)
    elseif (${CMAKE_C_COMPILER} STREQUAL ${VS2010_COMPILER}/${VS_WIN64})
        set (OUTPUT_PATH "vs2010_win64")
        set (file_prefix "64")
        set (WITH_64BIT TRUE)
    elseif (${CMAKE_C_COMPILER} STREQUAL ${VS2015_COMPILER}/${VS_WIN32})
        set (OUTPUT_PATH "vs2015_win32")
        set (file_prefix "")
        set (WITH_64BIT FALSE)
    elseif (${CMAKE_C_COMPILER} STREQUAL ${VS2015_COMPILER}/${VS_WIN64})
        set (OUTPUT_PATH "vs2015_win64")
        set (file_prefix "64")
        set (WITH_64BIT TRUE)
    endif ()
else ()
    set (OUTPUT_PATH "unknown")
endif ()

if (NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set (CMAKE_DEBUG_POSTFIX "_d")
endif ()

set(HEADER_DIR ${PROJECT_SOURCE_DIR}/component/header)
set(THIRD_HEADER_DIR ${PROJECT_SOURCE_DIR}/third/header)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/)

set (THIRD_STATIC_LIB_DIR ${PROJECT_SOURCE_DIR}/third/static/${OUTPUT_PATH})

set (LINK_LIBRARIES_DIR
${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
${THIRD_STATIC_LIB_DIR}/jsoncpp
${THIRD_STATIC_LIB_DIR}/framework
${THIRD_STATIC_LIB_DIR}/zmq
${THIRD_STATIC_LIB_DIR}/sqlite3
${THIRD_STATIC_LIB_DIR}/mysql
${THIRD_STATIC_LIB_DIR}/mysqlwrap
${THIRD_STATIC_LIB_DIR}/curl
${THIRD_STATIC_LIB_DIR}/libevent
${THIRD_STATIC_LIB_DIR}/boost
${THIRD_STATIC_LIB_DIR}/uuid
${THIRD_STATIC_LIB_DIR}/openssl
${THIRD_STATIC_LIB_DIR}/pthread
)

include (script/makelists.cmake)
