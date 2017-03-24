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

#include "winUSNRecord.h"

#include "misc/debugMsgs.h"
#include "misc/endianSwitch.h"

#include <sstream>
using namespace std;

winUSNRecord::winUSNRecord(USN_RECORD_VER2 stUSNJrnl, string strFilename, u_int64_t posRecord)
		:	m_stUSNJrnl(stUSNJrnl),
		  	m_strFilename(strFilename),
			m_posRecord(posRecord)  {
}

winUSNRecord::~winUSNRecord() {
}

string winUSNRecord::getReasons(u_int32_t* pfxReasons, u_int32_t* pfxUnkReasons, bool fShortMessages) {
	if (pfxReasons) { 
		*pfxReasons = m_stUSNJrnl.fxReasons; 
	}
	if (pfxUnkReasons) { 
		*pfxUnkReasons = findUnknownCodes(m_stUSNJrnl.fxReasons, USNJRNL_REASONS, sizeof(USNJRNL_REASONS)); 
	}
	if (fShortMessages) {
		return getShorts(m_stUSNJrnl.fxReasons, USNJRNL_REASONS, sizeof(USNJRNL_REASONS));
	} else {
		return getMessages(m_stUSNJrnl.fxReasons, USNJRNL_REASONS, sizeof(USNJRNL_REASONS));
	}
}

string winUSNRecord::getSources(u_int32_t* pfxSources, u_int32_t* pfxUnkSources, bool fShortMessages) {
	if (pfxSources) { 
		*pfxSources = m_stUSNJrnl.fxSources; 
	}
	if (pfxUnkSources) { 
		*pfxUnkSources = findUnknownCodes(m_stUSNJrnl.fxSources, USNJRNL_SOURCES, sizeof(USNJRNL_SOURCES)); 
	}
	if (fShortMessages) {
		return getMessages(m_stUSNJrnl.fxSources, USNJRNL_SOURCES, sizeof(USNJRNL_SOURCES));
	} else {
		return getMessages(m_stUSNJrnl.fxSources, USNJRNL_SOURCES, sizeof(USNJRNL_SOURCES));
	}
}

string winUSNRecord::getFileAttrs(u_int32_t* pfxFileAttrs, u_int32_t* pfxUnkFileAttrs, bool fShortMessages) {
	if (pfxFileAttrs) { 
		*pfxFileAttrs = m_stUSNJrnl.fxFileAttrs; 
	}
	if (pfxUnkFileAttrs) { 
		*pfxUnkFileAttrs = findUnknownCodes(m_stUSNJrnl.fxFileAttrs, FILE_ATTRIBUTES, sizeof(FILE_ATTRIBUTES)); 
	}
	if (fShortMessages) {
		return getMessages(m_stUSNJrnl.fxFileAttrs, FILE_ATTRIBUTES, sizeof(FILE_ATTRIBUTES));
	} else {
		return getMessages(m_stUSNJrnl.fxFileAttrs, FILE_ATTRIBUTES, sizeof(FILE_ATTRIBUTES));
	}
}

