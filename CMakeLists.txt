cmake_minimum_required(VERSION 3.27)
project(Gluon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_compile_options(-fdeclspec)
add_compile_options(-O3)

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

if (EXISTS "${CMAKE_SOURCE_DIR}/../unity.cmake")
    include(${CMAKE_SOURCE_DIR}/../unity.cmake)
else ()
    include(unity.cmake)
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
message(STATUS ${IL2CPP_DIR})
if (WIN32)
    add_compile_definitions(
            IL2CPP_TARGET_CUSTOM=1
            IL2CPP_TARGET_WINDOWS=1
            IL2CPP_TARGET_WINDOWS_DESKTOP=1

            IL2CPP_PLATFORM_SUPPORTS_SYSTEM_CERTIFICATES=1
            IL2CPP_PLATFORM_SUPPORTS_CPU_INFO=1

            _UNICODE=1
            UNICODE=1
            STRICT=1
    )
endif ()

add_library(Gluon SHARED
        include/il2cpp_functions.hpp)

target_include_directories(Gluon PRIVATE ${IL2CPP_DIR})

set_target_properties(
        Gluon PROPERTIES
        OUTPUT_NAME "Gluon"
)

target_include_directories(
        Gluon PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
)

RECURSE_FILES(
        gluon_sources
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

target_sources(
        Gluon PRIVATE
        ${gluon_sources}
)

target_compile_definitions(Gluon PRIVATE GLUON_EXPORT)