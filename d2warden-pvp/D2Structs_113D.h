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
struct ActMisc;
struct Path;
struct StaticPath;
struct StatList;
struct Inventory;
struct Light;
struct Level;
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
struct ObjectRegion;
struct BitBuffer;

struct pSpellTbl
{
	int (__fastcall *SpellPrepareFunc)(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
	int (__fastcall *SpellDoFunc)(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
};


struct PacketTbl
{
	DWORD (__fastcall *Callback)(Game *pGame, UnitAny *pUnit, BYTE *aPacket, int PacketLen);
	BOOL _1;
};


struct PartyExp // Tymczasowo rozszerzylem do 16 (originally sized 0x4C)
{
	UnitAny *pMonster;
	UnitAny *pPartyMembers[64];
	DWORD PartyLevels[64];
	DWORD nMembers;
	DWORD LvlSum;
};

struct TradeContainer	//sizeof 0x0C
{
	DWORD nOriginalId;		//0x00	An Item Id
	DWORD nTradeId;			//0x04 - || -
	TradeContainer* pNext;	//0x08
};

struct PlayerTrade		//size 0x18 -> resized to 0x20 // Need to check 
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


struct PacketData //size 0x208 (1.13d)
{
	DWORD ClientID;			//0x00 or PacketLen
	BYTE aPacket[512];			//0x04
	PacketData* pNext;			//0x204
};


struct ItemCon //size 0x08 (1.13d)
{
	DWORD ItemId;
	ItemCon *pNext;
};

struct Corpse	//size 0x10 (1.13d)
{
	BOOL isCorpse;			//0x00 If true, game will increase player's corpse number
	DWORD UnitId;			//0x04
	DWORD _1;				//0x08 arg passed = 0
	Corpse* pNext;			//0x0C
};

struct Inventory //size 0x40 (1.13d)
{
	DWORD dwSignature;				//0x00
	D2PoolManager* pMemPool;					//0x04
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

struct CollMap  // (1.13d)
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



struct DrlgPreset // sizeof(0x08), type 2
{
	DWORD * _1;						//0x00
	DWORD dwPresetId;				//0x04 ds1 id used to generate level
};

struct DrlgWildreness // sizeof(0x268), type 3
{

};

struct Level //sizeof(0x230)
{
	DWORD dwDrlgType;		//0x00 eDrlgType
	DWORD dwLevelFlags;		//0x04
	DWORD _1[2];			//0x08
	Room2* pRoom2First;		//0x10
	DWORD *pDrlgUNK;		//0x14 union depends on DrlgType: 1: pLvlWarpTxt, 2: DrlgPreset, 3: DrlgWildreness
	DWORD _2;				//0x18
	DWORD dwPosX;			//0x1C
	DWORD dwPosY;			//0x20
	DWORD dwSizeX;			//0x24
	DWORD dwSizeY;			//0x28
	DWORD _3[96];			//0x2C
	Level* pNextLevel;		//0x1AC
	DWORD _4;				//0x1B0
	ActMisc* pMisc;			//0x1B4
	DWORD _5[2];			//0x1B8
	DWORD dwLevelType;		//0x1C0 The type of the Level (Id from lvltypes.txt)
	D2Seed hSeed;			//0x1C4
	DWORD _5b;				//x01CC
	DWORD dwLevelNo;		//0x1D0
	DWORD _6[3];			//0x1D4
	union {
		DWORD RoomCenterX[9];
		DWORD WarpX[9];
	};						//0x1E0
	union {
		DWORD RoomCenterY[9];
		DWORD WarpY[9];
	};						//0x204
	DWORD dwRoomEntries;	//0x228
	DWORD _7;				//0x22C pointer to mem 4 * _1[0]
};


struct Room1 // Checked. Need to find: nRoomsNear, nRoomUnits, 0x6FD742D0
{
	Room1** pRoomsNear; 	//0x00
	DWORD _1;				//0x04
	void* _1s;				//0x08
	DWORD _1b;				//0x0C
	Room2* pRoom2;			//0x10
	DWORD _2[2];			//0x14
	UnitAny* pUnitChanged;	//0x1C
	CollMap* Coll;			//0x20
	DWORD dwRoomsNear;		//0x24 9 rooms
	DWORD nPlayerUnits;		//0x28
	Act* pAct;				//0x2C
	DWORD _4;				//0x30
	DWORD nUnknown;			//0x34
	DWORD _5[4];			//0x38
	ClientData** pClients;	//0x48
	DWORD dwXStart;			//0x4C
	DWORD dwYStart;			//0x50
	DWORD dwXSize;			//0x54
	DWORD dwYSize;			//0x58
	DWORD _6[4];			//0x5C
	DWORD dwSeed[2];		//0x6C
	UnitAny* pUnitFirst;	//0x74
	DWORD nNumClients;		//0x78
	Room1* pRoomNext;		//0x7C
};

struct Room2 //sizeof(0xEC)
{
	DWORD _1[2];			//0x00
	Room2** pRoom2Near;		//0x08
	DWORD _2[5];			//0x0C
	struct {
		DWORD dwRoomNumber; //0x00
		DWORD _1;			//0x04
		DWORD* pdwSubNumber;//0x08
		//... dwPresetType == 1, sizeof(0x70), 2, sizeof(0xF8)
	} *pType2Info;			//0x20
	Room2* pRoom2Next;		//0x24
	DWORD dwRoomFlags;		//0x28 if Vis[1-8] : 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800
	DWORD dwRoomsNear;		//0x2C
	Room1* pRoom1;			//0x30
	DWORD dwPosX;			//0x34
	DWORD dwPosY;			//0x38
	DWORD dwSizeX;			//0x3C
	DWORD dwSizeY;			//0x40
	DWORD _3;				//0x44
	DWORD dwPresetType;		//0x48
	void* pRoomTiles;		//0x4C
	DWORD _4[2];			//0x50
	Level* pLevel;			//0x58
	void* pPreset;			//0x5C
	DWORD _5;				//0x60
	DWORD _6;				//0x64
	DWORD _7;				//0x68
	DWORD _8;				//0x6C
	DWORD _9;				//0x70
	DWORD _10;				//0x74
	DWORD _11;				//0x78
	DWORD _12;				//0x7C
	DWORD _13;				//0x80
	DWORD _14;				//0x84
	DWORD _15;				//0x88
	DWORD _16;				//0x8C
	DWORD _17;				//0x90
	DWORD _18;				//0x94
	DWORD _19;				//0x98
	DWORD _20;				//0x9C
	DWORD _21;				//0xA0
	DWORD _22;				//0xA4
	DWORD _23;				//0xA8
	DWORD _24;				//0xAC
	DWORD _25;				//0xB0
	DWORD _26;				//0xB4
	DWORD _27;				//0xB8
	DWORD _28;				//0xBC
	DWORD _29;				//0xC0
	DWORD _30;				//0xC4
	DWORD _31;				//0xC8
	DWORD _32;				//0xCC
	DWORD _33;				//0xD0
	DWORD _34;				//0xD4
	DWORD _35;				//0xD8
	DWORD _36;				//0xDC
	DWORD _37;				//0xE0
	DWORD _38;				//0xE4
	DWORD _39;				//0xE8
};

struct ActMisc // sizeof(0x48C) aka Drlg
{
	D2Seed hSeed;			//0x00
	DWORD _1;				//0x08
	DWORD* pTile;			//0x0C
	DWORD _1g[15];			//0x10
	DWORD pfnCallBack;		//0x4c
	DWORD _1a[17];			//0x50
	DWORD dwStaffTombLevel; //0x94
	DWORD _2a;				//0x98
	Game* pGame;			//0x9C
	DWORD _2[236];			//0xA0
	DWORD nDiffLvl;			//0x450
	DWORD* fnCallback;		//0x454
	DWORD InitSeed;			//0x458
	DWORD _2d[4];			//0x45C
	Act* pAct;				//0x46C
	DWORD _3[2];			//0x470
	D2PoolManager *pMemPool;			//0x478
	Level* pLevelFirst;		//0x47C
	DWORD nAct;				//0x480
	DWORD _5;				//0x484 Tomb Levels Related 66 + rand(7)
	DWORD* fnCallback2;		//0x488
};

struct ActEnvironment // sizeof(0x38)
{
	DWORD _1;						//0x00  2
	DWORD _2;						//0x04  0
	DWORD _3;						//0x08  0 * 128
	DWORD _4;						//0x0C  32
	DWORD dwCreatedTick;			//0x10
	DWORD _6;						//0x14
	DWORD _7;						//0x18  Some angle (sin( (_3 / _11)  * (1/180) * PI)
	DWORD _8;						//0x1C  Some angle (-cos( (_3 / _11) * (1/180) * PI)
	DWORD _9;						//0x20
	DWORD _10;						//0x24
	DWORD _11;						//0x28  128
	DWORD _12;						//0x2C
	DWORD _13;						//0x30
	DWORD _14;						//0x34
};

struct Act // sizeof(0x60)
{
	DWORD _1a;						//0x00
	ActEnvironment* pEnviroment;	//0x04
	DWORD dwTownLvl;				//0x08
	DWORD dwMapSeed;				//0x0C
	Room1* pRoom1;					//0x10
	DWORD dwAct;					//0x14
	DWORD hTile[12];				//0x18 not sure, some inline struct of 0x30 size seems fit
	ActMisc* pMisc;					//0x48
	DWORD _4;						//0x4C
	DWORD _5;						//0x50
	DWORD _6;						//0x54
	DWORD _7;						//0x58
	D2PoolManager* pMemPool;		//0x5C
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


struct AiGeneral
{
	DWORD SpecialState;										  //0x00 - stuff like terror, confusion goes here
	void* (__fastcall * fpAiFunction)(Game*, UnitAny*, DWORD*);  //0x04 - the primary ai function to call
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

struct AiParams // sizeof = 0x24
{
	DWORD _1;				//0x00
	DWORD _2;				//0x04
	DWORD _3;				//0x08
	DWORD _4;				//0x0C
	DWORD _5;				//0x10
	DWORD _6;				//0x14
	DWORD _7;				//0x18
	DWORD _8;				//0x1C
	DWORD _9;				//0x20
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
	MonStatsTxt* pMonStatsTxt;				//0x00
	BYTE Components[16];					//0x04 Order: HD, TR, LG, RA, LA, RH, LH, SH, S1, S2, S3, S4, S5, S6, S7, S8
	WORD NameSeed;							//0x14
	struct
	{
		BYTE fOther : 1;
		BYTE fSuperUniq : 1;
		BYTE fChampion : 1;
		BYTE fUnique : 1;
		BYTE fMinion : 1;
		BYTE fPossesed : 1;
		BYTE fGhostly : 1;
		BYTE fMultishot : 1;
	};				//0x16
	BYTE dwLastMode;				//0x17
	DWORD dwDuriel;					//0x18
	BYTE anEnchants[9];				//0x1C
	BYTE _1;						//0x25
	WORD wUniqueNo;					//0x26
	AiGeneral* pAiGeneral;			//0x28
	AiParams* pAiParams;			//0x2C
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
	D2PoolManager* pMemPool;		//0x00
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
	D2PoolManager* pMemPool;		//0x00
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

#pragma pack(push, 1)
struct QuestGUID
{
	DWORD dwPlayerGUID[32];
	WORD nGUIDCount;
};
#pragma pack(pop)


struct Quest	//size 0xF8  (Warning 1.11b was 0xF4!)- thx to SVR - D2QuestDataStrc 
{
	DWORD	QuestID;        //0x00
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
	void*	pQuestEx;		//0x18 - quest specific data
	QuestGUID hGUIDs;		//0x1C
	WORD _2;				//0x9E
	DWORD(__fastcall *pCallback[15])(Quest *pQuest, QParam *pqParam);      //0xA0 - CallBack functions
	NPCMsgs* pNPCMsgs;      //0xDC - ptr to NPC Messages
	DWORD nQuest;           //0xE0 - index used in bit calls (#11107 etc)
	DWORD _3;				//0xE4	
	void* pStatusFilter;     //0xE8 - function ptr
	void* pActiveFilter;     //0xEC - function ptr - return 1 to activate (!) bubble
	void* pSequence;         //0xF0 - function ptr
	Quest* pPrev;	         //0xF4

};


struct QuestArray	//size 0x18
{
	DWORD (__fastcall *pQuestInit)(Quest *pQuest);		//0x00
	BYTE nAct;											//0x04
	BYTE _1[3];											//0x05
	DWORD nVersion;										//0x08
	BYTE bNoSetState;									//0x0C - used by the sequences for quest init flags
	BYTE _2[3];											//0x0D
	DWORD nChainNo;										//0x10
	DWORD nQuestNo;										//0x14
};


struct QuestControl //size 0x24
{
	Quest *pQuest;			//0x00
	BOOL bExectuing;		//0x04
	BOOL bPickedSet;		//0x08
	BitBuffer *pQuestFlags; //0x0C  ->size 0x60 (BitBuffer)
	DWORD *pQuestTimer;		//0x10
	DWORD dwTick;			//0x14
	DWORD LowSeed;			//0x18
	DWORD HighSeed;			//0x1C
	DWORD _8;				//0x20
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
	WORD PlayerStatus;			   //0x0A 0x4 connected, 0x8 death
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
	D2PoolManager* pMemPool;	//0x00
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


#endif