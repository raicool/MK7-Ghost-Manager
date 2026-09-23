cmake_minimum_required(VERSION 3.8)

project("ghost-manager" LANGUAGES C CXX)

option(SDL_IMAGE_VENDORED "use vendored SDL3_image repository" ON)
option(SDL_VENDORED "use vendored SDL3 repository" ON)
option(BUILD_WITH_SSL_SUPPORT "requires OpenSSL package, used to fetch repository status" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENSSL_CRYPTO_LIBRARY C:/opt/openssl-3.6.0)
add_compile_definitions("DEBUG=$<CONFIG:Debug>")

find_package(Git)
if(Git_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} --no-pager log -n 1 HEAD "--pretty=format:%h"
    OUTPUT_VARIABLE output
    RESULT_VARIABLE result
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if(result EQUAL 0)
    add_compile_definitions("GIT_COMMIT_HASH=\"${output}\"")
  endif()
endif()

file(READ "version" VERSION)
add_compile_definitions("VERSION=\"${VERSION}\"")

file(GLOB_RECURSE CXX_SOURCE "src/*.cpp")
file(GLOB_RECURSE C_SOURCE "src/*.c")

# imgui
file(GLOB IMGUI_SOURCE 
    "vendor/imgui/imgui.cpp"
    "vendor/imgui/imgui_widgets.cpp"
    "vendor/imgui/imgui_tables.cpp"
    "vendor/imgui/imgui_draw.cpp"
    "vendor/imgui/imgui_demo.cpp"
    "vendor/imgui/backends/imgui_impl_sdlrenderer3.cpp"
    "vendor/imgui/backends/imgui_impl_sdl3.cpp"
)

# osdialog, for specifically linux builds
if (UNIX)
    file(GLOB OSDIALOG_SOURCE
        "vendor/osdialog/osdialog.c"
        "vendor/osdialog/osdialog_gtk2.c"
    )
    set_source_files_properties(${OSDIALOG_SOURCE} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

add_executable(${PROJECT_NAME} ${OSDIALOG_SOURCE} ${C_SOURCE} ${CXX_SOURCE} ${IMGUI_SOURCE})
target_sources(${PROJECT_NAME} PRIVATE "resource.rc")

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()

# packages
find_package(PkgConfig)

# sdl3
#set(SDL_GPU OFF)
set(SDL_CAMERA OFF)
set(SDL_JOYSTICK OFF)
set(SDL_HAPTIC OFF)
set(SDL_HIDAPI OFF)
set(SDL_POWER OFF)
set(SDL_SENSOR OFF)
set(SDL_DIALOG OFF)
set(SDL_DISKAUDIO OFF)
set(SDL_DUMMYAUDIO OFF)
set(SDL_DUMMYCAMERA OFF)
set(SDL_DUMMYVIDEO OFF)
set(SDL_OFFSCREEN OFF)
#set(SDL_RENDER_GPU OFF)
set(SDL_TRAY OFF)
set(SDL_VIRTUAL_JOYSTICK OFF)
set(SDL_XINPUT OFF)
if (SDL_VENDORED)
    add_subdirectory("vendor/SDL3" EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL-static)
endif (SDL_VENDORED)
unset(SDL_VENDORED CACHE) 
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# openssl
if (BUILD_WITH_SSL_SUPPORT)    
    add_compile_definitions("BUILD_WITH_SSL_SUPPORT")
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenSSL::SSL)
    target_link_directories(${PROJECT_NAME} PRIVATE $ENV{OPENSSL_ROOT_DIR})
    target_include_directories(${PROJECT_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})
    message("--")
    message("-- OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY}")
    message("-- OPENSSL_CRYPTO_LIBRARY: ${OPENSSL_CRYPTO_LIBRARY}")
    message("-- OPENSSL_LIBRARIES: ${OPENSSL_LIBRARIES}")
    message("--")
endif (BUILD_WITH_SSL_SUPPORT)
unset(BUILD_WITH_SSL_SUPPORT CACHE)

# sdl2_image

set(SDLIMAGE_ANI OFF)
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_GIF OFF)
set(SDLIMAGE_JPG OFF)
set(SDLIMAGE_JXL OFF)
set(SDLIMAGE_LBM OFF)
set(SDLIMAGE_PCX OFF)
#set(SDLIMAGE_PNG OFF)
set(SDLIMAGE_PNM OFF)
set(SDLIMAGE_QOI OFF)
set(SDLIMAGE_SVG OFF)
set(SDLIMAGE_TGA OFF)
set(SDLIMAGE_TIF OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_XCF OFF)
set(SDLIMAGE_XPM OFF)
set(SDLIMAGE_XV OFF)
if (SDL_IMAGE_VENDORED)
    set(SDLIMAGE_VENDORED ON)
    add_subdirectory("vendor/SDL_image" EXCLUDE_FROM_ALL)
else()
    find_package(SDL_image REQUIRED)
endif (SDL_IMAGE_VENDORED)
unset(SDL_IMAGE_VENDORED CACHE) 
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIR})

# yaml-cpp
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/yaml-cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp::yaml-cpp)

# unix packages
if (UNIX)
    # osdialog
    target_include_directories(${PROJECT_NAME} PRIVATE "vendor/osdialog")
    # gtkmm 2.4
    pkg_check_modules(GTKMM gtkmm-2.4)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARIES})
    target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
    "include"
    "vendor/cpp-httplib"
    "vendor/imgui"
    "vendor/sdl3/include/sdl3"
    "vendor/sdl3/include"
    "vendor/sdl_image/include/sdl3_image"
    "vendor/sdl_image/include"
    "vendor/spdlog/include"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}/res" 
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)