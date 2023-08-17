#pragma once

// Standard Library
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <stdio.h>

// Windows
#ifdef WIN32
#include <Windows.h>
#include <shlobj_core.h>
#endif
// Linux
#ifdef __linux__ 
#include <unistd.h>
#endif

// SDL
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

// spdlog
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// Dear Imgui
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>