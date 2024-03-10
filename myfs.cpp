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
	else		// If file system instance already exists
	{
		// Fetching the file count
		char fetchedFileCount[FILE_COUNT_SIZE];
		this->blkdevsim->read(FILE_COUNT_ADDRESS, FILE_COUNT_SIZE, fetchedFileCount);
		this->_fileCount = std::stoi(fetchedFileCount);
	}
}


MyFs::~MyFs()
{
	// Writing the file count into the memory before exiting the program
	std::string fileCount = std::to_string(this->_fileCount);
	fileCount.append(FILE_COUNT_SIZE - fileCount.length(), '\0');

	this->blkdevsim->write(FILE_COUNT_ADDRESS, FILE_COUNT_SIZE, (fileCount).c_str());
	std::cout << CYAN << "\n\nSaved file count to memory (" << this->_fileCount << ")\n" << RESET << std::endl;
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
	if (path_str.length() > MAX_FILE_NAME)		// Checking if the file name length is valid
	{
		throw std::runtime_error("File name too long");
	}

	// Writing an entry in the Table
	std::string stringEntry = path_str + "|" + std::to_string(TABLE_END_ADDRESS + this->_fileCount * FILE_SIZE) + "|0";
	stringEntry.append(TABLE_ENTRY_SIZE - stringEntry.length(), '\0');
	const char* entry = (stringEntry).c_str();

	this->blkdevsim->write(TABLE_START_ADDRESS + (this->_fileCount * TABLE_ENTRY_SIZE), TABLE_ENTRY_SIZE, entry);

	std::cout << GREEN <<
	"Entry: " << entry << "      -------> " << TABLE_START_ADDRESS + (this->_fileCount * TABLE_ENTRY_SIZE) <<
	RESET << std::endl;

	// Writing an initial message to the file
	std::string message = "This file is named " + path_str;
	this->blkdevsim->write(TABLE_END_ADDRESS + this->_fileCount * FILE_SIZE, message.length(), message.c_str());

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
	// Iterating over the files table searching for the given path_str
	for (int i = TABLE_START_ADDRESS; i < this->_fileCount * TABLE_ENTRY_SIZE; i+=TABLE_ENTRY_SIZE)
	{
		char entry[TABLE_ENTRY_SIZE];
		this->blkdevsim->read(i, TABLE_ENTRY_SIZE, entry);

		std::vector<std::string> entryTokens = splitEntry(entry);		// Splitting the entry into its 3 sections
		if (entryTokens.at(FILE_NAME_INDEX) == path_str)
		{
			// Reading the file contents and saving into a variable
			char fileContents[MAX_FILE_SIZE];
			this->blkdevsim->read(std::stoi(entryTokens.at(FILE_ADDRESS_INDEX)), MAX_FILE_SIZE, fileContents);
			
			return fileContents;
		}
	}

	throw std::runtime_error("File not found");
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
