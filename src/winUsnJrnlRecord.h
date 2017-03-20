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

typedef enum { WIN_USRJRNL_ERROR, WIN_USRJRNL_SUCCESS } WIN_USRJRNL_RV;

class winUsnJrnlRecordFile;

class winUsnJrnlRecord {
	public:
		winUsnJrnlRecord(winUsnJrnlRecordFile* pUsnJrnlRecordFile, char* pData, long lOffset, DWORD dwLength);
		~winUsnJrnlRecord();
		
		u_int64_t	getRecordOffset() 	{ return m_dwlOffset; };
		u_int32_t	getRecordLength()		{ return m_usnJrnlRecord.dwRecordLen; };
		u_int16_t	getVersion(u_int32_t* pMajorVer, u_int32_t* pMinorVer);
		{ return m_usnJrnlRecord.wMajorVer; };
		u_int64_t	getMFT(u_int64_t* pFileNumber, u_int16_t* pSequence);
		u_int64_t	getParentMFT(u_int64_t* pParentFileNumber, u_int16_t* pParentSequence);
		u_int64_t	getUSN()					{ return m_usnJrnlRecord.dwlUSN; };
		u_int64_t	getTimestamp()			{ return m_usrJrnlRecord.liTimestamp; };
		u_int32_t	getReason()				{ return m_usnJrnlRecord.dwReason; };
		u_int32_t	getSourceInfo()		{ return m_usrJrnlRecord.dwSrcInfo; };
		u_int32_t	getSecurityID ()		{ return m_usrJrnlRecord.dwSecId; };
		u_int32_t	getFileAttributes()	{ return m_usrJrnlRecord.dwFileAttributes; };
		u_int16_t	getFilenameLen()		{ return m_usrJrnlRecord.wFileNameLen; };
		u_int16_t	getFilenameOffset()	{ return m_usrJrnlRecord.wFileNameOffset; };
		string_t		getFilename() 			{ return m_strFilename};

	private:
		winEventFile*		m_pUsnJrnlRecordFile;
		u_int64 				m_dwlOffset;
		USN_RECORD_VER2	m_usnJrnlRecord;
		string_t				m_strFilename;
};

#endif //_WINUSRJRNLRECORD_H_
