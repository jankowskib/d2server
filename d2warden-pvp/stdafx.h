/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2011-2013 Bartosz Jankowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================== */
#define _ENGLISH_LOGS
#define WIN32_LEAN_AND_MEAN
//******** SET HERE WORKING VERSION *********
#define VER_113D
//*******************************************
#include <Windows.h>

using namespace std;

#include <string>
#include <vector>
#include <sstream>

#include <list>
#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/random.hpp>
#include <shellapi.h>
#include <urlmon.h>
#include <math.h>


#ifdef VER_113D
#include "D2Structs_113D.h"
#include "D2Ptrs_113D.h"
#elif defined VER_111B
#include "D2Structs_111B.h"
#include "D2Ptrs_111B.h"
#else
#error("Not supported version code :()")
#endif

#include "D2Stubs.h"
#include "Vars.h"

#include "WardenMisc.h"
#include "Offset.h"

#define ASSERT(e) if (e == 0) { Log("Critical error in line %d, file '%s' , function: '%s'.",__LINE__,__FILE__,__FUNCTION__); exit(-1); }
#define D2ERROR(s) { Log("Critical error '%s' in line %d, file '%s' , function: '%s'.",s,__LINE__,__FILE__,__FUNCTION__); exit(-1); }

#ifdef _DEBUG
#define DEBUGMSG(s,...) Debug((s));
#else
#define DEBUGMSG(s,...) {}
#endif

#define LOCK   {/*Debug("--> CS : %d : %s",__LINE__,__FUNCTION__); */EnterCriticalSection(&hWarden.WardenLock);}
#define UNLOCK {/*Debug("<-- CS : %d : %s",__LINE__,__FUNCTION__); */LeaveCriticalSection(&hWarden.WardenLock);}