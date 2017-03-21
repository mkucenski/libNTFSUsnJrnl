// Copyright 2007 Matthew A. Kucenski
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
																							m_lNextRecordPos(0) {
	DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile()");

	int iBufferCount = 1024;
	DWORD[iBufferCount] dwBuffer;
	long lBufferPos = 0;
	bool bFound = false;

	DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile() Reading from <" << strFilename << "> to find start of non-null data.");
	while (!bFound) {
		if (getData(&dwBuffer, iBufferCount * sizeof(DWORD), lBufferPos, NULL) >= 0) {
			lBufferPos += iBufferCount * sizeof(DWORD);
			DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile() Parsing read buffer to find start of non-null data.");
			for (int i=0; i<iBufferCount; i++) {
				if (dwBuffer[i] != 0) {
					bFound = true; 
					lBufferPos += i * sizeof(DWORD);
					DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile() Found non-null data at position <" << lBufferPos << ">.");
					break;
				}
			}
		} else {
			DEBUG_ERROR("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile() Failed reading buffer data.");
		}
	}

	if (bFound) {
		DEBUG_INFO("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile() Found valid data, setting m_lNextRecordPos = " << lBufferPos << ".");
		m_lNextRecordPos = lBufferPos;
	} else {
		DEBUG_ERROR("winUsnJrnlRecordsFile::winUsnJrnlRecordsFile() Unable to find start of valid data.");
		m_lNextRecordPos = -1;
	}
}

winUsnJrnlRecordsFile::~winUsnJrnlRecordsFile() {
	DEBUG_INFO("winUsnJrnlRecordsFile::~winUsnJrnlRecordsFile()");
}

WIN_USNJRNL_RV winUsnJrnlRecordsFile::getNextRecord(winUsnJrnlRecord** ppUsnJrnlRecord) {
	WIN_USNJRNL_RV rv = WIN_USNJRNL_ERROR;
		
	if (ppUsnJrnlRecord && *ppUsnJrnlRecord == NULL) {
		if (m_lNextRecordPos != -1) {
		} else {
			DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Invalid record position.");
		}
	} else {
		DEBUG_ERROR("winUsnJrnlRecordsFile::getNextRecord() Invalid destination pointer.");
	}

	return rv;

		DWORD dwLength = 0;
		WORD wMajorVer = 0;
		WORD wMinorVer = 0;

		if (getData(&dwLength, sizeof(DWORD), m_lNextRecordPos, NULL) >= 0 && 
			 getData(&wMajorVer, sizeof(WORD), NULL) >= 0 &&
			 getData(&wMinorVer, sizeof(WORD), NULL) >= 0) {
			LITTLETOHOST32(dwLength);
			LITTLETOHOST32(wMajorVer);
			LITTLETOHOST32(wMinorVer);
			
			// For event records, there is a clear header ID that can be searched for. In this case, I am searching DWORD by DWORD for that
			// header. For UsnJrnl records, there is no clear header. The first DWORD is the length of the record. However, the $J file
			// can potentially have a significant amount of null data prior to the first record... how do we scan past it quickly?
			bool bFound = false;
			
			if (dwHeaderID == EVENTLOGRECORD_HEADER_ID) {
				bFound = true;
			} else {
				DEBUG_INFO("winEventFile::getNextRecord() Next record was not found in the expected location.  Searching...");
				while (!bFound) {
					if (getData(&dwHeaderID, sizeof(DWORD), NULL) >= 0) {
						LITTLETOHOST32(dwHeaderID);
						
						if (dwHeaderID == EVENTLOGRECORD_HEADER_ID) {
							DEBUG_INFO("winEventFile::getNextRecord() Next record found at offset: " << offset() - 8 << " (diff = " << ((offset() - 8) - m_lNextRecordPos) << ")");
							bFound = true;
							m_lNextRecordPos = offset() - 8;
						} else {
							dwLength = dwHeaderID;
						}
					} else {
						DEBUG_ERROR("winEventFile::getNextRecord() Failure reading data while trying to find record header.");
						break;
					}
				}
			}
			
			if (bFound) {
 				char* buffer = (char*)calloc(EVENTLOGRECORD_LENGTH, 1);
				if (buffer) {
					if (getData(buffer, EVENTLOGRECORD_LENGTH, m_lNextRecordPos, NULL) >= 0) {
						*ppEvent = new winEvent(this, buffer, m_lNextRecordPos, dwLength);
						m_lNextRecordPos += dwLength;
						rv = WIN_EVENT_SUCCESS;
					} else {
						DEBUG_ERROR("winEventFile::getNextRecord() Failure reading next record.");
					}
					free(buffer);
					buffer = NULL;
				} else {
					DEBUG_ERROR("winEventFile::getNextRecord() Unable to allocate buffer for record storage.");
				}
			} else {
				DEBUG_ERROR("winEventFile::getNextRecord() Unable to find next record.");
			}
		} else {
			DEBUG_ERROR("winEventFile::getNextRecord() Failed reading length/header.");
		}
	} else {
		DEBUG_ERROR("winEventFile::getNextRecord() Invalid destination pointer.");
	}
	
	return rv;
}
