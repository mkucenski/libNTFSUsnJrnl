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

#include "winUsnJrnlRecord.h"
#include "winUsnJrnlRecordsFile.h"
#include "misc/debugMsgs.h"
#include "misc/endianSwitch.h"

#include <sstream>
using namespace std;

winUsnJrnlRecord::winUsnJrnlRecord(winUsnJrnlRecordsFile* pUsnJrnlRecordsFile, char* pData, long lOffset, DWORD dwLength)
		:	m_pUsnJrnlRecordsFile(pUsnJrnlRecordsFile),
			m_uiOffset(lOffset) {

	memset(&m_usnJrnlRecord, 0, USN_RECORD_VER2_BASE_LENGTH);	
	memcpy(&m_usnJrnlRecord, pData, (dwLength < USN_RECORD_VER2_BASE_LENGTH ? dwLength : USN_RECORD_VER2_BASE_LENGTH));
	
	LITTLETOHOST32(m_usnJrnlRecord.dwRecordLen);
	LITTLETOHOST16(m_usnJrnlRecord.wMajorVer);
	LITTLETOHOST16(m_usnJrnlRecord.wMinorVer);
	LITTLETOHOST64(m_usnJrnlRecord.dwlFileRefNum);
	LITTLETOHOST64(m_usnJrnlRecord.dwlParentFileRefNum);
	LITTLETOHOST64(m_usnJrnlRecord.dwlUSN);
	LITTLETOHOST64(m_usnJrnlRecord.liTimestamp);
	LITTLETOHOST32(m_usnJrnlRecord.dwReason);
	LITTLETOHOST32(m_usnJrnlRecord.dwSrcInfo);
	LITTLETOHOST32(m_usnJrnlRecord.dwSecId);
	LITTLETOHOST32(m_usnJrnlRecord.dwFileAttributes);
	LITTLETOHOST16(m_usnJrnlRecord.wFileNameLen);
	LITTLETOHOST16(m_usnJrnlRecord.wFileNameOffset);
	
	if (dwLength > USN_RECORD_VER2_BASE_LENGTH) {
		m_pUsnJrnlRecordsFile->getTwoByteCharString(&m_strFilename, m_uiOffset + USN_RECORD_VER2_BASE_LENGTH, 0, true);
	} else {
		DEBUG_ERROR("winUsnJrnlRecord::winUsnJrnlRecord() Invalid length value.");
	}
}

winUsnJrnlRecord::~winUsnJrnlRecord() {
}

u_int16_t winUsnJrnlRecord::getVersion(u_int16_t* pMajorVer, u_int16_t* pMinorVer) {
	if (pMajorVer != NULL) { *pMajorVer = m_usnJrnlRecord.wMajorVer; }
	if (pMinorVer != NULL) { *pMinorVer = m_usnJrnlRecord.wMinorVer; }
	return m_usnJrnlRecord.wMajorVer;
}

u_int64_t winUsnJrnlRecord::getMFT(u_int64_t* pFileNumber, u_int16_t* pSequence) {
	if (pFileNumber != NULL && pSequence != NULL) {
		*pFileNumber = m_usnJrnlRecord.dwlFileRefNum >> 16;
		*pSequence = m_usnJrnlRecord.dwlFileRefNum & 0xFFFF;
	}
	return m_usnJrnlRecord.dwlFileRefNum;
}

u_int64_t winUsnJrnlRecord::getParentMFT(u_int64_t* pParentFileNumber, u_int16_t* pParentSequence) {
	if (pParentFileNumber != NULL && pParentSequence != NULL) {
		*pParentFileNumber = m_usnJrnlRecord.dwlFileRefNum >> 16;
		*pParentSequence = m_usnJrnlRecord.dwlFileRefNum & 0xFFFF;
	}
	return m_usnJrnlRecord.dwlParentFileRefNum;
}

string winUsnJrnlRecord::getReasonStr(u_int32_t* pReasonFlags, u_int32_t* pUnknownReasonFlags) {
	if (pReasonFlags) { *pReasonFlags = m_usnJrnlRecord.dwReason; }
	if (pUnknownReasonFlags) { *pUnknownReasonFlags = findUnknownCodes(m_usnJrnlRecord.dwReason, USNJRNL_REASONS, sizeof(USNJRNL_REASONS)); }
	return getMessages(m_usnJrnlRecord.dwReason, USNJRNL_REASONS, sizeof(USNJRNL_REASONS));
}

string winUsnJrnlRecord::getSourceInfoStr(u_int32_t* pSourceFlags, u_int32_t* pUnknownSourceFlags) {
	if (pSourceFlags) { *pSourceFlags = m_usnJrnlRecord.dwSrcInfo; }
	if (pUnknownSourceFlags) { *pUnknownSourceFlags = findUnknownCodes(m_usnJrnlRecord.dwSrcInfo, USNJRNL_SOURCES, sizeof(USNJRNL_SOURCES)); }
	return getMessages(m_usnJrnlRecord.dwSrcInfo, USNJRNL_SOURCES, sizeof(USNJRNL_SOURCES));
}

string winUsnJrnlRecord::getFileAttributesStr(u_int32_t* pFileAttrFlags, u_int32_t* pUnknownFileAttrFlags) {
	if (pFileAttrFlags) { *pFileAttrFlags = m_usnJrnlRecord.dwFileAttributes; }
	if (pUnknownFileAttrFlags) { *pUnknownFileAttrFlags = findUnknownCodes(m_usnJrnlRecord.dwFileAttributes, FILE_ATTRIBUTES, sizeof(FILE_ATTRIBUTES)); }
	return getMessages(m_usnJrnlRecord.dwFileAttributes, FILE_ATTRIBUTES, sizeof(FILE_ATTRIBUTES));
}

