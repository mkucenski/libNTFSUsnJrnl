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
#include "winUsnJrnlRecordFile.h"
#include "misc/debugMsgs.h"
#include "misc/stringType.h"
#include "misc/endianSwitch.h"

#include <sstream>
using namespace std;

winUsnJrnlRecord::winUsnJrnlRecord(winUsnJrnlRecordFile* pUsnJrnlRecordFile, char* pData, long lOffset, DWORD dwLength)
		:	m_pUsnJrnlRecordFile(pUsnJrnlRecordFile),
			m_lOffset(lOffset) {

	memset(&m_usnJrnlRecord, 0, USN_RECORD_VER2_BASE_LENGTH);	
	memcpy(&m_usnJrnlRecord, pData, (dwLength < USN_RECORD_VER2_BASE_LENGTH ? dwLength : USN_RECORD_VER2_BASE_LENGTH));
	
	BIGTOHOST32(m_usnJrnlRecord.dwRecordLen);
	BIGTOHOST16(m_usnJrnlRecord.wMajorVer);
	BIGTOHOST16(m_usnJrnlRecord.wMinorVer);
	BIGTOHOST64(m_usnJrnlRecord.dwlFileRefNum);
	BIGTOHOST64(m_usnJrnlRecord.dwlParentFileRefNum);
	BIGTOHOST64(m_usnJrnlRecord.dwlUSN);
	BIGTOHOST64(m_usnJrnlRecord.liTimestamp);
	BIGTOHOST32(m_usnJrnlRecord.dwReason);
	BIGTOHOST32(m_usnJrnlRecord.dwSrcInfo);
	BIGTOHOST32(m_usnJrnlRecord.dwSecId);
	BIGTOHOST32(m_usnJrnlRecord.dwFileAttributes);
	BIGTOHOST16(m_usnJrnlRecord.wFileNameLen);
	BIGTOHOST16(m_usnJrnlRecord.wFileNameOffset);
	
	if (dwLength > USN_RECORD_VER2_BASE_LENGTH) {
		m_pUsnJrnlRecordFile->getTwoByteCharString(&m_strFilename, m_lOffset + USN_RECORD_VER2_BASE_LENGTH, 0, true);
	} else {
		DEBUG_ERROR("winUsnJrnlRecord::winUsnJrnlRecord() Invalid length value.");
	}
}

winUsnJrnlRecord::~winUsnJrnlRecord() {
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

string_t winUsnJrnlRecord::getReasonStr(u_int32_t* pReasonFlags) {
	if (pReasonFlags) {
		*pReasonFlags = m_usnJrnlRecord.dwReason;
	}

	string_t strReason;
	bool bFirstFound = false;
	u_int32_t dwReason = m_usnJrnlRecord.dwReason;

	if ((dwReason & USN_REASON_BASIC_INFO_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_BASIC_INFO_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_BASIC_INFO_CHANGE;
	}
	if ((dwReason & USN_REASON_CLOSE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_CLOSE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_CLOSE;
	}
	if ((dwReason & USN_REASON_COMPRESSION_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_COMPRESSION_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_COMPRESSION_CHANGE;
	}
	if ((dwReason & USN_REASON_DATA_EXTEND) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_DATA_EXTEND_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_DATA_EXTEND;
	}
	if ((dwReason & USN_REASON_DATA_OVERWRITE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_DATA_OVERWRITE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_DATA_OVERWRITE;
	}
	if ((dwReason & USN_REASON_DATA_TRUNCATION) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_DATA_TRUNCATION_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_DATA_TRUNCATION;
	}
	if ((dwReason & USN_REASON_EA_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_EA_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_EA_CHANGE;
	}
	if ((dwReason & USN_REASON_ENCRYPTION_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_ENCRYPTION_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_ENCRYPTION_CHANGE;
	}
	if ((dwReason & USN_REASON_FILE_CREATE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_FILE_CREATE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_FILE_CREATE;
	}
	if ((dwReason & USN_REASON_FILE_DELETE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_FILE_DELETE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_FILE_DELETE;
	}
	if ((dwReason & USN_REASON_HARD_LINK_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_HARD_LINK_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_HARD_LINK_CHANGE;
	}
	if ((dwReason & USN_REASON_INDEXABLE_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_INDEXABLE_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_INDEXABLE_CHANGE;
	}
	if ((dwReason & USN_REASON_INTEGRITY_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_INTEGRITY_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= ;
	}
	if ((dwReason & USN_REASON_NAMED_DATA_EXTEND) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_NAMED_DATA_EXTEND_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_INTEGRITY_CHANGE;
	}
	if ((dwReason & USN_REASON_NAMED_DATA_OVERWRITE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_NAMED_DATA_OVERWRITE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_NAMED_DATA_OVERWRITE;
	}
	if ((dwReason & USN_REASON_NAMED_DATA_TRUNCATION) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_NAMED_DATA_TRUNCATION_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_NAMED_DATA_TRUNCATION;
	}
	if ((dwReason & USN_REASON_OBJECT_ID_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_OBJECT_ID_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_OBJECT_ID_CHANGE;
	}
	if ((dwReason & USN_REASON_RENAME_NEW_NAME) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_RENAME_NEW_NAME_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_RENAME_NEW_NAME;
	}
	if ((dwReason & USN_REASON_RENAME_OLD_NAME) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_RENAME_OLD_NAME_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_RENAME_OLD_NAME;
	}
	if ((dwReason & USN_REASON_REPARSE_POINT_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_REPARSE_POINT_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_REPARSE_POINT_CHANGE;
	}
	if ((dwReason & USN_REASON_SECURITY_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_SECURITY_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_SECURITY_CHANGE;
	}
	if ((dwReason & USN_REASON_STREAM_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_STREAM_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_STREAM_CHANGE;
	}
	if ((dwReason & USN_REASON_TRANSACTED_CHANGE) > 0) {
		strReason << (bFirstFound ? "," : "") << USN_REASON_TRANSACTED_CHANGE_STR;
		bFirstFound = true;
		dwReason ^= USN_REASON_TRANSACTED_CHANGE;
	}

	// As they are found, reasons are XOR'd out of dwReason--this value should be zero by the end or there
	// are unknown/invalid reasons left behind.
	if (dwReason != 0) {
		DEBUG_ERROR("winUsnJrnlRecord::getReasonStr() Unknown/invalid reasons found <" << dwReason << ">");
		strReason << (bFirstFound ? "," : "") << USN_REASON_UNKNOWN_STR;
	}

	return strReason;
}

string_t winUsnJrnlRecord::getSourceInfoStr(u_int32_t* pSourceFlags) {
	if (pSourceFlags) {
		*pSourceFlags = m_usnJrnlRecord.dwSrcInfo;
	}

	string_t str;
	bool bFirstFound = false;
	u_int32_t dwSrcInfo = m_usnJrnlRecord.dwSrcInfo;

	if ((dwSrcInfo & USN_SOURCE_AUXILIARY_DATA) > 0) {
		str << (bFirstFound ? "," : "") << USN_SOURCE_AUXILIARY_DATA_STR;
		bFirstFound = true;
		dwSrcInfo ^= USN_SOURCE_AUXILIARY_DATA;
	}
	if ((dwSrcInfo & USN_SOURCE_DATA_MANAGEMENT) > 0) {
		str << (bFirstFound ? "," : "") << USN_SOURCE_DATA_MANAGEMENT_STR;
		bFirstFound = true;
		dwSrcInfo ^= USN_SOURCE_DATA_MANAGEMENT;
	}
	if ((dwSrcInfo & USN_SOURCE_REPLICATION_MANAGEMENT) > 0) {
		str << (bFirstFound ? "," : "") << USN_SOURCE_REPLICATION_MANAGEMENT_STR;
		bFirstFound = true;
		dwSrcInfo ^= USN_SOURCE_REPLICATION_MANAGEMENT;
	}
	if ((dwSrcInfo & USN_SOURCE_CLIENT_REPLICATION_MANAGEMENT) > 0) {
		str << (bFirstFound ? "," : "") << USN_SOURCE_CLIENT_REPLICATION_MANAGEMENT_STR;
		bFirstFound = true;
		dwSrcInfo ^= USN_SOURCE_CLIENT_REPLICATION_MANAGEMENT;
	}

	// As they are found, sources are XOR'd out of dwSrcInfo--this value should be zero by the end or there
	// are unknown/invalid sources left behind.
	if (dwSrcInfo != 0) {
		DEBUG_ERROR("winUsnJrnlRecord::getSourceInfoStr() Unknown/invalid sources found <" << dwSrcInfo << ">");
		str << (bFirstFound ? "," : "") << USN_SOURCE_UNKNOWN_STR;
	}

	return str;
}

string_t winUsnJrnlRecord::getFileAttributesStr(u_int32_t* pFileAttrFlags) {
	if (pFileAttrFlags) {
		*pFileAttrFlags = m_usnJrnlRecord.dwFileAttributes;
	}

	string_t str;
	bool bFirstFound = false;
	u_int32_t dwFileAttributes = m_usnJrnlRecord.dwFileAttributes;

	if ((dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_ARCHIVE_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_ARCHIVE;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_COMPRESSED_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_COMPRESSED;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_DEVICE) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_DEVICE_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_DEVICE;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_DIRECTORY_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_DIRECTORY;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_ENCRYPTED_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_ENCRYPTED;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_HIDDEN_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_HIDDEN;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_INTEGRITY_STREAM_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_INTEGRITY_STREAM;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_NORMAL) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_NORMAL_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_NORMAL;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_NOT_CONTENT_INDEXED_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_NO_SCRUB_DATA_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_NO_SCRUB_DATA;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_OFFLINE_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_OFFLINE;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_READONLY) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_READONLY_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_READONLY;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_REPARSE_POINT_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_REPARSE_POINT;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_SPARSE_FILE_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_SPARSE_FILE;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_SYSTEM_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_SYSTEM;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_TEMPORARY_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_TEMPORARY;
	}
	if ((dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL) > 0) {
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_VIRTUAL_STR;
		bFirstFound = true;
		dwFileAttributes ^= FILE_ATTRIBUTE_VIRTUAL;
	}

	// As they are found, attributes are XOR'd out of dwFileAttributes--this value should be zero by the end or there
	// are unknown/invalid attributes left behind.
	if (dwFileAttributes != 0) {
		DEBUG_ERROR("winUsnJrnlRecord::getFileAttributesStr() Unknown/invalid attributes found <" << dwFileAttributes << ">");
		str << (bFirstFound ? "," : "") << FILE_ATTRIBUTE_UNKNOWN_STR;
	}

	return str;
}

