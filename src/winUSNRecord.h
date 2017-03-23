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

#ifndef _WINCHANGEJOURNALRECORD_H_
#define _WINCHANGEJOURNALRECORD_H_

#include "usnJrnl.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef enum { WIN_USNJRNL_ERROR, WIN_USNJRNL_SUCCESS } WIN_USNJRNL_RV;

class winUSNJournal;

class winUSNRecord {
	public:
		winUSNRecord(USN_RECORD_VER2 stUSNJournal, string strFilename, u_int64_t posRecord);
		~winUSNRecord();
		
		u_int64_t	getUSN()					{ return m_stUSNJrnl.idUSN; };
		u_int64_t	getTimestamp()			{ return m_stUSNJrnl.dtmTimestamp; };
		u_int32_t	getReasons()			{ return m_stUSNJrnl.fxReasons; };
		u_int32_t	getSources()			{ return m_stUSNJrnl.fxSources; };
		u_int32_t	getSecurityID ()		{ return m_stUSNJrnl.idSecurityID; };
		u_int32_t	getFileAttrs()			{ return m_stUSNJrnl.fxFileAttrs; };
		string		getFilename() 			{ return m_strFilename; };

		u_int16_t	getVersion(u_int16_t* pvMajorVer = NULL, u_int16_t* pvMinorVer = NULL);
		u_int64_t	getMFT(u_int64_t* pidFileNumber = NULL, u_int16_t* pcSequence = NULL);
		u_int64_t	getParentMFT(u_int64_t* pidParentNumber = NULL, u_int16_t* pcParentSequence = NULL);
		string		getReasons(u_int32_t* pfxReasons = NULL, u_int32_t* pfxUnkReasons = NULL);
		string		getSources(u_int32_t* pfxSources = NULL, u_int32_t* pfxUnkSources = NULL);
		string		getFileAttrs(u_int32_t* pfxFileAttrs = NULL, u_int32_t* pfxUnkFileAttrs = NULL);

	private:
		u_int64_t					m_posRecord;
		USN_RECORD_VER2			m_stUSNJrnl;
		string						m_strFilename;
};

#endif //_WINCHANGEJOURNALRECORD_H_
