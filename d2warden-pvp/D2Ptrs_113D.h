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
#pragma optimize ("", off)

#ifdef _DEFINE_PTRS
enum { DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2LANG, DLLNO_D2WIN, DLLNO_D2NET, DLLNO_D2GAME, DLLNO_D2LAUNCH, DLLNO_FOG, DLLNO_BNCLIENT, DLLNO_STORM, DLLNO_D2CMP, DLLNO_D2MULTI };

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|((b1)<<8))

//NOTE :- reference vars buggy 
#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2;  namespace D2Funcs { d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *) GetDllOffset(DLLOFFSET(d1,o1)); }
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;  namespace D2Vars { d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)GetDllOffset(DLLOFFSET(d1,o1)); }
#define D2ASMPTR(d1,v1,o1)        namespace D2Ptrs { DWORD d1##_##v1 = GetDllOffset(DLLOFFSET(d1,o1)); }

#else

#define D2FUNCPTR(d1,v1,t1,t2,o1)	typedef t1 d1##_##v1##_t t2; namespace D2Funcs { extern d1##_##v1##_t *d1##_##v1; }
#define D2VARPTR(d1,v1,t1,o1)		typedef t1 d1##_##v1##_t; namespace D2Vars { extern d1##_##v1##_t *d1##_##v1; }
#define D2ASMPTR(d1,v1,o1)		    namespace D2Ptrs { extern  DWORD d1##_##v1; }

#endif

#define _D2PTRS_START D2Funcs::D2GAME_GetGameByNetSocket

//pfnCallbackTbl: 0x6FD31C28

//D2GAME ptrs (0x6FC20000)
D2FUNCPTR(D2GAME, GetGameByNetSocket, Game* __stdcall, (DWORD NetSocket), 0xBC700) //1.13d -- Uwaga ! Kazde uzycie zostawia watek w sekcji krytycznej!!!
D2FUNCPTR(D2GAME, SpawnMonster, UnitAny* __fastcall, (signed int MonIdx, int MonCount, Game* ptGame, Room1* ptRoom, int xPos, int yPos, int a7, int a8), 0xC8D70) // 1.13d
D2FUNCPTR(D2GAME, GetUnitX, int __fastcall, (UnitAny* ptUnit), 0x10F0) // 1.13d
D2FUNCPTR(D2GAME, GetUnitY, int __fastcall, (UnitAny* ptUnit), 0x1120) // 1.13d
D2FUNCPTR(D2GAME, ExecuteEvent, int __stdcall, (Game *pGame, int EventNo, UnitAny *pUnit, void *Data), 0x9C780) // 1.13d
D2FUNCPTR(D2GAME, UpdatePlayerStats, int __fastcall, (UnitAny *ptOwnerUnit, int dwStat, int dwValue, UnitAny *pDestUnit), 0x68260) // 1.13d
D2FUNCPTR(D2GAME, CreateCorpse, int __stdcall, (Game *pGame, UnitAny *pUnit, int xPos, int yPos, Room1 *pRoom), 0x93640) // 1.13d
D2FUNCPTR(D2GAME, CreateItemEx, UnitAny* __stdcall, (Game *pGame, CreateItem *srCreateItem, int a5), 0xE3110) //1.13d
D2FUNCPTR(D2GAME, SetMonSkill, void __fastcall, (UnitAny *pUnit, int HowSet, int SkillId, int SkillFlags), 0x19FA0) // 1.13d
D2FUNCPTR(D2GAME, AddItemToNPC, int __fastcall, (char *szFile, int aLine, Game *pGame, UnitAny *pNPCUnit, int ItemId, WORD iX, WORD iY, int a6, int a7, int a8), 0xB7640) // 1.13d
D2FUNCPTR(D2GAME, DeleteUnit, int __fastcall, (Game *ptGame, UnitAny *ptUnit), 0x6FC70) // 1.13d
D2FUNCPTR(D2GAME, CopyPortal, UnitAny* __stdcall, (Game *pGame, UnitAny *pSourceUnit, int nLevel, POINT Coords), 0x15F40) // 1.13d
D2FUNCPTR(D2GAME, CreateUnit, UnitAny* __fastcall, (DWORD UnitType, DWORD ClassId, int xPos, int yPos, Game *pGame, Room1 *pRoom1, WORD wFlags, DWORD InitMode, int UnitId), 0x6FE10) //1.13d
D2FUNCPTR(D2GAME, SetUnitMode, int __fastcall, (Game *pGame, UnitAny *pUnit, Skill *pSkill, int aMode, int xPos, int yPos, BOOL bForce), 0x922E0) // 1.13d
D2FUNCPTR(D2GAME, RemoveFromPickedUp, void __stdcall, (UnitAny *pPlayer), 0xB41D0) // 1.13d
D2FUNCPTR(D2GAME, KillPlayer, void __fastcall, (Game *pGame, UnitAny *pVictim, int nMode, UnitAny *pKiller), 0x93C80) // 1.13d

//D2COMMON
D2FUNCPTR(D2COMMON, AddStatToStatList, int __stdcall, (StatList* ptStatList, int nStat, int nValue, int nValue2), -10818) // 1.13d
D2FUNCPTR(D2COMMON, GetStateStatList, StatList* __stdcall, (UnitAny* ptUnit, int StateNo), -10429) //1.13d
D2FUNCPTR(D2COMMON, RemoveStatList, int __stdcall, (UnitAny* ptUnit, StatList* ptStatList), -10669) // 1.13d
D2FUNCPTR(D2COMMON, FreeStatList, int __stdcall, (StatList* ptStatList), -10125) // 1.13d

D2FUNCPTR(D2COMMON, UpdateOverlay, void __stdcall, (UnitAny* ptUnit), -10229) // 1.13d
D2FUNCPTR(D2COMMON, AssignOverlay, void __stdcall, (UnitAny* ptUnit, int OverlayNo, int _1), -10399) // 1.13d

D2FUNCPTR(D2COMMON, CreateStatListEx, StatListEx* __stdcall, (UnitAny *pUnit, char ListFlags, void *pfnCallBack, Game *pGame), -11099) // 1.13d

D2FUNCPTR(D2COMMON, SetAuraState, void __stdcall, (UnitAny *ptUnit, int AuraState, int HowSet), -11152) // 1.13d
D2FUNCPTR(D2COMMON, SetStatListStat, void __stdcall, (StatList* ptStatList, int nStatNo, int nDuration, int Unk), -10560) // 1.13d

D2FUNCPTR(D2COMMON, GetStatSigned, int __stdcall, (UnitAny *ptUnit, int nStat, int nLayer), -10550) // 1.13d
D2FUNCPTR(D2COMMON, GetBaseStatSigned, int __stdcall, (UnitAny *ptUnit, int nStat, int nLayer), -10216) // 1.13d
D2FUNCPTR(D2COMMON, GetUnitState, int __stdcall, (UnitAny *ptUnit, DWORD dwStateNo), -10706) //k
D2FUNCPTR(D2COMMON, SetStat, int __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer), -10590) // 1.13d
D2FUNCPTR(D2COMMON, AddStat, int __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer), -10645) // 1.13d

D2FUNCPTR(D2COMMON, GetExpToAchiveLvl, int __stdcall, (int ClassId, int ExpLvl), -10949) // 1.13d
D2FUNCPTR(D2COMMON, GetMaxCLvl, int __stdcall, (int ClassId), -10835) // 1.13d
D2FUNCPTR(D2COMMON, GetNextCLvl, int __stdcall, (int ClassId, int Experience), -11171) // 1.13d

//ITEM RELATED
D2FUNCPTR(D2COMMON, AllocInventory, Inventory* __stdcall, (void *pMemPool, UnitAny *pOwner), -11011) // 1.13d
D2FUNCPTR(D2COMMON, GetItemIdx, ItemsTxt* __stdcall, (DWORD ItemCode, int * Idx), -10000) // 1.13d
D2FUNCPTR(D2COMMON, GetItemTxt, ItemsTxt* __stdcall, (int RecordNo), -10994) //k
D2FUNCPTR(D2COMMON, GetItemCode, DWORD __stdcall, (UnitAny* ptItem), -10604) // 1.13d
D2FUNCPTR(D2COMMON, SetItemQuality, void __stdcall, (UnitAny* ptItem, int iQual), -11122) // 1.13d
D2FUNCPTR(D2COMMON, SetBeginFlag, void __stdcall, (UnitAny* ptPlayer, int HowSet), -10163) // 1.13d
D2FUNCPTR(D2COMMON, SetItemFlag, void __stdcall, (UnitAny *ptItem, DWORD dwFlag, int HowSet), -10876) // 1.13d
D2FUNCPTR(D2COMMON, SetInvPage, void __stdcall, (UnitAny* ptItem, int iPage), -11026) // 1.13d
D2FUNCPTR(D2COMMON, GetInvPage, int __stdcall, (UnitAny* ptItem), -10291) // 1.13d
D2FUNCPTR(D2COMMON, AddToContainer, void __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10044) // 1.13d

D2FUNCPTR(D2COMMON, SetCursorItem, int __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -11097) // 1.13d
D2FUNCPTR(D2COMMON, GetCursorItem, UnitAny* __stdcall, (Inventory* ptInventory), -10867) // 1.13d
D2FUNCPTR(D2COMMON, GetFirstItem, UnitAny* __stdcall, (Inventory* ptInventory), -11040) // 1.13d
D2FUNCPTR(D2COMMON, GetNextItem, UnitAny* __stdcall, (UnitAny* ptItem), -10879) // 1.13d
D2FUNCPTR(D2COMMON, UnitIsItem, UnitAny* __stdcall, (UnitAny* ptItem), -10897) // 1.13d
D2FUNCPTR(D2COMMON, FlushItem, UnitAny* __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10490) // 1.13d

D2FUNCPTR(D2COMMON, GetItemColor, BYTE* __stdcall, (UnitAny *ptPlayer, UnitAny* ptItem, BYTE* out, BOOL a4), -11062) // 1.13d

//Skills
D2FUNCPTR(D2COMMON, GetSkillById, Skill* __fastcall, (UnitAny *ptUnit, int SkillId, int SkillFlags), -10984) //k
D2FUNCPTR(D2COMMON, GetSkillLevel, int __stdcall, (UnitAny* ptUnit, Skill* ptSkill, int aType), -10007) //k
D2FUNCPTR(D2COMMON, GetLeftSkill, Skill* __stdcall, (UnitAny* ptUnit), -10909) //k
D2FUNCPTR(D2COMMON, GetRightSkill, Skill* __stdcall, (UnitAny* ptUnit), -10942) //k
D2FUNCPTR(D2COMMON, GetCurrentSkill, Skill* __stdcall, (UnitAny* ptUnit), -10459) //k
D2FUNCPTR(D2COMMON, GetSkillId, int __stdcall, (Skill* ptSkill, char* szFile, int aLine), -11151) //k
D2FUNCPTR(D2COMMON, GetMissileSkillId, int __stdcall, (UnitAny* ptUnit), -11155) // 1.13d
D2FUNCPTR(D2COMMON, EvaluateSkill, int __stdcall, (UnitAny* ptUnit, int FormulaNo, int SkillId, int SkillLvl), -11081) //k
D2FUNCPTR(D2COMMON, AddSkillToUnit, int __stdcall, (UnitAny *ptUnit, DWORD SkillNo, DWORD SkillLvl, BOOL a4, char *szFile, int aLine), -10335) // 1.13d
D2FUNCPTR(D2COMMON, RefreshSkills, int __stdcall, (UnitAny* ptUnit), -10090) // 1.13d
D2FUNCPTR(D2COMMON, SetStartFlags, void __stdcall, (UnitAny* ptUnit, BOOL a2), -10163) // 1.13d
D2FUNCPTR(D2COMMON, ResetFlag, int __stdcall, (UnitAny* ptUnit, Skill* pSkill), -10816) // 1.13d

D2FUNCPTR(D2COMMON, GetPlayerSkillCount, int __stdcall, (int ClassId), -10407) // 1.13d
D2FUNCPTR(D2COMMON, GetPlayerSkillIdFromList, int __stdcall, (int ClassId, int SkillNo), -10340) // 1.13d

D2FUNCPTR(D2COMMON, isInShapeForm, int __stdcall, (UnitAny* ptUnit), -10535) // 1.13d

//Levels, Act etc
D2FUNCPTR(D2COMMON, GetLevelNoByRoom, int __stdcall, (Room1* ptRoom), -10691) // k
D2FUNCPTR(D2COMMON, isRoomInTown, BOOL __stdcall, (Room1* ptRoom), -10803) // 1.13d
D2FUNCPTR(D2COMMON, GetActNoByLevelNo, int __stdcall, (int dwLevelNo), -10864) //k
D2FUNCPTR(D2COMMON, GetTownLevel, int __stdcall, (int dwActNo), -10353) //k
D2FUNCPTR(D2COMMON, GetUnitMaxLife, unsigned int __stdcall, (UnitAny *ptUnit), -10574) //k
D2FUNCPTR(D2COMMON, GetUnitMaxMana, unsigned int __stdcall, (UnitAny *ptUnit), -10084) //k
D2FUNCPTR(D2COMMON, LoadAct, Act* __stdcall, (DWORD ActNumber, DWORD InitSeed, DWORD Unk0, Game *pGame, DWORD DiffLvl, DWORD* pMemPool, DWORD TownLevelId, DWORD Func1, DWORD Func2), -10024) // 1.13d
D2FUNCPTR(D2COMMON, GetRoomXYByLevel, Room1* __stdcall, (Act* ptAct, int LevelNo, int Unk0, int* xPos, int* yPos, int UnitAlign), -10632) // 1.13d
D2FUNCPTR(D2COMMON, ChangeCurrentMode, int __stdcall, (UnitAny* ptUnit, int Mode), -10193) // 1.13d
D2FUNCPTR(D2COMMON, GetUnitRoom, Room1* __stdcall, (UnitAny *ptUnit), -10846) //k
D2FUNCPTR(D2COMMON, GetPathX, int __stdcall, (Path* ptPath), -10465) // 1.13d
D2FUNCPTR(D2COMMON, GetPathY, int __stdcall, (Path* ptPath), -10030) // 1.13d
D2FUNCPTR(D2COMMON, GetPathUNK, int __stdcall, (Path* ptPath), -10349) // 1.13d
D2FUNCPTR(D2COMMON, SetPathTarget, int __stdcall, (Path* ptPath, UnitAny* pUnit), -10331) // 1.13d
D2FUNCPTR(D2COMMON, SetPathType, int __stdcall, (Path* ptPath, int aType), -10325) // 1.13d
D2FUNCPTR(D2COMMON, AssignPath, int __stdcall, (Path* ptPath, UnitAny* pUnit, int aZero), -10474) // 1.13d
D2FUNCPTR(D2COMMON, SetGfxState, void __stdcall, (UnitAny *ptUnit, int StateNo, int HowSet), -10708) // 1.13d
D2FUNCPTR(D2COMMON, isUnitInMeleeRange, int __stdcall, (UnitAny* pUnit1, UnitAny* pUnit2, int Range), -11023) // 1.13d
D2FUNCPTR(D2COMMON, GetPortalFlags, int __stdcall, (UnitAny* pUnit), -10040) // 1.13d
D2FUNCPTR(D2COMMON, SetPortalFlags, void __stdcall, (UnitAny* pUnit, int Flags), -10033) // 1.13d
D2FUNCPTR(D2COMMON, UpdateRoomWithPortal, void __stdcall, (Room1* pRoom, int bUnk), -10907) // 1.13d
//QUESTS

//D2LANG
D2FUNCPTR(D2LANG, GetLocaleText, wchar_t* __fastcall, (short nLocaleTxtNo), -10004) //k

//D2CMP
D2FUNCPTR(D2CMP, MixPalette, BYTE* __stdcall, (int TransLvl, int ColorNo), -10009) //k

//FOG -- unchaned for 1.13d
D2FUNCPTR(FOG, EnterCriticalSection, void __fastcall, (CRITICAL_SECTION *ptLock, int aLine), -10050)
D2FUNCPTR(FOG, Error, void __cdecl, (const char* File, void* Addr, int Line), -10024)
D2FUNCPTR(FOG, GetErrorData, void* __cdecl, (), -10265)
D2FUNCPTR(FOG, AllocServerMemory, void* __fastcall, (void *pMemPool, int nBytes, char *szFile, int Line, int aNull), -10045)
D2FUNCPTR(FOG, FreeServerMemory, void __fastcall, (void *pMemPool, void *Mem, char *szFile, int Line, int aNull), -10046)

//D2NET 0xa30000+ 0x??? +  2*(4*0xPACKET) -> TO SVR PACKET HANDLER
D2FUNCPTR(D2NET, SendPacket, DWORD __stdcall, (DWORD unk1, DWORD ClientID, BYTE *ThePacket, DWORD PacketLen), -10012) // 1.13d Bardzo zla metoda wysylania pakietow (16.06.11 -> jednak jest bezpieczniejsza)
D2FUNCPTR(D2NET, ReceivePacket, bool __stdcall, (int PacketLen, int _1, BYTE *aPacket), -10015) //k
D2FUNCPTR(D2NET, GetClient, DWORD __stdcall, (DWORD ClientID), -10005) // 1.13d Get NetSocket


// -- COMMON --
D2ASMPTR(D2GAME, GetGameByClientID_I, 0xE4A30)   //WARNING: Every use remains thread in CRITICAL_SECTION!
D2ASMPTR(D2GAME, RemoveItem_I, 0x45DD0)
D2ASMPTR(D2GAME, SaveItems_I, 0x24DB0)
D2ASMPTR(D2GAME, SendPacket_I, 0xDB780) //  DWORD __fastcall, (ClientData *ptClientData, BYTE *aPacket, int Length) 1.13d
D2ASMPTR(D2GAME, Send0XAEPacket_I, 0xA0EA0) //  DWORD __fastcall, (void *ptPlayer,DWORD Length,DWORD *Packet)
D2ASMPTR(D2GAME, LeaveCriticalSection_I, 0xE3AD0)
D2ASMPTR(D2GAME, SpawnMonsterInCurrentRoom_I, 0xEC7E0)

//EXP SHARE RELATED
D2ASMPTR(D2GAME, ForEachInParty_I, 0xE1E80)
D2ASMPTR(D2GAME, GetExpGained_I, 0x7B3E0)
D2ASMPTR(D2GAME, LevelAwards_I, 0x79540)
//NODES
D2ASMPTR(D2GAME, GetDistanceFromXY_I, 0xB0C40) // 1.13d
D2ASMPTR(D2GAME, NodesUnk_I, 0xC6FE0) // 1.13d
D2ASMPTR(D2GAME, NodesUnk_II, 0x2C670) // 1.13d
//ITEMS
D2ASMPTR(D2GAME, CreateItem_I, 0xDFDE0) // 1.13d
D2ASMPTR(D2GAME, ForEach_I, 0x6D590) // 1.13d
D2ASMPTR(D2GAME, FE_Remove, 0xDFDC0) // 1.13d

D2ASMPTR(D2GAME, UpdateClient_I, 0xA1AE0) //Works for durablity like stats ;(
D2ASMPTR(D2GAME, DropItem_I, 0xDF970) // 1.13d
D2ASMPTR(D2GAME, DupeItem_I, 0xE2C40) // 1.13d
D2ASMPTR(D2GAME, CreatePlayer_I, 0x26E40)
D2ASMPTR(D2GAME, MoveUnitToLevelId_I, 0x622C0)
D2ASMPTR(D2GAME, MoveUnitToXY_I, 0xC0610)
D2ASMPTR(D2GAME, CheckXYOccupy_I, 0x1340) // 1.13d
D2ASMPTR(D2GAME, isUnitInRange_I, 0x2CF20)
D2ASMPTR(D2GAME, FindFreeCoords_I, 0xCE770)

D2ASMPTR(D2GAME, ChatOldCode_J, 0x2E676)
D2ASMPTR(D2GAME, UseableItems_J, 0x4BC88)

//DAMAGE STUFF
D2ASMPTR(D2GAME, ApplyBurnDmg_J, 0x7BCF0) // void __stdcall, (UnitAny* pAttacker, UnitAny *pDefender, int BurnDmg, int BurnLen)
D2ASMPTR(D2GAME, CreateDamage_J, 0x99136)

D2ASMPTR(D2GAME, ManaLeech_P, 0x7AEE8)
D2ASMPTR(D2GAME, LifeLeech_P, 0x7AF2E)
D2ASMPTR(D2GAME, CastSkill_I, 0x2EDD0)
D2ASMPTR(D2GAME, CastSkillOnUnit_I, 0x2F2B0)
//PARTY
D2ASMPTR(D2GAME, GetPartyId_I, 0xBEDC0)
D2ASMPTR(D2GAME, FreePartyMem_I, 0xE1BA0)  // edi = ptGame

//OTHERS
D2ASMPTR(D2GAME, CreatePackets_J, 0x703D0)
D2ASMPTR(D2GAME, BroadcastEvent_J, 0x2DC95)

D2ASMPTR(D2GAME, BroadcastEvent, 0x2DC90) // eax = aPacket, push pGame

D2ASMPTR(D2GAME, Corpse_P, 0x93BC6)  //1.13d -- Prevents from corpse creation
D2ASMPTR(D2GAME, GetUnitX_P, 0xEC6A) //bs tele
D2ASMPTR(D2GAME, GetUnitY_P, 0xEC96) //bs tele
D2ASMPTR(D2GAME, Monster_I, 0x14C80) //Monster Old Call 1.13d
D2ASMPTR(D2GAME, CreateAuraStatList_I, 0xF0C0)

//ITEMS

D2ASMPTR(D2GAME, Send0x21_I, 0xA3F20)
D2ASMPTR(D2GAME, SetSkill_I, 0xEBA0)
D2ASMPTR(D2GAME, UpdateBonuses_I, 0x4B970)
//TRADE
D2ASMPTR(D2GAME, RemoveBonuses_I, 0x47A00)
D2ASMPTR(D2GAME, UpdateQuantity_I, 0x46680)
D2ASMPTR(D2GAME, UpdateQuantity_II, 0xB3ED0) // 1.13d
D2ASMPTR(D2GAME, AddGold_I, 0xD29B0)
D2ASMPTR(D2GAME, RestoreItems_I, 0x25D50)
D2ASMPTR(D2GAME, DeleteTimer_I, 0x70C50)
D2ASMPTR(D2GAME, DeleteTimers_I, 0x6E320) // 1.13d
D2ASMPTR(D2GAME, MoveItems_I, 0x364D0)
D2ASMPTR(D2GAME, UpdateItems_I, 0x7EFE0)
//DEATH handler
D2ASMPTR(D2GAME, StopSequence_I, 0xBEB30)
D2ASMPTR(D2GAME, RemoveBuffs_I, 0x92480) // 1.13d
D2ASMPTR(D2GAME, ResetTimers_I, 0x6D150) // 1.13d
D2ASMPTR(D2GAME, RemoveInteraction_I, 0x9A950) // 1.13d
D2ASMPTR(D2GAME, SetPlayerUnitMode_I, 0x41870)

D2ASMPTR(D2NET, ReceivePacket_I, -10004) //wrong (its get packet size is ok)

//Variables

D2VARPTR(D2GAME, pSpell, pSpellTbl, 0x108690) // 1.13d
D2VARPTR(D2NET, ToSrvPacketSizes, int, 0xABD8) // 1.13d
D2VARPTR(D2COMMON, sgptDataTables, sgptDataTable*, -11173) // 1.13d

#define _D2PTRS_END D2Vars::D2COMMON_sgptDataTables
//087CB0

#undef D2FUNCPTR
#undef D2ASMPTR
#undef D2VARPTR

#pragma optimize ("", on)

