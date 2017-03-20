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

#ifndef _WINUSNJRNLRECORD_H_
#define _WINUSNJRNLRECORD_H_

#include "usnJrnl.h"
#include "misc/stringType.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef enum { WIN_USNJRNL_ERROR, WIN_USNJRNL_SUCCESS } WIN_USNJRNL_RV;

class winUsnJrnlRecordFile;

class winUsnJrnlRecord {
	public:
		winUsnJrnlRecord(winUsnJrnlRecordFile* pUsnJrnlRecordFile, char* pData, long lOffset, DWORD dwLength);
		~winUsnJrnlRecord();
		
		u_int16_t	getVersion(u_int32_t* pMajorVer = NULL, u_int32_t* pMinorVer = NULL);
		u_int64_t	getMFT(u_int64_t* pFileNumber = NULL, u_int16_t* pSequence = NULL);
		u_int64_t	getParentMFT(u_int64_t* pParentFileNumber = NULL, u_int16_t* pParentSequence = NULL);
		u_int64_t	getUSN()					{ return m_usnJrnlRecord.dwlUSN; };
		u_int64_t	getTimestamp()			{ return m_usnJrnlRecord.liTimestamp; };
		u_int32_t	getReason()				{ return m_usnJrnlRecord.dwReason; };
		string_t		getReasonStr(u_int32_t* pReasonFlags = NULL);
		u_int32_t	getSourceInfo()		{ return m_usnJrnlRecord.dwSrcInfo; };
		string_t		getSourceInfoStr(u_int32_t* pSourceFlags = NULL);
		u_int32_t	getSecurityID ()		{ return m_usnJrnlRecord.dwSecId; };
		u_int32_t	getFileAttributes()	{ return m_usnJrnlRecord.dwFileAttributes; };
		string_t		getFileAttributesStr(u_int32_t* pFileAttrFlags = NULL);
		string_t		getFilename() 			{ return m_strFilename};

	private:
		winUsnJrnlRecordFile*	m_pUsnJrnlRecordFile;
		u_int64_t					m_dwlOffset;
		USN_RECORD_VER2			m_usnJrnlRecord;
		string_t						m_strFilename;
};

#endif //_WINUSNJRNLRECORD_H_
