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
message(STATUS ${IL2CPP_DIR})
if (WIN32)
    if (${UNITY_MAJOR} GREATER_EQUAL 6000)
        include_directories("${CMAKE_SOURCE_DIR}/platform")
        add_compile_definitions(
                IL2CPP_USE_PLATFORM_CONFIG=1
        )
    else ()
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

message(STATUS "PROCESSOR ${CMAKE_SYSTEM_PROCESSOR}")

add_subdirectory(capstone)
target_include_directories(Gluon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/capstone/include)
target_link_libraries(Gluon PRIVATE capstone)
target_link_libraries(Gluon PRIVATE dbghelp)
target_link_libraries(Gluon PRIVATE psapi)

target_compile_definitions(Gluon PRIVATE GLUON_EXPORT)