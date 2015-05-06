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

#include "stdafx.h"

int WardenUpTime;
Warden hWarden;
HANDLE DumpHandle = 0;
HANDLE hEvent = 0;
HANDLE hWardenCheckEvent = 0;

CRITICAL_SECTION MEM_CRITSECT;
CRITICAL_SECTION LOG_CS;
DWORD MOD_Length = 0;
unsigned char *MOD_Enrypted = 0;
BYTE wcfgMaxPlayers;
int wcfgExpRange;
int DmgRekord;

BYTE wcfgD2EXVersion;
BYTE wcfgSpectator;
BYTE wcfgAddKillerClass;
BYTE wcfgAutoIdentify;
BYTE wcfgHostileLevel;
BYTE wcfgAllowLoggin;
BYTE wcfgAllowTourMode;
BYTE wcfgAllowVanilla;
BYTE wcfgAllowGU;
BYTE wcfgAllowHB;
BYTE wcfgDisableHostileDelay;
BYTE wcfgMoveToTown;
BYTE wcfgDropEarGold;
BYTE wcfgStrBugFix;
BYTE wcfgFFAMode;
BYTE wcfgEnableSeed;
BYTE wcfgAllowD2Ex;
BYTE wcfgAllowNLWW;
BYTE wcfgDetectTrick;
BYTE wcfgDisableRepairCost;
std::string wcfgDatabase;
std::string wcfgDBUser;
std::string wcfgDBPass;
std::string wcfgClansURL;
std::string wcfgConfigFile;
std::string wcfgUpdateURL;
std::string DmgOwner;
std::string wcfgGSName;
std::list<std::string> wcfgAdmins;

bool Warden_Enable;
Vote * sVote = 0;
unsigned char AE_Packet00[40];
BYTE TeleChars[7];
int wcfgDumpInterval;
unsigned StatID = 0;
BYTE DesyncFix = 0;
unsigned int AfkDrop = 0;
UnitAny* ptMonster;
UnitAny* glptItem;

int PNo = 1;

int SellCount = 0;
BYTE wcfgEnableWE = 0;
int NextDC = 0;
int MinSell = 0;
int MaxSell = 0;
int InfoDelay = 0;
DWORD wcfgRespawnTimer = 0;
WEItem WItem;



