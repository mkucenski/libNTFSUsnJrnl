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
																							m_uiDataStartPos(0) {
	DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile()");
	if (findNonNull(&m_uiDataStartPos) >= 0) {
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
 		char* buffer = (char*)calloc(USN_RECORD_VER2_BASE_LENGTH, 1);
		if (buffer) {
			if (getData(buffer, USN_RECORD_VER2_BASE_LENGTH, m_uiDataStartPos, NULL) >= 0) {
				*ppUsnJrnlRecord = new winUsnJrnlRecord(this, buffer, m_uiDataStartPos, dwLength);
				m_iNextRecordPos += dwLength;
				rv = WIN_USNJRNL_SUCCESS;
			} else {
				DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Failure reading next record.");
			}
			free(buffer);
			buffer = NULL;
		} else {
			DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Unable to allocate buffer for record storage.");
		}
	} else {
		DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Invalid destination pointer.");
	}

	
	return rv;
}
