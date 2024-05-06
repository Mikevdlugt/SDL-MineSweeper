cmake_minimum_required(VERSION 3.22)
project(sdlMinesweeper)
set(CMAKE_CXX_STANDARD 23)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE 1)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_VERBOSE_MAKEFILE 0)
endif()

# Add subdirectories
add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_image)

# Set include paths
include_directories(src/ vendor/SDL/include vendor/imgui/include vendor/SDL_image/include)

# Add files
file(GLOB VENDOR_SOURCES vendor/imgui/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS src/*.hpp src/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.cxx src/*.cc src/*.c)
file(GLOB PROJECT_CONFIGS CMakeLists.txt README.md .gitignore .gitmodules)

# Organizing files
source_group("headers" FILES ${PROJECT_HEADERS})
source_group("sources" FILES ${PROJECT_SOURCES})

# Set executable and target link libraries
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_CONFIGS} ${VENDOR_SOURCES})
target_link_libraries(${PROJECT_NAME} SDL3::SDL3 SDL3_image::SDL3_image)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Copy SDL3 library to binary directory
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_BINARY_DIR}/vendor/SDL/SDL3.dll
            ${PROJECT_BINARY_DIR}
            COMMENT "Copying SDL library to output directory"
    )
elseif(UNIX AND NOT APPLE) # Linux
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_BINARY_DIR}/vendor/SDL/libSDL3.so
            ${PROJECT_BINARY_DIR}
            COMMENT "Copying SDL library to output directory"
    )
endif()

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_BINARY_DIR}/vendor/SDL_image/SDL3_image.dll
            ${PROJECT_BINARY_DIR}
            COMMENT "Copying SDL_image library to output directory"
    )
elseif(UNIX AND NOT APPLE) # Linux
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_BINARY_DIR}/vendor/SDL_image/libSDL3_image.so
            ${PROJECT_BINARY_DIR}
            COMMENT "Copying SDL_image library to output directory"
    )
endif()

# Copy resource files to binary directory
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res
        ${PROJECT_BINARY_DIR}/res
        COMMENT "Copying ${PROJECT_SOURCE_DIR}/res to ${PROJECT_BINARY_DIR}/res")
add_dependencies(${PROJECT_NAME} copy_resources)


# Try to run without console in release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        if(MINGW)
            target_link_options(${PROJECT_NAME} PRIVATE "-mwindows")
        endif()
    else()
        if(MINGW)
            target_link_options(${PROJECT_NAME} PRIVATE "-mwindows")
        endif()
    endif()
endif()
