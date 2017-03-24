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

#ifndef _WINUSNJOURNAL_H_
#define _WINUSNJOURNAL_H_

#include "winUSNRecord.h"
#include "libbinData/src/binDataFile.h"
#include <string>
using namespace std;

typedef enum { USNJRNL_ERROR, USNJRNL_SUCCESS } USNJRNL_RV;

class winUSNJournal: public binDataFile {
	friend class winUSNRecord;
	
	public:
		winUSNJournal(string strFilename);
		~winUSNJournal();
		
		USNJRNL_RV getNextRecord(winUSNRecord** ppCusnRecord);
};

#endif //_WINUSNJOURNAL_H_
