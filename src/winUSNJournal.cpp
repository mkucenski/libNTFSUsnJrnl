// Copyright 2017 Matthew A. Kucenski
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// #define _DEBUG_ 1

#include "winUSNJournal.h"
#include "usnJrnl.h"
#include "misc/debugMsgs.h"
#include "misc/endianSwitch.h"

winUSNJournal::winUSNJournal(string strFilename)	:	binDataFile(strFilename) {
	DEBUG_INFO("winUSNJournal::winUSNJournal()");
	if (skipNullBlocks(8, NULL) < 0) {
		DEBUG_ERROR("winUSNJournal:winUSNJournal() Unable to find non-null start of data!");
	}
}

winUSNJournal::~winUSNJournal() {
	DEBUG_INFO("winUSNJournal::~winUSNJournal()");
}

USNJRNL_RV winUSNJournal::getNextRecord(winUSNRecord** pp_clUSNRecord) {
	USNJRNL_RV rv = USNJRNL_ERROR;
		
	if (pp_clUSNRecord && *pp_clUSNRecord == NULL) {

		USN_RECORD_VER2 stUSNRecord;
		string strFilename;
		u_int64_t cDataRead = 0;
		u_int64_t posFile = currPos();

		if (getData((char*)&stUSNRecord, USN_RECORD_VER2_BASE_LENGTH, &cDataRead) >=0 && cDataRead == USN_RECORD_VER2_BASE_LENGTH) {
			LITTLETOHOST32(stUSNRecord.cRecordLen);
			LITTLETOHOST16(stUSNRecord.vMajorVer);
			LITTLETOHOST16(stUSNRecord.vMinorVer);
			LITTLETOHOST64(stUSNRecord.idFileRefNum);
			LITTLETOHOST64(stUSNRecord.idParentRefNum);
			LITTLETOHOST64(stUSNRecord.idUSN);
			LITTLETOHOST64(stUSNRecord.dtmTimestamp);
			LITTLETOHOST32(stUSNRecord.fxReasons);
			LITTLETOHOST32(stUSNRecord.fxSources);
			LITTLETOHOST32(stUSNRecord.idSecurityID);
			LITTLETOHOST32(stUSNRecord.fxFileAttrs);
			LITTLETOHOST16(stUSNRecord.cFilenameLen);
			LITTLETOHOST16(stUSNRecord.posFilename);

			DEBUG_INFO("winUSNJournal::getNextRecord() posFile=" << posFile << ", cRecordLen=" << stUSNRecord.cRecordLen << ", vMajorVer=" << stUSNRecord.vMajorVer << ", vMinorVer=" << stUSNRecord.vMinorVer << ", USN=" << stUSNRecord.idUSN);

			if (	stUSNRecord.cRecordLen <= (USN_RECORD_VER2_BASE_LENGTH + sizeof(char) * 2 * WIN_MAX_PATH) &&
					stUSNRecord.vMajorVer == 2 &&
					stUSNRecord.vMinorVer == 0) {

				if (getTwoByteCharString(&strFilename, stUSNRecord.cFilenameLen/2, false) >= 0) {
					DEBUG_INFO("winUSNJournal::getNextRecord() strFilename='" << strFilename << "' (offset=" << stUSNRecord.posFilename << " len=" << stUSNRecord.cFilenameLen << ")");
					*pp_clUSNRecord = new winUSNRecord(stUSNRecord, strFilename, posFile);
					rv = USNJRNL_SUCCESS;

					// Can't rely on the various read functions above to position the pointer in the right place for the next record.
					// Also can't assume there won't be a sparse set of null data between records that has to be skipped.
					if (movePos(posFile + stUSNRecord.cRecordLen) >=0 && skipNullBlocks(8, NULL) < 0) {
						DEBUG_ERROR("winUSNJournal:getNextRecord() Unable to find non-null data!");
					}
				} else {
					DEBUG_ERROR("winUSNJournal::getNextRecord() Unable to read filename string.");
				}
			} else {
				DEBUG_ERROR("winUSNJournal::getNextRecord() Invalid USN record components.");
			}
		} else {
			DEBUG_ERROR("winUSNJournal::getNextRecord() Unable to read USN_RECORD_VER2 (cDataRead=" << cDataRead << ")." << "\n" << (char*)&stUSNRecord);
		}
	} else {
		DEBUG_ERROR("winUSNJournal::getNextRecord() Invalid destination pointer.");
	}
	
	return rv;
}
