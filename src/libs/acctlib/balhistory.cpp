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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include <algorithm>
#include "balhistory.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBalHistory, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextBalhistoryChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextBalhistoryChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CBalHistory::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBalhistoryChunk, this);
}

//---------------------------------------------------------------------------
string_q nextBalhistoryChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CBalHistory *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBalHistory::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "balance" ) { balance = str_2_Wei(fieldValue); return true; }
            break;
        case 'r':
            if ( fieldName % "recordID" ) { recordID = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = str_2_Ts(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBalHistory::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBalHistory::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> recordID;
    archive >> timestamp;
    archive >> balance;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBalHistory::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << recordID;
    archive << timestamp;
    archive << balance;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBalHistoryArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CBalHistoryArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CBalHistory::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CBalHistory, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBalHistory, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBalHistory, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBalHistory, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CBalHistory, "recordID", T_TEXT, ++fieldNum);
    ADD_FIELD(CBalHistory, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CBalHistory, "balance", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBalHistory, "schema");
    HIDE_FIELD(CBalHistory, "deleted");
    HIDE_FIELD(CBalHistory, "showing");
    HIDE_FIELD(CBalHistory, "cname");

    builtIns.push_back(_biCBalHistory);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBalhistoryChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CBalHistory *bal = reinterpret_cast<const CBalHistory *>(dataPtr);
    if (bal) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, bal);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CBalHistory::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CBalHistory& bal) {
    bal.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBalHistory& bal) {
    bal.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CBalHistory::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextBalhistoryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "balance" ) return bni_2_Str(balance);
            break;
        case 'r':
            if ( fieldName % "recordID" ) return recordID;
            break;
        case 't':
            if ( fieldName % "timestamp" ) return ts_2_Str(timestamp);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBalHistory& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

