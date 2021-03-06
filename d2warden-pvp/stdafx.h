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
#define D2EX_COLOR_STAT		183					// ItemStatCost.Txt record which stores item color value
#define D2EX_LOOTED_STAT	184					// ItemStatCost.Txt record which stores monster id which gave the item 
#define D2EX_SPECTATOR_STATE 185				// States.Txt record set on spectators
//#define D2EX_MYSQL							// Replace gold with mysql database currency
#define ENABLE_LEVEL_COMMAND

#define WIN32_LEAN_AND_MEAN
//******** SET HERE WORKING VERSION *********
#define VER_113D
//*******************************************
#include <Windows.h>

#include <string>
#include <vector>
#include <sstream>

#include <list>
#include <map>
#include <atomic>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/random.hpp>
#include <boost/timer.hpp>

#include <shellapi.h>
#include <urlmon.h>
#include <math.h>

#include "D2DataTables.h"
#include "CommonStructs.h"

#ifdef VER_113D
#include "D2Structs_113D.h"
#include "ExPointers_113D.h"
#elif defined VER_111B
#include "D2Structs_111B.h"
#include "ExPointers_111B.h"
#endif

#ifdef D2EX_MYSQL
#pragma comment(lib, "mysqlcppconn.lib")
#endif

#include "D2Stubs.h"

#include "Misc.h"
#include "Warden.h"
#include "WardenClient.h"
#include "Offset.h"

#define ASSERT(e) if (e == 0) {Log("Critical error in line %d, file '%s' , function: '%s'm eip 0x%X.",__LINE__,__FILE__,__FUNCTION__,GetEIP()); exit(-1); }
#define D2ERROR(s) { Log("Critical error '%s' in line %d, file '%s' , function: '%s'.",s,__LINE__,__FILE__,__FUNCTION__); exit(-1); }

#ifdef _DEBUG
#define DEBUGMSG(s,...) Debug(__FUNCTION__, s, ##__VA_ARGS__);
#define BEGINDEBUGMSG(s,...)  DebugNoEnter(__FUNCTION__, s, ##__VA_ARGS__);
#define FINISHDEBUGMSG(s,...)  DebugFinishEnter(s, ##__VA_ARGS__);
#else
#define DEBUGMSG(s,...) {}
#define BEGINDEBUGMSG(s,...) {}
#define FINISHDEBUGMSG(s,...) {}
#endif
