#include "myfs.h"
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>


const char *MyFs::MYFS_MAGIC = "MYFS";


MyFs::MyFs(BlockDeviceSimulator *blkdevsim_) : blkdevsim(blkdevsim_), _fileCount(0)
{
	struct myfs_header header;
	blkdevsim->read(0, sizeof(header), (char *)&header);
	
	// If didn't find file system instance
	if ((strncmp(header.magic, MYFS_MAGIC, sizeof(header.magic)) != 0) || ((header.version != CURR_VERSION)))
	{
		std::cout << "Did not find myfs instance on blkdev" << std::endl;
		std::cout << "Creating..." << std::endl;
		format();
		std::cout << "Finished!" << std::endl;
	}
	else
	{

	}
}


/**
 * format method
 * This function discards the current content in the blockdevice and
 * create a fresh new MYFS instance in the blockdevice.
 */
void MyFs::format()
{
	// put the header in place
	struct myfs_header header;
	strncpy(header.magic, MYFS_MAGIC, sizeof(header.magic));
	header.version = CURR_VERSION;
	blkdevsim->write(0, sizeof(header), (const char*)&header);

	// // TODO: put your format code here
	// const char* tableStr = "TABLE";
	// blkdevsim->write(TABLE_ADDRESS, 6, tableStr);
}


/**
 * create_file method
 * Creates a new file in the required path.
 * @param path_str the file path (e.g. "/newfile")
 * @param directory boolean indicating whether this is a file or directory
 */
void MyFs::create_file(std::string path_str, bool directory)
{
	// Write an entry in the Table
	std::cout << "Address in memory: " << TABLE_END_ADDRESS + this->_fileCount * FILE_SIZE << std::endl;

	std::string stringEntry = path_str + "|" + std::to_string(TABLE_END_ADDRESS + this->_fileCount * FILE_SIZE) + "|0";
	stringEntry.append(TABLE_ENTRY_SIZE - stringEntry.length(), '\0');
	const char* entry = (stringEntry).c_str();

	this->blkdevsim->write(TABLE_START_ADDRESS + (this->_fileCount * TABLE_ENTRY_SIZE), TABLE_ENTRY_SIZE, entry);

	std::cout << "Entry: " << entry << std::endl;
	std::cout << "Wrote entry to this address: " << TABLE_START_ADDRESS + (this->_fileCount * TABLE_ENTRY_SIZE) << std::endl;

	this->_fileCount++;
}


/**
 * get_content method
 * Returns the whole content of the file indicated by path_str param.
 * Note: this method assumes path_str refers to a file and not a
 * directory.
 * @param path_str the file path (e.g. "/somefile")
 * @return the content of the file
 */
std::string MyFs::get_content(std::string path_str)
{
	throw std::runtime_error("not implemented");
	return "";
}


/**
 * set_content method
 * Sets the whole content of the file indicated by path_str param.
 * Note: this method assumes path_str refers to a file and not a
 * directory.
 * @param path_str the file path (e.g. "/somefile")
 * @param content the file content string
 */
void MyFs::set_content(std::string path_str, std::string content)
{
	throw std::runtime_error("not implemented");
}


/**
 * list_dir method
 * Returns a list of a files in a directory.
 * Note: this method assumes path_str refers to a directory and not a
 * file.
 * @param path_str the file path (e.g. "/somedir")
 * @return a vector of dir_list_entry structures, one for each file in
 *	the directory.
 */
MyFs::dir_list MyFs::list_dir(std::string path_str)
{
	dir_list ans;
	throw std::runtime_error("not implemented");
	return ans;
}
