#include "pch.h"
#include "archive.h"

#include "common/utils.h"

#define ARCHIVE_SIZE 0x50D4

extern std::vector<std::shared_ptr<SerializedFile>> g_opened_files;

void open_archive_file(const char* file_path)
{
	std::shared_ptr<SaveArchiveFile> archive = std::make_shared<SaveArchiveFile>();
	archive->load(file_path);

	if (archive->ready)
	{
		g_opened_files.emplace_back(archive);
	}
}

void SaveArchiveFile::load(std::string dir)
{
	file_directory = dir;
	display_name = "system.dat";

	if (file_stream.is_open())
	{
		file_stream.close();
	}

	file_stream.open(dir, std::ios::in | std::ios::binary | std::ios::ate);
	const size_t file_size = file_stream.tellg();

	if (file_size != ARCHIVE_SIZE)
	{
		LOG_ERROR("SaveArchiveFile::load() : invalid file size");
		return;
	}

	serialized = std::make_unique<SystemSaveArchive>(sizeof(SystemSaveArchive));

	bin_read<SystemSaveArchive>(this->serialized.get(), file_stream);

	if (serialized->magic != 0x43445353) // SSDC
	{
		LOG_ERROR("SaveArchiveFile::load() : invalid archive header magic");
		this->close();
		return;
	}

	ready = true;
}

void SaveArchiveFile::save(bool prompt_file)
{
	file_stream.open(file_directory, std::ios::out | std::ios::binary | std::ios::trunc);

	if (file_stream.is_open() == false)
	{
		LOG_ERROR("SaveArchiveFile::save() : could not open file");
		return;
	}

	file_stream.write((char*)serialized.get(), sizeof(SystemSaveArchive));
	file_stream.close();
}

void SaveArchiveFile::reload()
{
	this->close();
	this->load(file_directory);
}

void SaveArchiveFile::close()
{
	if (serialized)
	{
		serialized.release();
	}

	file_stream.close();
}