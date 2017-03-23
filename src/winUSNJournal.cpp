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

#include "winUSNJournal.h"
#include "usnJrnl.h"
#include "misc/debugMsgs.h"
#include "misc/endianSwitch.h"

typedef enum { USNJRNL_ERROR, USNJRNL_SUCCESS } USNJRNL_RV;

winUSNJournal::winUSNJournal(string strFilename)	:	binDataFile(strFilename),
																		m_posFileData(0) {
	DEBUG_INFO("winUSNJournal::winUSNJournal()");
	if (findNonNull(&m_posFileData, m_posFileData) >= 0) {	//Start at beginning of file and update m_posFileData when non-null found
		DEBUG_INFO("winUSNJournal::winUSNJournal() Successfully found non-null start of data!");
	} else {
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
		u_int32_t cDataRead = 0;

		if (getData((char*)&stUSNRecord, USN_RECORD_VER2_BASE_LENGTH, &cDataRead) && cDataRead == USN_RECORD_VER2_BASE_LENGTH) {

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

			if (	stUSNRecord.cRecordLen <= (USN_RECORD_VER2_BASE_LENGTH + sizeof(char) * 2 * WIN_MAX_PATH) &&
					stUSNRecord.vMajorVer == 2 &&
					stUSNRecord.vMinorVer == 0 &&
					stUSNRecord.idUSN == m_posFileData) {

				if (getTwoByteCharString(&strFilename, m_posFileData + stUSNRecord.posFilename, stUSNRecord.cFilenameLen, false)) {
					*pp_clUSNRecord = new winUSNRecord(stUSNRecord, strFilename, m_posFileData);
					m_posFileData += stUSNRecord.cRecordLen;
					rv = USNJRNL_SUCCESS;
				} else {
					DEBUG_ERROR("winUSNJournal::getNextRecord() Unable to read filename string.");
				}
			} else {
				DEBUG_ERROR("winUSNJournal::getNextRecord() Invalid USN record components.");
			}
		} else {
			DEBUG_ERROR("winUSNJournal::getNextRecord() Unable to read USN_RECORD_VER2.");
		}
	} else {
		DEBUG_ERROR("winUSNJournal::getNextRecord() Invalid destination pointer.");
	}
	
	return rv;
}
