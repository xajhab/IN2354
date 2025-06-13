cmake_minimum_required(VERSION 3.6)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

project(exercise_5)

# For MSVC: fix error C1128 by allowing more sections per obj file
if (MSVC)
    add_compile_options(/bigobj)
endif()

# Use pre-compiled FreeImage on Windows
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FreeImage_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Libs/FreeImage-3.18.0/Dist/x64/ CACHE PATH "Path to FreeImage header file")
        set(FreeImage_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Libs/FreeImage-3.18.0/Dist/x64/ CACHE PATH "Path to FreeImage .lib/.dll folder")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(FreeImage_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Libs/FreeImage-3.18.0/Dist/x32/ CACHE PATH "Path to FreeImage header file")
        set(FreeImage_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Libs/FreeImage-3.18.0/Dist/x32/ CACHE PATH "Path to FreeImage .lib/.dll folder")
    endif()
endif()

# Set C++ flags
set(CMAKE_CXX_STANDARD 14)

# Use vcpkg packages
find_package(Eigen3 CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(Ceres CONFIG REQUIRED)
find_package(lz4 REQUIRED)

get_target_property(GLOG_DLL_PATH_DEBUG glog::glog IMPORTED_LOCATION_DEBUG)
get_target_property(GLOG_DLL_PATH_RELEASE glog::glog IMPORTED_LOCATION_RELEASE)

add_definitions("-D_DISABLE_EXTENDED_ALIGNED_STORAGE")

set(HEADER_FILES 
    Eigen.h 
    SimpleMesh.h 
    PointCloud.h 
    VirtualSensor.h 
    NearestNeighbor.h 
    ProcrustesAligner.h 
    ICPOptimizer.h 
    FreeImageHelper.h
)

set(SOURCE_FILES 
    FreeImageHelper.cpp
)

link_directories(${FreeImage_LIBRARY_DIR})

add_executable(exercise_5 main.cpp ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(exercise_5 PUBLIC 
    ${FreeImage_INCLUDE_DIR}
)

target_link_libraries(exercise_5
    Eigen3::Eigen
    glog::glog
    lz4::lz4
    ceres
    freeimage
)

# On Windows copy DLLs
if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT exercise_5)
    set_property(TARGET exercise_5 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/)

    add_custom_command(TARGET exercise_5 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FreeImage_LIBRARY_DIR}/FreeImage.dll"
            ${CMAKE_BINARY_DIR})

    add_custom_command(TARGET exercise_5 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GLOG_DLL_PATH_DEBUG}"
            ${CMAKE_BINARY_DIR})

    add_custom_command(TARGET exercise_5 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GLOG_DLL_PATH_RELEASE}"
            ${CMAKE_BINARY_DIR})
endif()
