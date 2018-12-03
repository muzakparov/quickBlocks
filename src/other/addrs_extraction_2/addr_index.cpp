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
#include "etherlib.h"

//-----------------------------------------------------------------------------
extern bool cutFiles(const string_q& path, void *data);
extern bool writeRecord(const char *line, void *data);

#define SORT_THE_FILES
#ifdef SORT_THE_FILES
//-----------------------------------------------------------------------------
class CSortHolder {
public:
    CStringArray lines;
    CSortHolder(void) { lines.reserve(70000000); }
};

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    colorsOff();
    CSortHolder thing;
    forEveryFileInFolder(blockCachePath("addr_index/unsorted_by_block_temp"), cutFiles, &thing);
    return 0;
}

//-----------------------------------------------------------------------------
bool cutFiles(const string_q& path, void *data) {

    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*", cutFiles, data);

    } else {

        if (endsWith(path, ".txt")) {
            CSortHolder *thing = (CSortHolder*)data;
            cerr << "Visiting " << path << "...";
            forEveryLineInAsciiFile(path, writeRecord, data);
            CArchive output(READING_ARCHIVE);
            string_q filename = substitute(path, "/unsorted_by_block_temp", "/sorted_by_addr");
            if (!output.Lock(filename, asciiWriteCreate, LOCK_NOWAIT))
                return false;
            cerr << "\nSorting...";
            sort(thing->lines.begin(), thing->lines.end());
            cerr << "Writing...";
            for (auto line : thing->lines)
                output.WriteLine(line);
            cerr << "Done\n";
            output.Release();
            thing->lines.clear();
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
bool writeRecord(const char *line, void *data) {
    CSortHolder *thing = (CSortHolder*)data;
    thing->lines.push_back(line);
    return true;
}
#endif

//#define CUT_TO_SIZE
#ifdef CUT_TO_SIZE
//-----------------------------------------------------------------------------
class CFileSplitter {
public:
    string_q outputFilename;
    string_q sourceFile;
    CArchive *output;
    blknum_t prevBlock;
    uint64_t fileSize;
    CFileSplitter(void) { output = NULL; prevBlock = NOPOS; fileSize = 0; }
    bool isFileFull(blknum_t bn) {
        if (fileSize >= 50000000 && bn != prevBlock) {
            prevBlock = bn;
            fileSize = 0;
            return true;
        }
        return false;
    }
};

//-----------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    colorsOff();
    CFileSplitter thing;
    forEveryFileInFolder(blockCachePath("addr_index/unsorted_by_block"), cutFiles, &thing);
    return 0;
}

//-----------------------------------------------------------------------------
bool cutFiles(const string_q& path, void *data) {

    if (endsWith(path,'/')) {
        return forEveryFileInFolder(path+"*", cutFiles, data);

    } else {

        if (endsWith(path, ".txt")) {
            CFileSplitter *thing = (CFileSplitter*)data;
            thing->sourceFile = path;
            forEveryLineInAsciiFile(path, writeRecord, thing);
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
bool writeRecord(const char *line, void *data) {

    CFileSplitter *thing = (CFileSplitter*)data;

    blknum_t bn = str_2_Uint(line);
    if (!thing->output || thing->isFileFull(bn)) {
        string_q newPath = blockCachePath("addr_index/unsorted_by_block_temp/") + padLeft(uint_2_Str(bn), 9, '0') + ".txt";
        if (!thing->output || newPath != thing->outputFilename) {
            thing->outputFilename = newPath;
            if (thing->output) {
                thing->output->Release();
                delete thing->output;
                thing->output = NULL;
            }
            thing->output = new CArchive(READING_ARCHIVE);
            if (!thing->output) {
                cerr << "Could allocate file pointer " << thing->outputFilename << "\n";
                return false;
            }
            cerr << "\tOpening file " << thing->outputFilename << "\n";
            if (!thing->output->Lock(thing->outputFilename, asciiWriteCreate, LOCK_NOWAIT)) {
                cerr << "Could not open file " << thing->outputFilename << "\n";
                return false;
            }
        }
    }

    size_t len = strlen(line);
    thing->fileSize += len;
    thing->prevBlock = bn;

    static uint64_t counter = 0;
    if (strstr(line, "0x") == line) {
        thing->output->WriteLine(line);
        if (!(++counter%71)) {
            ((char*)line)[len-1] = '\r';
            cerr << "\t" << cGreen << thing->fileSize << ": " << cTeal << line << cOff;
        }
    } else {
        CStringArray fields;
        explode(fields, line, '\t');
        ostringstream os;
        os << fields[3] << "\t" << padLeft(fields[0], 9, '0') << "\t" << padLeft(fields[1], 5, '0');
        string_q str = os.str();
        thing->output->WriteLine(str + "\n");
        if (!(++counter%71)) {
            cerr << "\t" << cGreen << thing->fileSize << ": " << cTeal << str << "\r" << cOff;
        }
    }
    thing->output->flush();
    cerr.flush();

    return true;
}
#endif