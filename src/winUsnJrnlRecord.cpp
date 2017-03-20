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

#include "winEvent.h"
#include "winEventFile.h"
#include "misc/debugMsgs.h"
#include "misc/stringType.h"
#include "misc/endianSwitch.h"

#include <sstream>
using namespace std;

winEvent::winEvent(winEventFile* pEventFile, char* pData, long lOffset, DWORD dwLength)	:	m_pEventFile(pEventFile),
																							m_lOffset(lOffset) {
	memset(&m_eventLogRecord, 0, EVENTLOGRECORD_LENGTH);	
	memcpy(&m_eventLogRecord, pData, (dwLength < EVENTLOGRECORD_LENGTH ? dwLength : EVENTLOGRECORD_LENGTH));
	
	BIGTOHOST32(m_eventLogRecord.dwClosingRecordNumber);
	BIGTOHOST32(m_eventLogRecord.dwDataLength);
	BIGTOHOST32(m_eventLogRecord.dwDataOffset);
	BIGTOHOST32(m_eventLogRecord.dwEventID);
	BIGTOHOST32(m_eventLogRecord.dwHeaderID);
	BIGTOHOST32(m_eventLogRecord.dwLength);
	BIGTOHOST32(m_eventLogRecord.dwRecordNumber);
	BIGTOHOST32(m_eventLogRecord.dwStringOffset);
	BIGTOHOST32(m_eventLogRecord.dwTimeGenerated);
	BIGTOHOST32(m_eventLogRecord.dwTimeWritten);
	BIGTOHOST32(m_eventLogRecord.dwUserSidLength);
	BIGTOHOST32(m_eventLogRecord.dwUserSidOffset);
	BIGTOHOST16(m_eventLogRecord.wEventCategory);
	BIGTOHOST16(m_eventLogRecord.wEventType);
	BIGTOHOST16(m_eventLogRecord.wNumStrings);
	BIGTOHOST16(m_eventLogRecord.wReservedFlags);	
	
	if (dwLength > EVENTLOGRECORD_LENGTH) {
		m_pEventFile->getTwoByteCharString(&m_strSource, m_lOffset + EVENTLOGRECORD_LENGTH, 0, true);
		m_pEventFile->getTwoByteCharString(&m_strComputer, 0, true);
	} else {
		DEBUG_ERROR("winEvent::winEvent() Invalid length value.");
	}
}

winEvent::~winEvent() {
}

u_int16_t winUsnJrnlRecord::getVersion(u_int16_t* pMajorVer, u_int16_t* pMinorVer) {
	if (pMajorVer != NULL && pMinorVer != NULL) {
		*pMajorVer = m_usnJrnlRecord.wMajorVer;
		*pMinorVer = m_usnJrnlRecord.wMinorVer;
	}
	return m_usnJrnlRecord.wMajorVer;
}

u_int64_t winUsnJrnlRecord::getMFT(u_int64_t* pFileNumber, u_int16_t* pSequence) {
	if (pFileNumber != NULL && pSequence != NULL) {
		//*pFileNumber = ...
		//*pSequence = ...
	}
	return m_usnJrnlRecord.dwlFileRefNum;
}

u_int64_t winUsnJrnlRecord::getParentMFT(u_int64_t* pParentFileNumber, u_int16_t* pParentSequence) {
	if (pParentFileNumber != NULL && pParentSequence != NULL) {
		//*pParentFileNumber = ...
		//*pParentSequence = ...
	}
	return m_usnJrnlRecord.dwlParentFileRefNum;
}

string_t winEvent::getSourceName() {
	return m_strSource;
}

string_t winEvent::getComputerName() {
	return m_strComputer;
}

string winEvent::getSIDString() {
	string strSID;
	
	SID sid;
	if (getSID(&sid) == WIN_EVENT_SUCCESS) {
		unsigned long long ullIdentifierAuthority = (unsigned long long)sid.identifierAuthority.bValue[0] << 40 | (unsigned long long)sid.identifierAuthority.bValue[1] << 32 | (unsigned long long)sid.identifierAuthority.bValue[2] << 24 | (unsigned long long)sid.identifierAuthority.bValue[3] << 16 | (unsigned long long)sid.identifierAuthority.bValue[4] << 8 | (unsigned long long)sid.identifierAuthority.bValue[5];

		ostringstream ostr;
		ostr << "S-" << (int)sid.bRevision << "-" << ullIdentifierAuthority;
		for (int i=0; i<sid.bSubAuthorityCount; i++) {
			ostr << "-" << sid.dwSubAuthority[i];
		}
		strSID = ostr.str();
	} else {
		DEBUG_ERROR("winEvent::getSIDString() Failure on getSID.");
	}
	
	return strSID;
}

WIN_EVENT_RV winEvent::getSID(SID* pSID) {
	WIN_EVENT_RV rv = WIN_EVENT_ERROR;
	
	if (pSID) {
		if (0 < m_eventLogRecord.dwUserSidOffset && m_eventLogRecord.dwUserSidOffset < m_eventLogRecord.dwLength) {
			if (0 < m_eventLogRecord.dwUserSidLength && m_eventLogRecord.dwUserSidLength < SID_LENGTH) {
				if (m_pEventFile->getData(pSID, m_eventLogRecord.dwUserSidLength, m_lOffset + m_eventLogRecord.dwUserSidOffset, NULL) >= 0) {
					BIGTOHOST32(pSID->dwSubAuthority[0]);
					BIGTOHOST32(pSID->dwSubAuthority[1]);
					BIGTOHOST32(pSID->dwSubAuthority[2]);
					BIGTOHOST32(pSID->dwSubAuthority[3]);
					BIGTOHOST32(pSID->dwSubAuthority[4]);
					BIGTOHOST32(pSID->dwSubAuthority[5]);
					BIGTOHOST32(pSID->dwSubAuthority[6]);
					BIGTOHOST32(pSID->dwSubAuthority[7]);
					BIGTOHOST32(pSID->dwSubAuthority[8]);
					BIGTOHOST32(pSID->dwSubAuthority[9]);
					BIGTOHOST32(pSID->dwSubAuthority[10]);
					BIGTOHOST32(pSID->dwSubAuthority[11]);
					BIGTOHOST32(pSID->dwSubAuthority[12]);
					BIGTOHOST32(pSID->dwSubAuthority[13]);
					BIGTOHOST32(pSID->dwSubAuthority[14]);
					rv = WIN_EVENT_SUCCESS;
				} else {
					DEBUG_ERROR("winEvent::getSID() Failure on m_pEventFile->getData()");
				}
			} else {
				DEBUG_WARNING("winEvent::getSID() Invalid SID length (" << m_eventLogRecord.dwUserSidLength << ").");
			}
		} else {
			DEBUG_WARNING("winEvent::getSID() Invalid SID offset (" << m_eventLogRecord.dwUserSidOffset << ").");
		}
	} else {
		DEBUG_ERROR("winEvent::getSID() Invalid destination.");
	}

	return rv;
}

WIN_EVENT_RV winEvent::getStrings(vector<string_t>* pvStrings) {
	WIN_EVENT_RV rv = WIN_EVENT_ERROR;

	string_t strtmp;
	
	if (pvStrings) {
		if (m_eventLogRecord.wNumStrings > 0) {
			if (m_pEventFile->getTwoByteCharString(&strtmp, m_lOffset + m_eventLogRecord.dwStringOffset, 0, true) >= 0) {
				pvStrings->push_back(strtmp);
				rv = WIN_EVENT_SUCCESS;
				for (int i=0; i<m_eventLogRecord.wNumStrings-1; i++) {
					strtmp.clear();
					if (m_pEventFile->getTwoByteCharString(&strtmp, 0, true) >= 0) {
						pvStrings->push_back(strtmp);
					} else {
						DEBUG_ERROR("winEvent::getStrings() Failure reading string " << i);
						rv = WIN_EVENT_ERROR;
						break;
					}
				}
			} else {
				DEBUG_ERROR("winEvent::getStrings() Failure reading the first string.");
			}
		} else {
			DEBUG_WARNING("winEvent::getStrings() Number of strings <= 0");
		}
	} else {
		DEBUG_ERROR("winEvent::getStrings() Invalid destination.");
	}

	return rv;
}

WIN_EVENT_RV winEvent::getData(char** ppData) {
	WIN_EVENT_RV rv = WIN_EVENT_ERROR;
	
	if (ppData && *ppData == NULL) {
		if (0 < m_eventLogRecord.dwDataOffset && m_eventLogRecord.dwDataOffset < m_eventLogRecord.dwLength) {
			if (0 < m_eventLogRecord.dwDataLength) {
				*ppData = (char*)malloc(m_eventLogRecord.dwDataLength);
				if (*ppData) {
					if (m_pEventFile->getData(*ppData, m_eventLogRecord.dwDataLength, m_lOffset + m_eventLogRecord.dwDataOffset, NULL) >= 0) {
						rv = WIN_EVENT_SUCCESS;
					} else {
						DEBUG_ERROR("winEvent::getData() Failure on m_pEventFile->getData().");
						free(*ppData);
					}
				} else {
					DEBUG_ERROR("winEvent::getData() Failure allocating memory.");
				}
			} else {
				DEBUG_ERROR("winEvent::getData() Invalid data length.");
			}
		} else {
			DEBUG_ERROR("winEvent::getData() Invalid data offset.");
		}
	} else {
		DEBUG_ERROR("winEvent::getData() Invalid destination.");
	}

	return rv;
}
