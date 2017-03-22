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

#include "winUsnJrnlRecordsFile.h"

#include "usnJrnl.h"
#include "misc/debugMsgs.h"
#include "misc/endianSwitch.h"

winUsnJrnlRecordsFile::winUsnJrnlRecordsFile(string strFilename)	:	binDataFile(strFilename),
																							m_uiDataPos(0) {
	DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile()");
	if (findNonNull(&m_uiDataPos, m_uiDataPos) >= 0) {	//Start at beginning of file and update m_uiDataPos when non-null found
		DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile() Successfully found non-null start of data!");
	} else {
		DEBUG_ERROR("winUsnJrnlRecordsFile:winUsnJrnlRecordsFile() Unable to find non-null start of data!");
	}
}

winUsnJrnlRecordsFile::~winUsnJrnlRecordsFile() {
	DEBUG_INFO("winUsnJrnlRecordsFile::~winUsnJrnlRecordsFile()");
}

WIN_USNJRNL_RV winUsnJrnlRecordsFile::getNextRecord(winUsnJrnlRecord** ppUsnJrnlRecord) {
	WIN_USNJRNL_RV rv = WIN_USNJRNL_ERROR;
		
	if (ppUsnJrnlRecord && *ppUsnJrnlRecord == NULL) {

		USN_RECORD_VER2 usnJrnlRecord;
		string strFilename;
		u_int32_t uiSizeRead = 0;

		if (getData((char*)&usnJrnlRecord, USN_RECORD_VER2_BASE_LENGTH, &uiSizeRead) && uiSizeRead == USN_RECORD_VER2_BASE_LENGTH) {

			LITTLETOHOST32(usnJrnlRecord.dwRecordLen);
			LITTLETOHOST16(usnJrnlRecord.wMajorVer);
			LITTLETOHOST16(usnJrnlRecord.wMinorVer);
			LITTLETOHOST64(usnJrnlRecord.dwlFileRefNum);
			LITTLETOHOST64(usnJrnlRecord.dwlParentRefNum);
			LITTLETOHOST64(usnJrnlRecord.dwlUSN);
			LITTLETOHOST64(usnJrnlRecord.dwlTimestamp);
			LITTLETOHOST32(usnJrnlRecord.dwReason);
			LITTLETOHOST32(usnJrnlRecord.dwSourceInfo);
			LITTLETOHOST32(usnJrnlRecord.dwSecurityID);
			LITTLETOHOST32(usnJrnlRecord.dwFileAttributes);
			LITTLETOHOST16(usnJrnlRecord.wFilenameLen);
			LITTLETOHOST16(usnJrnlRecord.wFilenameOffset);

			if (	usnJrnlRecord.dwRecordLen <= (USN_RECORD_VER2_BASE_LENGTH + sizeof(char) * 2 * WIN_MAX_PATH) &&
					usnJrnlRecord.wMajorVer == 2 &&
					usnJrnlRecord.wMinorVer == 0 &&
					usnJrnlRecord.dwlUSN == m_uiDataPos) {

				if (getTwoByteCharString(&strFilename, m_uiDataPos + usnJrnlRecord.wFilenameOffset, usnJrnlRecord.wFilenameLen, false)) {
					*ppUsnJrnlRecord = new winUsnJrnlRecord(usnJrnlRecord, strFilename, m_uiDataPos);
					m_uiDataPos += usnJrnlRecord.dwRecordLen;
				} else {
					DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Unable to read filename string.");
				}
			} else {
				DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Invalid USN record components.");
			}
		} else {
			DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Unable to read USN_RECORD_VER2.");
		}
	} else {
		DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Invalid destination pointer.");
	}
	
	return rv;
}
