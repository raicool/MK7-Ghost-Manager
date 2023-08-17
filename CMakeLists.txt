cmake_minimum_required(VERSION 3.8)

project("ghost-manager" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# add c++ files
file(GLOB_RECURSE CXX_SOURCE "src/*.cpp")

# imgui
file(GLOB IMGUI_SOURCE 
    "dep/imgui/imgui.cpp"
    "dep/imgui/imgui_widgets.cpp"
    "dep/imgui/imgui_tables.cpp"
    "dep/imgui/imgui_draw.cpp"
    "dep/imgui/imgui_demo.cpp"
    "dep/imgui/backends/imgui_impl_sdlrenderer2.cpp"
    "dep/imgui/backends/imgui_impl_sdl2.cpp"
)

# osdialog, for specifically linux builds
if (UNIX)
    file(GLOB OSDIALOG_SOURCE
        "dep/osdialog/osdialog.c"
        "dep/osdialog/osdialog_gtk2.c"
    )
    set_source_files_properties(${OSDIALOG_SOURCE} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
endif()

add_executable(${PROJECT_NAME} ${OSDIALOG_SOURCE} ${CXX_SOURCE} ${IMGUI_SOURCE})
target_precompile_headers(${PROJECT_NAME} PRIVATE "src/pch.h")

# packages
# sdl2
find_package(SDL2_LIBRARY NAMES SDL2)
#link_libraries(SDL2::main)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})

# sdl2_image
find_package(SDL2_image REQUIRED)
#link_libraries(SDL2::Image)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIBRARY})
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})

# unix packages
if (UNIX)
    # gtkmm 2.4
    find_package(PkgConfig)
    pkg_check_modules(GTKMM gtkmm-2.4)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARIES})
    target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
    # osdialog
    target_include_directories(${PROJECT_NAME} PRIVATE "dep/osdialog")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE 
    "src"
    "dep/imgui"
    "dep/spdlog/include"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}/res"
)