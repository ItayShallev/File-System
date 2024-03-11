#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

const std::string FS_NAME = "myfs";

#define MAX_FILE_NAME       20
#define MAX_FILE_SIZE       1024

// Table Entries
#define ENTRY_DELIMITER     '|'
#define ENTRY_NOT_FOUND     "-1"

#define FILE_NAME_INDEX     0
#define FILE_ADDRESS_INDEX  1
#define FILE_SIZE_INDEX     2

// Console colors
#define RESET               "\033[0m"
#define RED                 "\033[31m"
#define GREEN               "\033[32m"
#define YELLOW              "\033[33m"
#define BLUE                "\033[34m"
#define MAGENTA             "\033[35m"
#define CYAN                "\033[36m"
#define WHITE               "\033[37m"
#define BOLDYELLOW          "\033[1m\033[33m"

#define COLUMN_SPACING      20

// Commands
const std::string LIST_CMD 			= "ls";
const std::string CONTENT_CMD 		= "cat";
const std::string CREATE_FILE_CMD 	= "touch";
// const std::string CREATE_DIR_CMD 	= "mkdir";
const std::string EDIT_CMD 			= "edit";
const std::string TREE_CMD 			= "tree";
const std::string HELP_CMD 			= "help";
const std::string EXIT_CMD 			= "exit";

const int FILE_COUNT_ADDRESS    = 5;
const int FILE_COUNT_SIZE       = 11;
const int FILE_SIZE             = 1024;

const int TABLE_START_ADDRESS   = 16;
const int TABLE_END_ADDRESS     = 1024;
const int TABLE_ENTRY_SIZE      = 32;


const std::string MENU_ASCII_ART = 
    "                   $$$$$$$$$\\$$\\ $$\\                  $$$$$$\\                        $$\\                             \n"
    "                   $$  _____|\\__|$$ |                $$  __$$\\                       $$ |                            \n"
    "                   $$ |      $$\\ $$ | $$$$$$\\        $$ /  \\__|$$\\   $$\\  $$$$$$$\\ $$$$$$\\    $$$$$$\\  $$$$$$\\$$$$\\  \n"
    "                   $$$$$$\\   $$ |$$ |$$  __$$\\       \\$$$$$$\\  $$ |  $$ |$$  _____|\\_$$  _|  $$  __$$\\ $$  _$$  _$$\\ \n"
    "                   $$  __|   $$ |$$ |$$$$$$$$ |       \\____$$\\ $$ |  $$ |\\$$$$$$\\    $$ |    $$$$$$$$ |$$ / $$ / $$ |\n"
    "                   $$ |      $$ |$$ |$$   ____|      $$\\   $$ |$$ |  $$ | \\____$$\\   $$ |$$\\ $$   ____|$$ | $$ | $$ |\n"
    "                   $$ |      $$ |$$ |\\$$$$$$$\\       \\$$$$$$  |\\$$$$$$$ |$$$$$$$  |  \\$$$$  |\\$$$$$$$\\ $$ | $$ | $$ |\n"
    "                   \\__|      \\__|\\__| \\_______|       \\______/  \\____$$ |\\_______/    \\____/  \\_______|\\__| \\__| \\__|\n"
    "                                                               $$\\   $$ |                                            \n"
    "                                                               \\$$$$$$  |                                            \n"
    "                                                                \\______/                                             \n";


std::vector<std::string> splitEntry(const std::string& entry);

void printHelpMessage();
