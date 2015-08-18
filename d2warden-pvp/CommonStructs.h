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
*/

#ifndef COMMONSTRUCTS_H__
#define COMMONSTRUCTS_H__

struct PlayerTrade;
struct ClientData;
struct QuestFlags;
struct Waypoint;
struct Game;
struct Room1;
struct PlayerData;
struct MonsterData;
struct ItemData;
struct ObjectData;
struct StatListEx;
struct Inventory;
struct UnitEvent;
struct SkillData;
struct Act;
struct Path;
struct StaticPath;
struct Combat;
struct UnitEvent;
struct UnitMsg;
struct UnitNode;
struct Timer;
struct InactiveRoom;
struct TimerQuene;
struct QuestControl;
struct NpcControl;
struct Arena;
struct Party;
struct UnitAny;
struct Quest;
struct D2PoolMemory;

struct EventCallbackTable
{
	void* fpCloseGame;									//0x00
	void* fpLeaveGame;									//0x04
	void* fpGetDatabaseCharacter;						//0x08
	void* fpSaveDatabaseCharacter;						//0x0C
	void* fpServerLogMessage;							//0x10
	void* fpEnterGame;									//0x14
	void* fpFindPlayerToken;							//0x18
	void* fpSaveDatabaseGuild;							//0x1C
	void* fpUnlockDatabaseCharacter;					//0x20
	void* _2;											//0x24
	void* fpUpdateCharacterLadder;						//0x28
	void* fpUpdateGameInformation;						//0x2C
	void* _3;											//0x30
	void* fpSetGameData;								//0x34
	void* fpRelockDatabaseCharacter;					//0x38
	void* fpLoadComplete;								//0x3C
	void* _4;											//0x40
	void* _5;											//0x44
	void* _6;											//0x48
	void* _7;											//0x4C
	void* _8;											//0x50
#ifdef VER_113D
	void(__fastcall *fpValidateFileTime)(FILETIME **ft);//0x54 1.13d only
#endif
};


#pragma pack(push, 1)

// sizeof 16
struct Waypoint
{
	WORD wBank[8];							//0x00 --> contains flags for 8 Waypoint banks, each bank has 16 slots, so yoy
											// can have 128 wps, but using of 1st bank will crash game so really its 112 waypoints.
};

struct D2PoolBlock
{
	D2PoolBlock* pNextChunk;
	//...+ raw data chunk
};


// sizeof 0x30
struct D2Pool {
	CRITICAL_SECTION poolLock;	//0x00
	DWORD _1;					//0x18 => 16 for nPools #0, 32 for #1 and so
	DWORD nMemBlocksPerPool;	//0x1C => 4096
	DWORD nBlockSize;			//0x20 => 65536 ( _1 * nMemBlocksPerPool)
	DWORD nTableCounter;		//0x24 <- number of D2PoolMemory elements (increases each act visited)
	D2PoolMemory* pFirst;		//0x28 <- first element of memory
	D2PoolMemory* pLast;		//0x2C <- last element of pMemoryTbl (aka pTail)
};

//sizeof 0x18?
struct D2PoolMemory
{
	void* pMemory;				//0x00
	DWORD _2;					//0x04
	DWORD _3;					//0x08
	D2PoolMemory* pPrev;		//0x0C
	D2PoolMemory* pNext;		//0x10
	D2Pool* pPool;				//0x14
};

//sizeof = 6092
struct D2PoolManager
{
	D2PoolManager* pNext;			 //0x00
	CRITICAL_SECTION managerLock;	 //0x04
	DWORD nPools;					 //0x1C
	D2Pool pPool[40];		  		 //0x20
	DWORD nUsedBlocks;				 //0x7A0
	DWORD nMaxBlocks;				 //0x7A4
	D2PoolBlock* pChainBlocks[1024]; //0x7A8
	DWORD _7;						 //0x17A8 not pointer for sure
	char szGameName[32];			 //0x17AC
};

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

struct D2POINT
{
	unsigned int x;
	unsigned int y;
};


#pragma pack(push, 1)
struct QuestIntroArray
{
	unsigned int(__fastcall *pQuestInit)(Quest *);
	BYTE nAct;
	BYTE _pad[3];
};
#pragma pack(pop)

/*
	Array for PlayerModes (mby for others too)
*/
struct StartChart
{
	void(__fastcall *pfnStartXY)(Game *pGame, UnitAny *pUnit, int nMode, WORD nX, WORD nY);
	void(__fastcall *pfnStartID)(Game *pGame, UnitAny *pUnit, int nMode, UnitAny *pTarget);
};

struct LRosterData
{
	char szName[16];
	int Assists;
	int Kills;
	int Deaths;
	LRosterData *ptNext;
};

struct LSpectator
{
	DWORD WatcherUID;
	DWORD UnitUID;
	LSpectator *pNext;
	LSpectator *pPrev;
};


struct Path  // same as 1.11b (1.13d), sizof 0x200
{
	WORD xOffset;					//0x00 32768 <- Changes only when moving
	WORD xPos;						//0x02 WORD(nX)
	WORD yOffset;					//0x04 32768 <- Changes only when moving
	WORD yPos;						//0x06 WORD(nY)
	DWORD xUnknown;					//0x08  16 * (wInitX - wInitY) <- Mby AutomapX
	DWORD yUnknown;					//0x0C  8 * (wInitX + wInitY + 1) <- Mby AutoampY
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
	DWORD _5[2];					//0x48
	DWORD dwCollisionFlag;			//0x50  0x1804 <- bFlying, 0x3401 <- bOpenDoors, 0x3C01 <- Cannot Open Doors, 0x804 <- Ghost, 0x1C09 <- Player
	DWORD _5d;						//0x54
	UnitAny *pTargetUnit;			//0x58
	DWORD dwTargetType;				//0x5C
	DWORD dwTargetId;				//0x60
	BYTE bDirection;				//0x64
	DWORD _6;						//0x68
	DWORD _7;						//0x6C
	DWORD _8;						//0x70
	DWORD _9;						//0x74
	DWORD _10;						//0x78
	DWORD _11;						//0x7C
	DWORD _12;						//0x80
	DWORD _13;						//0x84
	DWORD _14;						//0x88
	DWORD _15;						//0x8C
	BYTE _16a;						//0x90
	BYTE _16b;						//0x91
	BYTE _16c;						//0x92
	BYTE _16d;						//0x93
	DWORD _17;						//0x94
	DWORD _18;						//0x98
	DWORD _19;						//0x9C
	DWORD _20;						//0xA0
	DWORD _21;						//0xA4
	DWORD _22;						//0xA8
	DWORD _23;						//0xAC
	DWORD _24;						//0xB0
	DWORD _25;						//0xB4
	DWORD _26;						//0xB8
	DWORD _27;						//0xBC
	DWORD _28;						//0xC0
	DWORD _29;						//0xC4
	DWORD _30;						//0xC8
	DWORD _31;						//0xCC
	DWORD _32;						//0xD0
	DWORD _33;						//0xD4
	DWORD _34;						//0xD8
	DWORD _35;						//0xDC
	DWORD _36;						//0xE0
	DWORD _37;						//0xE4
	DWORD _38;						//0xE8
	DWORD _39;						//0xEC
	DWORD _40;						//0xF0
	DWORD _41;						//0xF4
	DWORD _42;						//0xF8
	DWORD _43;						//0xFC
	DWORD _44;						//0x100
	DWORD _45;						//0x104
	DWORD _46;						//0x108
	DWORD _47;						//0x10C
	DWORD _48;						//0x110
	DWORD _49;						//0x114
	DWORD _50;						//0x118
	DWORD _51;						//0x11C
	DWORD _52;						//0x120
	DWORD _53;						//0x124
	DWORD _54;						//0x128
	DWORD _55;						//0x12C
	DWORD _56;						//0x130
	DWORD _57;						//0x134
	DWORD _58;						//0x138
	DWORD _59;						//0x13C
	DWORD _60;						//0x140
	DWORD _61;						//0x144
	DWORD _62;						//0x148
	DWORD _63;						//0x14C
	DWORD _64;						//0x150
	DWORD _65;						//0x154
	DWORD _66;						//0x158
	DWORD _67;						//0x15C
	DWORD _68;						//0x160
	DWORD _69;						//0x164
	DWORD _70;						//0x168
	DWORD _71;						//0x16C
	DWORD _72;						//0x170
	DWORD _73;						//0x174
	DWORD _74;						//0x178
	DWORD _75;						//0x17C
	DWORD _76;						//0x180
	DWORD _77;						//0x184
	DWORD _78;						//0x188
	DWORD _79;						//0x18C
	DWORD _80;						//0x190
	DWORD _81;						//0x194
	DWORD _82;						//0x198
	DWORD _83;						//0x19C
	DWORD _84;						//0x1A0
	DWORD _85;						//0x1A4
	DWORD _86;						//0x1A8
	DWORD _87;						//0x1AC
	DWORD _88;						//0x1B0
	DWORD _89;						//0x1B4
	DWORD _90;						//0x1B8
	DWORD _91;						//0x1BC
	DWORD _92;						//0x1C0
	DWORD _93;						//0x1C4
	DWORD _94;						//0x1C8
	DWORD _95;						//0x1CC
	DWORD _96;						//0x1D0
	DWORD _97;						//0x1D4
	WORD wInitX;					//0x1D8
	WORD wInitY;					//0x1DA
	DWORD _99;						//0x1DC
	DWORD _100;						//0x1E0
	DWORD _101;						//0x1E4
	DWORD _102;						//0x1E8
	DWORD _103;						//0x1EC
	DWORD _104;						//0x1F0
	DWORD _105;						//0x1F4
	DWORD _106;						//0x1F8
	DWORD _107;						//0x1FC
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


struct InactiveMonster // sizeof 0x5C
{
	DWORD xPos;				//0x00
	DWORD yPos;				//0x04
	DWORD dwClassId;		//0x08
	DWORD dwUnitId;			//0x0C
	DWORD dwFlags;			//0x10
	DWORD dwFlags2;			//0x14
	DWORD dwFlags3;			//0x18
	DWORD dwOwnerUid;		//0x1C
	DWORD* pMinionList;		//0x20 -> ai related
	DWORD* pAi;				//0x24 -> ai related
	DWORD dwSpecialState;	//0x28
	DWORD dwMonLvl;			//0x2C
	WORD dwNameSeed;		//0x30
	BYTE anEnchants[10];	//0x32 <- 10 is align
	DWORD dwUniqueIdx;		//0x3C
	DWORD dwExp;			//0x40
	DWORD dwMaxLife;		//0x44
	DWORD dwLife;			//0x48
	DWORD dwDEAD;			//0x4C '0xDEADDEAD' -> ai arg?
	DWORD dwMonFlag;		//0x50 '1' if no exp gain flag, '2' if no drop flag
	DWORD dwGameFrame;		//0x54
	InactiveMonster* pNext;	//0x58
};

struct InactiveObject // sizeof 0x34
{
	DWORD xPos;				//0x00
	DWORD yPos;				//0x04
	DWORD dwType;			//0x08
	DWORD dwClassId;		//0x0C
	DWORD dwMode;			//0x10
	DWORD dwGameFrame;		//0x14
	DWORD dwFlags;			//0x18
	DWORD dwFlags2;			//0x1C
	union
	{
		DWORD dwMaxActiveQueue;	//0x20
		DWORD dwUnitId;			//0x20 Valid if Class id is 59, 60, 100 (portals)
	};
	union 
	{
		DWORD dwMaxActiveQueue2;	//0x24
		DWORD dwSparkyChest;		//0x24 ??? Valid if Class id is 59, 60, 100 (portals)
	};
	DWORD dwIntractType;		//0x28
	DWORD dwItemCodeToDrop;		//0x2C
	InactiveObject* pNext;		//0x30
};

struct InactiveItem // sizeof 0xF + (vary)
{
	InactiveItem* pNext;		//0x00
	// ...
};

/*
union
{
DWORD dwXSize;					//0x08
InactiveItem *pItem;
};
union
{
DWORD dwYSize;					//0x0C
InactiveMonster *pMonster;		//0x0C
};
// probably wrong
*/
struct InactiveRoom // sizeof(0x18)
{
	DWORD dwXStart;					//0x00
	DWORD dwYStart;					//0x04
	InactiveItem *pItem;			//0x08
	InactiveMonster *pMonster;		//0x0C
	InactiveObject *pObject;		//0x10
	InactiveRoom* pNextRoom;		//0x14
};

struct RoomCoords // sizeof(0x20) 32 bytes of Room1+4C
{
	DWORD dwXStart;					//0x00
	DWORD dwYStart;					//0x04
	DWORD dwXSize;					//0x08
	DWORD dwYSize;					//0x0C
	WORD wTileX;					//0x10
	WORD _1b;						//0x12
	WORD wTileY;					//0x14
	WORD _2b;						//0x16
	DWORD _3;						//0x18
	DWORD _4;						//0x1C
};


struct ObjectShrines
{
	DWORD nEffectClass[8];
	DWORD *pShrineIdx[8]; // -> points to structure contains txt id
};

struct ObjectLevel // size 0x90
{
	BYTE nAct;				//0x00
	BYTE _1;				//0x01
	WORD _2;				//0x02
	DWORD _3;				//0x04 some counter
	DWORD nRooms;			//0x08 inited as 7FFFFFFFh,  !(pRoom->dwRoomFlags & 0x800000) for every room in level
	DWORD _5[4];			//0x0C
	DWORD _6;				//0x1C  intied as -1
	DWORD _7;				//0x20
	DWORD _9[28];			//0x24
};

struct ObjectRegion // sizeof 0x1114
{
	DWORD GameSeed[2];		//0x00
	ObjectShrines hShrines; //0x04
	ObjectLevel *pLevel[256];     //0x48 -> x90 struct 0x08, first entry unused
	DWORD _3[100];
	DWORD _4[100];
	DWORD _5[100];
	DWORD _6[100];
	DWORD _7[100];
	DWORD _8[100];
	DWORD _9[100];
	DWORD _10[68];
	DWORD hShrines2[50];
	DWORD _12;				//0x1110
};


#pragma pack(push, 1)

struct MonsterNode		//sizoef = 0x34
{
	WORD nTxtRecord;			//0x00
	BYTE bRarity;				//0x02
	BYTE _1;					//0x03
	DWORD _2;					//0x04
	DWORD _3;					//0x08
	DWORD _4;					//0x0C
	DWORD _5;					//0x10
	DWORD _6;					//0x14
	DWORD _7;					//0x18
	DWORD _8;					//0x1C
	DWORD _9;					//0x20
	DWORD _10;					//0x24
	DWORD _11;					//0x28
	DWORD _12;					//0x2C
	DWORD _13;					//0x30
};


struct MonsterRegion		//sizeof = 0x2E4
{
	BYTE nAct;					//0x00
	BYTE f1[3];					//0x01
	DWORD nRoomsASSUME;			//0x04
	DWORD _2;					//0x08
	DWORD nActiveRooms;			//0x0C
	BYTE nNodes;				//0x10
	BYTE mRaritySum;			//0x11
	BYTE nSpawned;				//0x12
	BYTE f11;					//0x13				
	MonsterNode hNodes[13];		//0x14
	DWORD bMonDensity;			//0x2B8
	BYTE bMonUniqMin;			//0x2BC
	BYTE bMonUniqMax;			//0x2BD
	BYTE bMonWndr;				//0x2BE
	BYTE f2BF;					//0x2BF
	DWORD nLevel;				//0x2C0
	DWORD nOtherMonsterCounter;	//0x2C4
	DWORD _5;					//0x2C8
	DWORD nMonstersASSUME;		//0x2CC
	DWORD nMonstersAligmented;	//0x2D0
	DWORD dword2D4;				//0x2D4
	BYTE nQuest;				//0x2D8
	BYTE f2D9[3];				//0x2D9
	DWORD dwMonLvl;				//0x2DC
	DWORD dwMonLvlCopy;			//0x2E0
};
#pragma pack(pop)


struct PresetMonster // sizeof 0x26
{
	Game * pGame;	//0x00
	Room1* pRoom;	//0x04
	DWORD _1;		//0x08
	DWORD nMonIdx;  //0x0C
	DWORD nMonCount;//0x10
	DWORD _2;		//0x14
	DWORD xPos;		//0x18
	DWORD yPos;		//0x1C
	DWORD _3;		//0x20
	WORD _4;		//0x24
};

struct PresetItem  //size 0x84
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

struct PartyRelation //size 0x14
{
	DWORD dwUnitId;					//0x00
	DWORD dwFlags;					//0x04
	DWORD dwRelationTick;			//0x08
	DWORD wPartyId;					//0x0C
	PartyRelation *pNext;			//0x10
};

struct PlayerData { //size 0x16C same as 1.11b (1.13d) -- probably --
	char szName[16];				//0x00
	QuestFlags *QuestsFlags[3];		//0x10
	Waypoint *pWaypoints[3];		//0x1C aka pHistory
	DWORD _1;						//0x28
	DWORD _2[2];					//0x2C
	WORD* KillCounter;				//0x34 Monster -> ptArena struct
	PartyRelation* pPartyRelation;	//0x38
	DWORD _3[4];					//0x3C 3[2] is merc related
	DWORD dwBusyTick;				//0x4C Packet 0x48 nullifies
	DWORD nTradeState;				//0x50
	DWORD _4;						//0x54
	DWORD dwAcceptTradeTick;		//0x58
	PlayerTrade* pTrade;			//0x5C
	DWORD* QuestUnk[3];				//0x60 for each difficulty
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
	BYTE  bTelportCount;			//0xA0
	BYTE  _7[3];					//0xA1
	DWORD dwTeleportTick;			//0xA4
	D2POINT hTeleportHistory[20];	//0xA8
	DWORD dwLastTeleportX;			//0x148 These two are set when you've been just teleported
	DWORD dwLastTeleportY;			//0x14C
	DWORD _7c;						//0x150
	DWORD _7d;						//0x154
	DWORD _7e;						//0x158
	DWORD _7f;						//0x15C
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
	DWORD tDeathTime;				//0x194
	DWORD dwLastForumGold;			//0x198
	DWORD dwLastGoldTick;			//0x19C
};

struct ObjectData
{
  ObjectTxt *pObjectTxt;			//0x00
  BYTE InteractType;				//0x04 Also Level id of portal dest
  BYTE _1[3];						//0x08
  DWORD _2[8];							
  char szOwner[16];
};

struct QuestRecord                     //sizeof 0x8
{
	Quest*    pQuestData;			   //+00
	QuestRecord*   pNext;              //+04
};

struct UnitAny
{
	DWORD dwType;					//0x00
	DWORD dwClassId;				//0x04
	D2PoolManager* pMemPool;		//0x08
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
	QuestRecord *pQuestRecord;		//0x74
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
	DWORD dwUnkType;				//0x9C <- another instace of unit id & type, timer related
	DWORD dwUnkId;					//0xA0 < ^- mby last attacker (last trace)
	DWORD _8;						//0xA4
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
	UnitAny*  pRoomNext;			//0xE4 E8 is RoomNext!
	UnitAny*  pListNext;			//0xE8
	UnitMsg* pMsgFirst;				//0xEC
	UnitMsg* pMsgLast;				//0xF0
};

struct D2Seed
{
	DWORD nLowSeed;
	DWORD nHiSeed;
};


struct Game
{
	DWORD _1[3];						//0x00
	DWORD * ptGameData8;				//0x0C
	Game * pNext;						//0x10
	DWORD _1a;							//0x14
	CRITICAL_SECTION* ptLock;			//0x18
	D2PoolManager * pMemPool;			//0x1C - not used, always NULL
	void * GameData;					//0x20
	DWORD _2;							//0x24
	WORD  nServerToken;					//0x28 called 'Server Ticket' aswell
	char GameName[16];					//0x2A
	char GamePass[16];					//0x3A
	char GameDesc[32];					//0x4A
	BYTE GameType;						//0x6A - whenever this is single player (etc)
	BYTE bArenaLvlId;					//0x6B
	BYTE _3;							//0x6C
	BYTE DifficultyLevel;				//0x6D
	BYTE _4[2];							//0x6E
	DWORD bExpansion;					//0x70
	DWORD ldGameType;					//0x74
	WORD  ItemFormat;					//0x78
	WORD  _5;							//0x7A
	DWORD InitSeed;						//0x7C
	DWORD ObjSeed;						//0x80 - seed used for object spawning
	DWORD InitSeed2;					//0x84 (another instance, dunno why)
	ClientData* pClientList;			//0x88 - (pClient structure of last player that entered the game)
	DWORD nClients;						//0x8C
	DWORD nUnits[6];					//0x90 - array of 6 counters, one for each unit type, this is the next GUID used too
	DWORD GameFrame;					//0xA8 - the current frame of the game, used for timers (buff duration etc)
	DWORD _6[3];						//0xAC
	TimerQuene* pTimerQueue;			//0xB8 a queue of all current active and inactive timers
	Act* pDrlgAct[5];					//0xBC
	union {
		DWORD GameSeed[2];				//0xD0
		__int64 GameSeed64;				//0xD0
		D2Seed hSeed;					//0xD0
	};
	InactiveRoom* pDrlgRoomList[5];		//0xD8
	DWORD MonSeed;						//0xEC - seed used for monster spawning
	MonsterRegion* pMonsterRegion[1024];//0xF0 - one pointer for each of the 1024 possible levels
	ObjectRegion* pObjectRegion;		//0x10F0 - a controller holding all object region structs
	QuestControl* pQuestControl;		//0x10F4 - a controller holding all quest info
	UnitNode* pOldNodes[10];			//0x10F8 - ten lists of unit node lists, this is used by the AI target seeking code (and other stuff)
	UnitAny* pUnitList[5][128];			//0x1120 - 5 lists of 128 lists of units (see pUnit documentation), second index is GUID & 127, BEWARE: since ever, missiles are array #4 and items are array #3 (so type3=index4 and type4=index3)
	DWORD* pTileList;					//0x1B20 - a list for all VisTile units
	DWORD UniqueFlags[128];				//0x1B24 - 128 DWORDS worth of flags that control if a unique item got spawned [room for 4096]
	NpcControl *pNpcControl;			//0x1D24 - a controller holding all npc info (like store inventories, merc list)
	Arena *pArenaControl;				//0x1D28 - a controller for arena stuff, functional and also used in game init
	Party *pPartyControl;				//0x1D2C - a controller for all party related stuff
	BYTE BossFlags[64];					//0x1D30 - 64 bytes for handling 512 super unique monsters (if they were spawned etc)
	DWORD MonModeData[17];				//0x1D70 - related to monsters changing mode
	DWORD nMonModeData;					//0x1DB4 - counter related to the above
	DWORD _7;							//0x1DB8
	DWORD nCreateTick;					//0x1DBC
	DWORD _8;							//0x1DC0
	DWORD nSyncTimer;					//0x1DC4 - used to sync events
	DWORD _9[8];						//0x1DC8
	BOOL bKilledBaal;					//0x1DE8 - killed uber baal
	BOOL bKilledDia;					//0x1DEC - killed uber diablo
	BOOL bKilledMeph;					//0x1DF0 - killed uber mephisto
	// --- Additions to original struct ---
	LRosterData *pLRosterData;			//0x1DF4
	DWORD dwKillCount;					//0x1DF8 
	DWORD dwGameState;					//0x1DFC 
	UnitNode* pNewNodes[130];			//0x1E00
	BOOL bSpawnedClone;					//0x2008
	BOOL bFestivalMode;					//0x200C
	int nPlaying;						//0x2010
	int DmgRekord;						//0x2014
	DWORD LastKiller;					//0x2018
	DWORD LastVictim;					//0x201C
	char szRekordHolder[16];			//0x2020
	LSpectator *pLSpectator;			//0x2030
	struct {			
		BOOL bOpenedDurielPortal : 1;		
		BOOL bOpenedLilithPortal : 1;
		BOOL bOpenedIzualPortal : 1;
		BOOL bOpenedTristramPortal : 1;
		BOOL bSpawnedDuriel : 1;
		BOOL bSpawnedLilith : 1;
		BOOL bSpawnedIzual : 1;	
		BOOL bSpawnedDiablo : 1;
		BOOL bSpawnedMephisto : 1;
		BOOL bSpawnedBaal : 1;
	} bUberQuestFlags;					//0x2034
};


#pragma pack(push, 1)
struct ExEvent //(size 0x4)
{
	BYTE P_A6;		//0x00
	BYTE MsgType;	//0x01 ExEventMsgs
	WORD PacketLen;	//0x02
};

struct ExEventTextMsg : ExEvent  //(size 0xD+strlen+1)
{
	BYTE Argument;	//0x04
	BYTE Color;		//0x05
	WORD wX;		//0x06
	WORD wY;		//0x08
	WORD Sound;		//0x0A
	char szMsg[255];//0x0E
};

struct ExEventOverhead : ExEvent  //(size 0xD+strlen+1)
{
	BYTE Argument;			//0x04
	BYTE Color;				//0x05
	DWORD UnitId;			//0x06
	WORD CellID;			//0x0A
	char szCellPath[255];	//0x0E
};

struct ExEventDownload : ExEvent  //(size 0xD+strlen+1)
{
	BYTE bExec;		//0x04
	BYTE _1;		//0x05 <- left for backward comp.
	WORD _2;		//0x06
	WORD _3;		//0x08
	WORD _4;		//0x0A
	char szURL[255];//0x0E
};

struct ExEventReveal : ExEvent  //(size 0x5)
{
	BYTE nLevel;	//0x04
};

struct ExEventGameOptions : ExEvent // size 0x9
{
	BYTE bOption;	//0x04 <- Option type check ExEventOption
	DWORD nValue;	//0x05
};

struct ExEventSpecatorStart : ExEvent // (size 0x8)
{
	DWORD UnitId;		//0x04
};

struct ExEventSpecatorEnd : ExEvent // (size 0x4)
{
};

/*
Interact with a unit
*/
struct px13
{
	BYTE P_13;			//0x00
	DWORD dwUnitType;	//0x01
	DWORD dwUnitId;		//0x05
};

/*
Put an item into holder
*/
struct px44
{
	BYTE P_44;			//0x00
	DWORD dwHolderType;	//0x01
	DWORD dwHolderId;	//0x05
	DWORD dwItemId;		//0x09
	DWORD dwObjectState;//0x0D 2 -> Cancel, 3-> Accept
};

/*
Request players chat relation change
*/
struct px5d
{
	BYTE P_5D;		//0x00
	BYTE nButton;	//0x01
	BYTE nState;	//0x02
	DWORD dwUnitId; //0x03
};

/*
Request party relation change
*/
struct px5e
{
	BYTE P_5E;		//0x00
	BYTE nButton;	//0x01
	DWORD dwUnitId; //0x02
};

struct EventPacket // (1.13d)
{
	BYTE P_5A;		//0x00
	BYTE MsgType;	//0x01
	BYTE Color;		//0x02
	DWORD Param1;   //0x03 
	BYTE Param2;	//0x07
	char Name1[16]; //0x08
	char Name2[16]; //0x18
};

struct SendDWORDStatPacket  //0x1F (1.13d)
{
	BYTE Header;	//0x00
	BYTE Attrib;	//0x01
	DWORD Amount;	//0x02
};

struct PostTradePacket //0x92 (1.13d)
{
	BYTE Header;
	BYTE UnitType;
	DWORD UnitId;
};

struct GoldPacket	//0x2C, size = 18 (1.13d)
{
	BYTE Header;
	char szPass[16];
};

struct TradeGold	//0x79, size = 0x06 (1.13d)
{
	BYTE Header;
	BYTE bMyUnit;
	DWORD nGold;
};

struct TradeData // 0x78, size = 0x15 (21) (1.13d)
{
	BYTE Header;
	char szName[16];
	DWORD UnitId;
};

struct TradePacket // 0x7 (1.13d)
{
	BYTE Header;
	WORD ButtonId;
	WORD Arg1;
	WORD Arg2;
};

struct SkillPacket // (1.13d)
{
	BYTE Header;
	WORD xPos;
	WORD yPos;
};

struct SkillTargetPacket // (1.13d)
{
	BYTE Header;
	DWORD UnitType;
	DWORD UnitId;
};


struct RosterPacket //0x066, size = 0x07  (1.13d)
{
	BYTE Header;
	DWORD UnitId;
	BYTE EventType;
	BYTE EventCount;
};

struct ReassignPacket // (1.13d)
{
	BYTE Header;
	BYTE UnitType;
	DWORD UnitId;
	WORD xPos;
	WORD yPos;
	BYTE Reassign;
};

#pragma pack(pop)


#endif