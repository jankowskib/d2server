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
struct Timer;


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

struct srCreateMonster
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

struct PartyRelation //size 0x14
{
	DWORD dwUnitId;					//0x00
	DWORD dwFlags;					//0x04
	DWORD _2;						//0x08
	DWORD wPartyId;					//0x0C
	PartyRelation *pNext;			//0x10
};

struct PlayerData { //size 0x16C same as 1.11b (1.13d) -- probably --
	char szName[16];				//0x00
	QuestFlags *QuestsFlags[3];		//0x10
	Waypoint *pNormalWaypoint[3];	//0x1C
	DWORD _1;						//0x28
	DWORD _2[2];					//0x2C
	WORD* KillCounter;				//0x34 Monster
	PartyRelation* pPartyRelation;	//0x38
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
	DWORD tDeathTime;				//0x194
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
	UnitAny*  pRoomNext;			//0xE4
	UnitAny*  pListNext;			//0xE8
	UnitMsg* pMsgFirst;				//0xEC
	UnitMsg* pMsgLast;				//0xF0
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