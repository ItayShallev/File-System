#include "Helper.h"


/**
 * @brief       Splits an entry into sections (file name, file address, file size) and returns it.
 * @param       entry       The entry to split into sections
 * @return      A vector of the entry sections
 */
std::vector<std::string> splitEntry(const std::string& entry)
{
    std::vector<std::string> tokens;
    std::istringstream ss(entry);
    std::string token;

    while (std::getline(ss, token, ENTRY_DELIMITER))
    {
        tokens.push_back(token);
    }

    return tokens;
}


/**
 * @brief       Prints a help message (menu).
 * @return      void
 */
void printHelpMessage()
{
    std::cout
            << std::setw(COLUMN_SPACING) << std::left << MAGENTA + LIST_CMD + "    <dir>"           << YELLOW "Lists directory content.\n"      RESET
            << std::setw(COLUMN_SPACING) << std::left << MAGENTA + CONTENT_CMD + "   <path>"        << YELLOW "Shows file content.\n"           RESET
            << std::setw(COLUMN_SPACING) << std::left << MAGENTA + CREATE_FILE_CMD + " <path>"      << YELLOW "Creates an empty file.\n"        RESET
            // << std::setw(COLUMN_SPACING) << std::left << RED + CREATE_DIR_CMD + " <path>"           << YELLOW "Creates an empty directory.\n"   RESET
            << std::setw(COLUMN_SPACING) << std::left << MAGENTA + EDIT_CMD + "  <path>"            << YELLOW "Re-sets file content.\n"         RESET
            << std::setw(COLUMN_SPACING) << std::left << MAGENTA + HELP_CMD                         << YELLOW "Shows this help message.\n"      RESET
            << std::setw(COLUMN_SPACING) << std::left << MAGENTA + EXIT_CMD                         << YELLOW "Gracefully exit.\n"              RESET;
}
