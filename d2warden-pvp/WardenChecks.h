/* ==========================================================
* d2warden
* https://github.com/lolet/d2warden
* ==========================================================
* Copyright 2011-2014 Bartosz Jankowski
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

// -- Warden checks

#ifndef __WARDENCHECKS_H__
#define __WARDENCHECKS_H__

#ifdef VER_111B

#define D2CLIENT_MOUSEXY 0x11B414
#define D2CLIENT_UIVAR 0x1040C0
#define BNCLIENT_REDVEX 0x1D330
#define D2CLIENT_TMC 0x341404
#define D2CLIENT_STING 0x521D1
#define D2CLIENT_WTFPK 0x66DC5
#define D2CLIENT_ONPACKETRCV 0xBDFB1
#define D2COMMON_SKILLSTXT 0xA1328
#define D2CLIENT_GAMELOOP 0x32B40

#elif defined VER_113D

#define D2CLIENT_MOUSEXY 0x11C94C
#define D2CLIENT_UIVAR 0x11C890
#define BNCLIENT_REDVEX 0x1EEF8
#define D2CLIENT_TMC 0
#define D2CLIENT_STING 0
#define D2CLIENT_WTFPK 0
#define D2CLIENT_ONPACKETRCV 0
#define D2COMMON_SKILLSTXT 0
#define D2CLIENT_GAMELOOP 0

#endif

#endif