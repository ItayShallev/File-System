#ifndef __MYFS_H__
#define __MYFS_H__

#include <memory>
#include <vector>
#include <stdint.h>
#include "blkdev.h"


class MyFs
{
public:
	MyFs(BlockDeviceSimulator *blkdevsim_);

	/**
	 * dir_list_entry struct
	 * This struct is used by list_dir method to return directory entry
	 * information.
	 */
	struct dir_list_entry
	{
		std::string name;		// The directory entry name
		bool is_dir;			// whether the entry is a file or a directory
		int file_size;			// File size
	};
	typedef std::vector<struct dir_list_entry> dir_list;


	void format();
	
	void create_file(std::string path_str, bool directory);

	std::string get_content(std::string path_str);
	void set_content(std::string path_str, std::string content);
	
	dir_list list_dir(std::string path_str);


private:

	/**
	 * This struct represents the first bytes of a myfs filesystem.
	 * It holds some magic characters and a number indicating the version.
	 * Upon class construction, the magic and the header are tested - if
	 * they both exist than the file is assumed to contain a valid myfs
	 * instance. Otherwise, the blockdevice is formated and a new instance is
	 * created.
	 */
	struct myfs_header
	{
		char magic[4];
		uint8_t version;
	};

	BlockDeviceSimulator *blkdevsim;

	static const uint8_t CURR_VERSION = 0x03;
	static const char *MYFS_MAGIC;
};

#endif // __MYFS_H__
