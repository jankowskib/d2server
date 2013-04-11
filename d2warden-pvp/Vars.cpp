/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2013 lolet
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

//DWORD D2Funcs::D2NET_SendPacket(DWORD a,DWORD b,BYTE* c,DWORD d)
//{ 
//
//	DEBUGMSG("S->C[%d] 0x%x, %d",b, c[0], d);
//	return DEBUG_D2Funcs::D2NET_SendPacket(a,b,c,d);
//}

int WardenUpTime;
hWarden_Struct hWarden;
HANDLE DumpHandle = 0;
HANDLE hEvent = 0;

CRITICAL_SECTION MEM_CRITSECT;
CRITICAL_SECTION LOG_CS;
DWORD MOD_Length = 0;
unsigned char *MOD_Enrypted = 0;
BYTE Max_Players;
int ExpRange;
int DmgRekord;
BYTE AllowLoggin;
BYTE AllowTourMode;
BYTE AllowVanilla;
BYTE AllowGU;
BYTE AllowHB;
BYTE MoveToTown;
BYTE DropEarGold;
BYTE StrBugFix;
BYTE FFAMode;
BYTE EnableSeed;
BYTE AllowD2Ex;
BYTE AllowNLWW;
BYTE DetectTrick;
string ClansURL;
string ConfigFile;
string UpdateURL;
string DmgOwner;
string GSName;
map<string,BYTE> ColorSet;
list<string> Admins;
vector<DWORD> Pointers;

bool Warden_Enable;
Vote * sVote = 0;
unsigned char AE_Packet00[40];
BYTE TeleChars[7];
int DumpInterval;
unsigned StatID = 0;
BYTE DesyncFix = 0;
unsigned int AfkDrop = 0;
UnitAny* ptMonster;
UnitAny* glptItem;

int PNo = 1;

int SellCount = 0;
BYTE EnableWE = 0;
int NextDC = 0;
int MinSell = 0;
int MaxSell = 0;
int InfoDelay = 0;
WEITem WItem;



