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
#include "misc/boost_lexical_cast_wrapper.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class winUSNJournal;

class winUSNRecord {
	public:
		winUSNRecord(USN_RECORD_VER2 stUSNJournal, string strFilename, u_int64_t posRecord);
		~winUSNRecord();
		
		u_int64_t	getUSN()					{ return m_stUSNJrnl.idUSN; };
		u_int64_t	getTimestamp()			{ return m_stUSNJrnl.dtmTimestamp; };
		u_int32_t	getSecurityID()		{ return m_stUSNJrnl.idSecurityID; };
		string		getFilename() 			{ return m_strFilename; };
		u_int64_t	getRecordPos()			{ return m_posRecord; };
		u_int32_t	getRecordLen()			{ return m_stUSNJrnl.cRecordLen; };
		u_int64_t	getMFTEntry() 			{ return m_stUSNJrnl.idFileRefNum & 0xFFFFFFFFFFFF; };
		u_int16_t	getMFTSeq()				{ return m_stUSNJrnl.idFileRefNum >> 48; };
		u_int64_t	getParentMFTEntry()	{ return m_stUSNJrnl.idParentRefNum & 0xFFFFFFFFFFFF; };
		u_int16_t	getParentMFTSeq()		{ return m_stUSNJrnl.idParentRefNum >> 48; };
		string		getVersion()			{ return boost_lexical_cast_wrapper<string>(m_stUSNJrnl.vMajorVer) + "." + boost_lexical_cast_wrapper<string>(m_stUSNJrnl.vMinorVer); };

		string		getReasons(u_int32_t* pfxReasons = NULL, u_int32_t* pfxUnkReasons = NULL, bool fShortMessages=false);
		string		getSources(u_int32_t* pfxSources = NULL, u_int32_t* pfxUnkSources = NULL, bool fShortMessages=false);
		string		getFileAttrs(u_int32_t* pfxFileAttrs = NULL, u_int32_t* pfxUnkFileAttrs = NULL, bool fShortMessages=false);

	private:
		u_int64_t					m_posRecord;
		USN_RECORD_VER2			m_stUSNJrnl;
		string						m_strFilename;
};

#endif //_WINCHANGEJOURNALRECORD_H_
