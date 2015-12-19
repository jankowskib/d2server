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
#include "Offset.h"

#ifndef __DEFINE_EXPTRS

#define EXFUNCPTR(d1,v1,t1,t2,t3, o1)  t1 (t2 *d1##_##v1)t3;
#define EXASMPTR(d1,v1,o1)   DWORD d1##_##v1;
#define EXVARPTR(d1,v1,t1,o1)  t1 *d1##_##v1;

#else
enum { DDLL_D2CLIENT, DDLL_D2COMMON, DDLL_D2GFX, DDLL_D2LANG, DDLL_D2WIN, DDLL_D2NET, DDLL_D2GAME, DDLL_D2LAUNCH, DDLL_FOG, DDLL_BNCLIENT, DDLL_STORM, DDLL_D2CMP, DDLL_D2MULTI, DDLL_D2SOUND, DDLL_D2GDI, DDLL_D2DDRAW, DDLL_D2DIRECT3D, DDLL_D2GLIDE };

#define DLLOFFSET(a1,b1) ((DDLL_##a1)|((b1)<<8))

#define EXFUNCPTR(d1,v1,t1,t2,t3, o1) typedef t1 t2 d1##_##v1##_t t3; D2Funcs.##d1##_##v1 = (d1##_##v1##_t *)GetDllOffset(DLLOFFSET(d1,o1));
#define EXASMPTR(d1,v1,o1)   D2Ptrs.##d1##_##v1 = GetDllOffset(DLLOFFSET(d1,o1));
#define EXVARPTR(d1,v1,t1,o1)   D2Vars.##d1##_##v1 = (t1*)GetDllOffset(DLLOFFSET(d1,o1));
#endif

//pfnCallbackTbl: 0x6FD31C28

#ifndef __DEFINE_EXPTRS
struct _d2f {
#else
_d2f D2Funcs = { 0 }; void SetupD2Funcs() {
#endif
EXFUNCPTR(D2GAME, GetGameByNetSocket, Game*, __stdcall, (DWORD NetSocket), 0xBC700) //1.13d -- Uwaga ! Kazde uzycie zostawia watek w sekcji krytycznej!!!
EXFUNCPTR(D2GAME, SpawnMonster, UnitAny*, __fastcall, (DWORD MonIdx, int MonCount, Game* ptGame, Room1* ptRoom, int xPos, int yPos, int nSearchRadius, WORD wFlags), 0xC8D70) // 1.13d
EXFUNCPTR(D2GAME, SpawnSuperUniqueMonster, UnitAny*, __fastcall, (Game *pGame, int pRoom, int nX, int nY, DWORD nTxtIdx), 0xCFB40)
EXFUNCPTR(D2GAME, SpawnPresetMonster, UnitAny*, __fastcall, (PresetMonster* pPreset), 0xCA320)
EXFUNCPTR(D2GAME, GetUnitX, int, __fastcall, (UnitAny* ptUnit), 0x10F0) // 1.13d
EXFUNCPTR(D2GAME, GetUnitY, int, __fastcall, (UnitAny* ptUnit), 0x1120) // 1.13d
EXFUNCPTR(D2GAME, ExecuteEvent, int, __stdcall, (Game *pGame, int EventNo, UnitAny *pUnit, void *Data), 0x9C780) // 1.13d
EXFUNCPTR(D2GAME, UpdatePlayerStats, int, __fastcall, (UnitAny *ptOwnerUnit, int dwStat, int dwValue, UnitAny *pDestUnit), 0x68260) // 1.13d
EXFUNCPTR(D2GAME, CreateCorpse, int, __stdcall, (Game *pGame, UnitAny *pUnit, int xPos, int yPos, Room1 *pRoom), 0x93640) // 1.13d
EXFUNCPTR(D2GAME, CreateItemEx, UnitAny*, __fastcall, (Game *pGame, PresetItem *srPresetItem, int a5), 0xE3110) //1.13d <-- its __fastcall on 1.13d
EXFUNCPTR(D2GAME, SetMonSkill, void, __fastcall, (UnitAny *pUnit, int HowSet, int SkillId, int SkillFlags), 0x19FA0) // 1.13d
EXFUNCPTR(D2GAME, AddItemToNPC, int, __fastcall, (char *szFile, int aLine, Game *pGame, UnitAny *pNPCUnit, int ItemId, WORD iX, WORD iY, int a6, int a7, int a8), 0xB7640) // 1.13d
EXFUNCPTR(D2GAME, DeleteUnit, int, __fastcall, (Game *ptGame, UnitAny *ptUnit), 0x6FC70) // 1.13d
EXFUNCPTR(D2GAME, CopyPortal, UnitAny*, __stdcall, (Game *pGame, UnitAny *pSourceUnit, int nLevel, int OriginLevel), 0x15F40) // 1.13d
EXFUNCPTR(D2GAME, CreateUnit, UnitAny*, __fastcall, (DWORD UnitType, DWORD ClassId, int xPos, int yPos, Game *pGame, Room1 *pRoom1, WORD wFlags, DWORD InitMode, int UnitId), 0x6FE10) //1.13d
EXFUNCPTR(D2GAME, SetUnitModeXY, int, __fastcall, (Game *pGame, UnitAny *pUnit, Skill *pSkill, int aMode, int xPos, int yPos, BOOL bForce), 0x922E0) // 1.13d
EXFUNCPTR(D2GAME, RemoveFromPickedUp, void, __stdcall, (UnitAny *pPlayer), 0xB41D0) // 1.13d
EXFUNCPTR(D2GAME, KillPlayer, void, __fastcall, (Game *pGame, UnitAny *pVictim, int nMode, UnitAny *pKiller), 0x93C80) // 1.13d
EXFUNCPTR(D2GAME, SetTimer, void, __fastcall, (Game *pGame, UnitAny *pUnit, int nTimerType, DWORD nGameFrame, DWORD fTimerFunc, void* dwArg, void *dwArgEx), 0xC07A0) // 1.11b: 0x70F00
EXFUNCPTR(D2GAME, FindMonsterRoom, BOOL, __stdcall, (Game *pGame, Room1 *pRoom, CoordsInfo *pCoords, DWORD nMonIdx, DWORD *nX, DWORD *nY, BOOL bTryHard), 0xD4360)

//D2COMMON
EXFUNCPTR(D2COMMON, AddStatToStatList, int, __stdcall, (StatList* ptStatList, int nStat, int nValue, int nValue2), -10818) // 1.13d
EXFUNCPTR(D2COMMON, GetStateStatList, StatList*, __stdcall, (UnitAny* ptUnit, int StateNo), -10429) //1.13d
EXFUNCPTR(D2COMMON, RemoveStatList, int, __stdcall, (UnitAny* ptUnit, StatList* ptStatList), -10669) // 1.13d
EXFUNCPTR(D2COMMON, FreeStatList, int, __stdcall, (StatList* ptStatList), -10125) // 1.13d

EXFUNCPTR(D2COMMON, UpdateOverlay, void, __stdcall, (UnitAny* ptUnit), -10229) // 1.13d
EXFUNCPTR(D2COMMON, AssignOverlay, void, __stdcall, (UnitAny* ptUnit, int OverlayNo, int _1), -10399) // 1.13d

EXFUNCPTR(D2COMMON, CreateStatListEx, StatListEx*, __stdcall, (UnitAny *pUnit, char ListFlags, void *pfnCallBack, Game *pGame), -11099) // 1.13d

EXFUNCPTR(D2COMMON, SetAuraState, void, __stdcall, (UnitAny *ptUnit, int AuraState, int HowSet), -11152) // 1.13d
EXFUNCPTR(D2COMMON, SetStatListStat, void, __stdcall, (StatList* ptStatList, int nStatNo, int nDuration, int Unk), -10560) // 1.13d

EXFUNCPTR(D2COMMON, GetStatSigned, int, __stdcall, (UnitAny *ptUnit, int nStat, int nLayer), -10550) // 1.13d
EXFUNCPTR(D2COMMON, GetBaseStatSigned, int, __stdcall, (UnitAny *ptUnit, int nStat, int nLayer), -10216) // 1.13d
EXFUNCPTR(D2COMMON, GetUnitState, int, __stdcall, (UnitAny *ptUnit, DWORD dwStateNo), -10706) //k
EXFUNCPTR(D2COMMON, SetStat, int, __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer), -10590) // 1.13d
EXFUNCPTR(D2COMMON, AddStat, int, __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer), -10645) // 1.13d

EXFUNCPTR(D2COMMON, GetExpToAchiveLvl, int, __stdcall, (int ClassId, int ExpLvl), -10949) // 1.13d
EXFUNCPTR(D2COMMON, GetMaxCLvl, int, __stdcall, (int ClassId), -10835) // 1.13d
EXFUNCPTR(D2COMMON, GetNextCLvl, int, __stdcall, (int ClassId, int Experience), -11171) // 1.13d

EXFUNCPTR(D2COMMON, GetGoldLimit, int, __stdcall, (UnitAny* ptUnit), -11159)
EXFUNCPTR(D2COMMON, GetBankGoldLimit, int, __stdcall, (UnitAny* ptUnit), -11025)

//ITEM RELATED
EXFUNCPTR(D2COMMON, AllocInventory, Inventory*, __stdcall, (D2PoolManager *pMemPool, UnitAny *pOwner), -11011) // 1.13d
EXFUNCPTR(D2COMMON, GetItemIdx, ItemsTxt*, __stdcall, (DWORD ItemCode, int * Idx), -10000) // 1.13d
EXFUNCPTR(D2COMMON, GetItemTxt, ItemsTxt*, __stdcall, (int RecordNo), -10994) //k
EXFUNCPTR(D2COMMON, GetItemCode, DWORD, __stdcall, (UnitAny* ptItem), -10604) // 1.13d
EXFUNCPTR(D2COMMON, GetItemType, DWORD, __stdcall, (UnitAny* ptItem), -10121) // 1.13d
EXFUNCPTR(D2COMMON, SetItemQuality, void, __stdcall, (UnitAny* ptItem, int iQual), -11122) // 1.13d
EXFUNCPTR(D2COMMON, SetBeginFlag, void, __stdcall, (UnitAny* ptPlayer, int HowSet), -10163) // 1.13d
EXFUNCPTR(D2COMMON, SetItemFlag, void, __stdcall, (UnitAny *ptItem, DWORD dwFlag, int HowSet), -10876) // 1.13d
EXFUNCPTR(D2COMMON, SetInvPage, void, __stdcall, (UnitAny* ptItem, int iPage), -11026) // 1.13d
EXFUNCPTR(D2COMMON, GetInvPage, int, __stdcall, (UnitAny* ptItem), -10291) // 1.13d
EXFUNCPTR(D2COMMON, AddToContainer, void, __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10044) // 1.13d

EXFUNCPTR(D2COMMON, SetCursorItem, int, __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -11097) // 1.13d
EXFUNCPTR(D2COMMON, GetCursorItem, UnitAny*, __stdcall, (Inventory* ptInventory), -10867) // 1.13d
EXFUNCPTR(D2COMMON, GetFirstItem, UnitAny*, __stdcall, (Inventory* ptInventory), -11040) // 1.13d
EXFUNCPTR(D2COMMON, GetNextItem, UnitAny*, __stdcall, (UnitAny* ptItem), -10879) // 1.13d
EXFUNCPTR(D2COMMON, UnitIsItem, UnitAny*, __stdcall, (UnitAny* ptItem), -10897) // 1.13d
EXFUNCPTR(D2COMMON, FlushItem, UnitAny*, __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10490) // 1.13d

EXFUNCPTR(D2COMMON, GetItemColor, BYTE*, __stdcall, (UnitAny *ptPlayer, UnitAny* ptItem, BYTE* out, BOOL a4), -11062) // 1.13d
EXFUNCPTR(D2COMMON, GetItemCost, int, __stdcall, (UnitAny *pPlayer, UnitAny *ptItem, int DiffLvl, QuestFlags *pQuestFlags, int NpcClassId, int InvPage), -10186)

//Skills
EXFUNCPTR(D2COMMON, GetSkillById, Skill*, __fastcall, (UnitAny *ptUnit, int SkillId, int SkillFlags), -10984) //k
EXFUNCPTR(D2COMMON, GetSkillLevel, int, __stdcall, (UnitAny* ptUnit, Skill* ptSkill, int aType), -10007) //k
EXFUNCPTR(D2COMMON, GetLeftSkill, Skill*, __stdcall, (UnitAny* ptUnit), -10909) //k
EXFUNCPTR(D2COMMON, GetRightSkill, Skill*, __stdcall, (UnitAny* ptUnit), -10942) //k
EXFUNCPTR(D2COMMON, GetCurrentSkill, Skill*, __stdcall, (UnitAny* ptUnit), -10459) //k
EXFUNCPTR(D2COMMON, GetSkillId, short, __stdcall, (Skill* ptSkill, char* szFile, int aLine), -11151) //k
EXFUNCPTR(D2COMMON, GetMissileSkillId, int, __stdcall, (UnitAny* ptUnit), -11155) // 1.13d
EXFUNCPTR(D2COMMON, EvaluateSkill, int, __stdcall, (UnitAny* ptUnit, int FormulaNo, int SkillId, int SkillLvl), -11081) //k
EXFUNCPTR(D2COMMON, AddSkillToUnit, int, __stdcall, (UnitAny *ptUnit, DWORD SkillNo, DWORD SkillLvl, BOOL a4, char *szFile, int aLine), -10335) // 1.13d
EXFUNCPTR(D2COMMON, RefreshSkills, int, __stdcall, (UnitAny* ptUnit), -10090) // 1.13d
EXFUNCPTR(D2COMMON, SetStartFlags, void, __stdcall, (UnitAny* ptUnit, BOOL a2), -10163) // 1.13d
EXFUNCPTR(D2COMMON, SetCurrentSkill, int, __stdcall, (UnitAny* ptUnit, Skill* pSkill), -10816) // 1.13d

EXFUNCPTR(D2COMMON, GetPlayerSkillCount, int, __stdcall, (int ClassId), -10407) // 1.13d
EXFUNCPTR(D2COMMON, GetPlayerSkillIdFromList, int, __stdcall, (int ClassId, int SkillNo), -10340) // 1.13d

EXFUNCPTR(D2COMMON, isInShapeForm, int, __stdcall, (UnitAny* ptUnit), -10535) // 1.13d
EXFUNCPTR(D2COMMON, GetStateNoBySkillId, int, __stdcall, (WORD SkillId), -11162) // 1.13d
EXFUNCPTR(D2COMMON, RefreshAura, int, __stdcall, (UnitAny* pUnit, WORD SkillId), -10374) // 1.13d

//Levels, Act etc
EXFUNCPTR(D2COMMON, GetLevelNoByRoom, int, __stdcall, (Room1* ptRoom), -10691) // k
EXFUNCPTR(D2COMMON, isRoomInTown, BOOL, __stdcall, (Room1* ptRoom), -10803) // 1.13d
EXFUNCPTR(D2COMMON, GetActNoByLevelNo, int, __stdcall, (int dwLevelNo), -10864) //k
EXFUNCPTR(D2COMMON, GetTownLevel, int, __stdcall, (int dwActNo), -10353) //k
EXFUNCPTR(D2COMMON, GetUnitMaxLife, unsigned int, __stdcall, (UnitAny *ptUnit), -10574) //k
EXFUNCPTR(D2COMMON, GetUnitMaxMana, unsigned int, __stdcall, (UnitAny *ptUnit), -10084) //k
EXFUNCPTR(D2COMMON, LoadAct, Act*, __stdcall, (DWORD ActNumber, DWORD InitSeed, DWORD Unk0, Game *pGame, DWORD DiffLvl, D2PoolManager* pMemPool, DWORD TownLevelId, DWORD Func1, DWORD Func2), -10024) // 1.13d
EXFUNCPTR(D2COMMON, GetRoomXYByLevel, Room1*, __stdcall, (Act* ptAct, int LevelNo, int Unk0, int* xPos, int* yPos, int UnitSize), -10632) // 1.13d
EXFUNCPTR(D2COMMON, ChangeCurrentMode, BOOL, __stdcall, (UnitAny* ptUnit, int Mode), -10193) // 1.13d
EXFUNCPTR(D2COMMON, GetUnitRoom, Room1*, __stdcall, (UnitAny *ptUnit), -10846) //k
EXFUNCPTR(D2COMMON, GetPathX, int, __stdcall, (Path* ptPath), -10465) // 1.13d
EXFUNCPTR(D2COMMON, GetPathY, int, __stdcall, (Path* ptPath), -10030) // 1.13d
EXFUNCPTR(D2COMMON, GetPathUNK, int, __stdcall, (Path* ptPath), -10349) // 1.13d
EXFUNCPTR(D2COMMON, SetPathTarget, int, __stdcall, (Path* ptPath, UnitAny* pUnit), -10331) // 1.13d
EXFUNCPTR(D2COMMON, SetPathType, int, __stdcall, (Path* ptPath, int aType), -10325) // 1.13d
EXFUNCPTR(D2COMMON, AssignPath, int, __stdcall, (Path* ptPath, UnitAny* pUnit, int aZero), -10474) // 1.13d
EXFUNCPTR(D2COMMON, SetGfxState, void, __stdcall, (UnitAny *ptUnit, int StateNo, int HowSet), -10708) // 1.13d
EXFUNCPTR(D2COMMON, isUnitInMeleeRange, int, __stdcall, (UnitAny* pUnit1, UnitAny* pUnit2, int Range), -11023) // 1.13d
EXFUNCPTR(D2COMMON, GetPortalFlags, int, __stdcall, (UnitAny* pUnit), -10040) // 1.13d
EXFUNCPTR(D2COMMON, SetPortalFlags, void, __stdcall, (UnitAny* pUnit, int Flags), -10033) // 1.13d
EXFUNCPTR(D2COMMON, UpdateRoomWithPortal, void, __stdcall, (Room1* pRoom, int bUnk), -10907) // 1.13d
EXFUNCPTR(D2COMMON, 10555, int, __stdcall, (Act* ptPath, int nLevel), -10555)
EXFUNCPTR(D2COMMON, 10736, void, __stdcall, (Level * pLevel), -10736)
EXFUNCPTR(D2COMMON, GetLevel, Level*, __fastcall, (ActMisc* pMisc, int dwLevelNo), -10283) //k
EXFUNCPTR(D2COMMON, GetCoordsInfo, CoordsInfo*, __stdcall, (Room1* pRoom, DWORD nX, DWORD nY), -10463)
EXFUNCPTR(D2COMMON, GetRoomCoords, BOOL, __stdcall, (Room1* pRoom, RoomCoords** pCoords), -10463)

EXFUNCPTR(D2COMMON, GetWaypointId, BOOL, __stdcall, (int nLevel, int *wpId), -10978) // 1.13d
EXFUNCPTR(D2COMMON, EnableWaypoint, BOOL, __stdcall, (Waypoint* pWaypoint, int wpId), -10967) // 1.13d
//QUESTS
EXFUNCPTR(D2COMMON, SetQuestFlag, int, __stdcall, (QuestFlags* pFlags, int nQuest, int nFlag), -10595) // 1.13d

//D2LANG
EXFUNCPTR(D2LANG, GetLocaleText, wchar_t*, __fastcall, (short nLocaleTxtNo), -10004) //k

//D2CMP
EXFUNCPTR(D2CMP, MixPalette, BYTE*, __stdcall, (int TransLvl, int ColorNo), -10009) //k

//FOG -- unchaned for 1.13d
EXFUNCPTR(FOG, EnterCriticalSection, void, __fastcall, (CRITICAL_SECTION *ptLock, int aLine), -10050)
EXFUNCPTR(FOG, Error, void, __cdecl, (const char* File, void* Addr, int Line), -10024)
EXFUNCPTR(FOG, GetErrorData, void*, __cdecl, (), -10265)
EXFUNCPTR(FOG, AllocServerMemory, void*, __fastcall, (D2PoolManager *pMemPool, int nBytes, char *szFile, int Line, int aNull), -10045)
EXFUNCPTR(FOG, FreeServerMemory, void, __fastcall, (D2PoolManager *pMemPool, void *Mem, char *szFile, int Line, int aNull), -10046)
EXFUNCPTR(FOG, GetTime, DWORD, __cdecl, (), -10055)
EXFUNCPTR(FOG, InitBitBuffer, void, __stdcall, (BitBuffer* ptBitBuffer, void * ptBuffer, int nSize), -10126)
EXFUNCPTR(FOG, GetBinTxtIndex, int, __stdcall, (void *pLink, DWORD dwOrgin, BOOL bLogError), -10213)
EXFUNCPTR(FOG, GetBinTxtRowByText, int, __stdcall, (void *pLink, const char* szText, DWORD nColumn), -10217)

//D2NET 0xa30000+ 0x??? +  2*(4*0xPACKET) -> TO SVR PACKET HANDLER
EXFUNCPTR(D2NET, SendPacket, DWORD, __stdcall, (DWORD nSocket, DWORD ClientID, BYTE *ThePacket, DWORD PacketLen), -10012) // 1.13d Bardzo zla metoda wysylania pakietow (16.06.11 -> jednak jest bezpieczniejsza)
EXFUNCPTR(D2NET, ReceivePacket, bool, __stdcall, (int PacketLen, int _1, BYTE *aPacket), -10015) //k
EXFUNCPTR(D2NET, GetClient, DWORD, __stdcall, (DWORD ClientID), -10019) // 1.13d Get NetSocket
EXFUNCPTR(D2NET, isCorrectClientPacket, BOOL, __fastcall, (BYTE *aPacket, DWORD aLen, DWORD *result), -10004) // 1.13d
EXFUNCPTR(D2NET, GetIpAddress, void, __stdcall, (DWORD ClientID, char *szBuffer, int nLen), -10014)

#ifndef __DEFINE_EXPTRS
};
extern _d2f D2Funcs;
struct _d2p {
#else
}
_d2p D2Ptrs = { 0 };
void SetupD2Pointers() {
#endif
// -- COMMON --
EXASMPTR(D2GAME, GetGameByClientID_I, 0xBC790)   //WARNING: Every use remains thread in CRITICAL_SECTION! 1.13d
EXASMPTR(D2GAME, RemoveItem_I, 0xB5AF0) // 1.13d
EXASMPTR(D2GAME, SaveItems_I, 0x39570) // 1.13d
EXASMPTR(D2GAME, SendPacket_I, 0xDB780) //  DWORD __fastcall, (ClientData *ptClientData, BYTE *aPacket, int Length) 1.13d
EXASMPTR(D2GAME, Send0XAEPacket_I, 0xDB8D0) // 1.13d  DWORD __fastcall, (void *ptPlayer,DWORD Length,DWORD *Packet) unused?
EXASMPTR(D2GAME, LeaveCriticalSection_I, 0xBB8E0) // 1.13d
EXASMPTR(D2GAME, SpawnMonsterInCurrentRoom_I, 0xCDE20) // 1.13d
EXASMPTR(D2GAME, Broadcast5A_I, 0xBECF0) // int __userpurge BroadcastLeaving5A_6FD06CA0<eax>(ClientData *ptClient<esi>, Game *ptGame, BYTE MsgType) 1.13d

//EXP SHARE RELATED
EXASMPTR(D2GAME, ForEachInParty_I, 0x3CDC0) // 1.13d
EXASMPTR(D2GAME, GetExpGained_I, 0x89370) // 1.13d
EXASMPTR(D2GAME, LevelAwards_I, 0xEDB50) // 1.13d
//NODES
EXASMPTR(D2GAME, GetDistanceFromXY_I, 0xB0C40) // 1.13d
EXASMPTR(D2GAME, NodesUnk_I, 0xC6FE0) // 1.13d
EXASMPTR(D2GAME, NodesUnk_II, 0x2C670) // 1.13d
//ITEMS
EXASMPTR(D2GAME, CreateItem_I, 0xDFDE0) // 1.13d
EXASMPTR(D2GAME, ForEach_I, 0x6D590) // 1.13d
EXASMPTR(D2GAME, FE_Remove, 0xDFDC0) // 1.13d

EXASMPTR(D2GAME, UpdateClient_I, 0xDCCC0) // 1.13d Works for durablity like stats ;(
EXASMPTR(D2GAME, DropItem_I, 0xDF970) // 1.13d
EXASMPTR(D2GAME, DupeItem_I, 0xE2C40) // 1.13d
EXASMPTR(D2GAME, CreatePlayer_I, 0x3B4C0) // 1.13d
EXASMPTR(D2GAME, MoveUnitToLevelId_I, 0x5DFC0) // 1.13d
EXASMPTR(D2GAME, MoveUnitToXY_I, 0x6F3A0) // 1.13d
EXASMPTR(D2GAME, CheckXYOccupy_I, 0x1340) // 1.13d
EXASMPTR(D2GAME, isUnitInRange_I, 0x67EA0) // 1.13d
EXASMPTR(D2GAME, FindFreeCoords_I, 0xE0000) // 1.13d

EXASMPTR(D2GAME, ChatOldCode_J, 0x69306) // 1.13d
EXASMPTR(D2GAME, UseableItems_J, 0xB9208) // 1.13d

//DAMAGE STUFF
EXASMPTR(D2GAME, ApplyBurnDmg_J, 0x8B320) // 1.13d void __stdcall, (UnitAny* pAttacker, UnitAny *pDefender, int BurnDmg, int BurnLen)
EXASMPTR(D2GAME, CreateDamage_J, 0x41BF6) // 1.13d

EXASMPTR(D2GAME, ManaLeech_P, 0x88F98) // 1.13d
EXASMPTR(D2GAME, LifeLeech_P, 0x88FDE) // 1.13d
EXASMPTR(D2GAME, CastSkill_I, 0x69680) // 1.13d
EXASMPTR(D2GAME, CastSkillOnUnit_I, 0x69D80) // 1.13d
//PARTY
EXASMPTR(D2GAME, GetPartyId_I, 0x6D3E0) // 1.13d
EXASMPTR(D2GAME, FreePartyMem_I, 0x3CAE0)  // 1.13d edi = ptGame

//OTHERS
EXASMPTR(D2GAME, CreatePackets_J, 0xD53E0) // 1.13d
EXASMPTR(D2GAME, BroadcastEvent_J, 0x68955) // 1.13d

EXASMPTR(D2GAME, BroadcastEvent, 0x68950) // 1.13d eax = aPacket, push pGame

EXASMPTR(D2GAME, Corpse_P, 0x93BC6)  //1.13d -- Prevents from corpse creation
EXASMPTR(D2GAME, GetUnitX_P, 0x16A5A) //bs tele 1.13d
EXASMPTR(D2GAME, GetUnitY_P, 0x16A86) //bs tele 1.13d
EXASMPTR(D2GAME, Monster_I, 0x14C80) //Monster Old Call 1.13d
EXASMPTR(D2GAME, CreateAuraStatList_I, 0x18420) // 1.13d

//ITEMS

EXASMPTR(D2GAME, Send0x21_I, 0xDD4F0) // 1.13d
EXASMPTR(D2GAME, SetSkill_I, 0x17E20) // 1.13d
EXASMPTR(D2GAME, UpdateBonuses_I, 0xB3350) // 1.13d
//TRADE
EXASMPTR(D2GAME, RemoveBonuses_I, 0xB5D90) // 1.13d
EXASMPTR(D2GAME, UpdateQuantity_I, 0xB4290) // 1.13d
EXASMPTR(D2GAME, UpdateQuantity_II, 0xB3ED0) // 1.13d
EXASMPTR(D2GAME, AddGold_I, 0xE09F0) // 1.13d
EXASMPTR(D2GAME, RestoreItems_I, 0x3A4C0) // 1.13d
EXASMPTR(D2GAME, DeleteTimer_I, 0xC04F0) // 1.13d
EXASMPTR(D2GAME, DeleteTimers_I, 0x6E320) // 1.13d
EXASMPTR(D2GAME, MoveItems_I, 0x966A0) // 1.13d
EXASMPTR(D2GAME, UpdateItems_I, 0x99CB0) // 1.13d
//DEATH handler
EXASMPTR(D2GAME, StopSequence_I, 0x6D8B0) // 1.13d
EXASMPTR(D2GAME, RemoveBuffs_I, 0x92480) // 1.13d
EXASMPTR(D2GAME, ResetTimers_I, 0x6D150) // 1.13d
EXASMPTR(D2GAME, RemoveInteraction_I, 0x9A950) // 1.13d
EXASMPTR(D2GAME, SetPlayerUnitMode_I, 0x921C0) // 1.13d
EXASMPTR(D2GAME, RemovePets_I, 0xF03B0)
//UBERS
EXASMPTR(D2GAME, UpdateRoomUnits, 0xBE8E0)
#ifndef __DEFINE_EXPTRS
};
extern _d2p D2Ptrs;
struct _d2v {
#else
}
_d2v D2Vars = { 0 };
void SetupD2Vars() {
#endif
	EXVARPTR(D2GAME, gpfnEventCallbacks, EventCallbackTable*, 0x111C28)
	EXVARPTR(D2GAME, pSpell, pSpellTbl, 0x108690) // 1.13d
	EXVARPTR(D2GAME, ClientDataLock, CRITICAL_SECTION*, 0x1109E0) // 1.13d only for now, use when wanna search client by pure id
	EXVARPTR(D2GAME, ClientDataById, ClientData*, 0x1105E0) // 1.13d only for now testing ^^
	EXVARPTR(D2NET, ToSrvPacketSizes, int, 0xABD8) // 1.13d
	EXVARPTR(D2GAME, ClientPacketTable, PacketTbl, 0xFA698)
	EXVARPTR(D2COMMON, sgptDataTables, sgptDataTable*, -11173) // 1.13d
	EXVARPTR(D2GAME, PartyButtonCallbacks, int, 0xFA294)
	EXVARPTR(D2COMMON, ObjectTxt, ObjectTxt*, 0xA5830)
	EXVARPTR(D2COMMON, ObjectTxtRecs, DWORD, 0xA5830 + 4)
	EXVARPTR(D2GAME, QuestInits, QuestArray, 0xFFF20)
	EXVARPTR(D2GAME, QuestIntros, QuestIntroArray, 0x10029C)
	EXVARPTR(D2COMMON, ItemsTxt, ItemsTxt*, 0xA4CB4)
#ifndef __DEFINE_EXPTRS
};
extern _d2v D2Vars;
#else
}
#endif

#undef DLLOFFSET
#undef __DEFINE_EXPTRS
#undef EXFUNCPTR
#undef EXVARPTR
#undef EXASMPTR
