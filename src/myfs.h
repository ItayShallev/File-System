#ifndef __MYFS_H__
#define __MYFS_H__

#include <memory>
#include <vector>
#include <utility>
#include <stdint.h>
#include "blkdev.h"
#include "Helper.h"


class MyFs
{
public:
	MyFs(BlockDeviceSimulator *blkdevsim_);
	~MyFs();

	struct dir_list_entry
	{
		std::string name;
		bool is_dir;
		int file_size;
	};
	typedef std::vector<struct dir_list_entry> dir_list;

	void format();
	
	typedef std::pair<std::string, std::vector<std::string>> EntryInfo;
	MyFs::EntryInfo getEntryInfo(const std::string& fileName);

	void addTableEntry(const std::string& fileName, const int& fileAddress, const int& fileSize);
	void editTableEntry(const std::string& entryToEdit, const std::string& fileName, const int& fileAddress, const int& fileSize);
	
	bool isFileExists(const std::string& fileName);
	void create_file(const std::string& path_str, const bool& directory);

	std::string get_content(const std::string& path_str);
	void set_content(const std::string& path_str, std::string& content);
	
	dir_list list_dir(const std::string& path_str);


private:
	struct myfs_header
	{
		char magic[4];
		uint8_t version;
	};

	BlockDeviceSimulator *blkdevsim;

	static const uint8_t CURR_VERSION = 0x03;
	static const char *MYFS_MAGIC;

	int _fileCount;
};

#endif // __MYFS_H__
