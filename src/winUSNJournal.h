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

#ifndef _WINUSNJRNLFILE_H_
#define _WINUSNJRNLFILE_H_

#include "winUsnJrnlRecord.h"
#include "libbinData/src/binDataFile.h"

#include <string>
using namespace std;

class winUsnJrnlRecordsFile : public binDataFile {
	friend class winUsnJrnlRecord;
	
	public:
		winUsnJrnlRecordsFile(string strFilename);
		~winUsnJrnlRecordsFile();
		
		WIN_USNJRNL_RV getNextRecord(winUsnJrnlRecord** ppUsnJrnlRecord);
	
	private:
		u_int32_t m_uiDataPos;
};

#endif //_WINUSNJRNLFILE_H_
