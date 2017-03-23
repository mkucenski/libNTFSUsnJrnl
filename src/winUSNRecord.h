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

#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef enum { WIN_USNJRNL_ERROR, WIN_USNJRNL_SUCCESS } WIN_USNJRNL_RV;

class winUsnJrnlRecordsFile;

class winUsnJrnlRecord {
	public:
		winUsnJrnlRecord(USN_RECORD_VER2 usnJrnlRecord, string strFilename, u_int64_t uiOffset);
		~winUsnJrnlRecord();
		
		u_int16_t	getVersion(u_int16_t* pMajorVer = NULL, u_int16_t* pMinorVer = NULL);
		u_int64_t	getMFT(u_int64_t* pFileNumber = NULL, u_int16_t* pSequence = NULL);
		u_int64_t	getParentMFT(u_int64_t* pParentNumber = NULL, u_int16_t* pParentSequence = NULL);
		u_int64_t	getUSN()					{ return m_usnJrnlRecord.dwlUSN; };
		u_int64_t	getTimestamp()			{ return m_usnJrnlRecord.dwlTimestamp; };
		u_int32_t	getReason()				{ return m_usnJrnlRecord.dwReason; };
		string		getReasonStr(u_int32_t* pReasonFlags = NULL, u_int32_t* pUnknownReasonFlags = NULL);
		u_int32_t	getSourceInfo()		{ return m_usnJrnlRecord.dwSourceInfo; };
		string		getSourceInfoStr(u_int32_t* pSourceFlags = NULL, u_int32_t* pUnknownSourceFlags = NULL);
		u_int32_t	getSecurityID ()		{ return m_usnJrnlRecord.dwSecurityID; };
		u_int32_t	getFileAttributes()	{ return m_usnJrnlRecord.dwFileAttributes; };
		string		getFileAttributesStr(u_int32_t* pFileAttrFlags = NULL, u_int32_t* pUnknownFileAttrFlags = NULL);
		string		getFilename() 			{ return m_strFilename; };

	private:
		//winUsnJrnlRecordsFile*	m_pUsnJrnlRecordsFile;
		u_int64_t					m_uiOffset;
		USN_RECORD_VER2			m_usnJrnlRecord;
		string						m_strFilename;
};

#endif //_WINUSNJRNLRECORD_H_
