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


#ifndef __DEFINE_EXPTRS
struct _d2f {
#else
_d2f D2Funcs = { 0 }; void SetupD2Funcs() {
#endif

EXFUNCPTR(D2GAME, GetGameByNetSocket, Game*, __stdcall, (DWORD NetSocket), 0xE49A0)   //Uwaga ! Kazde uzycie zostawia watek w sekcji krytycznej!!!! (godzine siedzialem zeby dojsc ze ta funkcja wywala)
EXFUNCPTR(D2GAME, Send0X92Packet, void, __fastcall, (Game *pGame, UnitAny *pUnit, UnitAny *pUnit2), 0x35420)
EXFUNCPTR(D2GAME, KickCharFromGame, void, __stdcall, (DWORD ClientID), -10037)
EXFUNCPTR(D2GAME, SpawnUniqMon, UnitAny*, __stdcall, (Game *ptGame, Room1 *ptRoom, int _1, int _2, signed int MonIdx), 0xECE80)
EXFUNCPTR(D2GAME, SpawnMonster, UnitAny*, __fastcall, (signed int MonIdx, int MonCount, Game* ptGame, Room1* ptRoom, int xPos, int yPos, int a7, int a8), 0xEF870)
EXFUNCPTR(D2GAME, SpawnPresetMonster, UnitAny*, __fastcall, (PresetMonster* pPreset), 0xEF150)
EXFUNCPTR(D2GAME, GetUnitX, int, __fastcall, (UnitAny* ptUnit), 0x1370)
EXFUNCPTR(D2GAME, GetUnitY, int, __fastcall, (UnitAny* ptUnit), 0x13A0)
EXFUNCPTR(D2GAME, ExecuteEvent, int, __stdcall, (Game *pGame, int EventNo, UnitAny *pUnit, void *Data), 0xC3170)
EXFUNCPTR(D2GAME, InitTimer, void, __fastcall, (Game *pGame, UnitAny *pUnit, signed int nTimerType, signed int nGameFrame, DWORD fTimerFunc, int a6, Timer *PrevTimer), 0x70F00)
EXFUNCPTR(D2GAME, UpdatePlayerStats, int, __fastcall, (UnitAny *ptOwnerUnit, int dwStat, int dwValue, UnitAny *pDestUnit), 0x2D2E0)
EXFUNCPTR(D2GAME, CreateCorpse, int, __stdcall, (Game *pGame, UnitAny *pUnit, int xPos, int yPos, Room1 *pRoom), 0x424C0)
EXFUNCPTR(D2GAME, CreateItemEx, UnitAny*, __stdcall, (Game *pGame, PresetItem *srPresetItem, int a5), 0xD10C0)
EXFUNCPTR(D2GAME, SetMonSkill, void, __fastcall, (UnitAny *pUnit, int HowSet, int SkillId, int SkillFlags), 0x11C20)
EXFUNCPTR(D2GAME, AddItemToNPC, int, __fastcall, (char *szFile, int aLine, Game *pGame, UnitAny *pNPCUnit, int ItemId, WORD iX, WORD iY, int a6, int a7, int a8), 0x49B00)
EXFUNCPTR(D2GAME, DeleteUnit, int, __fastcall, (Game *ptGame, UnitAny *ptUnit), 0xC13A0)
EXFUNCPTR(D2GAME, Transmute, void, __stdcall, (Game *ptGame, UnitAny *ptUnit), 0x38370)
EXFUNCPTR(D2GAME, CopyPortal, UnitAny*, __stdcall, (Game *pGame, UnitAny *pSourceUnit, int nLevel, POINT Coords), 0x109F0)
EXFUNCPTR(D2GAME, CreateUnit, UnitAny*, __fastcall, (DWORD UnitType, DWORD ClassId, int xPos, int yPos, Game *pGame, Room1 *pRoom1, WORD wFlags, DWORD InitMode, int UnitId), 0xC09E0)
EXFUNCPTR(D2GAME, SetUnitModeXY, int, __fastcall, (Game *pGame, UnitAny *pUnit, Skill *pSkill, int aMode, int xPos, int yPos, BOOL bForce), 0x41990)
EXFUNCPTR(D2GAME, RemoveFromPickedUp, void, __stdcall, (UnitAny *pPlayer), 0x44B50)
EXFUNCPTR(D2GAME, KillPlayer, void, __fastcall, (Game *pGame, UnitAny *pVictim, int nMode, UnitAny *pKiller), 0x42F20)

//D2COMMON
EXFUNCPTR(D2COMMON, CreateStatList, StatList*, __stdcall, (void *ptMemPool, DWORD dwFlags, DWORD ExpireFrame, DWORD UnitType, DWORD UnitId), -10522)
EXFUNCPTR(D2COMMON, ConnectStatList, void, __stdcall, (UnitAny* ptUnit, StatList * ptStatList, BOOL _1), -10662)
EXFUNCPTR(D2COMMON, AddStatToStatList, int, __stdcall, (StatList* ptStatList, int nStat, int nValue, int nValue2), -10564)
EXFUNCPTR(D2COMMON, AddStateToStatList, void, __stdcall, (StatList* ptStatList, int StateNo), -10155)
EXFUNCPTR(D2COMMON, GetStateStatList, StatList*, __stdcall, (UnitAny* ptUnit, int StateNo), -10219)
EXFUNCPTR(D2COMMON, GetStatListByType, StatList*, __stdcall, (UnitAny* ptUnit, int Typeo), -10726)
EXFUNCPTR(D2COMMON, RemoveStatList, int, __stdcall, (UnitAny* ptUnit, StatList* ptStatList), -11093)
EXFUNCPTR(D2COMMON, FreeStatList, int, __stdcall, (StatList* ptStatList), -10032)

EXFUNCPTR(D2COMMON, UpdateOverlay, void, __stdcall, (UnitAny* ptUnit), -10200)
EXFUNCPTR(D2COMMON, AssignOverlay, void, __stdcall, (UnitAny* ptUnit, int OverlayNo, int _1), -11120)

EXFUNCPTR(D2COMMON, CreateStatListEx, StatListEx*, __stdcall, (UnitAny *pUnit, char ListFlags, void *pfnCallBack, Game *pGame), -10724)

EXFUNCPTR(D2COMMON, SetAuraState, void, __stdcall, (UnitAny *ptUnit, int AuraState, int HowSet), -10541)
EXFUNCPTR(D2COMMON, SetStatListStat, void, __stdcall, (StatList* ptStatList, int nStatNo, int nDuration, int Unk), -10415)

EXFUNCPTR(D2COMMON, GetStatSigned, int, __stdcall, (UnitAny *ptUnit, int nStat, int nLayer), -10061)
EXFUNCPTR(D2COMMON, GetBaseStatSigned, int, __stdcall, (UnitAny *ptUnit, int nStat, int nLayer), -10550)
EXFUNCPTR(D2COMMON, GetUnitState, int, __stdcall, (UnitAny *ptUnit, DWORD dwStateNo), -10604)
EXFUNCPTR(D2COMMON, SetStat, int, __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer), -10590)
EXFUNCPTR(D2COMMON, AddStat, int, __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer), -10627)

EXFUNCPTR(D2COMMON, 11117, int, __stdcall, (StatList* ptStatList, int SkillNo), -11117)
EXFUNCPTR(D2COMMON, 10083, int, __stdcall, (StatList* ptStatList, int SkillLvl), -10083)

EXFUNCPTR(D2COMMON, GetGoldLimit, int, __stdcall, (UnitAny* ptUnit), -11131)
EXFUNCPTR(D2COMMON, GetBankLimit, int, __stdcall, (UnitAny* ptUnit), -10864)

EXFUNCPTR(D2COMMON, GetExpToAchiveLvl, int, __stdcall, (int ClassId, int ExpLvl), -10152)
EXFUNCPTR(D2COMMON, GetMaxCLvl, int, __stdcall, (int ClassId), -11123)
EXFUNCPTR(D2COMMON, GetNextCLvl, int, __stdcall, (int ClassId, int Experience), -10988)

//ITEM RELATED
EXFUNCPTR(D2COMMON, AllocInventory, Inventory*, __stdcall, (void *pMemPool, UnitAny *pOwner), -10976)
EXFUNCPTR(D2COMMON, AllocTradeInventory, int, __stdcall, (void *pMemPool, UnitAny *pPlayer, UnitAny *pTrader, BOOL *bNoRoom), -10339)
EXFUNCPTR(D2COMMON, PresetItemFromStream, int, __stdcall, (), -11024) // dodaj argumenty
EXFUNCPTR(D2COMMON, GetItemIdx, ItemsTxt*, __stdcall, (DWORD ItemCode, int * Idx), -10322)
EXFUNCPTR(D2COMMON, GetItemTxt, ItemsTxt*, __stdcall, (int ItemNo), -10262)
EXFUNCPTR(D2COMMON, GetItemCode, DWORD, __stdcall, (UnitAny* ptItem), -10360)
EXFUNCPTR(D2COMMON, GetItemType, DWORD, __stdcall, (UnitAny* ptItem), -10808) 
EXFUNCPTR(D2COMMON, SetItemQuality, void, __stdcall, (UnitAny* ptItem, int iQual), -10297)
EXFUNCPTR(D2COMMON, SetBeginFlag, void, __stdcall, (UnitAny* ptPlayer, int HowSet), -10948)
EXFUNCPTR(D2COMMON, SetItemFlag, void, __stdcall, (UnitAny *ptItem, DWORD dwFlag, int HowSet), -10970)
EXFUNCPTR(D2COMMON, SetInvPage, void, __stdcall, (UnitAny* ptItem, int iPage), -10608)
EXFUNCPTR(D2COMMON, GetInvPage, int, __stdcall, (UnitAny* ptItem), -10399)
EXFUNCPTR(D2COMMON, GetItemCost, int, __stdcall, (UnitAny *pPlayer, UnitAny *ptItem, int DiffLvl, QuestFlags *pQuestFlags, int NpcClassId, int InvPage), -10511)

EXFUNCPTR(D2COMMON, AddToContainer, void, __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10371)
EXFUNCPTR(D2COMMON, SetItemCMDFlag, void, __stdcall, (UnitAny *pItem, int Flag, int HowSet), -10905)

EXFUNCPTR(D2COMMON, AddSockets, void, __stdcall, (UnitAny* ptItem, int SocketsNo), -10667)
EXFUNCPTR(D2COMMON, Personalize, void, __stdcall, (UnitAny* ptItem, PlayerData* ptPlayer), -10335)

EXFUNCPTR(D2COMMON, SetCursorItem, int, __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -11029)
EXFUNCPTR(D2COMMON, GetCursorItem, UnitAny*, __stdcall, (Inventory* ptInventory), -10914)
EXFUNCPTR(D2COMMON, GetFirstItem, UnitAny*, __stdcall, (Inventory* ptInventory), -10535)
EXFUNCPTR(D2COMMON, GetNextItem, UnitAny*, __stdcall, (UnitAny* ptItem), -11140)
EXFUNCPTR(D2COMMON, UnitIsItem, UnitAny*, __stdcall, (UnitAny* ptItem), -10748)
EXFUNCPTR(D2COMMON, FlushItem, UnitAny*, __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10827)

EXFUNCPTR(D2COMMON, GetItemColor, BYTE*, __stdcall, (UnitAny *ptPlayer, UnitAny* ptItem, BYTE* out, BOOL a4), -11106)

//Skills
EXFUNCPTR(D2COMMON, GetSkillById, Skill*, __fastcall, (UnitAny *ptUnit, int SkillId, int SkillFlags), -10704)
EXFUNCPTR(D2COMMON, GetSkillLevel, int, __stdcall, (UnitAny* ptUnit, Skill* ptSkill, int aType), -10109)
EXFUNCPTR(D2COMMON, GetLeftSkill, Skill*, __stdcall, (UnitAny* ptUnit), -10064)
EXFUNCPTR(D2COMMON, GetRightSkill, Skill*, __stdcall, (UnitAny* ptUnit), -11036)
EXFUNCPTR(D2COMMON, GetCurrentSkill, Skill*, __stdcall, (UnitAny* ptUnit), -10442)
EXFUNCPTR(D2COMMON, GetSkillId, int, __stdcall, (Skill* ptSkill, char* File, int Line), -10170)
EXFUNCPTR(D2COMMON, GetMissileSkillId, int, __stdcall, (UnitAny* ptUnit), -11037)
EXFUNCPTR(D2COMMON, EvaluateSkill, int, __stdcall, (UnitAny* ptUnit, int FormulaNo, int SkillId, int SkillLvl), -10074) // 3442 - Nat Res (153), 2977 - Salv (125)
EXFUNCPTR(D2COMMON, AddSkillToUnit, int, __stdcall, (UnitAny *ptUnit, DWORD SkillNo, DWORD SkillLvl, BOOL a4, char *szFile, int aLine), -10255)
EXFUNCPTR(D2COMMON, RefreshSkills, int, __stdcall, (UnitAny* ptUnit), -10421)
EXFUNCPTR(D2COMMON, SetStartFlags, void, __stdcall, (UnitAny* ptUnit, BOOL a2), -10948)
EXFUNCPTR(D2COMMON, SetCurrentSkill, int, __stdcall, (UnitAny* ptUnit, Skill* pSkill), -10884)

EXFUNCPTR(D2COMMON, GetPlayerSkillCount, int, __stdcall, (int ClassId), -10631)
EXFUNCPTR(D2COMMON, GetPlayerSkillIdFromList, int, __stdcall, (int ClassId, int SkillNo), -10229)

EXFUNCPTR(D2COMMON, isInShapeForm, int, __stdcall, (UnitAny* ptUnit), -10351)
EXFUNCPTR(D2COMMON, GetStateNoBySkillId, int, __stdcall, (WORD SkillId), -10721)
EXFUNCPTR(D2COMMON, RefreshAura, int, __stdcall, (UnitAny* pUnit, WORD SkillId), -10105)

//Levels, Act etc
EXFUNCPTR(D2COMMON, GetLevelNoByRoom, int, __stdcall, (Room1* ptRoom), -11021)
EXFUNCPTR(D2COMMON, isRoomInTown, BOOL, __stdcall, (Room1* ptRoom), -10845)
EXFUNCPTR(D2COMMON, GetActNoByLevelNo, int, __stdcall, (int dwLevelNo), -10026)
EXFUNCPTR(D2COMMON, GetTownLevel, int, __stdcall, (int dwActNo), -10394)
EXFUNCPTR(D2COMMON, GetUnitMaxLife, unsigned int, __stdcall, (UnitAny *ptUnit), -10983)
EXFUNCPTR(D2COMMON, GetUnitMaxMana, unsigned int, __stdcall, (UnitAny *ptUnit), -10042)
EXFUNCPTR(D2COMMON, GetUnitLifePercent, unsigned int, __stdcall, (UnitAny *ptUnit), -10677)
EXFUNCPTR(D2COMMON, LoadAct, Act*, __stdcall, (DWORD ActNumber, DWORD InitSeed, DWORD Unk0, Game *pGame, DWORD DiffLvl, DWORD* pMemPool, DWORD TownLevelId, DWORD Func1, DWORD Func2), -10669)
EXFUNCPTR(D2COMMON, GetRoomXYByLevel, Room1*, __stdcall, (Act* ptAct, int LevelNo, int Unk0, int* xPos, int* yPos, int UnitAlign), -10743)
EXFUNCPTR(D2COMMON, ChangeCurrentMode, int, __stdcall, (UnitAny* ptUnit, int Mode), -10572)
EXFUNCPTR(D2COMMON, GetUnitRoom, Room1*, __stdcall, (UnitAny* ptUnit), -10933)
EXFUNCPTR(D2COMMON, GetPathX, int, __stdcall, (Path* ptPath), -11081)
EXFUNCPTR(D2COMMON, GetPathY, int, __stdcall, (Path* ptPath), -10712)
EXFUNCPTR(D2COMMON, GetPathUNK, int, __stdcall, (Path* ptPath), -10546)
EXFUNCPTR(D2COMMON, SetPathTarget, int, __stdcall, (Path* ptPath, UnitAny* pUnit), -10183)
EXFUNCPTR(D2COMMON, SetPathType, int, __stdcall, (Path* ptPath, int aType), -10189)
EXFUNCPTR(D2COMMON, AssignPath, int, __stdcall, (Path* ptPath, UnitAny* pUnit, int aZero), -10945)
EXFUNCPTR(D2COMMON, SetGfxState, void, __stdcall, (UnitAny *ptUnit, int StateNo, int HowSet), -10702)
EXFUNCPTR(D2COMMON, CheckCoordType, int, __stdcall, (UnitAny *ptUnit, int pX, int pY, int Type), -10628)
EXFUNCPTR(D2COMMON, ValidateCoord, int, __stdcall, (Room1 * ptRoom, int pX, int pY, int Type), -10765)
EXFUNCPTR(D2COMMON, ValidateCoord2, int, __stdcall, (int pX, int pY, UnitAny* ptUnit, int Type), -10793)
EXFUNCPTR(D2COMMON, isUnitInMeleeRange, int, __stdcall, (UnitAny* pUnit1, UnitAny* pUnit2, int Range), -10913)
EXFUNCPTR(D2COMMON, isInLOS, int, __stdcall, (UnitAny* pUnit1, UnitAny* pUnit2, int Range), -10123)
EXFUNCPTR(D2COMMON, GetUnitAligment, int, __stdcall, (UnitAny* pUnit), -10972)
EXFUNCPTR(D2COMMON, 10330, int, __stdcall, (Room1* ptRoom, int xPos, int yPos), -10330)
EXFUNCPTR(D2COMMON, GetPortalFlags, int, __stdcall, (UnitAny* pUnit), -11160)
EXFUNCPTR(D2COMMON, SetPortalFlags, void, __stdcall, (UnitAny* pUnit, int Flags), -11048)
EXFUNCPTR(D2COMMON, UpdateRoomWithPortal, void, __stdcall, (Room1* pRoom, int bUnk), -11084)
//QUESTS

EXFUNCPTR(D2COMMON, GetQuestState, bool, __stdcall, (QuestFlags *pQuestStrc, QuestsId QuestNo, int QuestState), -10753)
EXFUNCPTR(D2COMMON, SetQuestState, bool, __stdcall, (QuestFlags *pQuestStrc, QuestsId QuestNo, int QuestState), -10930)

//6FC84550 <-zacznij od tego

//D2LANG
EXFUNCPTR(D2LANG, GetLocaleText, wchar_t*, __fastcall, (short nTxtNo), -10000)

//D2CMP
EXFUNCPTR(D2CMP, MixPalette, BYTE*, __stdcall, (int TransLvl, int ColorNo), -10071)

//FOG
EXFUNCPTR(FOG, EnterCriticalSection, void, __fastcall, (CRITICAL_SECTION *ptLock, int aLine), -10050)
EXFUNCPTR(FOG, Error, void, __cdecl, (const char* File, void* Addr, int Line), -10024)
EXFUNCPTR(FOG, GetErrorData, void*, __cdecl, (), -10265)
EXFUNCPTR(FOG, AllocServerMemory, void*, __fastcall, (void *pMemPool, int nBytes, char *szFile, int Line, int aNull), -10045)
EXFUNCPTR(FOG, FreeServerMemory, void, __fastcall, (void *pMemPool, void *Mem, char *szFile, int Line, int aNull), -10046)
EXFUNCPTR(FOG, GetTime, DWORD, __cdecl, (), -10055)
EXFUNCPTR(FOG, InitBitBuffer, void, __stdcall, (BitBuffer* ptBitBuffer, void * ptBuffer, int nSize), -10126)

//D2NET 0xa30000+0xFA7C0 +  2*(4*0xPACKET) -> TO SVR PACKET HANDLER
EXFUNCPTR(D2NET, SendPacket, DWORD, __stdcall, (DWORD unk1, DWORD ClientID, BYTE *ThePacket, DWORD PacketLen), -10018) // Bardzo zla metoda wysylania pakietow (16.06.11 -> jednak jest bezpieczniejsza)
EXFUNCPTR(D2NET, ReceivePacket, bool, __stdcall, (int PacketLen, int _1, BYTE *aPacket), -10020)
EXFUNCPTR(D2NET, GetClient, DWORD, __stdcall, (DWORD ClientID), -10005) //Get NetSocket
EXFUNCPTR(D2NET, isCorrectClientPacket, BOOL, __fastcall, (BYTE *aPacket, DWORD aLen, DWORD *result), -10029)

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
EXASMPTR(D2GAME, GameFindUnitFunc_I, 0xBEF80) // DWORD __fastcall (DWORD ptGame, DWORD dwUnitType, DWORD dwUnitId)
EXASMPTR(D2GAME, GetGameByClientID_I, 0xE4A30)   //WARNING: Every use remains thread in CRITICAL_SECTION!
EXASMPTR(D2GAME, RemoveItem_I, 0x45DD0)
EXASMPTR(D2GAME, SaveItems_I, 0x24DB0)
EXASMPTR(D2GAME, SendPacket_I, 0xA0D50) //  DWORD __fastcall, (ClientData *ptClientData, BYTE *aPacket, int Length)
EXASMPTR(D2GAME, Send0XAEPacket_I, 0xA0EA0) //  DWORD __fastcall, (void *ptPlayer,DWORD Length,DWORD *Packet)
EXASMPTR(D2GAME, LeaveCriticalSection_I, 0xE3AD0)
EXASMPTR(D2GAME, SpawnMonsterInCurrentRoom_I, 0xEC7E0)
EXASMPTR(D2GAME, Broadcast5A_I, 0xE6CA0) // int __userpurge BroadcastLeaving5A_6FD06CA0<eax>(ClientData *ptClient<esi>, Game *ptGame, BYTE MsgType)

//EXP SHARE RELATED
EXASMPTR(D2GAME, ForEachInParty_I, 0xE1E80)
EXASMPTR(D2GAME, GetExpGained_I, 0x7B3E0)
EXASMPTR(D2GAME, LevelAwards_I, 0x79540)
//NODES
EXASMPTR(D2GAME, GetDistanceFromXY_I, 0xC1890)
EXASMPTR(D2GAME, NodesUnk_I, 0x2A420)
EXASMPTR(D2GAME, NodesUnk_II, 0x80290)
//ITEMS
EXASMPTR(D2GAME, CreateItem_I, 0xD15B0)
EXASMPTR(D2GAME, ForEach_I, 0xBE830)
EXASMPTR(D2GAME, FE_Remove, 0xCE550)

EXASMPTR(D2GAME, UpdateClient_I, 0xA1AE0) //Works for durablity like stats ;(
EXASMPTR(D2GAME, DropItem_I, 0xCE040)
EXASMPTR(D2GAME, DupeItem_I, 0xD0410)
EXASMPTR(D2GAME, GetItemCost_P, 0x913CA)
EXASMPTR(D2GAME, CreatePlayer_I, 0x26E40)
EXASMPTR(D2GAME, MoveUnitToLevelId_I, 0x622C0)
EXASMPTR(D2GAME, MoveUnitToXY_I, 0xC0610)
EXASMPTR(D2GAME, CheckXYOccupy_I, 0x1DF0)
EXASMPTR(D2GAME, isUnitInRange_I, 0x2CF20)
EXASMPTR(D2GAME, FindFreeCoords_I, 0xCE770)

EXASMPTR(D2GAME, ChatOldCode_J, 0x2E676)
EXASMPTR(D2GAME, UseableItems_J, 0x4BC88)

//DAMAGE STUFF
EXASMPTR(D2GAME, OnMeleeDamage, 0x7B8E0)
EXASMPTR(D2GAME, OnMeleeDamage_I, 0x7C8C2)
EXASMPTR(D2GAME, CalDmg, 0x7C800)
EXASMPTR(D2GAME, ApplyBurnDmg_J, 0x7BCF0) // void __stdcall, (UnitAny* pAttacker, UnitAny *pDefender, int BurnDmg, int BurnLen)
EXASMPTR(D2GAME, CreateDamage_J, 0x99136)

EXASMPTR(D2GAME, ManaLeech_P, 0x7AEE8)
EXASMPTR(D2GAME, LifeLeech_P, 0x7AF2E)
EXASMPTR(D2GAME, CastSkill_I, 0x2EDD0)
EXASMPTR(D2GAME, CastSkillOnUnit_I, 0x2F2B0)
//PARTY
EXASMPTR(D2GAME, GetPartyId_I, 0xBEDC0)
EXASMPTR(D2GAME, FreePartyMem_I, 0xE1BA0)  // edi = ptGame

//OTHERS
EXASMPTR(D2GAME, CreatePackets_J, 0x703D0)
EXASMPTR(D2GAME, BroadcastEvent_J, 0x2DC95)

EXASMPTR(D2GAME, ToClient, 0xE71D2)
EXASMPTR(D2GAME, BroadcastEvent, 0x2DC90) // eax = aPacket, push pGame

EXASMPTR(D2GAME, CorpseNew_P, 0x42DAB) //Used in corpse realigment
EXASMPTR(D2GAME, Corpse_P, 0x42DA6)  //Prevents from corpse creation
EXASMPTR(D2GAME, Gold_P, 0x7F560)    //Prevents from gold drop
EXASMPTR(D2GAME, Ear_P, 0xD26C3)		//Prevents from ear creation/drop
EXASMPTR(D2GAME, Hostile_P, 0xF2F67) //Turn off hostile WP delay
EXASMPTR(D2GAME, Hostile2_P, 0xF2F2D) //Turn off unhost/host delay
EXASMPTR(D2GAME, GetUnitX_P, 0xEC6A) //bs tele
EXASMPTR(D2GAME, GetUnitY_P, 0xEC96) //bs tele
EXASMPTR(D2GAME, Stat_P, 0x2D34F) //Strdex bug fix
EXASMPTR(D2GAME, NPCHeal_P, 0x90171) //SM Fix
EXASMPTR(D2GAME, Ressurect_P, 0x32048) //RessurectFix
EXASMPTR(D2GAME, Chat_P, 0x2E670)
EXASMPTR(D2GAME, OnDeath_P, 0x43067) //Death Msg Intercept
EXASMPTR(D2GAME, Monster_I, 0x893A0) //Monster Old Call
EXASMPTR(D2GAME, OnGameDestroy_P, 0xE7A1A) //Need to free LRosterData memory
EXASMPTR(D2GAME, OnGameEnter_J, 0xE76E9) // Need to alloc roster
EXASMPTR(D2GAME, CreateAuraStatList_I, 0xF0C0)

//ITEMS
EXASMPTR(D2GAME, PermStore_P, 0x909EF) //Need to keep ffa items at vendor
EXASMPTR(D2GAME, PermStore2_P, 0x928FC)

EXASMPTR(D2GAME, Send0x21_I, 0xA3F20)
EXASMPTR(D2GAME, SetSkill_I, 0xEBA0)
EXASMPTR(D2GAME, UpdateBonuses_I, 0x4B970)
//TRADE
EXASMPTR(D2GAME, RemoveBonuses_I, 0x47A00)
EXASMPTR(D2GAME, UpdateQuantity_I, 0x46680)
EXASMPTR(D2GAME, UpdateQuantity_II, 0x44C10)
EXASMPTR(D2GAME, AddGold_I, 0xD29B0)
EXASMPTR(D2GAME, RestoreItems_I, 0x25D50)
EXASMPTR(D2GAME, DeleteTimer_I, 0x70C50)
EXASMPTR(D2GAME, DeleteTimers_I, 0xBF290)
EXASMPTR(D2GAME, MoveItems_I, 0x364D0)
EXASMPTR(D2GAME, UpdateItems_I, 0x7EFE0)

//DEATH handler

EXASMPTR(D2GAME, StopSequence_I, 0xBEB30)
EXASMPTR(D2GAME, RemoveBuffs_I, 0x41670)
EXASMPTR(D2GAME, ResetTimers_I, 0xC1230)
EXASMPTR(D2GAME, RemoveInteraction_I, 0x7FA00)
EXASMPTR(D2GAME, SetPlayerUnitMode_I, 0x41870)

#ifndef __DEFINE_EXPTRS
};
extern _d2p D2Ptrs;
struct _d2v {
#else
}
_d2v D2Vars = { 0 };
void SetupD2Vars() {
#endif

//EXVARPTR(D2GAME, pUnitListOffset, DWORD*, 0xFB2B4)
EXVARPTR(D2GAME, ClientPacketTable, PacketTbl, 0xFA7C0)
EXVARPTR(D2GAME, pSpell, pSpellTbl, 0x105098)

EXVARPTR(D2NET, ToSrvPacketSizes, int, 0xABD8)
EXVARPTR(D2COMMON, SkillTxt, SkillsTxt*, 0xA1328)
EXVARPTR(D2COMMON, SkillDescTxt, BYTE*, 0xA131C)
EXVARPTR(D2COMMON, sgptDataTables, sgptDataTable*, -11170)

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
