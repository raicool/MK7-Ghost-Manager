#pragma once

// Standard Library
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <cstdio>
#include <algorithm>

#ifdef WIN32
#define _WIN32_WINNT 0x0A00
#endif

// httplib
#ifdef BUILD_WITH_SSL_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#endif
#include <httplib.h>

// Windows
#ifdef WIN32
#include <Windows.h>
#include <ShlObj.h>
#endif
// Linux
#ifdef __linux__ 
#include <unistd.h>
#include <osdialog.h>
#endif

// curl
//#include <curl/curl.h>

// SDL
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

// spdlog
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// Dear Imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>