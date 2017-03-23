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

u_int16_t winUSNRecord::getVersion(u_int16_t* pvMajorVer, u_int16_t* pvMinorVer) {
	if (pvMajorVer != NULL) { 
		*pvMajorVer = m_stUSNJrnl.vMajorVer; 
	}
	if (pvMinorVer != NULL) { 
		*pvMinorVer = m_stUSNJrnl.vMinorVer; }

	return m_stUSNJrnl.vMajorVer;
}

u_int64_t winUSNRecord::getMFT(u_int64_t* pidFileNumber, u_int16_t* pcSequence) {
	if (pidFileNumber != NULL && pcSequence != NULL) {
		*pidFileNumber = m_stUSNJrnl.idFileRefNum >> 16;
		*pcSequence = m_stUSNJrnl.idFileRefNum & 0xFFFF;
	}
	return m_stUSNJrnl.idFileRefNum;
}

u_int64_t winUSNRecord::getParentMFT(u_int64_t* pidParentNumber, u_int16_t* pcParentSequence) {
	if (pidParentNumber != NULL && pcParentSequence != NULL) {
		*pidParentNumber = m_stUSNJrnl.idParentRefNum >> 16;
		*pcParentSequence = m_stUSNJrnl.idParentRefNum & 0xFFFF;
	}
	return m_stUSNJrnl.idParentRefNum;
}

string winUSNRecord::getReasons(u_int32_t* pfxReasons, u_int32_t* pfxUnkReasons) {
	if (pfxReasons) { 
		*pfxReasons = m_stUSNJrnl.fxReasons; 
	}
	if (pfxUnkReasons) { 
		*pfxUnkReasons = findUnknownCodes(m_stUSNJrnl.fxReasons, USNJRNL_REASONS, sizeof(USNJRNL_REASONS)); 
	}
	return getMessages(m_stUSNJrnl.fxReasons, USNJRNL_REASONS, sizeof(USNJRNL_REASONS));
}

string winUSNRecord::getSources(u_int32_t* pfxSources, u_int32_t* pfxUnkSources) {
	if (pfxSources) { 
		*pfxSources = m_stUSNJrnl.fxSources; 
	}
	if (pfxUnkSources) { 
		*pfxUnkSources = findUnknownCodes(m_stUSNJrnl.fxSources, USNJRNL_SOURCES, sizeof(USNJRNL_SOURCES)); 
	}
	return getMessages(m_stUSNJrnl.fxSources, USNJRNL_SOURCES, sizeof(USNJRNL_SOURCES));
}

string winUSNRecord::getFileAttrs(u_int32_t* pfxFileAttrs, u_int32_t* pfxUnkFileAttrs) {
	if (pfxFileAttrs) { 
		*pfxFileAttrs = m_stUSNJrnl.fxFileAttrs; 
	}
	if (pfxUnkFileAttrs) { 
		*pfxUnkFileAttrs = findUnknownCodes(m_stUSNJrnl.fxFileAttrs, FILE_ATTRIBUTES, sizeof(FILE_ATTRIBUTES)); 
	}
	return getMessages(m_stUSNJrnl.fxFileAttrs, FILE_ATTRIBUTES, sizeof(FILE_ATTRIBUTES));
}

