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

#ifndef __VARS_H__
#define __VARS_H__

#include "D2Warden.h"

extern int WardenUpTime;
extern Warden hWarden;
extern HANDLE DumpHandle;
extern HANDLE hEvent;
extern HANDLE hWardenCheckEvent;

extern CRITICAL_SECTION MEM_CRITSECT;
extern CRITICAL_SECTION LOG_CS;
extern BYTE wcfgMaxPlayers;
extern int wcfgExpRange;
extern int DmgRekord;
extern std::string DmgOwner;

extern BYTE wcfgD2EXVersion;
extern BYTE wcfgSpectator;
extern BYTE wcfgAddKillerClass;
extern BYTE wcfgAutoIdentify;
extern BYTE wcfgHostileLevel;
extern BYTE wcfgDisableHostileDelay;
extern BYTE wcfgMoveToTown;
extern BYTE wcfgDropEarGold;
extern BYTE wcfgStrBugFix;
extern BYTE wcfgFFAMode;
extern BYTE wcfgAllowVanilla;
extern BYTE wcfgAllowGU;
extern BYTE wcfgAllowNLWW;
extern BYTE wcfgAllowHB;
extern BYTE wcfgAllowTourMode;
extern BYTE wcfgAllowLoggin;
extern BYTE wcfgEnableSeed;
extern BYTE wcfgAllowD2Ex;
extern BYTE wcfgDetectTrick;
extern BYTE wcfgDisableRepairCost;

extern std::string wcfgDatabase;
extern std::string wcfgDBUser;
extern std::string wcfgDBPass;

extern std::string wcfgClansURL;
extern std::string wcfgConfigFile;
extern std::string wcfgUpdateURL;
extern std::string wcfgGSName;
extern std::list<std::string> wcfgAdmins;
extern std::map<std::string, BYTE> ColorSet;
extern std::vector<DWORD> Pointers;
extern std::map<DWORD, DWORD> Specers;
extern bool Warden_Enable;
extern Vote * sVote;
extern DWORD MOD_Length;
extern BYTE TeleChars[7];
extern int wcfgDumpInterval;
extern unsigned StatID;
extern BYTE DesyncFix;
extern unsigned int AfkDrop;
extern int PNo;
extern int wcfgRespawnTimer;

//-----------
extern int SellCount;
extern BYTE wcfgEnableWE;
extern int NextDC;
extern int MinSell;
extern int MaxSell;
extern int InfoDelay;
extern WEItem WItem;

//----------------
extern unsigned char *MOD_Enrypted;
extern unsigned char AE_Packet00[40];

extern UnitAny* ptMonster;
extern UnitAny* glptItem;

#endif