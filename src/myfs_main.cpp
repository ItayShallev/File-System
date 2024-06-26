#include "blkdev.h"
#include "myfs.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>


std::vector<std::string> split_cmd(const std::string& cmd)
{
	std::stringstream ss(cmd);
	std::string part;
	std::vector<std::string> ans;

	while (std::getline(ss, part, ' '))
	{
		ans.push_back(part);
	}

	return ans;
}


static void recursive_print(MyFs& myfs, const std::string& path, const std::string& prefix="")
{
	MyFs::dir_list dlist = myfs.list_dir(path);

	for (size_t i=0; i < dlist.size(); i++)
	{
		MyFs::dir_list_entry &curr_entry = dlist[i];

		std::string entry_prefix = prefix;
		if (i == dlist.size()-1)
		{
			entry_prefix += "└── ";
		}
		else
		{
			entry_prefix += "├── ";
		}

		std::cout << entry_prefix << curr_entry.name << std::endl;

		if (curr_entry.is_dir)
		{
			std::string dir_prefix = prefix;

			if (i == dlist.size()-1)
			{
				dir_prefix += "    ";
			}
			else
			{
				dir_prefix += "│   ";
			}

			recursive_print(myfs, path + "/" + curr_entry.name, dir_prefix);
		}
	}
}


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << RED "Please provide the file to operate on" RESET << std::endl;
		return -1;
	}

	BlockDeviceSimulator *blkdevptr = new BlockDeviceSimulator(argv[1]);
	MyFs myfs(blkdevptr);
	bool exit = false;

	std::cout << GREEN << MENU_ASCII_ART << RESET << std::endl;
	std::cout << "To get help, please type 'help' on the prompt below." << std::endl;
	std::cout << std::endl;

	while (!exit)
	{
		try
		{
			std::string cmdline;
			std::cout << YELLOW << FS_NAME << "$ " << RESET;

			std::cout << GREEN;
			std::getline(std::cin, cmdline, '\n');
			std::cout << RESET;
			if (cmdline == std::string(""))
			{
				continue;
			}

			std::vector<std::string> cmd = split_cmd(cmdline);

			if (cmd[0] == LIST_CMD)
			{
				MyFs::dir_list dlist;
				if (cmd.size() == 1)
				{
					dlist = myfs.list_dir("/");
				}
				else if (cmd.size() == 2)
				{
					dlist = myfs.list_dir(cmd[1]);
				}
				else
				{
					std::cout << RED << LIST_CMD << ": one or zero arguments requested" RESET << std::endl;
				}

				for (size_t i=0; i < dlist.size(); i++)
				{
					std::cout << CYAN << std::setw(25) << std::left
						<< dlist[i].name + (dlist[i].is_dir ? "/":"")
						<< std::setw(10) << std::right
						<< BOLDYELLOW << dlist[i].file_size << RESET << std::endl;
				}
			}

			else if (cmd[0] == EXIT_CMD)
			{
				exit = true;
			}

			else if (cmd[0] == HELP_CMD)
			{
				printHelpMessage();
			}

			else if (cmd[0] == CREATE_FILE_CMD)
			{
				if (cmd.size() == 2)
				{
					myfs.create_file(cmd[1], false);
				}
				else
				{
					std::cout << RED << CREATE_FILE_CMD << ": file path requested" RESET << std::endl;
				}
			}

			else if (cmd[0] == CONTENT_CMD)
			{
				if (cmd.size() == 2)
				{
					std::cout << myfs.get_content(cmd[1]) << std::endl;
				}
				else
				{
					std::cout << RED << CONTENT_CMD << ": file path requested" RESET << std::endl;
				}
			}

			else if (cmd[0] == TREE_CMD)
			{
				recursive_print(myfs, "");
			}

			else if (cmd[0] == EDIT_CMD)
			{
				if (cmd.size() == 2)
				{
					std::cout << CYAN "> " RESET;
					std::string content;
					std::string curr_line;
					std::getline(std::cin, curr_line);
					while (curr_line != "")
					{
						std::cout << CYAN "> " RESET;
						content += curr_line + "\n";
						std::getline(std::cin, curr_line);
					}

					myfs.set_content(cmd[1], content);
				}
				else
				{
					std::cout << RED << EDIT_CMD << ": file path requested" RESET << std::endl;
				}
			}

			// else if (cmd[0] == CREATE_DIR_CMD)
			// {
			// 	if (cmd.size() == 2)
			// 	{
			// 		myfs.create_file(cmd[1], true);
			// 	}
			// 	else
			// 	{
			// 		std::cout << CREATE_DIR_CMD << ": one argument requested" << std::endl;
			// 	}
			// }

			else
			{
				std::cout << RED "Unknown command: " << cmd[0] << RESET << std::endl;
			}
		}
		
		catch (std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}
