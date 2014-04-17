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
	DWORD (__fastcall *Callback)(Game *pGame, UnitAny *pUnit, BYTE *aPacket, int PacketLen);
	BOOL _1;
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

#pragma pack(push, 1)

struct bItemFlags //Taken from Necrolis post & Hero Editor  (1.13d)
{
	BYTE bNewItem : 1;		//1
	BYTE bTarget : 1;			//2
	BYTE bTargeting : 1;		//3
	BYTE bDeleted : 1;		//4
	BYTE bIdentified : 1;		//5
	BYTE bQuantity : 1;		//6
	BYTE bWeaponSetIn : 1;	//7
	BYTE bWeaponSetOut : 1;	//8
	BYTE bBroken : 1;			//9
	BYTE bRepaired : 1;		//10
	BYTE Flag11 : 1;			//11
	BYTE bSocketed : 1;		//12
	BYTE bNonSellable : 1;	//13
	BYTE bInStore : 1;		//14 Also Has Been Picked Up
	BYTE bNoEquip : 1;		//15
	BYTE bNamed : 1;			//16
	BYTE bOrgan : 1;			//17 Also bEar
	BYTE bStarter : 1;		//18 Also bSellCheap
	BYTE Flag19 : 1;			//19
	BYTE bInit : 1;			//20
	BYTE Flag21 : 1;			//21
	BYTE bCompactSave : 1;	//22 Also bSimple
	BYTE bEthereal : 1;		//23
	BYTE bJustSaved : 1;		//24
	BYTE bPersonalized : 1;	//25
	BYTE bLowQuality : 1;		//26
	BYTE bRuneword : 1;		//27
	BYTE bItem : 1;			//28
	BYTE _Unused1 : 1;		//29
	BYTE _Unused2 : 1;		//30
	BYTE _Unused3 : 1;		//31
	BYTE _Unused4 : 1;		//32
};

#pragma pack(pop)



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


struct Room1 // Checked. Need to find: nRoomsNear, nRoomUnits, 0x6FD742D0
{
	Room1** pRoomsNear; 	//0x00
	DWORD _1;				//0x04
	void* _1s;				//0x08
	DWORD _1b;				//0x0C
	Room2* pRoom2;			//0x10
	DWORD _2[3];			//0x14
	CollMap* Coll;			//0x20
	DWORD dwRoomsNear;		//0x24 9 rooms
	DWORD _3;				//0x28
	Act* pAct;				//0x2C
	DWORD _4;				//0x30
	DWORD nUnknown;			//0x34
	DWORD _5[5];			//0x38
	DWORD dwXStart;			//0x4C
	DWORD dwYStart;			//0x50
	DWORD dwXSize;			//0x54
	DWORD dwYSize;			//0x58
	DWORD _6[4];			//0x5C
	DWORD dwSeed[2];		//0x6C
	UnitAny* pUnitFirst;	//0x74
	DWORD nPlayers;			//0x78
	Room1* pRoomNext;		//0x7C
};


struct ActMisc // CHECKED
{
	DWORD _1[19];			//0x00
	DWORD pfnCallBack;		//0x4c
	DWORD _1a[17];			//0x50
	DWORD dwStaffTombLevel; //0x94
	DWORD _2[245];			//0x98
	Act* pAct;				//0x46C
	DWORD _3[3];			//0x470
	Level* pLevelFirst;		//0x47C
};

struct Act // need to find: pEnviroment, pMemPool, pfnCallBack
{
	DWORD _1[3];					//0x00
	DWORD dwMapSeed;				//0x0C
	Room1* pRoom1;					//0x10
	DWORD dwAct;					//0x14
	DWORD _3[12];					//0x18
	ActMisc* pMisc;					//0x48
	DWORD _2[2];					//0x4C
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


struct Path  // same as 1.11b (1.13d)
{
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

struct StaticPath //size 0x20 same as 1.11b (1.13d)
{ 
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
	DWORD(__fastcall *pCallback[15])(Quest *pQuest, QParam *pqParam);      //0xA0 - CallBack functions
	NPCMsgs* pNPCMsgs;      //0xDC - ptr to NPC Messages
	DWORD eFilter;          //0xE0 - index used in bit calls (#11107 etc)
	void* pStatusFilter;     //0xE4 - function ptr
	void* pActiveFilter;     //0xE8 - function ptr - return 1 to activate (!) bubble
	void* pSequence;         //0xEC - function ptr
	Quest* pPrev;	         //0xF0

};


struct QuestArray	//size 0x18
{
	DWORD(__fastcall *pQuestInit)(Quest *pQuest);		//0x00
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
	ObjectRegion* pObjectRegion;		//0x10F0 - a controller holding all object region structs
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
	DWORD bPersonalize;		//0x2C also allowing socketing bForce -> uses flag thx to Necrolis
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

#endif