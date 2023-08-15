#pragma once

#include "spdlog/spdlog.h"

struct logger
{
	inline static std::shared_ptr<spdlog::logger> debuglogger;

	static void init_logger()
	{
		std::filesystem::remove("console.log");
		spdlog::sink_ptr consolesink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		spdlog::sink_ptr filesink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("console.log");

		consolesink->set_pattern("%^[%T] : %v%$");
		filesink->set_pattern("[%T] : %v");

		debuglogger = std::make_shared<spdlog::logger>("console", spdlog::sinks_init_list{ consolesink, filesink });
		spdlog::register_logger(debuglogger);
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_on(spdlog::level::trace);
	}
}; 

#define LOG_TRACE(...) logger::debuglogger->trace(__VA_ARGS__)
#define LOG_DEBUG(...) logger::debuglogger->debug(__VA_ARGS__)
#define LOG_INFO(...)  logger::debuglogger->info(__VA_ARGS__)
#define LOG_WARN(...)  logger::debuglogger->warn(__VA_ARGS__)
#define LOG_ERROR(...) logger::debuglogger->error(__VA_ARGS__)
#define LOG_FATAL(...) logger::debuglogger->critical(__VA_ARGS__)