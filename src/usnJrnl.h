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

#ifndef _USNJRNL_H_
#define _USNJRNL_H_

#include "misc/windowsTypes.h"

typedef struct _USN_RECORD_VER2 {
	DWORD				dwRecordLen;		// 0: 5000 0000
	WORD				wMajorVer;			// 5: 0200
	WORD				wMinorVer;			// 7: 0000
	DWORDLONG		dwlFileRefNum;		// 9: 6340 0000 0000 1100 (0x4063 = 16483 (inode) + "sequence" 17/0x11)
  												//	(per Carrier p.277 MFT = 48bits + 16bit sequence number)
	DWORDLONG		dwlParentFileRefNum; // 17: 3940 0000 0000 1700 (0x4039 = 16441 (parent inode) + "sequence" 23/0x17)
	DWORDLONG 		usn;					// 25: 48a5 6e1e 0100 0000
	LARGE_INTEGER	liTimestamp;		// 33: e7e1 6e0b cbac d101
	DWORD				dwReason;			// 41: 0100 0080
	DWORD				dwSrcInfo;			// 45: 0000 0000
	DWORD				dwSecId; 			// 49: 0000 0000 // ??? SID's are larger than a DWORD, what is this?
	DWORD				dwFileAttributes;	// 53: 2620 0000
	WORD				wFileNameLen;		// 57: 1400
	WORD				wFileNameOffset;	// 59: 3c00
	WCHAR				wchFileName;		// 61: .... .... 4e00 5400 (....N.T.)
												//     5500 5300 4500 5200 (U.S.E.R.)
												//     2e00 4400 4100 5400 (..D.A.T.)
} __attribute__((packed)) USN_RECORD_VER2;

// SAMPLE
// 00000148: 5000 0000 0200 0000  P.......
// 00000150: 6340 0000 0000 1100  c@......
// 00000158: 3940 0000 0000 1700  9@......
// 00000160: 48a5 6e1e 0100 0000  H.n.....
// 00000168: e7e1 6e0b cbac d101  ..n.....
// 00000170: 0100 0080 0000 0000  ........
// 00000178: 0000 0000 2620 0000  ....& ..
// 00000180: 1400 3c00 4e00 5400  ..<.N.T.
// 00000188: 5500 5300 4500 5200  U.S.E.R.
// 00000190: 2e00 4400 4100 5400  ..D.A.T.
//
// MFT Entry Header Values:
// Entry: 16483        Sequence: 17
// $LogFile Sequence Number: 26559364125
// Allocated File
// Links: 1
//
// $STANDARD_INFORMATION Attribute Values:
// Flags: Hidden, System, Archive, Not Content Indexed
// Owner ID: 0
// Security ID: 1671  (S-1-5-21-1721276774-1232856448-2184185802-7403)
// Last User Journal Update Sequence Number: 4805535048
// Created:	2014-02-12 10:47:36.113035900 (MST)
// File Modified:	2016-05-12 15:53:06.051385800 (MDT)
// MFT Modified:	2016-05-12 15:53:06.051385800 (MDT)
// Accessed:	2016-05-12 07:49:50.285466200 (MDT)
//
// $FILE_NAME Attribute Values:
// Flags: Hidden, System, Archive
// Name: NTUSER.DAT
// Parent MFT Entry: 16441 	Sequence: 23
// Allocated Size: 262144   	Actual Size: 0
// Created:	2014-02-12 10:47:36.113035900 (MST)
// File Modified:	2014-02-12 10:47:36.113035900 (MST)
// MFT Modified:	2014-02-12 10:47:36.113035900 (MST)
// Accessed:	2014-02-12 10:47:36.113035900 (MST)
//
// Attributes: 
// Type: $STANDARD_INFORMATION (16-0)   Name: N/A   Resident   size: 72
// Type: $FILE_NAME (48-2)   Name: N/A   Resident   size: 86
// Type: $DATA (128-1)   Name: N/A   Non-Resident   size: 2621440  init_size: 2490368

// https://msdn.microsoft.com/en-us/library/aa365722(v=vs.85).aspx
#define USN_REASON_BASIC_INFO_CHANGE		0x00008000	//A user has either changed one or more file or directory
																		//attributes (for example, the read-only, hidden, system, archive,
																		//or sparse attribute), or one or more time stamps.
#define USN_REASON_CLOSE						0x80000000	//The file or directory is closed.
#define USN_REASON_COMPRESSION_CHANGE		0x00020000	//The compression state of the file or directory is changed from
																		//or to compressed.
#define USN_REASON_DATA_EXTEND				0x00000002	//The file or directory is extended (added to).
#define USN_REASON_DATA_OVERWRITE			0x00000001	//The data in the file or directory is overwritten.
#define USN_REASON_DATA_TRUNCATION			0x00000004	//The file or directory is truncated.
#define USN_REASON_EA_CHANGE					0x00000400	//The user made a change to the extended attributes of a file or
																		//directory.
#define USN_REASON_ENCRYPTION_CHANGE		0x00040000	//The file or directory is encrypted or decrypted.
#define USN_REASON_FILE_CREATE				0x00000100	//The file or directory is created for the first time.
#define USN_REASON_FILE_DELETE				0x00000200	//The file or directory is deleted.
#define USN_REASON_HARD_LINK_CHANGE			0x00010000	//An NTFS file system hard link is added to or removed from the
																		//file or directory.
#define USN_REASON_INDEXABLE_CHANGE			0x00004000	//A user changes the FILE_ATTRIBUTE_NOT_CONTENT_INDEXED attribute.
#define USN_REASON_INTEGRITY_CHANGE			0x00800000	//A user changed the state of the FILE_ATTRIBUTE_INTEGRITY_STREAM
																		//attribute for the given stream.
#define USN_REASON_NAMED_DATA_EXTEND		0x00000020	//The one or more named data streams for a file are extended
																		//(added to).
#define USN_REASON_NAMED_DATA_OVERWRITE	0x00000010	//The data in one or more named data streams for a file is
																		//overwritten.
#define USN_REASON_NAMED_DATA_TRUNCATION	0x00000040	//The one or more named data streams for a file is truncated.
#define USN_REASON_OBJECT_ID_CHANGE			0x00080000	//The object identifier of a file or directory is changed.
#define USN_REASON_RENAME_NEW_NAME			0x00002000	//A file or directory is renamed, and the file name in the
																		//USN_RECORD_V2 structure is the new name.
#define USN_REASON_RENAME_OLD_NAME			0x00001000	//The file or directory is renamed, and the file name in the
																		//USN_RECORD_V2 structure is the previous name.
#define USN_REASON_REPARSE_POINT_CHANGE	0x00100000	//The reparse point that is contained in a file or directory is
																		//changed, or a reparse point is added to or deleted from a file
																		//or directory.
#define USN_REASON_SECURITY_CHANGE			0x00000800	//A change is made in the access rights to a file or directory.
#define USN_REASON_STREAM_CHANGE				0x00200000	//A named stream is added to or removed from a file, or a named
																		//stream is renamed.
#define USN_REASON_TRANSACTED_CHANGE		0x00400000	//The given stream is modified through a TxF transaction.

#define USN_SOURCE_AUXILIARY_DATA			0x00000002	//The operation adds a private data stream to a file or directory.
#define USN_SOURCE_DATA_MANAGEMENT			0x00000001	//The operation provides information about a change to the file
																		//or directory made by the operating system.
#define USN_SOURCE_REPLICATION_MANAGEMENT	0x00000004	//The operation is modifying a file to match the contents of the
																		//same file which exists in another member of the replica set.
#define USN_SOURCE_CLIENT_REPLICATION_MANAGEMENT	0x00000008	//The operation is modifying a file on client systems to
																					//match the contents of the same file that exists in the
																					//cloud.

#endif //_USNJRNL_H_
