cmake_minimum_required(VERSION 3.27)
project(Gluon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED 26)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_link_options(-fdeclspec)
add_compile_options(-O3)
add_compile_options(-Wno-attributes) # probably fineeeeeee

if (WIN32)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

macro (RECURSE_FILES return_list filter)
    file(GLOB_RECURSE new_list ${filter})
    set(file_list "")
    foreach (file_path ${new_list})
        set(file_list ${file_list} ${file_path})
    endforeach ()
    list(REMOVE_DUPLICATES file_list)
    set(${return_list} ${file_list})
endmacro ()

if (EXISTS "${CMAKE_SOURCE_DIR}/unity.cmake")
    include("${CMAKE_SOURCE_DIR}/unity.cmake")
else ()
    include("${CMAKE_CURRENT_SOURCE_DIR}/unity.cmake")
endif ()

add_compile_definitions(
        UNITY_MAJOR=\"${UNITY_MAJOR}\"
        UNITY_MINOR=\"${UNITY_MINOR}\"
        UNITY_PATCH=\"${UNITY_PATCH}\"

        UNITY_${UNITY_MAJOR}
        UNITY_${UNITY_MAJOR}_${UNITY_MINOR}
        UNITY_${UNITY_MAJOR}_${UNITY_MINOR}_${UNITY_PATCH}
)

set(IL2CPP_DIR "${UNITY_DIRECTORY}/Editor/Data/il2cpp/libil2cpp")
if (WIN32)
    if (${UNITY_MAJOR} GREATER_EQUAL 6000)
        include_directories("${CMAKE_SOURCE_DIR}/platform")
        add_compile_definitions(
                IL2CPP_USE_PLATFORM_CONFIG=1
        )
    else ()
        message(SEND_ERROR "Unity versions below 6000 are not supported.")
    endif ()
endif ()

add_library(Gluon SHARED)

target_include_directories(Gluon PRIVATE ${IL2CPP_DIR})

set_target_properties(
        Gluon PROPERTIES
        OUTPUT_NAME "Gluon"
)

target_include_directories(
        Gluon PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

RECURSE_FILES(
        gluon_sources
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(
        Gluon PRIVATE
        ${gluon_sources}
)

# CAPSTONE DEPENDENCY
set(CAPSTONE_ARCHITECTURE_DEFAULT OFF)
set(CAPSTONE_X86_SUPPORT ON)

add_subdirectory(capstone)
target_include_directories(Gluon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/capstone/include)
target_link_libraries(Gluon PRIVATE capstone)

set(SPDLOG_BUILD_SHARED ON)
set(SPDLOG_BUILD_PIC ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
target_include_directories(Gluon PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include")
target_link_libraries(Gluon PRIVATE spdlog::spdlog)

target_link_libraries(Gluon PRIVATE dbghelp)
target_link_libraries(Gluon PRIVATE psapi)

target_compile_definitions(Gluon PRIVATE GLUON_EXPORT)