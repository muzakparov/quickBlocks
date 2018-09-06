/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//---------------------------------------------------------------------------------------------------
static COption params[] = {
    COption("-lo(g)s",       "display smart contract lo(g)s or events"),
    COption("-trace",        "display smart contract internal traces"),
    COption("-accounting",   "display credits and debits per account and reconcile at each block"),
    COption("-fmt:<fmt>",    "export format (on of [json|txt|csv] (ignored if trans_fmt is non-empty)"),
    COption("-bals",         "if a balance does not reconcile, export a message to a file"),
    COption("-kBlock:<num>", "start processing at block :k"),
    COption("",              "Index transactions for a given Ethereum address (or series of addresses).\n"),
};
static size_t nParams = sizeof(params) / sizeof(COption);

extern const char* defTransFmt;
extern const char* defTraceFmt;
extern string_q cleanFmt(const string_q& str);
extern void manageFields(const string_q& listIn, bool show);
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    while (!command.empty()) {
        string_q arg = nextTokenClear(command, ' ');
        if (startsWith(arg, "-k:") || startsWith(arg, "--kBlock:")) {

            arg = substitute(substitute(arg, "-k:", ""), "--kBlock:", "");
            if (!isNumeral(arg)) {
                cerr << substitute(usageStr("You must specify a block number (" + arg + ")"), "\n", "\n");
                return false;
            }
            kBlock = str_2_Uint(arg);

        } else if (arg == "-a" || arg == "--accounting") {
            accounting_on = true;

        } else if (arg == "-g" || arg == "--logs") {
            logs_on = true;

        } else if (arg == "-t" || arg == "--trace") {
            trace_on = true;

        } else if (arg == "-b" || arg == "--bals") {
            report_bals = true;

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {

            arg = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
            if (arg != "json" && arg != "txt" && arg != "csv")
                return usage("Export format must be one of [ json | txt | csv ]. Quitting...");
            defaultFmt = arg;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }
        }
    }

    kBlock = max((blknum_t)0, kBlock);
    if (!fileExists("./config.toml"))
        return usage("The config.toml file was not found. Are you in the right folder? Quitting...\n");

    CToml toml("./config.toml");
    if (!loadWatches(toml))
        return false;

    accounting_on  = toml.getConfigBool("display", "accounting",  false) || accounting_on;
    logs_on        = toml.getConfigBool("display", "logs",        false) || logs_on;
    trace_on       = toml.getConfigBool("display", "trace",       false) || trace_on;
    autocorrect_on = toml.getConfigBool("display", "autocorrect", false) || autocorrect_on;
//    json_on        = toml.getConfigBool("display", "json",        false) || json_on;
    hideFields     = toml.getConfigStr ("fields",  "hide",        "");
    showFields     = toml.getConfigStr ("fields",  "show",        "");

    // If we're not told to use Json, then we use format strings. No format string == export as json
    if (!(defaultFmt == "json")) {
        transFmt   = cleanFmt(toml.getConfigStr ("formats", "trans_fmt",   defTransFmt));
        traceFmt   = cleanFmt(toml.getConfigStr ("formats", "trace_fmt",   defTraceFmt));
    } else {
        accounting_on = trace_on = logs_on = false;
//        colorsOff();
    }

    replaceAll(transFmt, "{ISERROR}", "ERROR:{b:ISERROR}");
    replaceAll(traceFmt, "{ISERROR}", "ERROR:{b:ISERROR}");

    if (!(defaultFmt == "json")) {
//        cout << "Only --json option currently works. Please add --json to command line.\n";
//        return false;
//
    } else {
        // show certain fields and hide others
const char *defHide =
    "CTransaction: nonce, input"
"|" "CLogEntry: data, topics"
"|" "CTrace: blockHash, blockNumber, transactionHash, transactionPosition, traceAddress, subtraces"
"|" "CTraceAction: init"
"|" "CTraceResult: code"
"|" "CFunction: constant, payable, outputs"
"|" "CParameter: indexed, isPointer, isArray, isObject";

const char *defShow =
    "CTransaction: price, gasCost, articulatedTx, traces, isError, date, ether"
"|" "CLogEntry: articulatedLog"
"|" "CTraceAction: "
"|" "CTraceResult: "
"|" "CFunction: "
"|" "CParameter: ";

        manageFields(defHide, false);
        manageFields(defShow, true);
        manageFields(hideFields, false);
        manageFields(showFields, true);
    }

    theStartBlock = blk_minWatchBlock - 1;
    if (kBlock) {
        if (kBlock > theStartBlock)
            theStartBlock = kBlock;
        // Since we're not starting at the beginning, we have to
        // get balances for accounting to work
        for (uint32_t i = 0 ; i < watches.size() ; i++)
            watches.at(i).qbis.endBal = getNodeBal(watches.at(i).balanceHistory, watches.at(i).address, theStartBlock);
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    paramsPtr = params;
    nParamsRef = nParams;

    theStartBlock = 0;
    accounting_on = false;
    logs_on = false;
    trace_on = false;
    autocorrect_on = false;
    report_bals = false;
    defaultFmt = "json";
    hideFields = "";
    showFields = "";
    kBlock = 0;

    blk_nBlocks = 0;
    blk_firstBlock = 0;
    blk_lastBlock = 0;
    blk_minWatchBlock = 0;
    blk_maxWatchBlock = UINT32_MAX;

    tx_nAccountedFor = 0;
    tx_nDisplayed = 0;

    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : transFmt(""), traceFmt("") {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//-----------------------------------------------------------------------
string_q cleanFmt(const string_q& str) {
    return (
//            substitute(
                       substitute(
                                  substitute(
                                             substitute(
//                                                        substitute(
                                                                   (str),
//                                                                   "\\n\\\n", "\\n"),
                                                        "\n", ""),
                                             "\\n", "\n"),
                                  "\\t", "\t")
//            ,
//                       "\\r", "\r")
            );
}

//-----------------------------------------------------------------------
void manageFields(const string_q& listIn, bool show) {
    string_q list = substitute(listIn, " ", "");
    while (!list.empty()) {
        string_q fields = nextTokenClear(list, '|');
        string_q cl = nextTokenClear(fields, ':');
        CBaseNode *item =  createObjectOfType(cl);
        while (item && !fields.empty()) {
            string_q fieldName = nextTokenClear(fields, ',');
            if (fieldName == "all") {
                if (show) {
                    item->getRuntimeClass()->showAllFields();
                } else {
                    item->getRuntimeClass()->hideAllFields();
                }
            } else if (fieldName == "none") {
                if (show) {
                    item->getRuntimeClass()->hideAllFields();
                } else {
                    item->getRuntimeClass()->showAllFields();
                }
            } else {
                CFieldData *f = item->getRuntimeClass()->findField(fieldName);
                if (f)
                    f->setHidden(!show);
            }
        }
        delete item;
    }
}

//-----------------------------------------------------------------------
//const char* defTransFmt = "{ \"date\": \"[{DATE}]\", \"from\": \"[{FROM}]\", \"to\": \"[{TO}]\", \"value\": \"[{VALUE}]\" }";
const char* defTransFmt = "+=+ \"date\": \"[{DATE}]\", \"from\": \"[{FROM}]\", \"to\": \"[{TO}]\", \"value\": \"[{VALUE}]\" =+=";
const char* defTraceFmt = "{ \"[{DATESH}]\", \"[{TIME}]\", \"[{FROM}]\", \"[{TO}]\", \"[{VALUE}]\" }";
