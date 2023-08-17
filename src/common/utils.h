#pragma once

#include "log.h"
#include "type.h"

#include "gfx/texture.h"

#define BITMASK(x) (1U<<(x))

static char dir_buf[256]{ 0 };

// file management
/*
*	returns an fstream from an open file dialog
*	@param mode - ios open mode flags (ex: std::ios::binary | std::ios::in)
*/
static std::fstream open_file_s(uint32_t mode)
{
	std::fstream file;
#ifdef WIN32
	OPENFILENAMEA f{ 0 };
	f.lStructSize  = sizeof(f);
	f.lpstrFile    = dir_buf;         //< file name
	f.nMaxFile     = sizeof(dir_buf); //< max directory length
	f.lpstrFilter  = "All\0*.*\0Binary (*.bin)\0*.bin\0";
	f.nFilterIndex = 1;
	f.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileNameA(&f) ? LOG_INFO("file \"{}\" found", dir_buf) : void();
	file.open(dir_buf, mode);
#endif
	return file;
}

/*
*	returns a char array of the absolute path to a file from an open file dialog
*/
static const char* open_file()
{
#ifdef WIN32
	OPENFILENAMEA f{ 0 };
	f.lStructSize  = sizeof(f);
	f.lpstrFile    = dir_buf;         //< file name
	f.nMaxFile     = sizeof(dir_buf); //< max directory length
	f.lpstrFilter  = "All\0*.*\0Binary (*.bin)\0*.bin\0";
	f.nFilterIndex = 1;
	f.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileNameA(&f) ? LOG_INFO("file \"{}\" found", dir_buf) : void();
	return dir_buf;
#endif
#ifdef __LINUX__
	return osdialog_file(OSDIALOG_OPEN, NULL, NULL, NULL);
#endif
}

static const char* open_folder()
{
#ifdef WIN32
	BROWSEINFO bi{ 0 };
	bi.pidlRoot       = NULL;
	bi.pszDisplayName = dir_buf; // Address of a buffer to receive the display name of the folder selected by the user
	bi.lpszTitle      = "Select a folder with spotpass files."; // Title of the dialog
	bi.ulFlags        = BIF_USENEWUI;
	bi.lpfn           = NULL;
	bi.lParam         = 0;
	bi.iImage         = -1;
	LPITEMIDLIST lpItem = SHBrowseForFolder(&bi);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, dir_buf);
	}
	return dir_buf;
#endif
#ifdef __LINUX__
	return osdialog_file(OSDIALOG_OPEN_DIR, NULL, NULL, NULL);
#endif
}

/*
*	creates an empty file from a save file prompt and returns the absolute path
*/
static const char* create_file()
{
#ifdef WIN32
	OPENFILENAMEA f{ 0 };
	f.lStructSize  = sizeof(f);
	f.lpstrFile    = dir_buf;         //< file name
	f.nMaxFile     = sizeof(dir_buf); //< max directory length
	f.lpstrFilter  = "All\0*.*\0Binary (*.bin)\0*.bin\0";
	f.nFilterIndex = 1;
	f.Flags = OFN_PATHMUSTEXIST;
	GetSaveFileNameA(&f) ? LOG_INFO("file \"{}\" created", dir_buf) : void();
	return dir_buf;
#endif
	return osdialog_file(OSDIALOG_SAVE, ".", NULL, NULL);
}

static std::size_t folder_file_count(const char* path)
{
	return (std::size_t)std::distance(std::filesystem::directory_iterator{std::filesystem::path(path)}, std::filesystem::directory_iterator{});
}

/*
*	writes a set amount of bytes in a ofstream as a const char*
*	@param src  - location to copy data from to file
*   @param dst  - file's output stream
*   @param size - size of the data in bytes (optional, set to sizeof(T) if not specified)
*/
template<typename T>
static void bin_write(T* src, std::fstream& dst, size_t size = sizeof(T))
{
	dst.write(reinterpret_cast<const char*>(src), size);
}

/*
*	writes a set amount of bytes in a ofstream as a const char*
*	@param src    - location to copy data from to file
*   @param dst    - file's output stream
*   @param offset - offset of the data in file, in bytes (optional, set to 0 if not specified)
*   @param size   - size of the data in bytes (optional, set to sizeof(T) if not specified)
*/
template<typename T>
static void bin_write(T* src, std::fstream& dst, uint32_t offset = 0, size_t size = sizeof(T))
{
	dst.seekp(offset);
	dst.write(reinterpret_cast<const char*>(src), size);
}

/*
*	writes a set amount of bytes in a ofstream as a const char*
*	@param src    - location to copy data from to file
*   @param dst    - file's output stream
*   @param offset - pointer to offset of the data in file, in bytes (adds size param to offset after write)
*   @param size   - size of the data in bytes (optional, set to sizeof(T) if not specified)
*/
template<typename T>
static void bin_write(T* src, std::fstream& dst, uint32_t* offset, size_t size = sizeof(T))
{
	dst.seekp(*offset);
	dst.write(reinterpret_cast<const char*>(src), size);
	*offset += size;
}


/*
*	read a set amount of bytes in a ifstream as a char*
*	@param dst    - location to copy file data to
*   @param src    - file's input stream
*   @param offset - offset of the data in file, in bytes (optional, set to 0 if not specified)
*	@param size   - size of the data in bytes (optional, set to sizeof(T) if not specified)
*/
template<typename T>
static void bin_read(T* dst, std::fstream& src, uint32_t offset = 0, size_t size = sizeof(T))
{
	src.seekg(offset);
	src.read(reinterpret_cast<char*>(dst), size);
}

/* 
*	read a set amount of bytes in a ifstream as a char* 
*	@param dst    - location to copy file data to
*   @param src    - file's input stream
*	@param offset - pointer to offset of the data in file, in bytes (adds size param to offset after read)
*   @param size   - size of the data in bytes (optional, set to sizeof(T) if not specified)
*/
template<typename T>
static void bin_read(T* dst, std::fstream& src, uint32_t* offset, size_t size = sizeof(T))
{
	src.seekg(*offset);
	src.read(reinterpret_cast<char*>(dst), size);
	*offset += size;
}

/*
*	move a set amount of bytes to a different location in of a fstream
*/
static void bin_move(std::fstream& src, uint32_t old_offset, uint32_t new_offset, size_t size, bool zero_old_data)
{
	char* data = new char[size];

	src.seekg(old_offset);
	src.read(reinterpret_cast<char*>(data), size);
	src.seekp(new_offset);
	src.write(reinterpret_cast<const char*>(data), size); 

	if (zero_old_data)
	{
		char* zero = new char[size] {0};
		src.write(reinterpret_cast<const char*>(zero), size);
		delete[] zero;
	}

	delete[] data;
}

/*
*	verifies a file's magic number
*	@param header - header to look for
*	@param src    - file's input stream
*	@param size   - size of the magic number in bytes
*	@param offset - offset into file (optional, set as 0 by default)
* 
*   @return 1 if magic matches, 0 otherwise
*/
static bool verify_magic(const char* header, std::fstream& src, uint8_t size, uint32_t offset = 0)
{
	char* header_buffer = new char[0xff];
	bin_read<char>(header_buffer, src, offset, sizeof(char) * size);
	header_buffer[size] = 0x00; // add a null terminator after for the strcmp()

	return strcmp(header_buffer, header) == 0;
	delete[] header_buffer;
}

/*
*	returns an integer of the nearest multiple of a number
*	@param n   - number to be rounder
*	@param mul - multiple
* 
*	@return the nearest multiple of parameter mul to parameter n
*/
static int round_multiple(int n, int mul)
{
	int result = n + (mul >> 2);
	if (mul) result -= result % mul;
	return result;
}

static std::u16string utf16be(const char* input, uint16_t size)
{
	if (size % 2 != 0) assert("char array size must be even");

	std::u16string result(size / 2, 0);
	wide_char_t _test;

	for (int i = 0; i < result.size(); i++)
	{	
		_test.lower = input[i * 2 + 1];
		_test.upper = input[i * 2];
		result[i] = _test.wide;
	}

	return result;
}


inline uint16_t swap_byte(uint8_t lower, uint8_t upper)
{
	return ((lower & 0xff) << 8) | ((upper & 0xff00) >> 8);
}

template<typename T>
inline T swap_endian(T value)
{
	T result = 0;
	uint8_t byte = 0;

	result |= (value & 0x000000ff) << 24;
	result |= (value & 0x0000ff00) << 8;
	result |= (value & 0x00ff0000) >> 8;
	result |= (value & 0xff000000) >> 24;

	return result;
}

inline void process_sleep(uint32_t ms)
{
#ifdef WIN32
	Sleep(ms);
#endif
#ifdef __LINUX__
	usleep(ms / 1000);
#endif
}