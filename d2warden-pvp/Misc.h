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

#ifndef __WARDENMISC_H_
#define __WARDENMISC_H_

DWORD GetEIP();
short CalculateDistance(short x1, short y1, short x2, short y2);
void ClearSayGoFlag(Game* pGame);
void ClearCanAttackFlag(Game* pGame);
void ClearIsPlayingFlag(Game* pGame);
bool isAnAdmin(string szAcc);
BOOL isItemBelongToUnit(UnitAny* ptUnit, DWORD iCode);
BOOL isItemBelongToUnit(UnitAny* ptUnit, DWORD iCode, ItemQuality iQuality);
UnitAny* FindUnitByClassId(Game* pGame, DWORD UnitType, DWORD dwClassId);
ClientData* FindClientDataByName(Game* pGame, char* szName);
ClientData* FindClientDataById(Game* pGame, DWORD ClientID);
int TransQuality(string str);
BYTE GetColorNameByAcc(string szAcc);
DWORD TransCode(const char* ptCode);
char* TransCode(DWORD dwCode);
void KickPlayer(ClientData* pClient);
void KickPlayer(DWORD ClientID);
void UpdateStats(UnitAny* ptUnit, int StatNo, int StatValue);
void BroadcastMsg(Game* pGame,char *Msg...);
void BroadcastEventMsgEx(Game* pGame, int Color, string EngMsg, string PolMsg);
void BroadcastExEvent(Game* pGame, int Color, DWORD UnitId, int nCell, string szPath);
void SendExEvent(ClientData* pClient, int Color, int Sound, int Font, short X, short Y, string polMsg, string engMsg);
void SendExEvent(ClientData* pClient, ExEventOption op, DWORD value);
void BroadcastExEvent(Game* pGame, int Color, int Sound, int Font, short X, short Y, string polMsg, string engMsg);
void BroadcastEventMsg(Game* pGame, int Color, char *Msg...);
void BroadcastPacket(Game* pGame, BYTE * aPacket, int aLen);

BOOL WriteDword(DWORD* lpAddr, DWORD lpBuffer);
BOOL WriteBytes(void* lpAddr, void* lpBuffer, DWORD dwLen);
void PatchGS(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen,char* Type);
void UpdatePlayerXY(UnitAny* ptUnit);
void AltUpdatePlayerXY(UnitAny* ptUnit);
void SendPlayerFlag(UnitAny* ptUnit, BYTE FlagID);
void SendMsgToClient(ClientData* ptClient,char *Msg...);
void SendEventToClient(DWORD ClientID,BYTE Col,BYTE MsgType,char * Char1, char * Char2, DWORD P1, BYTE P2);
string ConvertSkill(WORD SkillID);
char * ConvertLevel(int LevelId);
char * ConvertClass(int ClassId);
int  isGoodSkill(WORD SkillID);
bool isMeleeSkill(int SkillID);
bool isSafeSkill(WORD SkillId);
void upcase(char * q);
void lowcase(char * q);

void Debug(char *format,...);
void Log(char *format,...);
void LogNoLock(char *format,...);
void LogToFile(char *FileName, bool PutTime, char *format,...);
void LogError(unsigned char log_level_in,char *format,...);
void LogMsg(char * GameName, char *format,...);
int GetHexValue(char c);
unsigned char Convert2HEX(unsigned char *tt);
DWORD Convert2DWORD(char *tt);
int GetHexSpan(const char* string);
int ConvertHexStringToBytes(const char* string, unsigned char* bytes, int length);
int ConvertBytesToHexString(const unsigned char* bytes, int dataSize, char* string, int stringSize, char delimiter);
int CalculateTextDumpSize(int size);
int DumpDataAsText(unsigned char* bytesIn, int sizeIn, char* dataOut, int sizeOut);
int removespace(char *str,int len);
void trimspaces(char *str);
void WideToChar(CHAR* Dest, const WCHAR* Source);

#endif