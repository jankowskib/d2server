#ifndef __D2PTRS_H__
#define __D2PTRS_H__
#include "D2Structs.h"
#include "WardenMisc.h"

#pragma optimize ( "", off )

#ifdef _DEFINE_PTRS

enum {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2LANG, DLLNO_D2WIN, DLLNO_D2NET, DLLNO_D2GAME, DLLNO_D2LAUNCH, DLLNO_FOG, DLLNO_BNCLIENT, DLLNO_STORM, DLLNO_D2CMP, DLLNO_D2MULTI};

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|((b1)<<8))

//NOTE :- reference vars buggy
#define D2FUNCPTR(d1,v1,t1,t2,o1) typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2VARPTR(d1,v1,t1,o1)     typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define D2ASMPTR(d1,v1,o1)        DWORD d1##_##v1 = DLLOFFSET(d1,o1);

#else

#define D2FUNCPTR(d1,v1,t1,t2,o1)	typedef t1 d1##_##v1##_t t2; extern d1##_##v1##_t *d1##_##v1;
#define D2VARPTR(d1,v1,t1,o1)		typedef t1 d1##_##v1##_t;    extern d1##_##v1##_t *p_##d1##_##v1;
#define D2ASMPTR(d1,v1,o1)			extern DWORD d1##_##v1;

#endif

#define _D2PTRS_START D2GAME_GetGameByClientID_I

//D2GAME ptrs (0x6FC20000)

D2FUNCPTR(D2GAME, GetGameByClientID_I, DWORD __fastcall, (DWORD ClientID), 0xE4A30)   //Uwaga ! Kazde uzycie zostawia watek w sekcji krytycznej!!!! (godzine siedzialem zeby dojsc ze ta funkcja wywala)
D2FUNCPTR(D2GAME, GetGameByNetSocket, Game* __stdcall, (DWORD NetSocket), 0xE49A0)   //Uwaga ! Kazde uzycie zostawia watek w sekcji krytycznej!!!! (godzine siedzialem zeby dojsc ze ta funkcja wywala)

D2ASMPTR(D2GAME, LeaveCriticalSection_I, 0xE3AD0)
D2FUNCPTR(FOG, EnterCriticalSection,  void __fastcall,(CRITICAL_SECTION *ptLock, int aLine), -10050)

D2FUNCPTR(D2GAME, Send0XAEPacket_I, DWORD __fastcall, (void *ptPlayer,DWORD Length,DWORD *Packet), 0xA0EA0)
D2FUNCPTR(D2GAME, Send0X92Packet, void __fastcall,(Game *pGame, UnitAny *pUnit, UnitAny *pUnit2),0x35420)
D2FUNCPTR(D2GAME, KickCharFromGame, void __stdcall, (DWORD ClientID), -10037)
D2FUNCPTR(D2GAME, SpawnUniqMon, UnitAny* __stdcall, (Game *ptGame, Room1 *ptRoom, int _1, int _2, signed int MonIdx),0xECE80)
D2FUNCPTR(D2GAME, SpawnMonster, UnitAny* __fastcall,(signed int MonIdx, int MonCount, Game* ptGame, Room1* ptRoom, int xPos, int yPos, int a7, int a8),0xEF870)
D2ASMPTR(D2GAME, SpawnMonsterInCurrentRoom_I,0xEC7E0)

D2FUNCPTR(D2GAME, GameFindUnitFunc_I, DWORD __fastcall, (DWORD ptGame, DWORD dwUnitType, DWORD dwUnitId), 0xBEF80)
D2FUNCPTR(D2GAME, GetUnitX, int __fastcall, (UnitAny* ptUnit), 0x1370)
D2FUNCPTR(D2GAME, GetUnitY, int __fastcall, (UnitAny* ptUnit), 0x13A0)
D2FUNCPTR(D2GAME, SendPacket_I, DWORD __fastcall, (ClientData *ptClientData, BYTE *aPacket, int Length), 0xA0D50)

D2FUNCPTR(D2GAME, ExecuteEvent, int __stdcall, (Game *pGame, int EventNo, UnitAny *pUnit, void *Data), 0xC3170)
D2FUNCPTR(D2GAME, InitTimer, void __fastcall, (Game *pGame, UnitAny *pUnit, signed int nTimerType, signed int nGameFrame, DWORD fTimerFunc, int a6, Timer *PrevTimer), 0x70F00)

D2FUNCPTR(D2GAME, UpdatePlayerStats, int __fastcall, (UnitAny *ptOwnerUnit, int dwStat, int dwValue, UnitAny *pDestUnit), 0x2D2E0)
D2FUNCPTR(D2GAME, CreateCorpse, int __stdcall, (Game *pGame, UnitAny *pUnit, int xPos, int yPos, Room1 *pRoom), 0x424C0)
D2FUNCPTR(D2GAME, CreateItemEx, UnitAny* __stdcall,(Game *pGame, CreateItem *srCreateItem, int a5), 0xD10C0)
D2FUNCPTR(D2GAME, SetMonSkill, void __fastcall,(UnitAny *pUnit, int HowSet, int SkillId, int SkillFlags),0x11C20)
D2FUNCPTR(D2GAME, AddItemToNPC, int __fastcall, (char *szFile, int aLine, Game *pGame, UnitAny *pNPCUnit, int ItemId, WORD iX, WORD iY, int a6, int a7, int a8),0x49B00)
D2FUNCPTR(D2GAME, DeleteUnit, int __fastcall, (Game *ptGame, UnitAny *ptUnit),0xC13A0)
D2FUNCPTR(D2GAME, Transmute, void __stdcall, (Game *ptGame, UnitAny *ptUnit),0x38370)
D2ASMPTR(D2GAME, RemoveItem_I, 0x45DD0)
D2ASMPTR(D2GAME, SaveItems_I,0x24DB0)

D2FUNCPTR(D2GAME, CopyPortal, UnitAny* __stdcall, (Game *pGame, UnitAny *pSourceUnit, int nLevel, POINT Coords), 0x109F0)
D2FUNCPTR(D2GAME, CreateUnit, UnitAny* __fastcall,(DWORD UnitType, DWORD ClassId, int xPos, int yPos, Game *pGame, Room1 *pRoom1, WORD wFlags, DWORD InitMode, int UnitId), 0xC09E0)
D2FUNCPTR(D2GAME, SetUnitMode, int __fastcall, (Game *pGame, UnitAny *pUnit, Skill *pSkill, int aMode, int xPos, int yPos, int a9), 0x41990)

D2FUNCPTR(D2GAME, RemoveFromPickedUp, void __stdcall, (UnitAny *pPlayer), 0x44B50)

//EXP SHARE RELATED
D2ASMPTR(D2GAME,ForEachInParty_I,0xE1E80)
D2ASMPTR(D2GAME,GetExpGained_I,0x7B3E0)
D2ASMPTR(D2GAME,LevelAwards_I,0x79540)
//NODES
D2ASMPTR(D2GAME, GetDistanceFromXY_I,0xC1890)
D2ASMPTR(D2GAME, NodesUnk_I,0x2A420)
D2ASMPTR(D2GAME, NodesUnk_II,0x80290)
//ITEMS

D2ASMPTR(D2GAME, CreateItem_I, 0xD15B0)

D2ASMPTR(D2GAME, ForEach_I, 0xBE830)
D2ASMPTR(D2GAME, FE_Remove, 0xCE550)

D2ASMPTR(D2GAME, UpdateClient_I, 0xA1AE0) //Works for durablity like stats ;(
D2ASMPTR(D2GAME, DropItem_I, 0xCE040)
D2ASMPTR(D2GAME, DupeItem_I, 0xD0410)
D2ASMPTR(D2GAME, GetItemCost_P, 0x913CA)
D2ASMPTR(D2GAME, CreatePlayer_I, 0x26E40)
D2ASMPTR(D2GAME, MoveUnitToLevelId_I, 0x622C0)
D2ASMPTR(D2GAME, MoveUnitToXY_I,0xC0610)
D2ASMPTR(D2GAME, CheckXYOccupy_I,0x1DF0)
D2ASMPTR(D2GAME, isUnitInRange_I,0x2CF20)
D2ASMPTR(D2GAME, FindFreeCoords_I,0xCE770)

D2ASMPTR(D2GAME, ChatOldCode_J,0x2E676)
D2ASMPTR(D2GAME, UseableItems_J,0x4BC88)

//DAMAGE STUFF
D2ASMPTR(D2GAME, OnMeleeDamage,0x7B8E0)
D2ASMPTR(D2GAME, OnMeleeDamage_I,0x7C8C2)
D2ASMPTR(D2GAME, CalDmg,0x7C800)
D2FUNCPTR(D2GAME, ApplyBurnDmg, void __stdcall, (UnitAny* pAttacker, UnitAny *pDefender, int BurnDmg, int BurnLen), 0x7BCF0)
D2ASMPTR(D2GAME, CreateDamage_J, 0x99136)

D2ASMPTR(D2GAME, ManaLeech_P,0x7AEE8)
D2ASMPTR(D2GAME, LifeLeech_P,0x7AF2E)
D2ASMPTR(D2GAME, CastSkill_I,0x2EDD0)
D2ASMPTR(D2GAME, CastSkillOnUnit_I,0x2F2B0)
//PARTY
D2ASMPTR(D2GAME, GetPartyId_I,0xBEDC0)
D2ASMPTR(D2GAME, FreePartyMem_I,0xE1BA0)  // edi = ptGame

//OTHERS
D2ASMPTR(D2GAME, CreatePackets_J,0x703D0)
D2ASMPTR(D2GAME, BroadcastEvent_J,0x2DC95)

D2ASMPTR(D2GAME, ToClient,0xE71D2)
D2ASMPTR(D2GAME, BroadcastEvent,0x2DC90) // eax = aPacket, push pGame

D2ASMPTR(D2GAME, CorpseNew_P,0x42DAB) //Used in corpse realigment
D2ASMPTR(D2GAME, Corpse_P,0x42DA6)  //Prevents from corpse creation
D2ASMPTR(D2GAME, Gold_P,0x7F560)    //Prevents from gold drop
D2ASMPTR(D2GAME, Ear_P,0xD26C3)		//Prevents from ear creation/drop
D2ASMPTR(D2GAME, Hostile_P,0xF2F67) //Turn off hostile WP delay
D2ASMPTR(D2GAME, Hostile2_P,0xF2F2D) //Turn off unhost/host delay
D2ASMPTR(D2GAME, GetUnitX_P,0xEC6A) //bs tele
D2ASMPTR(D2GAME, GetUnitY_P,0xEC96) //bs tele
D2ASMPTR(D2GAME, Stat_P,0x2D34F) //Strdex bug fix
D2ASMPTR(D2GAME, NPCHeal_P,0x90171) //SM Fix
D2ASMPTR(D2GAME, Ressurect_P,0x32048) //RessurectFix
D2ASMPTR(D2GAME, Chat_P,0x2E670)
D2ASMPTR(D2GAME, OnDeath_P,0x43067) //Death Msg Intercept
D2ASMPTR(D2GAME, Monster_I,0x893A0) //Monster Old Call
D2ASMPTR(D2GAME, OnGameDestroy_P,0xE7A1A) //Need to free LRoster memory
D2ASMPTR(D2GAME, OnGameEnter_J,0xE76E9) // Need to alloc roster

D2ASMPTR(D2GAME, CreateAuraStatList_I,0xF0C0)

//ITEMS
D2ASMPTR(D2GAME, PermStore_P,0x909EF) //Need to keep ffa items at vendor
D2ASMPTR(D2GAME, PermStore2_P,0x928FC)

D2ASMPTR(D2GAME, Send0x21_I,0xA3F20)
D2ASMPTR(D2GAME, SetSkill_I,0xEBA0)
D2ASMPTR(D2GAME, UpdateBonuses_I,0x4B970)
//TRADE
D2ASMPTR(D2GAME, RemoveBonuses_I, 0x47A00)
D2ASMPTR(D2GAME, UpdateQuantity_I, 0x46680)
D2ASMPTR(D2GAME, UpdateQuantity_II, 0x44C10)
D2ASMPTR(D2GAME, AddGold_I, 0xD29B0)
D2ASMPTR(D2GAME, RestoreItems_I, 0x25D50)
D2ASMPTR(D2GAME, DeleteTimer_I,0x70C50)
D2ASMPTR(D2GAME, DeleteTimers_I,0xBF290)
D2ASMPTR(D2GAME, MoveItems_I, 0x364D0)
D2ASMPTR(D2GAME, UpdateItems_I, 0x7EFE0)

//DEATH handler

D2ASMPTR(D2GAME, StopSequence_I,0xBEB30)
D2ASMPTR(D2GAME, RemoveBuffs_I,0x41670)
D2ASMPTR(D2GAME, ResetTimers_I,0xC1230)
D2ASMPTR(D2GAME, RemoveInteraction_I,0x7FA00)

D2VARPTR(D2GAME, pSpell, pSpellTbl, 0x105098)
D2VARPTR(D2GAME, PacketTable, PacketTbl, 0xFA7C0)

//00B0526D      83BD 8C000000>CMP DWORD PTR SS:[EBP+8C],10
//00AB027F      83BD 8C000000>CMP DWORD PTR SS:[EBP+8C],9
//6FF5EE50   /EB 15           JMP SHORT Fog.6FF5EE67
//6FB6C946                 cmp     edi, 8

//6FACD2B0

//D2COMMON
D2FUNCPTR(D2COMMON, CreateStatList, StatList* __stdcall, (void *ptMemPool, DWORD dwFlags, DWORD ExpireFrame, DWORD UnitType, DWORD UnitId),-10522)
D2FUNCPTR(D2COMMON, ConnectStatList, void __stdcall, (UnitAny* ptUnit, StatList * ptStatList, BOOL _1), -10662)
D2FUNCPTR(D2COMMON, AddStatToStatList, int __stdcall, (StatList* ptStatList, int nStat, int nValue, int nValue2),-10564)
D2FUNCPTR(D2COMMON, AddStateToStatList, void __stdcall, (StatList* ptStatList, int StateNo), -10155)
D2FUNCPTR(D2COMMON, GetStateStatList, StatList* __stdcall, (UnitAny* ptUnit, int StateNo), -10219)
D2FUNCPTR(D2COMMON, GetStatListByType, StatList* __stdcall, (UnitAny* ptUnit, int Typeo), -10726)
D2FUNCPTR(D2COMMON, RemoveStatList, int __stdcall, (UnitAny* ptUnit, StatList* ptStatList), -11093)
D2FUNCPTR(D2COMMON, FreeStatList, int __stdcall, (StatList* ptStatList), -10032)

D2FUNCPTR(D2COMMON, UpdateOverlay, void __stdcall, (UnitAny* ptUnit), -10200)
D2FUNCPTR(D2COMMON, AssignOverlay, void __stdcall, (UnitAny* ptUnit, int OverlayNo, int _1), -11120)

D2FUNCPTR(D2COMMON, CreateStatListEx, StatListEx* __stdcall, (UnitAny *pUnit, char ListFlags, void *pfnCallBack, Game *pGame), -10724)

D2FUNCPTR(D2COMMON, SetAuraState, void __stdcall, (UnitAny *ptUnit, int AuraState, int HowSet),-10541)
D2FUNCPTR(D2COMMON, SetStatListStat, void __stdcall, (StatList* ptStatList, int nStatNo, int nDuration, int Unk),-10415)

D2FUNCPTR(D2COMMON, GetStatSigned, int __stdcall, (UnitAny *ptUnit, int nStat, int nLayer),-10061)
D2FUNCPTR(D2COMMON, GetBaseStatSigned, int __stdcall, (UnitAny *ptUnit, int nStat, int nLayer),-10550)
D2FUNCPTR(D2COMMON, GetUnitState, int __stdcall, (UnitAny *ptUnit, DWORD dwStateNo), -10604)
D2FUNCPTR(D2COMMON, SetStat, int __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer),-10590)
D2FUNCPTR(D2COMMON, AddStat, int __stdcall, (UnitAny *ptUnit, int nStat, int nValue, int nLayer),-10627)

D2FUNCPTR(D2COMMON, 11117, int __stdcall, (StatList* ptStatList, int SkillNo), -11117)
D2FUNCPTR(D2COMMON, 10083, int __stdcall, (StatList* ptStatList, int SkillLvl), -10083)

D2FUNCPTR(D2COMMON, GetGoldLimit, int __stdcall, (UnitAny* ptUnit), -11131)
D2FUNCPTR(D2COMMON, GetBankLimit, int __stdcall, (UnitAny* ptUnit), -10864)

D2FUNCPTR(D2COMMON, GetExpToAchiveLvl, int __stdcall, (int ClassId, int ExpLvl), -10152)
D2FUNCPTR(D2COMMON, GetMaxCLvl, int __stdcall, (int ClassId), -11123)
D2FUNCPTR(D2COMMON, GetNextCLvl, int __stdcall, (int ClassId, int Experience), -10988)

//ITEM RELATED
D2FUNCPTR(D2COMMON, AllocInventory, Inventory* __stdcall, (void *pMemPool, UnitAny *pOwner), -10976)
D2FUNCPTR(D2COMMON, AllocTradeInventory, int __stdcall, (void *pMemPool, UnitAny *pPlayer, UnitAny *pTrader, BOOL *bNoRoom), -10339)
D2FUNCPTR(D2COMMON, CreateItemFromStream, int __stdcall, (), -11024) // dodaj argumenty
D2FUNCPTR(D2COMMON, GetItemIdx, ItemsTxt* __stdcall, (DWORD ItemCode, int * Idx), -10322)
D2FUNCPTR(D2COMMON, GetItemTxt, ItemsTxt* __stdcall, (int ItemNo), 0x42F60)
D2FUNCPTR(D2COMMON, GetItemCode, DWORD __stdcall, (UnitAny* ptItem), -10360)
D2FUNCPTR(D2COMMON, SetItemQuality, void __stdcall, (UnitAny* ptItem, int iQual), -10297)
D2FUNCPTR(D2COMMON, SetBeginFlag, void __stdcall, (UnitAny* ptPlayer, int HowSet), -10948)
D2FUNCPTR(D2COMMON, SetItemFlag, void __stdcall, (UnitAny *ptItem, DWORD dwFlag, int HowSet), -10970)
D2FUNCPTR(D2COMMON, SetInvPage, void __stdcall, (UnitAny* ptItem, int iPage), -10608)
D2FUNCPTR(D2COMMON, GetInvPage, int __stdcall, (UnitAny* ptItem), -10399)
D2FUNCPTR(D2COMMON, GetItemCost,int __stdcall, (UnitAny *pPlayer, UnitAny *ptItem, int DiffLvl, QuestFlags *pQuestFlags, int NpcClassId, int InvPage), -10511)

D2FUNCPTR(D2COMMON, AddToContainer, void __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10371)
D2FUNCPTR(D2COMMON, SetItemCMDFlag, void __stdcall, (UnitAny *pItem, int Flag, int HowSet), -10905)

D2FUNCPTR(D2COMMON, AddSockets, void __stdcall,  (UnitAny* ptItem, int SocketsNo), -10667)
D2FUNCPTR(D2COMMON, Personalize, void __stdcall,  (UnitAny* ptItem, PlayerData* ptPlayer), -10335)

D2FUNCPTR(D2COMMON, SetCursorItem, int __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -11029)
D2FUNCPTR(D2COMMON, GetCursorItem, UnitAny* __stdcall, (Inventory* ptInventory), -10914)
D2FUNCPTR(D2COMMON, GetFirstItem, UnitAny* __stdcall, (Inventory* ptInventory), -10535)
D2FUNCPTR(D2COMMON, GetNextItem, UnitAny* __stdcall, (UnitAny* ptItem), -11140)
D2FUNCPTR(D2COMMON, UnitIsItem, UnitAny* __stdcall, (UnitAny* ptItem), -10748)
D2FUNCPTR(D2COMMON, FlushItem, UnitAny* __stdcall, (Inventory* ptInventory, UnitAny* ptItem), -10827)

D2FUNCPTR(D2COMMON, GetItemColor, BYTE* __stdcall, (UnitAny *ptPlayer, UnitAny* ptItem, BYTE* out, BOOL a4), -11106)

//Skills
D2FUNCPTR(D2COMMON, GetSkillById, Skill* __fastcall, (UnitAny *ptUnit, int SkillId, int SkillFlags), -10704)
D2FUNCPTR(D2COMMON, GetSkillLevel, int __stdcall, (UnitAny* ptUnit, Skill* ptSkill, int aType),-10109)
D2FUNCPTR(D2COMMON, GetLeftSkill, Skill* __stdcall,(UnitAny* ptUnit), -10064)
D2FUNCPTR(D2COMMON, GetRightSkill, Skill* __stdcall,(UnitAny* ptUnit), -11036)
D2FUNCPTR(D2COMMON, GetCurrentSkill, Skill* __stdcall,(UnitAny* ptUnit), -10442)
D2FUNCPTR(D2COMMON, GetSkillId, int __stdcall,(Skill* ptSkill, char* File, int Line), -10170)
D2FUNCPTR(D2COMMON, GetMissileSkillId, int __stdcall,(UnitAny* ptUnit), -11037)
D2FUNCPTR(D2COMMON, EvaluateSkill, int __stdcall, (UnitAny* ptUnit, int FormulaNo, int SkillId, int SkillLvl),-10074) // 3442 - Nat Res (153), 2977 - Salv (125)
D2FUNCPTR(D2COMMON, AddSkillToUnit, int __stdcall, (UnitAny *ptUnit, DWORD SkillNo, DWORD SkillLvl, BOOL a4, char *szFile, int aLine), -10255)
D2FUNCPTR(D2COMMON, RefreshSkills, int __stdcall, (UnitAny* ptUnit), -10421)
D2FUNCPTR(D2COMMON, SetStartFlags, void __stdcall, (UnitAny* ptUnit, BOOL a2), -10948)
D2FUNCPTR(D2COMMON, ResetFlag, int __stdcall, (UnitAny* ptUnit, Skill* pSkill),-10884)

D2FUNCPTR(D2COMMON, GetPlayerSkillCount, int __stdcall, (int ClassId), -10631)
D2FUNCPTR(D2COMMON, GetPlayerSkillIdFromList, int __stdcall, (int ClassId, int SkillNo), -10229)

D2FUNCPTR(D2COMMON, isInShapeForm, int __stdcall, (UnitAny* ptUnit), -10351)

D2VARPTR(D2COMMON, SkillTxt, SkillsTxt*, 0xA1328)
D2VARPTR(D2COMMON, SkillDescTxt, BYTE*, 0xA131C)

//Levels, Act etc
D2FUNCPTR(D2COMMON, GetLevelNoByRoom, int __stdcall, (Room1* ptRoom), -11021)
D2FUNCPTR(D2COMMON, isRoomInTown, BOOL __stdcall, (Room1* ptRoom), -10845)
D2FUNCPTR(D2COMMON, GetActNoByLevelNo, int __stdcall, (int dwLevelNo), -10026)
D2FUNCPTR(D2COMMON, GetTownLevel, int __stdcall, (int dwActNo), -10394)
D2FUNCPTR(D2COMMON, GetUnitMaxLife, unsigned int __stdcall, (UnitAny *ptUnit),-10983)
D2FUNCPTR(D2COMMON, GetUnitMaxMana, unsigned int __stdcall, (UnitAny *ptUnit),-10042)
D2FUNCPTR(D2COMMON, GetUnitLifePercent, unsigned int __stdcall, (UnitAny *ptUnit),-10677)
D2FUNCPTR(D2COMMON, LoadAct, Act* __stdcall, (DWORD ActNumber, DWORD InitSeed, DWORD Unk0, Game *pGame, DWORD DiffLvl, DWORD* pMemPool, DWORD TownLevelId, DWORD Func1, DWORD Func2), -10669)
D2FUNCPTR(D2COMMON, GetRoomXYByLevel, Room1* __stdcall, (Act* ptAct, int LevelNo, int Unk0, int* xPos, int* yPos, int UnitAlign), -10743)
D2FUNCPTR(D2COMMON, ChangeCurrentMode, int __stdcall, (UnitAny* ptUnit, int Mode),-10572)
D2FUNCPTR(D2COMMON, GetUnitRoom, Room1* __stdcall, (UnitAny* ptUnit), -10933)
D2FUNCPTR(D2COMMON, GetPathX, int __stdcall, (Path* ptPath), -11081)
D2FUNCPTR(D2COMMON, GetPathY, int __stdcall, (Path* ptPath), -10712)
D2FUNCPTR(D2COMMON, GetPathUNK, int __stdcall, (Path* ptPath), -10546)
D2FUNCPTR(D2COMMON, SetPathTarget, int __stdcall, (Path* ptPath, UnitAny* pUnit), -10183)
D2FUNCPTR(D2COMMON, SetPathType, int __stdcall, (Path* ptPath, int aType), -10189)
D2FUNCPTR(D2COMMON, AssignPath, int __stdcall, (Path* ptPath, UnitAny* pUnit, int aZero), -10945)
D2FUNCPTR(D2COMMON, SetGfxState, void __stdcall, (UnitAny *ptUnit, int StateNo, int HowSet), -10702)
D2FUNCPTR(D2COMMON, CheckCoordType, int __stdcall ,(UnitAny *ptUnit, int pX, int pY, int Type), -10628)
D2FUNCPTR(D2COMMON, ValidateCoord, int __stdcall ,(Room1 * ptRoom, int pX, int pY, int Type), -10765)
D2FUNCPTR(D2COMMON, ValidateCoord2, int __stdcall ,(int pX, int pY,UnitAny* ptUnit, int Type), -10793)
D2FUNCPTR(D2COMMON, isUnitInMeleeRange, int __stdcall ,(UnitAny* pUnit1, UnitAny* pUnit2, int Range), -10913)
D2FUNCPTR(D2COMMON, isInLOS, int __stdcall, (UnitAny* pUnit1, UnitAny* pUnit2, int Range), -10123)
D2FUNCPTR(D2COMMON, GetUnitAligment, int __stdcall, (UnitAny* pUnit), -10972)
D2FUNCPTR(D2COMMON, 10330, int __stdcall, (Room1* ptRoom, int xPos, int yPos), -10330)
D2FUNCPTR(D2COMMON, GetPortalFlags, int __stdcall, (UnitAny* pUnit), -11160)
D2FUNCPTR(D2COMMON, SetPortalFlags, void __stdcall, (UnitAny* pUnit, int Flags), -11048)
D2FUNCPTR(D2COMMON, UpdateRoomWithPortal, void __stdcall, (Room1* pRoom, int bUnk), -11084)
//QUESTS

D2FUNCPTR(D2COMMON, GetQuestState, bool __stdcall, (QuestFlags *pQuestStrc, QuestsId QuestNo, int QuestState), -10753)
D2FUNCPTR(D2COMMON, SetQuestState, bool __stdcall, (QuestFlags *pQuestStrc, QuestsId QuestNo, int QuestState), -10930)


D2VARPTR(D2COMMON, sgptDataTables, sgptDataTable*, -11170)

//6FC84550 <-zacznij od tego

//D2LANG
D2FUNCPTR(D2LANG, GetLocaleText, wchar_t* __fastcall, (short nTxtNo), -10000)

//D2CMP
D2FUNCPTR(D2CMP, MixPalette, BYTE* __stdcall,(int TransLvl, int ColorNo), -10071)

//FOG

D2FUNCPTR(FOG, Error, void __cdecl, (const char* File ,void* Addr ,int Line), -10024)
D2FUNCPTR(FOG, GetErrorData, void* __cdecl, (), -10265)
D2FUNCPTR(FOG, AllocServerMemory, void* __fastcall, (void *pMemPool, int nBytes, char *szFile, int Line, int aNull), -10045)
D2FUNCPTR(FOG, FreeServerMemory, void __fastcall, (void *pMemPool, void *Mem, char *szFile, int Line, int aNull), -10046)

//D2NET 0xa30000+0xFA7C0 +  2*(4*0xPACKET) -> TO SVR PACKET HANDLER
D2FUNCPTR(D2NET, SendPacket, DWORD __stdcall, (DWORD unk1,DWORD ClientID,BYTE *ThePacket,DWORD PacketLen), -10018) // Bardzo zla metoda wysylania pakietow (16.06.11 -> jednak jest bezpieczniejsza)
D2FUNCPTR(D2NET, GetClient, DWORD __stdcall, (DWORD ClientID), -10005) //Get NetSocket
D2FUNCPTR(D2NET, ReceivePacket_I, DWORD __fastcall, (BYTE * ThePacket, DWORD PacketLen, DWORD* Result),-10029) //wrong (its get packet size is ok)

D2VARPTR(D2NET, ToSrvPacketSizes, int ,0xABD8)

D2FUNCPTR(D2NET, ReceivePacket, bool __stdcall, (int PacketLen, int _1, BYTE *aPacket),-10020)

#define _D2PTRS_END D2NET_ReceivePacket
//087CB0

void DefineOffsets();
DWORD GetDllOffset(char *dll, int offset);

#define ASSERT(e) if (e == 0) { Log("Critical error in line %d, plik '%s' , function: '%s'.",__LINE__,__FILE__,__FUNCTION__); exit(-1); }
#define D2ERROR(s) { Log("Critical error '%s' in line %d, plik '%s' , function: '%s'.",s,__LINE__,__FILE__,__FUNCTION__); exit(-1); }
#define LOCK   {/*Log("--> CS : %d : %s",__LINE__,__FUNCTION__); */EnterCriticalSection(&hWarden.WardenLock);}
#define UNLOCK {/*Log("<-- CS : %d : %s",__LINE__,__FUNCTION__); */LeaveCriticalSection(&hWarden.WardenLock);}
#undef D2FUNCPTR
#undef D2ASMPTR
#undef D2VARPTR

#pragma optimize ( "", on )

#endif