#include "myfs.h"
#include <string.h>
#include <iostream>
#include <math.h>
#include <sstream>


const char *MyFs::MYFS_MAGIC = "MYFS";


/**
 @brief		Constructor - Initializes the block device simulator and the file count.
 @param		blkdevsim_		The block device simulator
 */
MyFs::MyFs(BlockDeviceSimulator *blkdevsim_) : blkdevsim(blkdevsim_), _fileCount(0)
{
	struct myfs_header header;
	blkdevsim->read(0, sizeof(header), (char *)&header);
	
	// If didn't find file system instance
	if ((strncmp(header.magic, MYFS_MAGIC, sizeof(header.magic)) != 0) || ((header.version != CURR_VERSION)))
	{
		std::cout << CYAN "Did not find myfs instance on blkdev" << std::endl;
		std::cout << GREEN "Creating..." RESET << std::endl;
		format();
		std::cout << GREEN "Finished!" RESET << std::endl;
	}
	else		// If file system instance already exists
	{
		// Fetching the file count from the memory
		char fetchedFileCount[FILE_COUNT_SIZE];
		this->blkdevsim->read(FILE_COUNT_ADDRESS, FILE_COUNT_SIZE, fetchedFileCount);
		this->_fileCount = std::stoi(fetchedFileCount);
	}
}


/**
 @brief		Destructor - Writes the file count into the memory before exiting the program.
 */
MyFs::~MyFs()
{
	// Writing the file count into the memory before exiting the program
	std::string fileCount = std::to_string(this->_fileCount);
	fileCount.append(FILE_COUNT_SIZE - fileCount.length(), '\0');

	this->blkdevsim->write(FILE_COUNT_ADDRESS, FILE_COUNT_SIZE, (fileCount).c_str());
	std::cout << CYAN << "\n\nSaved file count to memory (" << this->_fileCount << ")\n" << RESET << std::endl;
}


/**
 @brief		Formats the block device simulator and puts the header in place.
 @return	void
 */
void MyFs::format()
{
	struct myfs_header header;
	strncpy(header.magic, MYFS_MAGIC, sizeof(header.magic));
	header.version = CURR_VERSION;
	blkdevsim->write(0, sizeof(header), (const char*)&header);
}


/**
 @brief		returns the entry info of the given file in the system.
 @param		fileName		The name of the file to search for its entry info
 @return	The entry info of the given file in the system.
 */
MyFs::EntryInfo MyFs::getEntryInfo(const std::string& fileName)
{
	for (int i = TABLE_START_ADDRESS; i < this->_fileCount * TABLE_ENTRY_SIZE; i+=TABLE_ENTRY_SIZE)
	{
		char entry[TABLE_ENTRY_SIZE];
		this->blkdevsim->read(i, TABLE_ENTRY_SIZE, entry);

		std::vector<std::string> entryTokens = splitEntry(entry);		// Splitting the entry into its 3 sections
		if (entryTokens.at(FILE_NAME_INDEX) == fileName)
		{
			return MyFs::EntryInfo(std::to_string(i), entryTokens);
		}
	}

	return MyFs::EntryInfo(ENTRY_NOT_FOUND, std::vector<std::string>());		// Returning an empty pair
}


/**
 @brief		Creates a new entry using the parameters and adds it to the files table.
 @param		fileName		The name of the file to use in the entry
 @param		fileAddress		The address of the file to use in the entry
 @param		fileSize		The size of the file to use in the entry
 @return	void
 */
void MyFs::addTableEntry(const std::string& fileName, const int& fileAddress, const int& fileSize)
{
	std::string entry = fileName + "|" + std::to_string(fileAddress) + "|" + std::to_string(fileSize);
	entry.append(TABLE_ENTRY_SIZE - entry.length(), '\0');

	// Writing a new entry to the table
	this->blkdevsim->write(TABLE_START_ADDRESS + (this->_fileCount * TABLE_ENTRY_SIZE), TABLE_ENTRY_SIZE, entry.c_str());
}


/**
 @brief		Edits the entry of the given file in the files table.
 @param		entryToEdit		The name of the file to edit its entry
 @param		fileName		The updated name of the file to use in the entry
 @param		fileAddress		The updated address of the file to use in the entry
 @param		fileSize		The updated size of the file to use in the entry
 @return	void
 */
void MyFs::editTableEntry(const std::string& entryToEdit, const std::string& fileName, const int& fileAddress, const int& fileSize)
{
	MyFs::EntryInfo entryInfo = this->getEntryInfo(entryToEdit);

	// Checking if the entry was found
	if (entryInfo.first == ENTRY_NOT_FOUND)
	{
		throw std::runtime_error(RED "File not found" RESET);
	}

	std::string updatedEntry = fileName + "|" + std::to_string(fileAddress) + "|" + std::to_string(fileSize);
	updatedEntry.append(TABLE_ENTRY_SIZE - updatedEntry.length(), '\0');

	// Overwriting the old entry and replacing it with the updated one
	this->blkdevsim->write(std::stoi(entryInfo.first), TABLE_ENTRY_SIZE, updatedEntry.c_str());
}


/**
 @brief		Checks if the given file exists in the system.
 @param		fileName		The name of the file to check if it exists
 @return	True if the file exists, false otherwise.
 */
bool MyFs::isFileExists(const std::string& fileName)
{
	// Iterating over the table searching for the given fileName
	for (int i = TABLE_START_ADDRESS; i < this->_fileCount * TABLE_ENTRY_SIZE; i+=TABLE_ENTRY_SIZE)
	{
		char entry[TABLE_ENTRY_SIZE];
		this->blkdevsim->read(i, TABLE_ENTRY_SIZE, entry);

		std::vector<std::string> entryTokens = splitEntry(entry);		// Splitting the entry into its 3 sections
		if (entryTokens.at(FILE_NAME_INDEX) == fileName)
		{
			return true;
		}
	}

	return false;
}


/**
 @brief		Creates a new file in the system and allocates it an entry in the files table.
 @param		path_str		The name of the file to create
 @param		directory		Whether the file is a directory or not
 @return	void
 */
void MyFs::create_file(const std::string& path_str, const bool& directory)
{
	if (path_str.length() > MAX_FILE_NAME)		// Checking if the file name length is valid
	{
		throw std::runtime_error(RED "File name is too long" RESET);
	}
	else if (this->isFileExists(path_str))		// Checking if the file name already exists in the system
	{
		throw std::runtime_error(RED "A file with this name already exists" RESET);
	}

	this->addTableEntry(path_str, TABLE_END_ADDRESS + (this->_fileCount * FILE_SIZE), 0);

	this->_fileCount++;
}


/**
 @brief		Returns the whole content of the file indicated by path_str param.
 @param		path_str		The file path to get its content
 @return	The content of the file
 */
std::string MyFs::get_content(const std::string& path_str)
{
	MyFs::EntryInfo entryInfo = this->getEntryInfo(path_str);

	// Checking if the entry was found
	if (entryInfo.first == ENTRY_NOT_FOUND)
	{
		throw std::runtime_error(RED "File not found" RESET);
	}

	// Reading the file contents and saving into a variable
	char fileContents[MAX_FILE_SIZE];
	this->blkdevsim->read(std::stoi(entryInfo.second.at(FILE_ADDRESS_INDEX)), MAX_FILE_SIZE, fileContents);
	
	return fileContents;
}


/**
 @brief		Overwrites the whole content of the given file and sets it a new content.
 @param		path_str		The file path to set its content
 @param		content			The new content of the file
 @return	void
 */
void MyFs::set_content(const std::string& path_str, std::string& content)
{
	// Checking if the content length is valid
	if (content.length() > MAX_FILE_SIZE)
	{
		std::runtime_error(RED "Content too long" RESET);
	}

	// Iterating over the files table searching for the given path_str
	MyFs::EntryInfo entryInfo = this->getEntryInfo(path_str);
	int actualContentLength = content.length();
	content = content.append(MAX_FILE_SIZE - content.length(), '\0');		// Padding the content with \0 to erase previous content

	// Checking if the entry was found. Creating a file with the given name if not.
	if (entryInfo.first == ENTRY_NOT_FOUND)
	{
		this->create_file(path_str, false);
		entryInfo = this->getEntryInfo(path_str);
	}

	// Updating the file entry
	this->editTableEntry(path_str, entryInfo.second.at(FILE_NAME_INDEX), std::stoi(entryInfo.second.at(FILE_ADDRESS_INDEX)), actualContentLength);
	this->blkdevsim->write(std::stoi(entryInfo.second.at(FILE_ADDRESS_INDEX)), MAX_FILE_SIZE - 1, content.c_str());
}


/**
 @brief		Returns a list of a files in a directory.
 @param		path_str		The directory path to list its files
 @return	a list of dir_list_entry structures, one for each file in the directory.
 */
MyFs::dir_list MyFs::list_dir(const std::string& path_str)
{
	if (path_str == "/")		// current working directory
	{
		dir_list directoryList;

		// Iterating over the files table searching for the given path_str
		for (int i = TABLE_START_ADDRESS; i < this->_fileCount * TABLE_ENTRY_SIZE; i+=TABLE_ENTRY_SIZE)
		{
			char entry[TABLE_ENTRY_SIZE];
			this->blkdevsim->read(i, TABLE_ENTRY_SIZE, entry);

			std::vector<std::string> entryTokens = splitEntry(entry);		// Splitting the entry into its 3 sections

			dir_list_entry dle;
			dle.name = entryTokens.at(FILE_NAME_INDEX);
			dle.is_dir = false;
			dle.file_size = std::stoi(entryTokens.at(FILE_SIZE_INDEX));

			directoryList.push_back(dle);
		}

		return directoryList;
	}
	else
	{
		// TODO: Add implementation for folders...
	}

	throw std::runtime_error(RED "not implemented" RESET);
}
