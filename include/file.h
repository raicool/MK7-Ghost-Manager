#pragma once

enum SerializedFileType
{
	BOSS, // spotpass cup file
	CEC, // streetpass file
	ARCHIVE // system save data file
};

struct SerializedFile
{
	bool ready = false;
	bool edited = false;

	SerializedFileType type;

	virtual void load(std::string path) = 0;
	virtual void save(bool prompt_file = true) = 0;
	virtual void reload() = 0;
	virtual void close() = 0;

	std::fstream file_stream;
	std::string file_directory;
	std::string display_name;
};