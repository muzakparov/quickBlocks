/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-named",    "Show addresses from named accounts as per ethName"),
    COption("-scraper",  "Show addresses from scraper config.toml (if found in current folder"),
    COption("",          "Show the list of Ethereum accounts known to the local node or named accounts."),
};
static size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (arg == "-n" || arg == "--named") {
            fromNamed = true;

        } else if (arg == "-s" || arg == "--scraper") {
            if (!fileExists("./config.toml"))
                return usage("--scraper option requires a file ./config.toml exists in current folder. Quitting...");
            fromScraper = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            return usage("Invalid option '" + arg + "'. Quitting...");
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {

    paramsPtr = params;
    nParamsRef = nParams;
    pOptions = this;

    fromNamed = false;
    fromScraper = false;
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // Will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    // If you need the names file, you have to add it in the constructor
    namesFile = CFilename(configPath("names/names.txt"));
    establishFolder(namesFile.getPath());
    if (!fileExists(namesFile.getFullPath()))
        stringToAsciiFile(namesFile.getFullPath(),
                substitute(
                substitute(string_q(STR_DEFAULT_NAMEDATA), " |", "|"), "|", "\t"));
    loadNames();
    Init();
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "notes" && (verbose || COptions::isReadme))
        return "To customize this list add an [{extra_accounts}] section to the config file (see documentation).\n";
    return str;
}
