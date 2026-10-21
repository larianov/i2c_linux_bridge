cmake_minimum_required(VERSION 3.25)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")

include(${CMAKE_CURRENT_LIST_DIR}/pico_sdk_import.cmake)

if(PICO_SDK_VERSION_STRING VERSION_LESS "2.2.0")
    message(FATAL_ERROR
        "Pico SDK 2.2.0 or later required. Current: ${PICO_SDK_VERSION_STRING}")
endif()

project(i2c_rp2040 VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

find_program(PICOTOOL_EXECUTABLE picotool)

function(firmware_setup target)
    get_target_property(sources ${target} SOURCES)
    set_source_files_properties(${sources}
        PROPERTIES COMPILE_OPTIONS "-Wall;-Wextra;-Wshadow;-Wnull-dereference;-Wpedantic"
    )

    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-Og;-ggdb3>
    )

    pico_add_extra_outputs(${target})

    if(NOT PICOTOOL_EXECUTABLE)
        message(WARNING "picotool was not found: target 'load' will not be available")
    elseif(NOT TARGET load)
        add_custom_target(load
            COMMAND ${PICOTOOL_EXECUTABLE} load -f -x
                    $<TARGET_FILE_DIR:${target}>/${target}.uf2
            DEPENDS ${target}
            COMMENT "Loading ${target}.uf2 to Pico"
            USES_TERMINAL
            VERBATIM
        )
    endif()
endfunction()

add_subdirectory(src)
