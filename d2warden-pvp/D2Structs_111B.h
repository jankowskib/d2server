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

#ifndef D2STRUCTS_H__
#define D2STRUCTS_H__
#include "D2Consts.h"

struct UnitAny;
struct ptClientData;
struct PlayerData;
struct ItemData;
struct MonsterData;
struct ObjectData;
struct Act;
struct Path;
struct StaticPath;
struct StatList;
struct Inventory;
struct Light;
struct Skill;
struct SkillData;
struct SkillsTxt;
struct Damage;
struct Game;
struct ClientData;
struct Waypoint;
struct QuestFlags;
struct Room1;
struct Room2;
struct AiGeneral;
struct sgptDataTable;
struct Stat;
struct StatEx;
struct StatList;
struct StatListEx;
struct ItemsTxt;
struct bItemFlags;
struct PlayerTrade;
struct Timer;
struct TimerList;
struct TimerQuene;

struct LRoster
{
char szName[16];
int Assists;
int Kills;
int Deaths;
LRoster *ptNext;
};


struct pSpellTbl
{
  int (__fastcall *SpellPrepareFunc)(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
  int (__fastcall *SpellDoFunc)(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
};


struct PacketTbl
{
  int (__fastcall *Callback)(Game *pGame, UnitAny *pUnit, BYTE *aPacket, int PacketLen);
  int _1;
};


struct PartyExp // Tymczasowo rozszerzylem do 16 (originally sized 0x4C)
{
  UnitAny *pMonster;
  UnitAny *pPartyMembers[16];
  DWORD PartyLevels[16];
  DWORD nMembers;
  DWORD LvlSum;
};

struct TradeContainer	//sizeof 0x0C
{
	DWORD nOriginalId;		//0x00	An Item Id
	DWORD nTradeId;			//0x04 - || -
	TradeContainer* pNext;	//0x08
};

struct PlayerTrade		//size 0x18 -> resized to 0x20
{
  DWORD nSaveLen;						//0x00
  BYTE *pSaveBuffer;					//0x04
  TradeContainer* pContainer;			//0x08
  int nOfferedGold;						//0x0C
  int GoldNo;							//0x10
  DWORD nGreenCheckTick;				//0x14
  int ARGold;							//0x18
  std::string GoldPass;					//0x1C
};

#pragma pack(push, 1)

struct EventPacket
{
BYTE P_5A;		//0x00
BYTE MsgType;	//0x01
BYTE Color;		//0x02
DWORD Param1;   //0x03 
BYTE Param2;	//0x07
char Name1[16]; //0x08
char Name2[16]; //0x18
};

struct SendDWORDStatPacket  //0x1F
{
	BYTE Header;	//0x00
	BYTE Attrib;	//0x01
	DWORD Amount;	//0x02
};

struct PostTradePacket //0x92
{
	BYTE Header;
	BYTE UnitType;
	DWORD UnitId;
};

struct GoldPacket	//0x2C, size = 18
{
	BYTE Header;
	char szPass[16];
};

struct TradeGold	//0x79, size = 0x06
{
	BYTE Header;
	BYTE bMyUnit;
	DWORD nGold;
};

struct TradeData // 0x78, size = 0x15 (21)
{
	BYTE Header;
	char szName[16];
	DWORD UnitId;
};

struct TradePacket // 0x7
{
  BYTE Header;
  WORD ButtonId;
  WORD Arg1;
  WORD Arg2;
};

struct SkillPacket
{
    BYTE Header;
    WORD xPos;
    WORD yPos;
};

struct SkillTargetPacket
{
    BYTE Header;
	DWORD UnitType;
	DWORD UnitId;
};


struct RosterPacket //0x066, size = 0x07
{
	BYTE Header;
	DWORD UnitId;
	BYTE EventType;
	BYTE EventCount;
};

struct ReassignPacket
{
	BYTE Header;
	BYTE UnitType;
	DWORD UnitId;
	WORD xPos;
	WORD yPos;
	BYTE Reassign;
};

struct bItemFlags //Taken from Necrolis post & Hero Editor
{
	BYTE bNewItem:1;		//1
	BYTE bTarget:1;			//2
	BYTE bTargeting:1;		//3
	BYTE bDeleted:1;		//4
	BYTE bIdentified:1;		//5
	BYTE bQuantity:1;		//6
	BYTE bWeaponSetIn:1;	//7
	BYTE bWeaponSetOut:1;	//8
	BYTE bBroken:1;			//9
	BYTE bRepaired:1;		//10
	BYTE Flag11:1;			//11
	BYTE bSocketed:1;		//12
	BYTE bNonSellable:1;	//13
	BYTE bInStore:1;		//14 Also Has Been Picked Up
	BYTE bNoEquip:1;		//15
	BYTE bNamed:1;			//16
	BYTE bOrgan:1;			//17 Also bEar
	BYTE bStarter:1;		//18 Also bSellCheap
	BYTE Flag19:1;			//19
	BYTE bInit:1;			//20
	BYTE Flag21:1;			//21
	BYTE bCompactSave:1;	//22 Also bSimple
	BYTE bEthereal:1;		//23
	BYTE bJustSaved:1;		//24
	BYTE bPersonalized:1;	//25
	BYTE bLowQuality:1;		//26
	BYTE bRuneword:1;		//27
	BYTE bItem:1;			//28
	BYTE _Unused1:1;		//29
	BYTE _Unused2:1;		//30
	BYTE _Unused3:1;		//31
	BYTE _Unused4:1;		//32
};

#pragma pack(pop)

struct ExEvent //(size 0xD++)
{
BYTE P_A6;		//0x00
BYTE MsgType;	//0x01
WORD PacketLen;	//0x02
BYTE Argument;	//0x04
BYTE Color;		//0x05
union {
	struct {
WORD wX;		//0x06
WORD wY;		//0x08
	};
DWORD UnitId;
};
WORD Sound;		//0x0A // Also CellID
char szMsg[255];//0x0E // Also CellPath
};



struct PacketData //size 0x208
{
 DWORD ClientID;			//0x00 or PacketLen
 BYTE aPacket[512];			//0x04
 PacketData* pNext;			//0x204
};


struct ItemCon //size 0x08
{
DWORD ItemId;
ItemCon *pNext;
};

struct Corpse	//size 0x10
{
	BOOL isCorpse;			//0x00 If true, game will increase player's corpse number
	DWORD UnitId;			//0x04
	DWORD _1;				//0x08 arg passed = 0
	Corpse* pNext;			//0x0C
};

struct Inventory //size 0x40
{
	DWORD dwSignature;				//0x00
	void* pMemPool;					//0x04
	UnitAny* pOwner;				//0x08
	UnitAny* pFirstItem;			//0x0C
	UnitAny* pLastItem;				//0x10
	void* pInvInfo;					//0x14
	DWORD nInvInfo;					//0x18
	DWORD dwLeftItemUid;			//0x1C (WeaponUId)
	UnitAny* pParentItem;			//0x20 (CursorItem)
	DWORD dwOwnerId;				//0x24
	DWORD nFilledSockets;			//0x28
	ItemCon* pFirstCointainer;		//0x2C
	ItemCon* pLastCointainer;		//0x30
	Corpse *pFirstCorpse;			//0x34
	Corpse *pLastCorpse;			//0x38
	DWORD CorpseNumber;				//0x3C
};

struct CollMap
{
	DWORD dwPosGameX;				//0x00
	DWORD dwPosGameY;				//0x04
	DWORD dwSizeGameX;				//0x08
	DWORD dwSizeGameY;				//0x0C
	DWORD dwPosRoomX;				//0x10
	DWORD dwPosRoomY;				//0x14
	DWORD dwSizeRoomX;				//0x18
	DWORD dwSizeRoomY;				//0x1C
	WORD* pMapStart;				//0x20
	WORD* pMapEnd;					//0x22
};

struct Room1
{
	DWORD dwSeed[2];				//0x00
	DWORD dwXStart;					//0x08
	DWORD dwYStart;					//0x0C
	DWORD dwXSize;					//0x10
	DWORD dwYSize;					//0x14
	DWORD _1[6];					//0x18
	DWORD nRoomUnits;				//0x30
	Room1** pRoomsNear;				//0x34 9 rooms
	Room2* pRoom2;					//0x38
	UnitAny* pUnitFirst;			//0x3C
	DWORD _2[6];					//0x40
	void* _1s;						//0x58
	DWORD _3;						//0x5c
	CollMap* Coll;					//0x60
	DWORD _4;						//0x64
	DWORD nPlayers;					//0x68
	Act* pAct;						//0x6C
	DWORD _5;						//0x70
	Room1* pRoomNext;				//0x74
	int nUnknown;					//0x78
	DWORD nRoomsNear;				//0x7C
};

struct ActMisc
{
	void* _1;						//0x00
	DWORD _2[31];					//0x04
	DWORD pfnCallBack;				//0x80
	Act* pAct;						//0x84
	DWORD dwBossTombLevel;			//0x88
	DWORD _3[248];					//0x8C There's at least 1 Room1 in here, leaving undefined for now
	void* pLevelFirst;				//0x46C
	DWORD _4[2];					//0x470
	DWORD dwStaffTombLevel;			//0x478	
};

struct Act
{
	DWORD _1;						//0x00
	void *pEnviroment;				//0x04
	ActMisc* pMisc;					//0x08
	Room1* pRoom1;					//0x0C
	DWORD _2;						//0x10
	DWORD dwAct;					//0x14
	DWORD pfnCallBack;				//0x18
	DWORD _3[13];					//0x1C
	void * pMemPool;				//0x50
};



struct SmallRoom1 // sizeof(0x18)
{
	DWORD dwXStart;					//0x00
	DWORD dwYStart;					//0x04
	DWORD dwXSize;					//0x08
	DWORD dwYSize;					//0x0C
	DWORD _1;						//0x10
	SmallRoom1* pNextRoom;			//0x14
};


struct Path {
	WORD xOffset;					//0x00
	WORD xPos;						//0x02
	WORD yOffset;					//0x04
	WORD yPos;						//0x06
	DWORD _1[2];					//0x08
	WORD xTarget;					//0x10
	WORD yTarget;					//0x12
	DWORD _2[2];					//0x14
	Room1 *pRoom1;					//0x1C
	Room1 *pRoomUnk;				//0x20
	DWORD _3[3];					//0x24
	UnitAny *pUnit;					//0x30
	DWORD dwFlags;					//0x34
	DWORD _4;						//0x38
	DWORD dwPathType;				//0x3C
	DWORD dwPrevPathType;			//0x40
	DWORD dwUnitSize;				//0x44
	DWORD _5[4];					//0x48
	UnitAny *pTargetUnit;			//0x58
	DWORD dwTargetType;				//0x5C
	DWORD dwTargetId;				//0x60
	BYTE bDirection;				//0x64
};

struct StaticPath { //size 0x20
	Room1* pRoom1;					//0x00
	DWORD	xOffset;				//0x04
	DWORD	yOffset;				//0x08
	DWORD	xPos;					//0x0C
	DWORD	yPos;					//0x10
	DWORD	_1[2];					//0x14
	DWORD	dwFlags;				//0x1C
};

struct Attack
{
   Game * pGame;				//0x00
   DWORD * pDifficultyLevelsTxt;//0x04
   UnitAny* pAttacker;			//0x08
   UnitAny* pDefender;			//0x0C
   DWORD dwAttackerType;		//0x10 (eType of the attacker, this is used so hirelings are treated like players)
   DWORD dwDefenderType;		//0x14 (eType of the defender, this is used so hirelings are treated like players)
   Damage *ptDamage;			//0x18
   DWORD _1;					//0x1C
   DWORD DamageReduced256;		//0x20
   DWORD MagDmgReduced256;		//0x24
};

struct PlayerData { //size 0x16C
	char szName[16];				//0x00
	QuestFlags *QuestsFlags[3];		//0x10
	Waypoint *pNormalWaypoint[3];	//0x1C
	DWORD _1;						//0x28
	DWORD _2[2];					//0x2C
	WORD* KillCounter;				//0x34 Monster
	DWORD _2b;						//0x38
	DWORD _3[4];					//0x3C 3[2] is merc related
	DWORD dwTradeTick;				//0x4C
	DWORD nTradeState;				//0x50
	DWORD _4;						//0x54
	DWORD dwAcceptTradeTick;		//0x58
	PlayerTrade* pTrade;			//0x5C
	DWORD _5[3];					//0x60
	DWORD dwBoughtItemId;			//0x6C
	DWORD dwRightSkill;				//0x70
	DWORD dwLeftSkill;				//0x74
	DWORD dwRightSkillFlags;		//0x78
	DWORD dwLeftSkillFlags;			//0x7C
	DWORD dwSwitchRightSkill;		//0x80
	DWORD dwSwitchLeftSkill;		//0x84
	DWORD dwSwitchRightSkillFlags;	//0x88
	DWORD dwSwitchLeftSkillFlags;	//0x8C
	DWORD _6[3];					//0x90
	ClientData* pClientData;		//0x9C
	DWORD _7[48];					//0x100
	DWORD dwHostileDelay;			//0x160
	DWORD _8;						//0x164
	DWORD GameFrame;				//0x168
//---My additions to original struct
	BOOL isPlaying;					//0x16C
	DWORD LastHitSkillId;			//0x170			
	BOOL CanAttack;					//0x174
	BOOL SaidGO;					//0x178
	DWORD FirstKillTick;			//0x17C
	DWORD KillCount;				//0x180
	DWORD LastDamage;				//0x184
	DWORD LastDamageId;				//0x188
	DWORD LastDamageTick;			//0x18C
	BYTE isSpecing;					//0x190
};

struct AiGeneral
{
  DWORD SpecialState;										  //0x00 - stuff like terror, confusion goes here
  void* ( __fastcall * fpAiFunction)(Game*,UnitAny*,DWORD*);  //0x04 - the primary ai function to call
  DWORD AiFlags;											  //0x08
  DWORD OwnerGUID;											  //0x0C- the global unique identifier of the boss or minion owner
  DWORD eOwnerType;											  //0x10 - the unit type of the boss or minion owner
  DWORD dwArgs[3];											  //0x14 - three dwords holding custom data used by ai func to store counters (etc)
  DWORD* pCmdCurrent;										  //0x20
  DWORD *pCmdLast;											  //0x24
  Game* pGame;												  //0x28
  DWORD OwnerGUIDEq;										  //0x2C - the same as +008
  DWORD eOwnerTypeEq;										  //0x30 - the same as +00C
  DWORD* pMinionList;										  //0x34 - list of all minions, for boss units (SetBoss in MonStats, Unique, SuperUnique etc)
  DWORD _1;													  //0x38
  DWORD eTrapNo;											  //0x3C - used by shadows for summoning traps (so they stick to one type usually)
};

struct AiParam
{
	AiGeneral* pAiGeneral;     //+00 
	DWORD _1;                  //+04 
	Unit* ptTarget;            //+08 
	DWORD _2;                  //+0C 
	DWORD _3;                  //+10 
	int nDistanceToTarget;     //+14 
	BOOL bEngagedInCombat;     //+18 
	void* pMonStatsRec;        //+1C 
	void* pMonStats2Rec;       //+20 
};

struct AiUnk1
{
	UnitAny* ptUnit;			//0x00
	DWORD _1;					//0x04 (some counter)
	AiUnk1 *ptNext;				//0x08
};

struct NPCAi
{
	AiUnk1* pAiUnk1;			//0x00
};

struct ItemData { //size = 0x74
	DWORD QualityNo;		//0x00
	DWORD LowSeed;			//0x04
	DWORD HighSeed;			//0x08
	DWORD OwnerID;			//0x0C
	DWORD InitSeed;			//0x10
	DWORD CommandFlags;		//0x14
	bItemFlags dwItemFlags;	//0x18
	DWORD _1[2];			//0x1C
	DWORD ActionStamp;		//0x24
	DWORD FileIndex;		//0x28
	DWORD ItemLevel;		//0x2C
	WORD ItemFormat;		//0x30
	WORD RarePrefix;		//0x32
	WORD RareSutfix;		//0x34
	WORD AutoPrefix;		//0x36
	WORD MagicPrefix[3];	//0x38
	WORD MagicSutfix[3];	//0x3E
	BYTE BodyLoc;		    //0x44					// location on body = xpos of item (only if on body) (spread)
	BYTE InvPage;			//0x45					// 0 = inventory,  3= cube, 4=stash (spread)
	BYTE _2[2];				//0x46	_2[1] - TradeInvPage
	BYTE EarLvl;			//0x48
	BYTE InvGfxIdx;			//0x49
	char szPlayerName[16];	//0x4A
	BYTE _3[2];				//0x5A
	Inventory *pNodeInv;	//0x5C
	UnitAny* pItem;			//0x60 points to self
	UnitAny* pNextItem;		//0x64
	BYTE NodePos;			//0x68
	BYTE NodePosOther;		//0x69
	DWORD _4;				//0x70
};


struct MonsterData { //size = 0x60
	DWORD* pMonStatsTxt;					//0x00
	BYTE Components[16];					//0x04 Order: HD, TR, LG, RA, LA, RH, LH, SH, S1, S2, S3, S4, S5, S6, S7, S8
	WORD NameSeed;							//0x14
	struct
	{
		BYTE fOther:1;
		BYTE fSuperUniq:1;
		BYTE fChampion:1;
		BYTE fUnique:1;
		BYTE fMinion:1;
		BYTE fPossesed:1;
		BYTE fGhostly:1;
		BYTE fMultishot:1;
	};				//0x16
	BYTE dwLastMode;				//0x17
	DWORD dwDuriel;					//0x18
	BYTE anEnchants[9];				//0x1C
	BYTE _1;						//0x25
	WORD wUniqueNo;					//0x26
	AiGeneral* pAiGeneral;			//0x28
	DWORD* pAiParams;				//0x2C
	NPCAi* pNPCAi;					//0x30
	DWORD _2[3];					//0x34
	DWORD dwNecroPet;				//0x40
	DWORD _3[3];					//0x44
	DWORD pVision;					//0x50
	DWORD AiState;					//0x54
	DWORD LvlNo;					//0x58
	DWORD SummonerFlags;			//0x5C
};


struct Stat // size 0x8
{
	WORD wSubIndex;					//0x00
	WORD wStatIndex;				//0x02
	DWORD dwStatValue;				//0x04
};

struct StatEx	//size 0x8
{
	Stat* pStat;					//0x00 An Array[wStatCount]
	WORD wStatCount;				//0x04
	WORD wnBitSize;					//0x06
};

struct StatListEx  //size 0x64
{
	DWORD pMemPool;					//0x00
	DWORD _1;						//0x04
	DWORD dwOwnerType;				//0x08
	DWORD dwOwnerId;				//0x0C
	DWORD dwListFlags;				//0x10
	DWORD _2[4];					//0x14
	StatEx BaseStats;				//0x24
	StatList* pLastList;			//0x2C - pointer to the last pStatList of the StatListEx owner (aka item owner in case list ex belongs to item)
	DWORD _3;						//0x30
	StatListEx* pListEx;			//0x34 - pStatListEx - pointer to owner StatListEx (if this one is owned by a item, this points to the item owners list)
	StatListEx* pNextListEx;		//0x38
	StatList *pMyLastList;			//0x3C Do tej listy dodawaj staty
	StatList *pMyStats;				//0x40
	UnitAny* pOwner;				//0x44
	StatEx FullStats;				//0x48
	StatEx ModStats;				//0x50
	BYTE* StatFlags;				//0x58 pt to an  array
	void *fpCallBack;				//0x5C
	Game* ptGame;					//0x60
};

struct StatList { //size 0x3C
	void* pMemPool;					//0x00
	UnitAny* pUnit;					//0x04
	DWORD dwOwnerType;				//0x08
	DWORD dwOwnerId;				//0x0C
	DWORD dwFlags;					//0x10
	DWORD dwStateNo;				//0x14
	DWORD ExpireFrame;				//0x18
	DWORD SkillNo;					//0x1C
	DWORD sLvl;						//0x20
	StatEx Stats;					//0x24
	StatList *pPrevLink;			//0x2C
	StatList *pNextLink;			//0x30
	StatList *pPrev;				//0x34
	void *fpStatRemove;				//0x38
	StatList *pNext;				//0x3C
};

struct Arena //size 0x10
{
DWORD dwAltStartTown;	//0x00
DWORD ArenaNo;			//0x04 (Row From Arena.Txt)
DWORD ArenaFlags;		//0x08 //0x2 - enables arena 0x100000 - enable party 0x1000- enable hostile
DWORD  _2;				//0x0C actually it's a byte
};

struct PartyMembers // size 0x08
{
DWORD dwUnitId;
PartyMembers* ptNext;
};

struct SubParty // size 0x0C
{
WORD PartyId;			//0x00
WORD _1;				//0x02 unused, align purpose
PartyMembers* ptMembers;//0x04
SubParty *ptPrev;		//0x08
};

struct Party // size = 8
{
WORD Counter;				//0x00 it starts from 3
WORD _1;					//0x02 i think its unused
SubParty* ptLastParty;		//0x04
};


struct UnitNode //size =0x10
{
UnitAny* ptUnit;			//0x00
UnitNode* pNode;			//0x04 im not sure (this is always zero)
UnitNode* pChildNode;		//0x08
UnitNode* pParentNode;		//0x0C
};

struct ItemRecord //size 0x0C
{
	BYTE NormalMin;	 //0x00
	BYTE NormalMax;	 //0x01
	BYTE MagicMin;	 //0x02
	BYTE MagicMax;	 //0x03
	DWORD ItemCode;	 //0x04
	DWORD MagicLvl;	 //0x08
};

struct ItemList
{
ItemRecord* pRecords;		//0x00
DWORD nRecords;				//0x04
DWORD* pPermRecords;		//0x08 ItemCodes
DWORD nPermRecords;			//0x0C
};

struct Npc	//size 0x44
{
DWORD npcNo;			//0x00 - hcIdx from MonStats.txt
Inventory* pInventory;	//0x04 - the inventory of the npc (what they can sell, regular pInventory class)
void* pGamble;			//0x08 +0x00 pInventory
BYTE bGambleBool;		//0x0C - a boolean related to gamble
BYTE _1[3];				//0x0D
DWORD _2;				//0x10 
void* _3;				//0x14
void* _4;				//0x18
BYTE _5[5];				//0x1C
BYTE bHirelingBool;		//0x21 - a boolean related to having hirelings
BYTE bDisableTrade;		//0x22
BYTE bNormalShopBool;	//0x23 - a boolean related to normal shops
BYTE _6[4];				//0x24
DWORD GenerateTick;		//0x28 - tick count from last time the items were generated
ItemList* pItemList;	//0x2C (called pSUnitProxy by Blizzard)
DWORD _7;				//0x30
DWORD* pPermStoreList;  //0x34 Array of Item Codes
DWORD nPermStoreRecords;//0x38
DWORD _8;				//0x38
DWORD dwUnitId;			//0x40
};

struct NpcControl // size 0x14
{
DWORD nNpcs;		// 0x00 - how many elements the dynamic array contains
Npc* pNpc;			// 0x04 [arrSize] - a pointer to a dynamic array of Npc structures
DWORD loSeed;		//0x08
DWORD hiSeed;		//0x0C
DWORD nNpcs2;		//0x10
};



struct NPCInfo //size 0xC - thx to SVR
{
        DWORD   NpcID;          //0x00
        WORD    MsgIndex;	    //0x04
		WORD    nPad;			//0x06
        DWORD   MenuIndex;      //0x08
};				               

struct NPCMsgs  //size 0xC4 - Quest message table entry.
{              
        NPCInfo aMsgs[16];      //0x00
        DWORD msgCount;         // number of valid NPCInfo items in array.
};

struct QuestBytes  //  thx to SVR
{ 
		BYTE Data[256];
};

struct QuestFlags //size 0x60;
{
	BYTE Flags[0x60];
};


struct QParam // 0x18 D2QuestArgStrc
{
		Game* pGame;	    	    //0x00
        DWORD   eCallback;	        //0x04 Callback ID
        UnitAny*   ptTarget;		//0x08 - NPC/Monster/Object... other Player ?
        UnitAny*   ptPlayer;        //0x0C
        DWORD dwUnk10;              //0x10
        void*  pTextHead;           //0x14 - Used in CB 0 to show msg
		DWORD _1;				    //0x18 - some vary data
};

struct Quest	//size 0xF4 - thx to SVR - D2QuestDataStrc 
{			
        DWORD QuestID;          //0x00
        Game   *pGame;			//0x04
        BYTE    bAct;           //0x08 - act the quest is for
        BYTE    bNotIntro;      //0x09 - checked in Callback 0 (+A0)
        BYTE    bActive;        //0x0A - checked in dispatch, if(!=1) skip callbacks
        BYTE    bLastState;     //0x0B - gets tweaked in Callback CC (which then calls0xA0)
        BYTE    bState;         //0x0C - Quest state counter
        BYTE    nInitNo;        //0x0D -
        WORD    _1;		        //0x0E -
        DWORD   nSeqId;         //0x10 - used in pSequence, calls Quest[nId]->pSequence
        DWORD   dwFlags;        //0x14
        void* pQuestEx;		    //0x18 - quest specific data
        DWORD nPlayerGUID[32];  //0x1C - playerID's - either players active in quest or completed ? (probably the later)
        WORD nPlayerCount;      //0x9C - playerID count
		WORD _2;				//0x9E
        DWORD (__fastcall *pCallback[15])(Quest *pQuest,QParam *pqParam);      //0xA0 - CallBack functions
        NPCMsgs* pNPCMsgs;      //0xDC - ptr to NPC Messages
        DWORD eFilter;          //0xE0 - index used in bit calls (#11107 etc)
        void* pStatusFilter;     //0xE4 - function ptr
        void* pActiveFilter;     //0xE8 - function ptr - return 1 to activate (!) bubble
        void* pSequence;         //0xEC - function ptr
        Quest* pPrev;	         //0xF0

};				            


struct QuestArray	//size 0x18
{
	 DWORD (__fastcall *pQuestInit)(Quest *pQuest);		//0x00
	 DWORD ActNo;										//0x04
	 DWORD _2;											//0x08  val 100 or 0  (mby version?)
	 DWORD _3;											//0x0C
	 DWORD QuestID;										//0x10
	 DWORD _5;											//0x14
};


struct QuestControl //size 0x24
{
	Quest *pQuest;			//0x00
	BOOL bExectuing;		//0x04
	BOOL bPickedSet;		//0x08
	QuestFlags *pQuestFlags; //0x0C  ->size 0x60 (BitBuffer)
	DWORD *pQuestTimer;		//0x10
	DWORD dwTick;			//0x14
	DWORD LowSeed;			//0x18
	DWORD HighSeed;			//0x1C
	DWORD _8;				//0x20
};

struct Game
{
  DWORD _1[3];				//0x00
  DWORD * ptGameData8;		//0x0C
  Game * pNext;				//0x10
  DWORD _1a;				//0x14
  CRITICAL_SECTION* ptLock; //0x18 (see MSDN please)
  void * pMemPool;			//0x1C - not used, always NULL
  void * GameData;			//0x20
  DWORD _2;					//0x24
  WORD  nServerToken;		//0x28 called 'Server Ticket' aswell
  char GameName[16];		//0x2A
  char GamePass[16];		//0x3A
  char GameDesc[32];		//0x4A
  BYTE GameType;			//0x6A - whenever this is single player (etc)
  BYTE _3a[2];				//0x6B _3a[0] - Arena's _2;
  BYTE DifficultyLevel;		//0x6D
  BYTE _4[2];				//0x6E
  DWORD bExpansion;			//0x70
  DWORD ldGameType;			//0x74
  WORD  ItemFormat;         //0x78
  WORD  _5;					//0x7A
  DWORD InitSeed;			//0x7C
  DWORD ObjSeed;            //0x80 - seed used for object spawning
  DWORD InitSeed2;          //0x84 (another instance, dunno why)
  ClientData* pClientList;  //0x88 - (pClient structure of last player that entered the game)
  DWORD nClients;			//0x8C
  DWORD nUnits[6];          //0x90 - array of 6 counters, one for each unit type, this is the next GUID used too
  DWORD GameFrame;          //0xA8 - the current frame of the game, used for timers (buff duration etc)
  DWORD _6[3];				//0xAC
  TimerQuene* pTimerQueue;       //0xB8 a queue of all current active and inactive timers
  Act* pDrlgAct[5];			//0xBC
  DWORD GameSeed[2];			//0xD0
  SmallRoom1* pDrlgRoomList[5];	//0xD8
  DWORD MonSeed;				//0xEC - seed used for monster spawning
  DWORD* pMonsterRegion[1024];  //0xF0 - one pointer for each of the 1024 possible levels
  DWORD* pObjectControl;		//0x10F0 - a controller holding all object region structs
  QuestControl* pQuestControl;	//0x10F4 - a controller holding all quest info
  UnitNode* pOldNodes[10];		//0x10F8 - ten lists of unit node lists, this is used by the AI target seeking code (and other stuff)
  UnitAny* pUnitList[5][128];	//0x1120 - 5 lists of 128 lists of units (see pUnit documentation), second index is GUID & 127, BEWARE: since ever, missiles are array #4 and items are array #3 (so type3=index4 and type4=index3)
  DWORD* pTileList;			    //0x1B20 - a list for all VisTile units
  DWORD UniqueFlags[128];		//0x1B24 - 128 DWORDS worth of flags that control if a unique item got spawned [room for 4096]
  NpcControl *pNpcControl;		//0x1D24 - a controller holding all npc info (like store inventories, merc list)
  Arena *pArenaControl;			//0x1D28 - a controller for arena stuff, functional and also used in game init
  Party *pPartyControl;			//0x1D2C - a controller for all party related stuff
  BYTE BossFlags[64];			//0x1D30 - 64 bytes for handling 512 super unique monsters (if they were spawned etc)
  DWORD MonModeData[17];		//0x1D70 - related to monsters changing mode
  DWORD nMonModeData;			//0x1DB4 - counter related to the above
  DWORD _7;						//0x1DB8
  DWORD nCreateTick;			//0x1DBC
  DWORD _8;						//0x1DC0
  DWORD nSyncTimer;				//0x1DC4 - used to sync events
  DWORD _9[8];					//0x1DC8
  BOOL bKilledBaal;				//0x1DE8 - killed uber baal
  BOOL bKilledDia;				//0x1DEC - killed uber diablo
  BOOL bKilledMeph;				//0x1DF0 - killed uber mephisto
  //Additions to original struct
  LRoster *ptLRoster;			//0x1DF4 added by me
  DWORD dwKillCount;			//0x1DF8 
  DWORD dwGameState;			//0x1DFC 
  UnitNode* pNewNodes[130];		//0x1E00
  BOOL bSpawnedClone;			//0x2008
  BOOL bFestivalMode;			//0x200C
  int nPlaying;					//0x2010
  int DmgRekord;				//0x2014
  DWORD LastKiller;				//0x2018
  DWORD LastVictim;				//0x201C
  char szRekordHolder[16];		//0x2020
};


struct CurseStateParam {
   UnitAny* pAttacker;			//0x00
   UnitAny* pDefender;	    	//0x04
   int skillNo;					//0x08
   int sLvl;					//0x0C
   int duration;				//0x10
   int statNo;					//0x14
   int statValue;				//0x18
   int stateNo;					//0x1C
   void *StateFunction;			//0x20
};

struct Damage
{
  DWORD HitFlags;			// 0x00
  DWORD ResultFlags;		// 0x04
  DWORD PhysicalDamage;		// 0x08
  DWORD EnhancedDamagePercent; //0x0C
  DWORD FireDamage;			// 0x10
  DWORD BurnDamage;			// 0x14
  DWORD BurnLength;			// 0x18
  DWORD LightningDamage;	// 0x1C
  DWORD MagicDamage;		// 0x20
  DWORD ColdDamage;			// 0x24
  DWORD PoisonDamage;		// 0x28
  DWORD PoisonLength;		// 0x2C
  DWORD ColdLength;			// 0x30
  DWORD FreezeLength;		// 0x34
  DWORD LifeSteal;			// 0x38
  DWORD ManaSteal;			// 0x3C
  DWORD StaminaSteal;		// 0x40
  DWORD StunLength;			// 0x44
  DWORD AbsorbedLife;		// 0x48
  DWORD DamageTotal;		// 0x4C
  DWORD _1;					// 0x50
  DWORD PiercePct;			// 0x54
  DWORD DamageRate;			// 0x58
  DWORD _2;					// 0x5C
  DWORD HitClass;			// 0x60
  BYTE HitClassActiveSet;	// 0x64 -bool
  BYTE eType;				// 0x65
  BYTE _3[2];				// 0x67
  DWORD eConvPercent;		// 0x68
  DWORD _4;					// 0x6C
  };

struct Combat  // size 0x88
{
  Game * ptGame;			// 0x00
  DWORD dwAttackerType;		// 0x04
  DWORD dwAttackerId;		// 0x08
  DWORD dwDefenderType;		// 0x0C
  DWORD dwDefenderId;		// 0x10
  Damage sDamage;			// 0x14
  Combat *ptNext;			// 0x84
  };

struct ClientData //size 0x518
{ 
   DWORD ClientID;                 //0x00 
   DWORD InitStatus;			   //0x04 Flag 0x4 - player is in game
   WORD ClassId;                   //0x08 Something with Arena, also could be equivalent of ClassId
   WORD PlayerStatus;			   //0x0A
   BYTE ClassId2;				   //0x0C
   char CharName[16];			   //0x0D 
   char AccountName[16];		   //0x1D 
   BYTE _3[50];                    //0x2D 
   DWORD _3b;					   //0x60
   DWORD _4;		               //0x64
   void* pGameData;				   //0x68 (seems to be added by d2gs, not by original blizz game) 
   DWORD _5[64];                   //0x6C 
   DWORD UnitType;				   //0x16C
   DWORD UnitId;                   //0x170
   UnitAny* pPlayerUnit;           //0x174
   DWORD _6;	                   //0x178 some bool
   void * ptSaveFile;			   //0x17C
   DWORD nOpenSize;				   //0x180
   DWORD _7[9];					   //0x184
   Game* pGame;					   //0x1A8 
   DWORD ActNo;					   //0x1AC
   DWORD _8;	                   //0x1B0
   Room1* ptRoom;				   //0x1B4
   PacketData * Packet[3];		   //0x1B8
   DWORD _10[132];				   //0x1C4
   DWORD dwFlags;				   //0x3D4
   DWORD LastPacketTime;		   //0x3D8 GetTickCount()
   struct
   {
   WORD SkillId;
   WORD SkillUnk;
   DWORD SkillFlags;
   } ClientHotKeys[16];  		   //0x3DC 
   DWORD bWeaponSwitch;			   //0x45C
   DWORD _11[11];				   //0x460
   DWORD InactivityTime;		   //0x48C (seconds)
   WORD CurrentLife;			   //0x490
   WORD CurrentMana;			   //0x492
   WORD CurrentStamina;			   //0x494
   BYTE LifePotionPercent;		   //0x496
   BYTE ManaPotionPercent;		   //0x497
   WORD xPos;					   //0x498
   WORD yPos;					   //0x49A
   WORD xTargetOffset;			   //0x49C send by packets 0x96, 0x95, 0x18 (last arg) its converted to BYTE
   WORD yTargetOffset;			   //0x49E like above
   DWORD BodyGold;				   //0x4A0
   DWORD CurrentExp;			   //0x4A4
   ClientData* ptPrevious;  	   //0x4A8 
   ClientData* ptNextByID;		   //0x4AC 
   ClientData* ptNextByName;       //0x4B0
   DWORD _12[19];				   //0x4B4
   DWORD SentPings;				   //0x500 Increasing every 10 secs
   DWORD bNeedToKnow;			   //0x504 u can set this true by packet 0x70
   DWORD ExpLoss;				   //0x508 its temp value, dont use
   DWORD LocaleID;				   //0x50C
   DWORD _13[2];                   //0x510 2 last elements are unused
}; 

struct UnitEvent //size 0x20
{
	DWORD EventType;	//0x00
	DWORD _1;			//0x04
	DWORD AuraState;	//0x08 also UnitId;
	DWORD SkillId;		//0x0C
	DWORD SkillLvl;		//0x10
	DWORD pfnCallback;	//0x14
	UnitEvent* pPrev;	//0x18
	UnitEvent* pNext;	//0x1C
};

struct UnitMsg  //size 0x14
{
	UnitMsg* pNext;		//0x00
	DWORD PacketHdr;	//0x04
	DWORD UnitType;		//0x08
	DWORD UnitId;		//0x0C
	DWORD SkillId;		//0x10
};

struct TimerQuene	//size 0xA20
{
	DWORD QueneNo;								//0x00
	BYTE pActiveTimersByType[5][256];			//0x04
	BYTE pDispatchedTimersByType[5][256];		//0x504
	TimerList* pTimerListByType[5];				//0xA04
	Timer* pTimer;								//0xA18
	TimerList* pTimerList;						//0xA1C
};

struct Timer		//size 0x30
{
 BYTE nTimerType;			//  +000 - TimerType - the type of the timer, this varies between unit types (byte)
 BYTE _1;					//  0x01
 WORD nTimerIdx;			//  +002 - TimerIdx - used as index in function table, unique for each unit type (word) (IIRC, has been a while)
 DWORD nQueneNo;			//  +004 - queueNo - which queue this timer belongs to (byte)
 UnitAny* pUnit;			//  +008 - pUnit - the unit the timer is assigned to
 DWORD UnitId;				//  +00C - GUID - the global unique identifier of the unit the list is assigned to
 DWORD UnitType;			//  +010 - eType - the unit type of the unit this timer is assigned to
 DWORD _3;					//  +014
 Timer* pPrevTimer;			//  +018 - pPreviousTimer - the previous timer overall
 Timer* pNextTimer;			//  +01C - pNextTimer - the next timer overall
 Timer* pPrevTimerOnUnit;	//  +020 - pPreviousTimerOnUnit - the previous timer assigned to this unit
 Timer* pNextTimerOnUnit;	//  +024 - pNextTimerOnUnit - the next timer assigned to this unit
 TimerList* pTimerList;		//  +028 - pTimerList - the timer list this timer belongs to (IIRC, or the first timer assigned to the unit, need to verify)
 DWORD fpTimerFunction;		//  +02C - fpTimerFunction(); - function called when the timer ends (void * __fastcall)(pGame,pUnit);
};

struct TimerList	 //size 0x7088
{
Timer hTimerList[600];	//0x00 - pTimerList[600] - allocated in realtime
TimerList* pFirstTimerList;  //+7080 - pFirstTimerList - points to self
TimerList* pNextTimerList;	 //+7084 - pNextTimerList - unknown whenever it actually points to another TimerList, always NULL
};



struct UnitAny 
{
	DWORD dwType;					//0x00
	DWORD dwClassId;				//0x04
	void* pMemPool;					//0x08
	DWORD dwUnitId;					//0x0C
	DWORD dwMode;					//0x10
	union
	{
		PlayerData*  pPlayerData;
		ItemData*    pItemData;
		MonsterData* pMonsterData;
		ObjectData*  pObjectData;
	};								//0x14
	DWORD dwAct;					//0x18
	Act * pAct;						//0x1C
	DWORD dwSeed[2];				//0x20
	DWORD dwInitSeed;				//0x28
	union
	{
		Path*       pPath;
		StaticPath* pStaticPath;
	};								//0x2C
	DWORD _3[5];					//0x30
	DWORD dwGfxFrame;				//0x44
	DWORD dwFrameRemain;			//0x48
	WORD wFrameRate;				//0x4C
	WORD _4;						//0x4E
	BYTE*  pGfxUnk;					//0x50
	DWORD* pGfxInfo;				//0x54
	DWORD _5;						//0x58
	StatListEx*  pStatsEx;			//0x5C
	Inventory* pInventory;			//0x60
	DWORD  dwInteractId;			//0x64
	DWORD  dwInteractType;			//0x68
	BYTE   bInteracting;			//0x6C
	BYTE   _6;						//0x6D
	WORD UpdateType;				//0x6E
	UnitAny *pUpdateUnit;			//0x70
	DWORD *pQuestRecord;			//0x74
	BYTE bSparkyChest;				//0x78
	BYTE _6a[3];					//0x79
	DWORD pTimerArgs;				//0x7C
	Game* pGame;					//0x80
	DWORD _7[2];					//0x84
	WORD wX;						//0x8C
	WORD wY;						//0x8E
	UnitEvent* pEvent;				//0x90
	DWORD dwOwnerType;				//0x94
	DWORD dwOwnerId;				//0x98
	DWORD _8[3];					//0x9C
	SkillData* pSkills;				//0xA8
	Combat* ptCombat;				//0xAC
	DWORD dwLastHitClass;			//0xB0
	DWORD _9;						//0xB4
	DWORD ItemCodeToDrop;			//0xB8
	DWORD _10[2];					//0xBC
	DWORD dwFlags;					//0xC4
	DWORD dwFlags2;					//0xC8
	DWORD _11;						//0xCC
	DWORD dwNodeIdx;				//0xD0
	DWORD dwOverlayTick;			//0xD4
	DWORD dwDoorTick;				//0xD8
	Timer* pTimer;					//0xDC
	UnitAny* pChangedNext;			//0xE0
	UnitAny*  pRoomNext;			//0xE4
	UnitAny*  pListNext;			//0xE8
	UnitMsg* pMsgFirst;				//0xEC
	UnitMsg* pMsgLast;				//0xF0
};

struct Skill		//size 0x40
{
	SkillsTxt* pTxt;				//0x00
	Skill* pNextSkill;				//0x04
	DWORD SkillMode;				//0x08
	DWORD _1[7];					//0x0C
	DWORD dwSkillLevel;				//0x28
	DWORD _2[2];					//0x2C
	DWORD dwFlags;					//0x34
	DWORD _3;						//0x38
	DWORD bUnknown;					//0x3C
};

struct SkillData	//size 0x18
{
	DWORD* pMemPool;			//0x00
	Skill* pFirstSkill;			//0x04
	Skill* pLeftSkill;			//0x08
	Skill* pRightSkill;			//0x0C
	Skill* pCurrentSkill;		//0x10
	DWORD _1;					//0x14
};


struct BitBuffer
{
   unsigned char *pBuffer;			//0x00
   unsigned int bitsInBuffer;		//0x04
   unsigned int bufferPos;			//0x08
   unsigned int bitsAtPos;			//0x0C num bits already read or written at cur pos
   unsigned int unk;				//0x10 could be a bit bucket
};


struct CreateItem //size 0x84
{
	UnitAny* pOwner;		//0x00
	DWORD dwSeed;			//0x04
	Game* pGame;			//0x08
	DWORD iLvl;				//0x0C
	DWORD _2;				//0x10
	DWORD iIdx;				//0x14
	DWORD iMode;			//0x18
	DWORD xPos;				//0x1C
	DWORD yPos;				//0x20
	Room1* pRoom;			//0x24
	WORD wCreateFlags;		//0x28 dont mess with ItemFlags 1 - pickable, 4 - sox
	WORD wItemFormat;		//0x2A
	DWORD bPersonalize;		//0x2C also allowing socketing bForce -> uzywa flag thx to Necrolis
	DWORD iQuality;			//0x30
	DWORD dwValue;			//0x34 GOLD // Quantity
	DWORD dwDurability;		//0x38
	DWORD dwMaxDurability;	//0x3C max 255
	DWORD dwFileIdx;		//0x40
	bItemFlags dwItemFlags;	//0x44
	DWORD dwInitSeed;		//0x48 a10 
	DWORD dwInitSeed2;		//0x4C a11
	DWORD OwnerLvl;			//0x50 ear lvl
	DWORD dwValueMax;		//0x54 Quantity
    char  szName[16];		//0x58
    DWORD nPrefix[3];		//0x68
    DWORD nSuffix[3];		//0x74
	DWORD _26;				//0x80 Some flags can be 2 | 8, a2 | a3 Flag 1 -> More magic items - thx Nefarius
};



struct ArenaTxt //size = 0x1C
{
	DWORD dwSuicide;			//0x00
	DWORD dwPlayerKill;			//0x04
	DWORD dwPKPercent;			//0x08
	DWORD dwMonsterKill;		//0x0C
	DWORD dwPlayerDeath;		//0x10
	DWORD dwPlayerDeathPercent;	//0x14
	DWORD dwMonsterDeath;		//0x18
};

struct ItemsTxt //size = 0x1A8, Valid for Weapons, Armors, Misc.txts
{
char	szflippyfile[32];		//0x00
char	szinvfile[32];			//0x20
char	szuniqueinvfile[32];	//0x40
char	szsetinvfile[32];		//0x60
DWORD	dwcode;					//0x80
DWORD	dwnormcode;				//0x84
DWORD	dwubercode;				//0x88
DWORD	dwultracode;			//0x8C
DWORD	dwalternategfx;			//0x90
DWORD	dwpSpell;				//0x94
WORD	wstate;					//0x98
WORD	wcstate1;				//0x9A
WORD	wcstate2;				//0x9C
WORD	wstat1;					//0x9E
WORD	wstat2;					//0xA0
WORD	wstat3;					//0xA2
DWORD	dwcalc1;				//0xA4
DWORD	dwcalc2;				//0xA8
DWORD	dwcalc3;				//0xAC
DWORD	dwlen;					//0xB0
WORD	bspelldesc;				//0xB4
WORD	wspelldescstr;			//0xB6
DWORD	dwspelldesccalc;		//0xB8
DWORD	dwBetterGem;			//0xBC
DWORD	dwwclass;				//0xC0
DWORD	dw2handedwclass;		//0xC4
DWORD	dwTMogType;				//0xC8
DWORD	dwminac;				//0xCC
DWORD	dwmaxac;				//0xD0
DWORD	dwgamblecost;			//0xD4
DWORD	dwspeed;				//0xD8
DWORD	dwbitfield1;			//0xDC
DWORD	dwcost;					//0xE0
DWORD	dwminstack;				//0xE4
DWORD	dwmaxstack;				//0xE8
DWORD	dwspawnstack;			//0xEC
DWORD	dwgemoffset;			//0xF0
WORD	wnamestr;				//0xF4
WORD	wversion;				//0xF6
WORD	wautoprefix;			//0xF8
WORD	wmissiletype;			//0xFA
BYTE	brarity;				//0xFC
BYTE	blevel;					//0xFD
BYTE	bmindam;				//0xFE
BYTE	bmaxdam;				//0xFF
BYTE	bminmisdam;				//0x100
BYTE	bmaxmisdam;				//0x101
BYTE	b2handmindam;			//0x102
BYTE	b2handmaxdam;			//0x103
WORD	brangeadder;			//0x104
WORD	wstrbonus;				//0x106
WORD	wdexbonus;				//0x108
WORD	wreqstr;				//0x10A
WORD	wreqdex;				//0x10C
BYTE	babsorbs;				//0x10E
BYTE	binvwidth;				//0x10F
BYTE	binvheight;				//0x110
BYTE	bblock;					//0x111
BYTE	bdurability;			//0x112
BYTE	bnodurability;			//0x113
BYTE	bmissile;				//0x114
BYTE	bcomponent;				//0x115
BYTE	brArm;					//0x116
BYTE	blArm;					//0x117
BYTE	btorso;					//0x118
BYTE	blegs;					//0x119
BYTE	brspad;					//0x11A
BYTE	blspad;					//0x11B
BYTE	b2handed;				//0x11C
BYTE	buseable;				//0x11D
WORD	wtype;					//0x11E
WORD	wtype2;					//0x120
WORD	bsubtype;				//0x122
WORD	wdropsound;				//0x124
WORD	wusesound;				//0x126
BYTE	bdropsfxframe;			//0x128
BYTE	bunique;				//0x129
BYTE	bquest;					//0x12A
BYTE	bquestdiffcheck;		//0x12B
BYTE	btransparent;			//0x12C
BYTE	btranstbl;				//0x12D
BYTE 	_1;						//0x12E
BYTE	blightradius;			//0x12F
BYTE	bbelt;					//0x130
BYTE	bautobelt;				//0x131
BYTE	bstackable;				//0x132
BYTE	bspawnable;				//0x133
BYTE	bspellicon;				//0x134
BYTE	bdurwarning;			//0x135
BYTE	bqntwarning;			//0x136
BYTE	bhasinv;				//0x137
BYTE	bgemsockets;			//0x138
BYTE	bTransmogrify;			//0x139
BYTE	bTMogMin;				//0x13A
BYTE	bTMogMax;				//0x13B
BYTE	bhitclass;				//0x13C
BYTE	b1or2handed;			//0x13D
BYTE	bgemapplytype;			//0x13E
BYTE	blevelreq;				//0x13F
BYTE	bmagiclvl;				//0x140
BYTE	bTransform;				//0x141
BYTE	bInvTrans;				//0x142
BYTE	bcompactsave;			//0x143
BYTE	bSkipName;				//0x144
BYTE	bNameable;				//0x145
BYTE	bAkaraMin;				//0x146
BYTE	bGheedMin;				//0x147
BYTE	bCharsiMin;				//0x148
BYTE	bFaraMin;				//0x149
BYTE	bLysanderMin;			//0x14A
BYTE	bDrognanMin;			//0x14B
BYTE	bHraltiMin;				//0x14C
BYTE	bAlkorMin;				//0x14D
BYTE	bOrmusMin;				//0x14E
BYTE	bElzixMin;				//0x14F
BYTE	bAshearaMin;			//0x150
BYTE	bCainMin;				//0x151
BYTE	bHalbuMin;				//0x152
BYTE	bJamellaMin;			//0x153
BYTE	bMalahMin;				//0x154
BYTE	bLarzukMin;				//0x155
BYTE	bDrehyaMin;				//0x156
BYTE	bAkaraMax;				//0x157
BYTE	bGheedMax;				//0x158
BYTE	bCharsiMax;				//0x159
BYTE	bFaraMax;				//0x15A
BYTE	bLysanderMax;			//0x15B
BYTE	bDrognanMax;			//0x15C
BYTE	bHraltiMax;				//0x15D
BYTE	bAlkorMax;				//0x15E
BYTE	bOrmusMax;				//0x15F
BYTE	bElzixMax;				//0x160
BYTE	bAshearaMax;			//0x161
BYTE	bCainMax;				//0x162
BYTE	bHalbuMax;				//0x163
BYTE	bJamellaMax;			//0x164
BYTE	bMalahMax;				//0x165
BYTE	bLarzukMax;				//0x166
BYTE	bDrehyaMax;				//0x167
BYTE	bAkaraMagicMin;			//0x168
BYTE	bGheedMagicMin;			//0x169
BYTE	bCharsiMagicMin;		//0x16A
BYTE	bFaraMagicMin;			//0x16B
BYTE	bLysanderMagicMin;		//0x16C
BYTE	bDrognanMagicMin;		//0x16D
BYTE	bHraltiMagicMin;		//0x16E
BYTE	bAlkorMagicMin;			//0x16F
BYTE	bOrmusMagicMin;			//0x170
BYTE	bElzixMagicMin;			//0x171
BYTE	bAshearaMagicMin;		//0x172
BYTE	bCainMagicMin;			//0x173
BYTE	bHalbuMagicMin;			//0x174
BYTE	bJamellaMagicMin;		//0x175
BYTE	bMalahMagicMin;			//0x176
BYTE	bLarzukMagicMin;		//0x177
BYTE	bDrehyaMagicMin;		//0x178
BYTE	bAkaraMagicMax;			//0x179
BYTE	bGheedMagicMax;			//0x17A
BYTE	bCharsiMagicMax;		//0x17B
BYTE	bFaraMagicMax;			//0x17C
BYTE	bLysanderMagicMax;		//0x17D
BYTE	bDrognanMagicMax;		//0x17E
BYTE	bHraltiMagicMax;		//0x17F
BYTE	bAlkorMagicMax;			//0x180
BYTE	bOrmusMagicMax;			//0x181
BYTE	bElzixMagicMax;			//0x182
BYTE	bAshearaMagicMax;		//0x183
BYTE	bCainMagicMax;			//0x184
BYTE	bHalbuMagicMax;			//0x185
BYTE	bJamellaMagicMax;		//0x186
BYTE	bMalahMagicMax;			//0x187
BYTE	bLarzukMagicMax;		//0x188
BYTE	bDrehyaMagicMax;		//0x189
BYTE	bAkaraMagicLvl;			//0x18A
BYTE	bGheedMagicLvl;			//0x18B
BYTE	bCharsiMagicLvl;		//0x18C
BYTE	bFaraMagicLvl;			//0x18D
BYTE	bLysanderMagicLvl;		//0x18E
BYTE	bDrognanMagicLvl;		//0x18F
BYTE	bHraltiMagicLvl;		//0x190
BYTE	bAlkorMagicLvl;			//0x191
BYTE	bOrmusMagicLvl;			//0x192
BYTE	bElzixMagicLvl;			//0x193
BYTE	bAshearaMagicLvl;		//0x194
BYTE	bCainMagicLvl;			//0x195
BYTE	bHalbuMagicLvl;			//0x196
BYTE	bJamellaMagicLvl;		//0x197
BYTE	bMalahMagicLvl;			//0x198
BYTE	bLarzukMagicLvl;		//0x199
WORD	bDrehyaMagicLvl;		//0x19A
DWORD	dwNightmareUpgrade;		//0x19C
DWORD	dwHellUpgrade;			//0x1A0
BYTE	bPermStoreItem;			//0x1A4
BYTE	bmultibuy;				//0x1A5
WORD   _ALIGN;					//0x1A6
};


struct SkillsTxt //size 0x23C
{
WORD wSkillId; 			//0x00
WORD _1a;				//0x02
struct 
{
BYTE bDecquant:1;				//0
BYTE bLob:1;					//1
BYTE bProgressive:1;			//2
BYTE bFinishing:1;				//3
BYTE bPassive:1;				//4
BYTE bAura:1;					//5
BYTE bPeriodic:1;				//6
BYTE bPrgStack:1;				//7
BYTE bInTown:1;					//8
BYTE bKick:1;					//9
BYTE bInGame:1;					//10
BYTE bRepeat:1;					//11
BYTE bStSuccessOnly:1;			//12
BYTE bStSoundDelay:1;			//13
BYTE bWeaponSnd:1;				//14
BYTE bImmediate:1;				//15
BYTE bNoAmmo:1;					//16
BYTE bEnhanceable:1;			//17
BYTE bDurability:1;				//18
BYTE bUseAttackRating:1;		//19
BYTE bTargetableOnly:1;			//20
BYTE bSearchEnemyXY:1;			//21
BYTE bSearchEnemyNear:1;		//22
BYTE bSearchOpenXY:1;			//23
BYTE bTargetCorpse:1;			//24
BYTE bTargetPet:1;				//25
BYTE bTargetAlly:1;				//26
BYTE bTargetItem:1;				//27
BYTE bAttackNoMana:1;			//28
BYTE bItemTgtDo:1;				//29
BYTE bLeftSkill:1;				//30
BYTE bInterrupt:1;				//31
BYTE bTgtPlaceCheck:1;			//32
BYTE bItemCheckStart:1;			//33
BYTE bItemCltCheckStart:1;		//34
BYTE bGeneral:1;				//35
BYTE bScroll:1;					//36
BYTE bUseManaOnDo:1;			//37
BYTE bWarp:1;					//38
} dwFlags;				//0x04 (5 bytes)
DWORD bClassId;			//0x0C
BYTE bAnim;				//0x10
BYTE bMonAnim;			//0x11
BYTE bSeqTrans;			//0x12
BYTE bSeqNum;			//0x13
BYTE bRange;			//0x14
BYTE bSelectProc;		//0x15
BYTE bSeqInput;			//0x16
BYTE _1;				//0x17
WORD wITypeA1;			//0x18
WORD wITypeA2;			//0x1A
WORD wITypeA3;			//0x1C
WORD wITypeB1;			//0x1E
WORD wITypeB2;			//0x20
WORD wITypeB3;			//0x22
WORD wETypeA1;			//0x24
WORD wETypeA2;			//0x26
WORD wETypeB1;			//0x28
WORD wETypeB2;			//0x2A
WORD wSrvStartFunc;		//0x2C
WORD wSrvDoFunc;		//0x2E
WORD wSrvPrgFunc1;		//0x30
WORD wSrvPrgFunc2;		//0x32
WORD wSrvPrgFunc3;		//0x34
WORD _2;				//0x36
DWORD dwPrgCalc1;		//0x38
DWORD dwPrgCalc2;		//0x3C
DWORD dwPrgCalc3;		//0x40
WORD bPrgDamage;		//0x44
WORD wSrvMissile;		//0x46
WORD wSrvMissileA;		//0x48
WORD wSrvMissileB;		//0x4A
WORD wSrvMissileC;		//0x4C
WORD wSrvOverlay;		//0x4E
DWORD dwAuraFilter;		//0x50
WORD wAuraStat1;		//0x54
WORD wAuraStat2;		//0x56
WORD wAuraStat3;		//0x58
WORD wAuraStat4;		//0x5A
WORD wAuraStat5;		//0x5C
WORD wAuraStat6;		//0x5E
DWORD dwAuraLenCalc;	//0x60
DWORD dwAuraRangeCalc;	//0x64
DWORD dwAuraStatCalc1;	//0x68
DWORD dwAuraStatCalc2;	//0x6C
DWORD dwAuraStatCalc3;	//0x70
DWORD dwAuraStatCalc4;	//0x74
DWORD dwAuraStatCalc5;	//0x78
DWORD dwAuraStatCalc6;	//0x7C
WORD wAuraState;		//0x80
WORD wAuraTargetState;	//0x82
WORD wAuraEvent1;		//0x84
WORD wAuraEvent2;		//0x86
WORD wAuraEvent3;		//0x88
WORD wAuraEventFunc1;	//0x8A
WORD wAuraEventFunc2;	//0x8C
WORD wAuraEventFunc3;	//0x8E
WORD wAuraTgtEvent;		//0x90
WORD wAuraTgtEventFunc;	//0x92
WORD wPassiveState;		//0x94
WORD wPassiveiType;		//0x96
WORD wPassiveStat1;		//0x98
WORD wPassiveStat2;		//0x9A
WORD wPassiveStat3;		//0x9C
WORD wPassiveStat4;		//0x9E
WORD wPassiveStat5;		//0xA0
WORD _3;				//0xA2
DWORD dwPassiveCalc1;	//0xA4
DWORD dwPassiveCalc2;	//0xA8
DWORD dwPassiveCalc3;	//0xAC
DWORD dwPassiveCalc4;	//0xB0
DWORD dwPassiveCalc5;	//0xB4
WORD wPassiveEvent;		//0xB8
WORD wPassiveEventFunc; //0xBA
WORD wSummon;			//0xBC
BYTE wPetType;			//0xBE
BYTE bSumMode;			//0xBF
DWORD dwPetMax;			//0xC0
WORD wSumSkill1;		//0xC4
WORD wSumSkill2;		//0xC6
WORD wSumSkill3;		//0xC8
WORD wSumSkill4;		//0xCA
WORD wSumSkill5;		//0xCC
WORD _4;				//0xCE
DWORD dwSumSkCalc1;		//0xD0
DWORD dwSumSkCalc2;		//0xD4
DWORD dwSumSkCalc3;		//0xD8
DWORD dwSumSkCalc4;		//0xDC
DWORD dwSumSkCalc5;		//0xE0
WORD wSumUMod;			//0xE4
WORD wSumOverlay;		//0xE6
WORD wCltMissile;		//0xE8
WORD wCltMissileA;		//0xEA
WORD wCltMissileB;		//0xEC
WORD wCltMissileC;		//0xEE
WORD wCltMissileD;		//0xF0
WORD wCltStFunc;		//0xF2
WORD wCltDoFunc;		//0xF4
WORD wCltPrgFunc1;		//0xF6
WORD wCltPrgFunc2;		//0xF8
WORD wCltPrgFunc3;		//0xFA
WORD wStSound;			//0xFC
WORD _5;				//0xFE
WORD wDoSound;			//0x100
WORD wDoSoundA;			//0x102
WORD wDoSoundB;			//0x104
WORD wCastOverlay;		//0x106
WORD wTgtOverlay;		//0x108
WORD wTgtSound;			//0x10A
WORD wPrgOverlay;		//0x10C
WORD wPrgSound;			//0x10E
WORD wCltOverlayA;		//0x110
WORD wCltOverlayB;		//0x112
DWORD dwCltCalc1;		//0x114
DWORD dwCltCalc2;		//0x118
DWORD dwCltCalc3;		//0x11C
WORD bItemTarget;		//0x120
WORD wItemCastSound;	//0x122
DWORD wItemCastOverlay;	//0x124
DWORD dwPerDelay;		//0x128
WORD wMaxLvl;			//0x12C
WORD wResultFlags;		//0x12E
DWORD dwHitFlags;		//0x130
DWORD dwHitClass;		//0x134
DWORD dwCalc1;			//0x138
DWORD dwCalc2;			//0x13C
DWORD dwCalc3;			//0x140
DWORD dwCalc4;			//0x144
DWORD dwParam1;			//0x148
DWORD dwParam2;			//0x14C
DWORD dwParam3;			//0x150
DWORD dwParam4;			//0x154
DWORD dwParam5;			//0x158
DWORD dwParam6;			//0x15C
DWORD dwParam7;			//0x160
DWORD dwParam8;			//0x164
WORD bWeapSel;			//0x168
WORD wItemEffect;		//0x16A
DWORD wItemCltEffect;	//0x16C
DWORD dwSkPoints;		//0x170
WORD wReqLevel;			//0x174
WORD wReqStr;			//0x176
WORD wReqDex;			//0x178
WORD wReqInt;			//0x17A
WORD wReqVit;			//0x17C
WORD wReqSkill1;		//0x17E
WORD wReqSkill2;		//0x180
WORD wReqSkill3;		//0x182
WORD wStartMana;		//0x184
WORD wMinMana;			//0x186
WORD wManaShift;		//0x188
WORD wMana;				//0x18A
WORD wLevelMana;		//0x18C
BYTE bAttackRank;		//0x18E
BYTE bLineOfSight;		//0x18F
DWORD dwDelay;			//0x190
DWORD wSkillDesc;		//0x194
DWORD dwToHit;			//0x198
DWORD dwLevToHit;		//0x19C
DWORD dwToHitCalc;		//0x1A0
BYTE bToHitShift;		//0x1A4
BYTE bSrcDam;			//0x1A5
WORD _6;				//0x1A6
DWORD dwMinDam;			//0x1A8
DWORD dwMaxDam;			//0x1AC
DWORD dwMinLvlDam1;		//0x1B0
DWORD dwMinLvlDam2;		//0x1B4
DWORD dwMinLvlDam3;		//0x1B8
DWORD dwMinLvlDam4;		//0x1BC
DWORD dwMinLvlDam5;		//0x1C0
DWORD dwMaxLvlDam1;		//0x1C4
DWORD dwMaxLvlDam2;		//0x1C8
DWORD dwMaxLvlDam3;		//0x1CC
DWORD dwMaxLvlDam4;		//0x1D0
DWORD dwMaxLvlDam5;		//0x1D4
DWORD dwDmgSymPerCalc;	//0x1D8
WORD bEType;			//0x1DC
WORD _7;				//0x1DE
DWORD dwEMin;			//0x1E0
DWORD dwEMax;			//0x1E4
DWORD dwEMinLev1;		//0x1E8
DWORD dwEMinLev2;		//0x1EC
DWORD dwEMinLev3;		//0x1F0
DWORD dwEMinLev4;		//0x1F4
DWORD dwEMinLev5;		//0x1F8
DWORD dwEMaxLev1;		//0x1FC
DWORD dwEMaxLev2;		//0x200
DWORD dwEMaxLev3;		//0x204
DWORD dwEMaxLev4;		//0x208
DWORD dwEMaxLev5;		//0x20C
DWORD dwEDmgSymPerCalc;	//0x210
DWORD dwELen;			//0x214
DWORD dwELevLen1;		//0x218
DWORD dwELevLen2;		//0x21C
DWORD dwELevLen3;		//0x220
DWORD dwELenSymPerCalc;	//0x224
WORD bRestrict;			//0x228
WORD wState1;			//0x22A
WORD wState2;			//0x22C
WORD wState3;			//0x22E
WORD bAiType;			//0x230
WORD wAiBonus;			//0x232
DWORD dwCostMult;		//0x234
DWORD dwCostAdd;		//0x238
}; 

struct CharStatsTxt
{
wchar_t	szwClass[16];	//0x0
char	szClass[16];	//0x20
BYTE	bStr;	//0x30
BYTE	bDex;	//0x31
BYTE	bEne;	//0x32
BYTE	bVita;	//0x33
BYTE	bStamina;	//0x34
BYTE	bHpAdd;	//0x35
BYTE	bPercentStr;	//0x36
BYTE	bPercentInt;	//0x37
BYTE	bPercentDex;	//0x38
BYTE	bPercentVit;	//0x39
BYTE	bManaRegen;	//0x3A
BYTE 	_1;			//0x3B
DWORD	dwToHitFactor;	//0x3C
BYTE	bWalkVelocity;	//0x40
BYTE	bRunVelocity;	//0x41
BYTE	bRunDrain;	//0x42
BYTE	bLifePerLevel;	//0x43
BYTE	bStaminaPerLevel;	//0x44
BYTE	bManaPerLevel;	//0x45
BYTE	bLifePerVitality;	//0x46
BYTE	bStaminaPerVitality;	//0x47
BYTE	bManaPerMagic;	//0x48
BYTE	bBlockFactor;	//0x49
BYTE	_2[2];			//0x4A
DWORD	dwBaseWClass;	//0x4C
BYTE	bStatPerLevel;	//0x50
BYTE	_3a;			//0x51
WORD	wStrAllSkills;	//0x52
WORD	wStrSkillTab1;	//0x54
WORD	wStrSkillTab2;	//0x56
WORD	wStrSkillTab3;	//0x58
WORD	wStrClassOnly;	//0x5A
DWORD	dwItem1;	//0x5C
BYTE	bItem1loc;	//0x60
BYTE	bItem1count;	//0x61
BYTE	_3[2];			//0x62
DWORD	dwItem2;	//0x64
BYTE	bItem2loc;	//0x68
BYTE	bItem2count;	//0x69
BYTE	_4[2];			//0x6A
DWORD	dwItem3;	//0x6C
BYTE	bItem3loc;	//0x70
BYTE	bItem3count;	//0x71
BYTE	_5[2];			//0x72
DWORD	dwItem4;	//0x74
BYTE	bItem4loc;	//0x78
BYTE	bItem4count;	//0x79
BYTE	_6[2];			//0x7A
DWORD	dwItem5;	//0x7C
BYTE	bItem5loc;	//0x80
BYTE	bItem5count;	//0x81
BYTE	_7[2];			//0x82
DWORD	dwItem6;	//0x84
BYTE	bItem6loc;	//0x88
BYTE	bItem6count;	//0x89
BYTE	_8[2];			//0x8A
DWORD	dwItem7;	//0x8C
BYTE	bItem7loc;	//0x90
BYTE	bItem7count;	//0x91
BYTE	_9[2];			//0x92
DWORD	dwItem8;	//0x94
BYTE	bItem8loc;	//0x98
BYTE	bItem8count;	//0x99
BYTE	_10[2];			//0x9A
DWORD	dwItem9;	//0x9C
BYTE	bItem9loc;	//0xA0
BYTE	bItem9count;	//0xA1
BYTE	_11[2];			//0xA2
DWORD	dwItem10;	//0xA4
BYTE	bItem10loc;	//0xA8
BYTE	bItem10count;	//0xA9
BYTE	_12[2];			//0xAA
WORD	wStartSkill;	//0xAC
WORD	wSkill1;	//0xAE
WORD	wSkill2;	//0xB0
WORD	wSkill3;	//0xB2
WORD	wSkill4;	//0xB4
WORD	wSkill5;	//0xB6
WORD	wSkill6;	//0xB8
WORD	wSkill7;	//0xBA
WORD	wSkill8;	//0xBC
WORD	wSkill9;	//0xBE
WORD	wSkill10;	//0xC0
WORD	_13;		//0xC2
};


struct sgptDataTable {
	BYTE*	pPlayerClass;			//0x00
	DWORD	dwPlayerClassRecords;	//0x04
	BYTE*	pBodyLocs;				//0x08
	DWORD	dwBodyLocsRecords;		//0x0C
	BYTE*	pStorePage;				//0x10
	DWORD	dwStorePageRecords;		//0x14
	BYTE*	pElemTypes;				//0x18
	DWORD	dwElemTypesRecords;		//0x1C
	BYTE*	pHitClass;				//0x20
	DWORD	dwHitClassRecords;		//0x24
	BYTE*	pMonMode;				//0x28
	DWORD	dwMonModeTxt;			//0x2C
	BYTE*	pPlrMode;				//0x30
	DWORD	dwPlrModeRecords;		//0x34
	BYTE*	pSkillCalc;				//0x38
	DWORD	dwSkillCalcRecords;		//0x3C
	BYTE*	pSkillCalcCache;		//0x40
	DWORD	dwSkillCalcCacheRecs;	//0x44
	DWORD	dwSkillCalcCacheDelta;	//0x48
	BYTE*	pSkillDescCalcCache;	//0x4C
	DWORD	dwSkillDescCalcCacheRecs;//0x50
	DWORD	dwSkillDescCalcCacheDelta;//0x54
	BYTE*	pMissCalc;				//0x58
	DWORD	dwMissCalcRecords;		//0x5C
	BYTE*	pMissCalcCache;			//0x60
	DWORD	dwMissCalcCacheRecs;	//0x64
	DWORD	dwMissCalcCacheDelta;	//0x68
	BYTE*	pSkillCodes;			//0x6C
	DWORD	dwSkillCodesRecs;		//0x70
	BYTE*	pEvents;				//0x74
	DWORD	dwEventsRecs;			//0x78
	BYTE*	pCompCodes;				//0x7C
	DWORD	dwCompCodesRecs;		//0x80
	DWORD	dwCompCodes;			//0x84
	BYTE*	pMonAi;					//0x88
	DWORD	dwMonAiRecs;			//0x8C
	DWORD	dwMonAi;				//0x90
	BYTE*	pItem;					//0x94
	BYTE*	pItemCalcCache;			//0x98
	DWORD	dwItemCalcCacheRecs;	//0x9C
	DWORD	dwItemCalcCache;		//0xA0
	BYTE*	pProperties;			//0xA4
	DWORD	dwPropertiesRecs;		//0xA8
	DWORD	dwProporties;			//0xAC
	BYTE*	pRunes;					//0xB0
	BYTE*	pHireDescs;				//0xB4
	DWORD	dwHireDescsRecs;		//0xB8
	BYTE*	pStates;				//0xBC
	DWORD	dwStatesRecs;			//0xC0
	DWORD	dwStates;				//0xC4
	BYTE*	pStateMaskFirst;		//0xC8
	BYTE*	pStateMaskArr[40];		//0xCC
	BYTE*	pProgressiveStates;		//0x16C
	DWORD	dwProgressiveStatesRecs;//0x170
	BYTE*	pCurseStates;			//0x174
	DWORD	dwCurseStatesRecs;		//0x178
	BYTE*	pTransStates;			//0x17C
	DWORD	dwTransStatesRecs;		//0x180
	BYTE*	pActionStates;			//0x184
	DWORD	dwActionStatesRecs;		//0x188
	BYTE*	pColorStates;			//0x18C
	DWORD	dwColorStatesRecs;		//0x190
	BYTE*	pSoundCodes;			//0x194
	BYTE*	pSoundCodesTxt;			//0x198
	DWORD	dwSoundCodesRecs;		//0x19C
	BYTE*	pHirelings;				//0x1A0
	DWORD	dwHirelingsRecs;		//0x1A4
	DWORD	HireNameStart[256];		//0x1A8
	DWORD	HireNameEnd[256];		//0x5A8
	BYTE*	pNpcs;					//0x9A8
	DWORD	dwNpcsRecs;				//0x9AC
	BYTE*	pColorsTxt;				//0x9B0
	BYTE*	pColors;				//0x9B4
	BYTE*	pTreasureClassEx;		//0x9B8
	BYTE*	pTreasureClassExTxt;	//0x9BC
	DWORD	dwTreasureClassExRecs;	//0x9C0
	BYTE*	pChestTreasureClassLst[45];	//0x9C4
	BYTE*	pMonStatsTxt;			//0xA78
	BYTE*	pMonStats;				//0xA7C
	DWORD	dwMonStatsRecs;			//0xA80
	BYTE*	pMonSoundsTxt;			//0xA84
	BYTE*	pMonSounds;				//0xA88
	DWORD	dwMonSoundsRecs;		//0xA8C
	BYTE*	pMonStats2Txt;			//0xA90
	BYTE*	pMonStats2;				//0xA94
	DWORD	dwMonStats2Recs;		//0xA98
	BYTE*	pMonPlaceTxt;			//0xA9C
	BYTE*	pMonPlace;				//0xAA0
	DWORD	dwMonPlaceRecs;			//0xAA4
	BYTE*	pMonPresetTxt;			//0xAA8
	BYTE*	pMonPresetActLst[5];	//0xAAC
	DWORD	dwMonPresetActRecs[5];	//0xAC0
	BYTE*	pSuperUniquesTxt;		//0xAD4
	BYTE*	pSuperUniques;			//0xAD8
	DWORD	dwSuperUniquesRecs;		//0xADC
	WORD	SuperUniqeIdxList[66];	//0xAE0
	BYTE*	pMissilesTxt;			//0xB64
	BYTE*	pMissiles;				//0xB68
	DWORD	dwMissilesRecs;			//0xB6C
	BYTE*	pMonLvl;				//0xB70
	DWORD	dwMonLvlRecs;			//0xB74
	BYTE*	pMonSeqTxt;				//0xB78
	BYTE*	pMonSeq;				//0xB7C
	DWORD	dwMonSeqRecs;			//0xB80
	BYTE*	pMonSequences;			//0xB84
	DWORD	dwMonSequencesRecs;		//0xB88
	BYTE*	pSkillDescTxt;			//0xB8C
	BYTE*	pSkillDesc;				//0xB90
	DWORD	dwSkillDescRecs;		//0xB94
	SkillsTxt*pSkillsTxt;			//0xB98
	BYTE*	pSkills;				//0xB9C
	DWORD	dwSkillsRecs;			//0xBA0
	BYTE*	pPlayerSkillCount;		//0xBA4
	DWORD	dwPlayerSkillCount;		//0xBA8
	BYTE*	pPlayerSkillList;		//0xBAC
	DWORD	dwPassiveSkillCount;	//0xBB0
	BYTE*	pPasssiveSkillList;		//0xBB4
	BYTE*	pOverlayTxt;			//0xBB8
	BYTE*	pOverlay;				//0xBBC
	DWORD	dwOverlayRecs;			//0xBC0
	CharStatsTxt*	pCharStatsTxt;	//0xBC4
	DWORD	dwCharsStatsRecs;		//0xBC8
	BYTE*	pItemStatCostTxt;		//0xBCC
	BYTE*	pItemStatCost;			//0xBD0
	DWORD	dwItemStatCostRecs;		//0xBD4
	BYTE*	pOpStatNesting;			//0xBD8
	DWORD	dwOpStatNestingRecs;	//0xBDC
	BYTE*	pMonEquip;				//0xBE0
	DWORD	dwMonEquipRecs;			//0xBE4
	BYTE*	pPetTypesTxt;			//0xBE8
	BYTE*	pPetTypes;				//0xBEC
	DWORD	dwPetTypesRecs;			//0xBF0
	BYTE*	pItemsType;				//0xBF4
	BYTE*	pItemsTypeTxt;			//0xBF8
	DWORD	dwItemsTypeRecs;		//0xBFC
	DWORD	dwItemsTypeNesting;		//0xC00
	BYTE*	pItemsTypeNesting;		//0xC04
	BYTE*	pSets;					//0xC08
	BYTE*	pSetsTxt;				//0xC0C
	DWORD	dwSetsRecs;				//0xC10
	BYTE*	pSetItems;				//0xC14
	BYTE*	pSetItemsTxt;			//0xC18
	DWORD	dwSetItemsRecs;			//0xC1C
	BYTE*	pUniqueItems;			//0xC20
	BYTE*	pUniqueItemsTxt;		//0xC24
	DWORD	dwUniqItemsRecs;		//0xC28
	BYTE*	pMonProp;				//0xC2C
	BYTE*	pMonPropTxt;			//0xC30
	DWORD	dwMonPropRecs;			//0xC34
	BYTE*	pMonType;				//0xC38
	BYTE*	pMonTypeTxt;			//0xC3C
	DWORD	dwMonTypeRecs;			//0xC40
	BYTE*	pMonTypeNesting;		//0xC44
	DWORD	dwMonTypeNestingRecs;	//0xC48
	BYTE*	pMonUMod;				//0xC4C
	BYTE*	pMonUModTxt;			//0xC50
	DWORD	dwMonUModRecs;			//0xC54
	BYTE*	pLevels;				//0xC58
	DWORD	dwLevelsRecs;			//0xC5C
	BYTE*	pLvlDefs;				//0xC60
	BYTE*	pLvlPrest;				//0xC64
	DWORD	dwLvlPrestRecs;			//0xC68
	DWORD	ItemStatCostStuff[2];	//0xC6C
	BYTE*	pAnimTables;			//0xC74
	BYTE*	pExperience;			//0xC78
	BYTE*	pDiffLvls;				//0xC7C
	DWORD	dwDiffLvlsRecs;			//0xC80
	BYTE*	pExpFieldD2;			//0xC84
	DWORD	ExpFieldData[10];		//0xC88
	BYTE*	pLvlSubExtra;			//0xCB0
	DWORD	dwLvlSubExtraRecs;		//0xCB4
	BYTE*	pCharTemp;				//0xCB8
	DWORD	dwCharTempRecs;			//0xCBC
	BYTE*	pArena;					//0xCC0
	BYTE*	pLvlTypes;				//0xCC4
	BYTE*	pWaypoints;				//0xCC8
	DWORD	dwWaypointsRecs;		//0xCCC
	DWORD	dwLvlTypes;				//0xCD0
	BYTE*	pLvlWarp;				//0xCD4
	DWORD	dwLvlWarpRecs;			//0xCD8
	BYTE*	pLvlMaze;				//0xCDC
	DWORD	dwLvlMazeRecs;			//0xCE0
	BYTE*	pLvlSub;				//0xCE4
	DWORD	dwLvlSubRecs;			//0xCE8
	BYTE*	pLvlSubCache;			//0xCEC
	DWORD	_1[3];					//0xCF0
	BYTE*	pMapCache;				//0xCFC
	DWORD	dwMapCacheRecs;			//0xD00
	BYTE*	pCubeMain;				//0xD04
	DWORD	dwCubeMainRecs;			//0xD08
	BOOL	bWriteBinFiles;			//0xD0C
	};


#endif