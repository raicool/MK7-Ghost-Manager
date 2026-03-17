cmake_minimum_required(VERSION 3.8)

project("ghost-manager" LANGUAGES C CXX)

option(SDL_IMAGE_VENDORED "use vendored SDL3_image repository" ON)
option(SDL_VENDORED "use vendored SDL3 repository" ON)
option(BUILD_WITH_SSL_SUPPORT "requires OpenSSL package, used to fetch repository status" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add c++ files
file(GLOB_RECURSE CXX_SOURCE "src/*.cpp")

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

add_executable(${PROJECT_NAME} ${OSDIALOG_SOURCE} ${CXX_SOURCE} ${IMGUI_SOURCE})
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.h")
target_sources(${PROJECT_NAME} PRIVATE "resource.rc")

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/utf-8")
endif()

# packages
# sdl3
if (SDL_VENDORED)
    add_subdirectory("vendor/SDL3" EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 CONFIG COMPONENTS SDL-static)
endif (SDL_VENDORED)
unset(SDL_VENDORED CACHE) 
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# openssl
if (BUILD_WITH_SSL_SUPPORT)
    find_package(OpenSSL REQUIRED)
    message("OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY}")
    message("OPENSSL_LIBRARIES: ${OPENSSL_LIBRARIES}")
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenSSL::SSL)
    target_include_directories(${PROJECT_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})
endif (BUILD_WITH_SSL_SUPPORT)
unset(BUILD_WITH_SSL_SUPPORT CACHE)

# sdl2_image
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_PNG_LIBPNG OFF)
if (SDL_IMAGE_VENDORED)
    set(SDLIMAGE_VENDORED ON)
    add_subdirectory("vendor/SDL_image" EXCLUDE_FROM_ALL)
else()
    find_package(SDL3_image REQUIRED)
endif (SDL_IMAGE_VENDORED)
unset(SDL_IMAGE_VENDORED CACHE) 
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIR})

# unix packages
if (UNIX)
    # osdialog
    target_include_directories(${PROJECT_NAME} PRIVATE "vendor/osdialog")
    # gtkmm 2.4
    find_package(PkgConfig)
    pkg_check_modules(GTKMM gtkmm-2.4)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARIES})
    target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
    "src"
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