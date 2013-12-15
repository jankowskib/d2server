/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2008 marsgod
 *			 2013 lolet
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


#include "stdafx.h"

#include "D2Warden.h"
#include "D2Handlers.h"
#include "StatDumpThread.h"

#include "NodesEx.h"
#include "PartyExp.h"
#include "ExMemory.h"
#include "LQuests.h"
#include "LItems.h"
#include "LWorldEvent.h"
#include "LSpectator.h"

#include "global.h"
#include "MD5.h"
#include "process.h"

#include "Build.h"

#pragma comment(lib, "urlmon.lib")

static boost::mt19937 rng;
static boost::uniform_int<int> dist(300, 10000);

void RemoveWardenPacket(list<WardenClient>::iterator ptCurrentClient)
{
//	LOCK
		if(ptCurrentClient->pWardenPackets)
		{  
		if(ptCurrentClient->pWardenPackets->ThePacket) delete[] ptCurrentClient->pWardenPackets->ThePacket;
		delete ptCurrentClient->pWardenPackets;
		ptCurrentClient->pWardenPackets = 0;
		ptCurrentClient->pWardenPackets_ReceiveTime = 0;
		ptCurrentClient->pWardenPacket_SendTime = 0;
		}
//	UNLOCK

};


DWORD SendPartOfMOD2Client(DWORD ClientID,unsigned char *RC4_KEY,DWORD MOD_Position)
{
	unsigned char MyMod[0x200];
	int ModLen;
	
	if (MOD_Length - 0x1f4 > MOD_Position) ModLen = 0x1f4;
	else
		ModLen = MOD_Length - MOD_Position;
	MyMod[0] = 0xAE;
	MyMod[1] = (unsigned char)((ModLen+3)%256);
	MyMod[2] = (unsigned char)((ModLen+3)>>8);
	MyMod[3] = 1;
	MyMod[4] = (unsigned char)(ModLen%256);
	MyMod[5] = (unsigned char)(ModLen>>8);
	memcpy(&MyMod[6],MOD_Enrypted+MOD_Position,ModLen);
	rc4_crypt(RC4_KEY,&MyMod[3],ModLen+3);
	D2Funcs::D2NET_SendPacket(1,ClientID,MyMod,ModLen+6);
	//D2Funcs::D2GAME_SendPacket(ptClient,MyMod,ModLen+6);
	return ModLen+MOD_Position;
};

//void Warden_DownloadIni()
//{
//	if(URLDownloadToFile(0,"http://realm.angrenost.org//txt//Colors.ini","Colors.ini",0,0) == S_OK)
//	{
//#ifdef _ENGLISH_LOGS
//	Log("Downloading configuration file...");
//#else
//	Log("Pobieram plik konfiguracyjny!");
//#endif
//	}
//	else
//	{
//#ifdef _ENGLISH_LOGS
//	Log("An error occured during configuration file download");
//#else
//	Log("Wystapil blad podczas pobierania pliku konfiguracyjnego");
//#endif
//	}
//}

void Warden_Config()
{
	char *tk =0, *rt =0;
	char temp[100] = {0};
	char filename[MAX_PATH] = {0};
	char URL[255] = {0};
	char ITEM[255] = {0};
	GetCurrentDirectory(MAX_PATH,filename);
	ConfigFile.assign(filename);
	ConfigFile+="\\D2Warden.ini";

	GetPrivateProfileString("Warden","AllowTeleport","1,5",temp,100,ConfigFile.c_str());
	for(rt = strtok_s(temp,", ",&tk);rt;rt = strtok_s(NULL,", ",&tk))
	{
	if(atoi(rt)<7) TeleChars[atoi(rt)]=TRUE;
	}
	AllowTourMode = GetPrivateProfileInt("Warden","AllowTourMode",1,ConfigFile.c_str());
	AllowVanilla = GetPrivateProfileInt("Warden","AllowVanilla",0,ConfigFile.c_str());
	AllowGU = GetPrivateProfileInt("Warden","AllowGU",0,ConfigFile.c_str());
	AllowHB = GetPrivateProfileInt("Warden","AllowHB",1,ConfigFile.c_str());
	AllowNLWW = GetPrivateProfileInt("Warden","AllowNLWW",0,ConfigFile.c_str());
	AllowLoggin = GetPrivateProfileInt("Warden","LogChat",1,ConfigFile.c_str());
	DumpInterval = GetPrivateProfileInt("Warden","ResDumpInterval",0,ConfigFile.c_str());
	Max_Players = GetPrivateProfileInt("Warden","MaxPlayers",8,ConfigFile.c_str());
	MoveToTown = GetPrivateProfileInt("Warden","MoveToTown",1,ConfigFile.c_str());
	DropEarGold = GetPrivateProfileInt("Warden","DontDropEarGold",1,ConfigFile.c_str());
	StrBugFix = GetPrivateProfileInt("Warden","StrBugFix",1,ConfigFile.c_str());
	FFAMode= GetPrivateProfileInt("Warden","FFAMode",1,ConfigFile.c_str());
	EnableSeed= GetPrivateProfileInt("Warden","EnableSeedChange",1,ConfigFile.c_str());
	AllowD2Ex = GetPrivateProfileInt("Warden","AllowD2Ex",1,ConfigFile.c_str());
	DetectTrick = GetPrivateProfileInt("Warden","DetectTrick",1,ConfigFile.c_str());
	ExpRange = GetPrivateProfileInt("Warden","ExpRange",6400,ConfigFile.c_str());
	//DmgRekord = GetPrivateProfileInt("Warden","DmgRekord",1,ConfigFile.c_str());
	GetPrivateProfileString("Warden","ClanDataURL","http://www.lolet.yoyo.pl/Clans.ini",URL,255,ConfigFile.c_str());
	ClansURL = URL;
	GetPrivateProfileString("Warden","UpdateURL","",URL,255,ConfigFile.c_str());
	UpdateURL = URL;
	//GetPrivateProfileString("Warden","DmgOwner","N/A",URL,255,ConfigFile.c_str());
	//DmgOwner = URL;
	GetPrivateProfileString("Warden","GSName","N/A",URL,255,ConfigFile.c_str());
	GSName = URL;
	GetPrivateProfileString("Warden","Admins","LOLET",URL, 255, ConfigFile.c_str());
	tk = 0;
	for(rt = strtok_s(URL,", ",&tk); rt; rt = strtok_s(NULL,", ",&tk))
	{
	boost::to_lower(rt);
	if(rt[0]) Admins.push_back(rt);
	}

	//World Event Stuff
	EnableWE = GetPrivateProfileInt("World Event","Enabled",0,ConfigFile.c_str());


	if(EnableWE)
	{
		SellCount = GetPrivateProfileInt("World Event","SellCount",0,ConfigFile.c_str());
		NextDC =  GetPrivateProfileInt("World Event","NextDC",0,ConfigFile.c_str());
		MinSell =  GetPrivateProfileInt("World Event","MinSell",10,ConfigFile.c_str());
		MaxSell =  GetPrivateProfileInt("World Event","MaxSell",50,ConfigFile.c_str());
		InfoDelay =  GetPrivateProfileInt("World Event","InfoDelay",10,ConfigFile.c_str());
		GetPrivateProfileString("World Event","Trigger","rin, unique, 122",ITEM,255,ConfigFile.c_str());

		string sItem = ITEM;
		boost::to_lower(sItem);
	
		typedef boost::tokenizer<boost::char_separator<char>>   tokenizer;
		boost::char_separator<char> sep(", ");
		boost::char_separator<char> subsep(": ");
		tokenizer tokens(sItem, sep);
		memset(&WItem.ItemCode,0,20*4);
		memset(&WItem.FileIdx[0], -1,20*4);
		memset(&WItem.ItemQuality[0], -1,7);

		int ntk =  1;
		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); tok_iter++,ntk++)
		{
		switch(ntk)
		{
		case 1:
			{
			tokenizer subtok(*tok_iter, subsep);
			int stk = 0;
			for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++,stk++)
			{
			WItem.ItemCode[stk] = TransCode((*iter).c_str());
			}
			}
		break;
		case 2:
			{
			tokenizer subtok(*tok_iter, subsep);
			int stk = 0;
			for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++,stk++)
			WItem.ItemQuality[stk] = TransQuality((*iter).c_str());
			}
		break;
		case 3:
			{
			tokenizer subtok(*tok_iter, subsep);
			int stk = 0;
			for (tokenizer::iterator iter = subtok.begin(); iter != subtok.end(); iter++,stk++)
			WItem.FileIdx[stk] = atoi((*iter).c_str());
			}
		break;
		}
		}

		if(!NextDC) WE_GenerateNextDC();

	}
//	Warden_DownloadIni();
#ifdef _ENGLISH_LOGS
	Log("Settings loaded.");
#else
	Log("Ustawienia zaladowane.");
#endif
}

void Warden_Init()
{
char Warden_MOD[256] = "3ea42f5ac80f0d2deb35d99b4e9a780b.mod";
unsigned char RC4_Key[17]="WardenBy_Marsgod"; //3ea42f5ac80f0d2deb35d99b4e9a780b98ff
char Maiev[10] = "MAIEV.MOD";
FILE * fp = 0;
int i = 0;

#define JUMP 0xE9
#define CALL 0xE8
#define NOP 0x90
/*
UNUSED
	PatchGS(CALL,(DWORD)D2Ptrs::D2GAME_LifeLeech_P,(DWORD)D2Stubs::D2GAME_OnLifeLeech_STUB,6,"Life Leech");
	PatchGS(CALL,(DWORD)D2Stubs::D2GAME_ManaLeech_P,(DWORD)D2Ptrs::D2GAME_OnManaLeech_STUB,6,"Mana Leech");
	
	PatchGS(NOP,(DWORD)D2Ptrs::D2GAME_Corpse_P,0,10,"Disable Corpse");
	PatchGS(CALL,(DWORD)D2Ptrs::D2GAME_GetUnitX_P,(DWORD)D2Stubs::D2COMMON_GetUnitX,5,"TargetX fix");
	PatchGS(CALL,(DWORD)D2Ptrs::D2GAME_GetUnitY_P,(DWORD)D2Stubs::D2COMMON_GetUnitY,5,"TargetY fix");
*/

#ifdef _ENGLISH_LOGS
	Log("Warden ver. 1.6a build %d by Lolet has started. Compiled on %s, %s",__BUILDNO__,__DATE__,__TIME__);
#else
	Log("Warden ver. 1.6a build %d by Lolet rozpoczal dzialanie. Skompilowano %s o %s",__BUILDNO__,__DATE__,__TIME__);
#endif
	Warden_Config();
//        HDR    ADDR                         FUNC                          SIZE    DESC
	//PatchGS(0,(DWORD)&D2Vars::D2GAME_PacketTable[0x66].Callback,(DWORD)d2warden_0X66Handler,4,"On Warden Packet");
	//PatchGS(JUMP,GetDllOffset("D2Game.dll",0x703D0),(DWORD)D2Stubs::D2GAME_OnJoinGame_STUB,5,"Join Game Handler");
	//PatchGS(CALL,GetDllOffset("D2Game.dll",0x7A6CE),(DWORD)D2Stubs::D2GAME_SetDRCap_STUB,5,"DR Cap");
//#ifdef _DEBUG
//	PatchGS(0x68,GetDllOffset("D2Client.dll",0x5E7CD),1,5,"TCP/IP Delay fix");
//	PatchGS(CALL,GetDllOffset("D2Game.dll",0xE832E),(DWORD)D2Stubs::D2GAME_ParseCreatePackets_STUB,5,"ParseCreatePackets");
//	PatchGS(0,GetDllOffset("D2Client.dll",0xBE919),0xEB,1,"AE Packet Fixture");
//#endif
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x7D66D),(DWORD)D2Stubs::D2GAME_LastHitIntercept_STUB,5,"LastHitIntercept");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x99130),(DWORD)D2Stubs::D2GAME_OnCreateDamage_STUB,6,"OnDamageCreation");
	PatchGS(CALL,GetDllOffset("D2Net.dll",0x62AD),(DWORD)D2Stubs::D2NET_ReceivePacket_STUB,5,"Client Packet Wrapper");
	if(AllowD2Ex) {
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2C1E0),(DWORD)OnD2ExPacket,5,"D2Ex Connect");
	D2Vars::D2NET_ToSrvPacketSizes[0x0B]=5;
	} 
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x617B5),(DWORD)OnActLoad,5,"Act Load");
	if(MoveToTown)
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x42DAB),(DWORD)OnCreateCorpse,5,"Move Player Corpse To Town");

	PatchGS(0x90,GetDllOffset("D2Game.dll",0xF2F67),0,19,"Hostile WP Delay");
	PatchGS(0x33,GetDllOffset("D2Game.dll",0xF2F2D),0xF6,2,"Hostile Button Delay"); //0x33F6 xor esi, esi

	if(DropEarGold) {
	PatchGS(0x90,GetDllOffset("D2Game.dll",0x7F560),0,7,"Gold drop on death");
	PatchGS(0xEB,GetDllOffset("D2Game.dll",0xD26C3),0,1,"Ear drop on death");
	}

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xBEF80),(DWORD)D2Stubs::D2GAME_FindUnit_STUB,9,"FindUnit Fix");

	//PatchGS(JUMP,GetDllOffset("D2Game.dll",0x41972),(DWORD)D2Stubs::D2GAME_OnPlayerModeChange_I,5,"Player mode intercept");

	if(StrBugFix)
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x2D34F),(DWORD)D2Stubs::D2GAME_SendStatToOther_STUB,5,"Str/dex bug fix");

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2E670),(DWORD)D2Stubs::D2GAME_Chat_STUB,6,"Chat Wrapper");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x90171),(DWORD)D2Stubs::D2GAME_NPCHeal_STUB,61,"NPC Slow Missile Heal");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x32048),(DWORD)D2Stubs::D2GAME_Ressurect_STUB,39,"Ressurect Fix");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x43067),(DWORD)D2Stubs::D2GAME_DeathMsg_STUB,5,"Roster Wrapper");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x7E326),(DWORD)D2Stubs::D2GAME_OnMonsterDeath_STUB,9,"On Monster Death");

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x89680),(DWORD)D2Stubs::D2GAME_SendKillsNULL_STUB,6,"Null Original S->C 0x66");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0xE7A1A),(DWORD)D2Stubs::D2GAME_GameDestroy_STUB,5,"Game Destroy Wrapper");

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xC6A32),(DWORD)D2Stubs::D2GAME_GameEnter_STUB,6, "Add ClientToGame Wrapper"); //0ld = 0xE76D6, The newest = 0xE76E4
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2DC90),(DWORD)D2Stubs::D2GAME_OnEventSend_STUB,5, "Player Events Wrapper");
	

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2BE20),(DWORD)d2warden_0X66Handler,5,"0x66 Warden Packet Wrapper");
	//Pakiety skilli
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F230),(DWORD)OnClickLoc,8,"0x05 Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F1B0),(DWORD)OnClickLoc,8,"0x0C Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F580),(DWORD)OnClickLoc,5,"0x08 Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F420),(DWORD)OnClickLoc,5,"0x0F Wrapper");
	//--------------
	//Pakiety skill on Unit 0x06*, 0x07, 0x09*, 0x0A,  || 0x0D*, 0x0E, 0x10*, 0x11
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F660),(DWORD)OnClickUnit,6,"0x06 Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F5E0),(DWORD)OnClickUnit,6,"0x07 Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F910),(DWORD)OnClickUnit,6,"0x09 Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F8B0),(DWORD)OnClickUnit,6,"0x0A Wrapper");

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F500),(DWORD)OnClickUnit,6,"0x0D Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F480),(DWORD)OnClickUnit,6,"0x0E Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F850),(DWORD)OnClickUnit,6,"0x10 Wrapper");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2F7F0),(DWORD)OnClickUnit,6,"0x11 Wrapper");
	//-----------

	//On Run to Location

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2EEE0),(DWORD)OnRunToLocation,6,"0x03 Wrapper");

	//PatchGS(JUMP,GetDllOffset("D2Game.dll",0xE6F00),(DWORD)D2Stubs::D2GAME_LogHack_STUB,5,"Lock Hack Wrapper");

	if(FFAMode)
	{
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x909EF),(DWORD)D2Stubs::D2GAME_PermStore_STUB,5, "Perm Store Stub");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x928FC),(DWORD)D2Stubs::D2GAME_PermStore_STUB,5, "Perm Store Stub");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x913CA),(DWORD)GetItemCost,5,"Item Cost Stub");
	}

	PatchGS(0x05,GetDllOffset("D2Client.dll",0x2589),sizeof(Game),5,"Extend Game Struct"); //6FAB2589  |.  05 F41D0000   ADD EAX,1DF4

	PatchGS(0x00,GetDllOffset("D2Common.dll",0x5BFC9),sizeof(PlayerData),4,"Extend PlayerData Struct");
	PatchGS(0x00,GetDllOffset("D2Common.dll",0x5BFDA),(sizeof(PlayerData)/4),4,"Extend PlayerData Struct");

	//EXP FIX
	PatchGS(0,GetDllOffset("D2Game.dll",0x7E248),0x5690,2,"Party Share Fix Prepare");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x7E24A),(DWORD)ExpShare_NEW,5, "Party Share Fix");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0xBD5B7),GetDllOffset("D2Game.dll",0xBD310),5,"Exp Increase Fix");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xBD310),(DWORD)nPlayersFormula,5,"Exp Increase Fix 2");

	//PatchGS(JUMP,GetDllOffset("D2Game.dll",0x7EBF0),(DWORD)D2Stubs::D2GAME_GetPlayers_STUB,5,"Max Players Change");

	//MISC FIXES
	PatchGS(0,GetDllOffset("D2Game.dll",0x424E6),7,1,"Decresae Corpse number");  //NEW
	PatchGS(0,GetDllOffset("D2Game.dll",0xFBF5C + 8),0,1,"Null Iron Maiden");
	PatchGS(0,GetDllOffset("D2Game.dll",0xFBF5C + 20),0,1,"Null Lower Resist");
	PatchGS(0,GetDllOffset("D2Game.dll",0xD156A),0xEB,1,"Cows' drop fix");
	PatchGS(CALL,GetDllOffset("D2Game.dll",0x368C2),(DWORD)QUESTS_CowLevelOpenPortal,5,"Cow Level Quest Override");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x4BC83),(DWORD)D2Stubs::D2GAME_OnUseItem_STUB,5,"Respec");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEBD50),(DWORD)DAMAGE_FireEnchanted,6,"Fire Enchanted Fix");
	PatchGS(CALL,GetDllOffset("D2Common.dll",0x41B6D),(DWORD)DYES_DrawItem,5,"Colour Item Intercept IV");
	PatchGS(CALL,GetDllOffset("D2Common.dll",0x418DC),(DWORD)DYES_DrawItem,5,"Colour Item Intercept V");
	D2Vars::D2GAME_pSpell[12].SpellPrepareFunc=&DYES_Prepare;
	D2Vars::D2GAME_pSpell[12].SpellDoFunc=&DYES_Colorize;
	D2Vars::D2GAME_pSpell[13].SpellDoFunc=&WE_Spawn;

	//PatchGS(CALL,GetDllOffset("D2Game.dll",0xD159B),(DWORD)ITEMS_AddKillerId,5,"Tresure Class Create Hndlr");
	

//	PatchGS(JUMP,GetDllOffset("Fog.dll",0x1D380),(DWORD)ExMemory::Alloc_STUB,5,"Mem Alloc Override");
//	PatchGS(JUMP,GetDllOffset("Fog.dll",0x1CEE0),(DWORD)ExMemory::Free_STUB,5,"Mem Free Override");
//	PatchGS(JUMP,GetDllOffset("Fog.dll",0x1DBF0),(DWORD)ExMemory::Realloc,5,"Mem Realloc Override");

//Zmieniam dozwolona liczbê graczy
#ifdef _ENGLISH_LOGS
Log("Setting allowed player limit on %d", Max_Players);
#else
Log("Ustalam dozwolona liczbe graczy w grze na %d", Max_Players);
#endif
int NEU_NODE = Max_Players + 3;
//NODES
#ifdef _ENGLISH_LOGS
	Log("Reconfiguring nodes...");
#else
	Log("Przeprogramowuje Nodes'y...");
#endif
	PatchGS(0x90,GetDllOffset("D2Game.dll",0x2341D),0x90909090,9,"NodesEX: Ai Temp Fix");
	
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x2A790),(DWORD)NODES_BaalCheck,7,"NodesEX: Baal Ai"); // bylo 0x2BB75
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0x80AA0),(DWORD)NODES_NormalCheck,7,"NodesEX: Diablo Ai");

	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEDF00),(DWORD)D2Stubs::D2GAME_AssignNode_STUB,6,"NodesEX: Alloc 16-players nodes");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEDD70),(DWORD)D2Stubs::D2GAME_FreeNode_STUB,6,"NodesEX: Free 16-players nodes");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEDC90),(DWORD)D2Stubs::D2GAME_FreeChildNode_STUB,8,"NodesEX: Free Child of 16-players nodes");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEDC50),(DWORD)NODES_GetUnitNode,6,"NodesEX: Get Unit Node");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEDDE0),(DWORD)D2Stubs::D2GAME_SetUnitsNode_STUB,6,"NodesEX: Set Unit Node As Parent (Monsters)");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEDE70),(DWORD)D2Stubs::D2GAME_SetNodeParented_STUB,6,"NodesEX: Set Unit Node As Parent (Players)");
	PatchGS(JUMP,GetDllOffset("D2Game.dll",0xEDFC0),(DWORD)D2Stubs::D2GAME_FreeNodes_STUB,5,"NodesEX: Free all nodes");

	PatchGS(0,GetDllOffset("D2Game.dll",0xC2BCB),0x1E00,4,"NodesEX: Change node offset I");
	PatchGS(0,GetDllOffset("D2Game.dll",0xC2BD1),0x1E00,4,"NodesEX: Change node offset II");
	PatchGS(0,GetDllOffset("D2Game.dll",0xD2E05),0x1E00,4,"NodesEX: Change node offset III");

//Basic fixes
	PatchGS(0,GetDllOffset("D2Game.dll",0x70285),Max_Players,1,"GameEnter: Check number of players");
	PatchGS(0,GetDllOffset("D2Game.dll",0xC5273),Max_Players,1,"GameEnter: Check number of players II");
#ifndef _SINGLEPLAYER
	PatchGS(0,GetDllOffset("D2GS.exe",0x240C),Max_Players,1,"GameEnter: Check number of players III");
	PatchGS(0,GetDllOffset("D2GS.exe",0x4FD4),Max_Players,1,"GameEnter: Check number of players IV");
#endif

	//TREASURE Class
	PatchGS(0,GetDllOffset("D2Game.dll",0xD20AB),Max_Players,1,"NodesEX: Fix Treasure Class Drop"); 
	PatchGS(0,GetDllOffset("D2Game.dll",0xD20B1),Max_Players,1,"NodesEX: Fix Treasure Class Drop II"); 
	
	
	
	PatchGS(0,GetDllOffset("D2Game.dll",0xC0B05),NEU_NODE,4,"NodesEX: Create UnitAny: Change neutral Node");

	//UNIT_Player

	PatchGS(0,GetDllOffset("D2Game.dll",0x266B4),NEU_NODE,4,"NodesEX: Change neutral Node: CreatePlayer I -f1 ");
	PatchGS(0,GetDllOffset("D2Game.dll",0x26F19),NEU_NODE,4,"NodesEX: Change neutral Node: CreatePlayer II -f2");
	PatchGS(0,GetDllOffset("D2Game.dll",0x2936F),NEU_NODE,4,"NodesEX: Change neutral Node: CreatePlayer III -f3");
	PatchGS(0,GetDllOffset("D2Game.dll",0x29B69),NEU_NODE,4,"NodesEX: Change neutral Node CreatePlayer IV -f4");
	PatchGS(0,GetDllOffset("D2Game.dll",0x425DD),NEU_NODE,4,"NodesEX: Change neutral Node CreateCorpse I");
	PatchGS(0,GetDllOffset("D2Game.dll",0x8327B),NEU_NODE,1,"NodesEX: Change neutral Node Player Event Controller");
	
	
	//UNIT_Object 

	//CreateObject Patches
	//po chuj blizzowi tyle funkcji do tworzenia obiektow...
	PatchGS(0,GetDllOffset("D2Game.dll",0x13FD3),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject I");
	PatchGS(0,GetDllOffset("D2Game.dll",0x14193),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject II");
	PatchGS(0,GetDllOffset("D2Game.dll",0x143F3),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject III - f2");
	PatchGS(0,GetDllOffset("D2Game.dll",0x1482B),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject IV - f3");
	PatchGS(0,GetDllOffset("D2Game.dll",0x14A9B),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject V - f3");
	PatchGS(0,GetDllOffset("D2Game.dll",0x14E44),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject VI - f4 - huge one");
	PatchGS(0,GetDllOffset("D2Game.dll",0x150E3),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject VII - f4 - huge one");
	PatchGS(0,GetDllOffset("D2Game.dll",0x1531C),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject VIII - f5 - small one");
	PatchGS(0,GetDllOffset("D2Game.dll",0x1571F),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject IX - f6");
	PatchGS(0,GetDllOffset("D2Game.dll",0x15AAF),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject X - moved one f6");
	PatchGS(0,GetDllOffset("D2Game.dll",0x15E37),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XI - f7");
	PatchGS(0,GetDllOffset("D2Game.dll",0x160F0),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XII - f8");
	PatchGS(0,GetDllOffset("D2Game.dll",0x163E8),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XIII - f9");
	PatchGS(0,GetDllOffset("D2Game.dll",0x16592),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XIV - f10");
	PatchGS(0,GetDllOffset("D2Game.dll",0x168A6),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XV - f11");
	PatchGS(0,GetDllOffset("D2Game.dll",0x16A38),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XVI - f12");
	PatchGS(0,GetDllOffset("D2Game.dll",0x1741A),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XVII - f13");
	PatchGS(0,GetDllOffset("D2Game.dll",0x31A7B),NEU_NODE,4,"NodesEX: Change neutral Node: CreatePortal I");
	PatchGS(0,GetDllOffset("D2Game.dll",0x539B7),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XVIII - f14");
	PatchGS(0,GetDllOffset("D2Game.dll",0x54443),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XIX - f15");
	PatchGS(0,GetDllOffset("D2Game.dll",0x548D2),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XX - f16");
	PatchGS(0,GetDllOffset("D2Game.dll",0xA806F),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XXI - f17");
	PatchGS(0,GetDllOffset("D2Game.dll",0xA81DF),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XXII - f17");
	PatchGS(0,GetDllOffset("D2Game.dll",0xAF348),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XXIII - f18");
	PatchGS(0,GetDllOffset("D2Game.dll",0xB03BF),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XXIV - f19");
	PatchGS(0,GetDllOffset("D2Game.dll",0xBA802),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XXV - f20");
	PatchGS(0,GetDllOffset("D2Game.dll",0xBA94D),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XXVI - f20");
	PatchGS(0,GetDllOffset("D2Game.dll",0xBABF8),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject XXVII - f21");
	
	//Teraz potwory

	PatchGS(0,GetDllOffset("D2Game.dll",0x22C78),Max_Players,1,"NodesEX: Change Players Nodes from 8"); 
	PatchGS(0,GetDllOffset("D2Game.dll",0x22C9C),NEU_NODE,1,"NodesEX: Change neutral Node"); 

	PatchGS(0,GetDllOffset("D2Game.dll",0x94056),NEU_NODE,4,"NodesEX: Change neutral Node: CreateMonster I - f1");
	
	PatchGS(0,GetDllOffset("D2Game.dll",0x23D0A), NEU_NODE-3, 1, "NodesEX: RemoveMonsterCorpse: Change iteration to 16");

	PatchGS(0,GetDllOffset("D2Game.dll",0xCC7DB), NEU_NODE,1,"NodesEX: Change neutral Node: Monsters Skill : Necro");
	PatchGS(0,GetDllOffset("D2Game.dll",0xCC836), NEU_NODE-2,4,"NodesEX: Change Monster Node to 17 same func as above");
	PatchGS(0,GetDllOffset("D2Game.dll",0xCC7EE), 0x1E00 + 4*(NEU_NODE-2),4,"NodesEX: Change offset to NewNodes[17]: same func as above");
	PatchGS(0,GetDllOffset("D2Game.dll",0xCC824), 0x1E00 + 4*(NEU_NODE-2),4,"NodesEX: Change offset to NewNodes[17]: same func as above");
	

	PatchGS(0,GetDllOffset("D2Game.dll",0x834D1), NEU_NODE,1,"NodesEX: Change neutral Node: Monsters");
	PatchGS(0,GetDllOffset("D2Game.dll",0x8353C), NEU_NODE-3,4,"NodesEX: Change iteration to 16: same func as above");
	PatchGS(0,GetDllOffset("D2Game.dll",0x834F4), 0x1E00 + 4*(NEU_NODE-3),4,"NodesEX: Change offset to NewNodes[16]: same func as above");
	PatchGS(0,GetDllOffset("D2Game.dll",0x8352A), 0x1E00 + 4*(NEU_NODE-3),4,"NodesEX: Change offset to NewNodes[16]: same func as above");
	
	PatchGS(0,GetDllOffset("D2Game.dll",0xC2D8A), NEU_NODE-3, 1, "NodesEX: Change iteration to 16");

	PatchGS(0,GetDllOffset("D2Game.dll",0x23417),NEU_NODE-3,4,"NodesEX: Change Monster Node to 16");
	PatchGS(0,GetDllOffset("D2Game.dll",0x2342E),NEU_NODE-2,4,"NodesEX: Change Monster Node to 17");

	//Missiles
	PatchGS(0,GetDllOffset("D2Game.dll",0x8E714),NEU_NODE,4,"NodesEX: Change neutral Node: CreateObject I");

	if (strlen(Warden_MOD) != 36)
	{
#ifdef _ENGLISH_LOGS
		Log("Incorrect name of module '%s' (len!=36)",Warden_MOD);
#else
		Log("Nieprawidlowa nawa modulu '%s' (len!=36)",Warden_MOD);
#endif
		Warden_Enable = false;
		return;
	}

	fopen_s(&fp,Warden_MOD,"rb");
	if (!fp)
	{
#ifdef _ENGLISH_LOGS
		Log("Cannot find module filename '%s'!",Warden_MOD);
#else
		Log("Nie moge znalesc pliku modulu '%s'!",Warden_MOD);
#endif
		Warden_Enable = false;
		return;
	}
	fseek(fp,0,SEEK_END);
	MOD_Length = ftell(fp);
	
	if (MOD_Length <= 260 || MOD_Length > 100000)
	{
#ifdef _ENGLISH_LOGS
		Log("Incorrect module size (%d)",MOD_Length);
#else
		Log("Nieprawidlowa wielkosc modulu (%d)",MOD_Length);
#endif
		Warden_Enable = false;
		fclose(fp);
		return;
	}
	
	MOD_Enrypted = new unsigned char[MOD_Length];
	if (MOD_Enrypted == NULL)
	{
#ifdef _ENGLISH_LOGS
		Log("No memory to allocate module!");
#else
		Log("Brak pamieci na zaincjowanie modulu!");
#endif
		Warden_Enable = false;
		fclose(fp);
		return;
	}
	
	fseek(fp,0,SEEK_SET);
	fread(MOD_Enrypted,1,MOD_Length,fp);
	fclose(fp);
	
	if (MOD_Enrypted[MOD_Length-260] != 'N' || MOD_Enrypted[MOD_Length-259] != 'G' || MOD_Enrypted[MOD_Length-258] != 'I' || MOD_Enrypted[MOD_Length-257] != 'S')
	{
#ifdef _ENGLISH_LOGS
		Log("Damaged module file!");
#else
		Log("Uszkodzony plik modulu!");
#endif
		Warden_Enable = false;
		delete[] MOD_Enrypted;
		return;
	}

	unsigned char s[258];
	rc4_setup(s,RC4_Key,16);
	rc4_crypt(s,MOD_Enrypted,MOD_Length);

	AE_Packet00[00] = 0xAE;
	AE_Packet00[01] = 0x25;
	AE_Packet00[02] = 0x00;
	AE_Packet00[03] = 0x00;
	
	unsigned char tt[2];
	
	for (i=0;i<16;i++)
	{
		tt[0] = Warden_MOD[2*i];
		tt[1] = Warden_MOD[2*i+1];
		AE_Packet00[4+i] = Convert2HEX(tt);
		AE_Packet00[20+i] = RC4_Key[i];
	}
		
	AE_Packet00[36] = (unsigned char)(MOD_Length % 256);
	AE_Packet00[37] = (unsigned char)((MOD_Length / 256) % 256);
	AE_Packet00[38] = (unsigned char)((MOD_Length / 65536) % 256);
	AE_Packet00[39] = (unsigned char)((MOD_Length / 16777216) % 256);
	
	rng.seed(static_cast<unsigned> (std::time(0)));

	Warden_Enable = true;

#ifdef _ENGLISH_LOGS
	Log("Module '%s' has loaded correctly!",Warden_MOD);
#else
	Log("Modul '%s' wczytany!",Warden_MOD);
#endif

	if(DumpInterval)
	{
	if(!DumpHandle)
	DumpHandle = (HANDLE)_beginthreadex(0,0,&StatThread,0,0,&StatID);
	if(DumpHandle)
#ifdef _ENGLISH_LOGS
	Log("Started stat dump, every %d second(s).",DumpInterval);
#else
	Log("Rozpoczeto zrzut statystyk, co %d sekund(y).",DumpInterval);
#endif
	}
	else
	{
//	if(DumpHandle) CloseHandle(DumpHandle); // To trzeba jeszcze dorobic
	}
	return;
}


void WardenLoop()
{
	unsigned char WardenCMD0_local[38] = {0};
	static boost::variate_generator<boost::mt19937&, boost::uniform_int<int>>random(rng, dist);

	LOCK //Zablokuj inne w¹tki

	for(list<WardenClient>::iterator ptCurrentClient = hWarden.Clients.begin(); ptCurrentClient !=hWarden.Clients.end();)
	{
	DWORD CurrentTick = GetTickCount();
	if (ptCurrentClient->NextCheckTime > CurrentTick && ptCurrentClient->WardenStatus != WARDEN_NOTHING) 
	{
		++ptCurrentClient;
		continue;
	}
	DEBUGMSG("Checking client %s", ptCurrentClient->AccountName.c_str());
	DWORD NetClient = D2Funcs::D2NET_GetClient(ptCurrentClient->ClientID);

		if(!NetClient)
		{
		DEBUGMSG("DEBUG: Usuwam klienta %d!, ilosc klientow w petli %d, Powód: %s",ptCurrentClient->ClientID,hWarden.Clients.size()-1,!NetClient ? "CheckValidClient == false" : ptCurrentClient->WardenStatus == WARDEN_NOTHING ? "WardenStatus == NOTHING" : "ErrorCount>5");
		RemoveWardenPacket(ptCurrentClient);
		ptCurrentClient = hWarden.Clients.erase(ptCurrentClient);
		continue;
		}

			switch (ptCurrentClient->WardenStatus)
			{
				case WARDEN_START:
					{
						memcpy(WardenCMD0_local,&AE_Packet00[3],37);
						rc4_crypt(ptCurrentClient->RC4_KEY_0XAE,&AE_Packet00[3],37);
						D2Funcs::D2NET_SendPacket(0,ptCurrentClient->ClientID,AE_Packet00,sizeof(AE_Packet00));
						memcpy(&AE_Packet00[3],WardenCMD0_local,37);	
						ptCurrentClient->WardenStatus = WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT;
						ptCurrentClient->NextCheckTime = CurrentTick+500;
						}
					break;
				case WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT:
					{
					if(ptCurrentClient->pWardenPackets)
					{
						if (ptCurrentClient->pWardenPackets->PacketLen != 1)
						{
							RemoveWardenPacket(ptCurrentClient);
							ptCurrentClient->WardenStatus = WARDEN_ERROR_RESPONSE;
							ptCurrentClient->NextCheckTime = CurrentTick + 100;
							break;
						}
						
						if (ptCurrentClient->pWardenPackets->ThePacket[0] == 0)
						{	
							RemoveWardenPacket(ptCurrentClient);
							ptCurrentClient->WardenStatus = WARDEN_DOWNLOAD_MOD;
							ptCurrentClient->NextCheckTime = CurrentTick;
							break;
						}
						else
						if (ptCurrentClient->pWardenPackets->ThePacket[0] == 1)
						{
							RemoveWardenPacket(ptCurrentClient);
							ptCurrentClient->WardenStatus = WARDEN_SEND_REQUEST;
							ptCurrentClient->NextCheckTime = CurrentTick;
							break;
						}
						else
						{
							RemoveWardenPacket(ptCurrentClient);
							ptCurrentClient->WardenStatus = WARDEN_ERROR_RESPONSE;
							ptCurrentClient->NextCheckTime = CurrentTick;
							break;
						}
					}
						else
						{
						if(GetTickCount()-ptCurrentClient->ClientLogonTime>20000) ptCurrentClient->WardenStatus=WARDEN_ERROR_RESPONSE; //Daje ci 20 sekund (Na cbn jest 45) 
						ptCurrentClient->NextCheckTime = CurrentTick + 500;
						}
					}
					break;
				case WARDEN_DOWNLOAD_MOD:
					{
						RemoveWardenPacket(ptCurrentClient);
						ptCurrentClient->MOD_Position = SendPartOfMOD2Client(ptCurrentClient->ClientID,ptCurrentClient->RC4_KEY_0XAE,ptCurrentClient->MOD_Position);
						if (ptCurrentClient->MOD_Position >= MOD_Length)
						{
							ptCurrentClient->WardenStatus = WARDEN_WAITING_DOWNLOAD_END;
							ptCurrentClient->NextCheckTime = CurrentTick;
						}
						else
						{
							ptCurrentClient->WardenStatus = WARDEN_DOWNLOAD_MOD;
							ptCurrentClient->NextCheckTime = CurrentTick+200;
						}
					}
					break;
				case WARDEN_WAITING_DOWNLOAD_END:
					{
							if (ptCurrentClient->pWardenPackets)
							{
								if (ptCurrentClient->pWardenPackets->PacketLen != 1)
								{
								ptCurrentClient->WardenStatus = WARDEN_ERROR_RESPONSE;
								ptCurrentClient->NextCheckTime = CurrentTick;
								RemoveWardenPacket(ptCurrentClient);
								break;
								}
								if (ptCurrentClient->pWardenPackets->ThePacket[0] == 0)
								{
								ptCurrentClient->ErrorCount++;
								ptCurrentClient->WardenStatus = WARDEN_CHECK_CLIENT_HAS_MOD_OR_NOT;
								ptCurrentClient->NextCheckTime = CurrentTick+50;
								RemoveWardenPacket(ptCurrentClient);
								break;
								}
								else
								if (ptCurrentClient->pWardenPackets->ThePacket[0] == 1)
								{
								ptCurrentClient->WardenStatus = WARDEN_SEND_REQUEST;
								ptCurrentClient->ErrorCount=0; // Nastepne 5 szans, jako ze udalo ci sie sciagnac modul
								ptCurrentClient->NextCheckTime = CurrentTick;
								RemoveWardenPacket(ptCurrentClient);
								break;
								}
								else
								{
								ptCurrentClient->WardenStatus = WARDEN_ERROR_RESPONSE;
								ptCurrentClient->NextCheckTime = CurrentTick;
								break;
								}
							}
							else
							{
								if(GetTickCount()-ptCurrentClient->ClientLogonTime>20000) ptCurrentClient->WardenStatus=WARDEN_ERROR_RESPONSE; //Daje ci 20 sekund (Na cbn jest 45)
								ptCurrentClient->NextCheckTime = CurrentTick + 100;
							}
					}
					break;
				case WARDEN_SEND_REQUEST:	
					{
					RemoveWardenPacket(ptCurrentClient);
					switch(ptCurrentClient->CheckCounter) //1.11b
					{
						//case 0 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0x0011B418,"D2Client.dll",0x0011B414);break;//MouseX/MouseY
						//case 1 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0x0011B418,"D2Client.dll",0x0011B414);break;//MouseX/MouseY
						//case 2 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0x1040C0,16*4);break;
						case 0 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0x0011B414,8,"D2Client.dll",0x1040C0,16*4); break;
						case 1 : SendPtrRequest(ptCurrentClient,"bnclient.dll",0x1D330,16);break; //RedVex
						case 2 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0x341404,4);break; //TMC
						case 3 : SendPtrRequest(ptCurrentClient,ptCurrentClient->AnimData,4);break; //TMC
						case 4 : SendPtrRequest(ptCurrentClient,ptCurrentClient->AnimData2,16);break; //TMC
						case 5 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0x521D1,6);break; //Sting's MH
						case 6 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0x66DC5,5);break; //WtfPk
						case 7 : SendPtrRequest(ptCurrentClient,"D2Client.dll",0xBDFB1,5);break; //OnPacketRecv
						case 8 : SendPtrRequest(ptCurrentClient,"D2Common.dll",0xA1328,4);break; //Get SkillsTxt
						case 9 : SendPtrRequest(ptCurrentClient,ptCurrentClient->ptSkillsTxt + (0x23C*54),8);break; //Get Teleport Record
						case 10: SendPtrRequest(ptCurrentClient,ptCurrentClient->ptSkillsTxt + (0x23C*59),8);break; //Get Blizzard Record
						case 11: SendPtrRequest(ptCurrentClient,ptCurrentClient->ptSkillsTxt + (0x23C*47),8);break; //Get Fireball Record
						case 12: SendPtrRequest(ptCurrentClient,ptCurrentClient->ptSkillsTxt + (0x23C*22),8);break; //Get Guided Arrow Record
						case 13: SendPtrRequest(ptCurrentClient,"D2Client.dll",0x32B40,7);break; //GameLoop
					}
					ptCurrentClient->WardenStatus = WARDEN_RECEIVE_CHECK;
					ptCurrentClient->NextCheckTime = CurrentTick;
					ptCurrentClient->pWardenPacket_SendTime = CurrentTick;
					}
				break;
				case WARDEN_RECEIVE_CHECK:
					{
						if(ptCurrentClient->pWardenPackets)
						{
							if(ptCurrentClient->pWardenPackets->PacketLen>=7)
							{
								switch(ptCurrentClient->CheckCounter)
								{
								case 0:
									{
										ptCurrentClient->MouseXPosition=*(WORD*)&(ptCurrentClient->pWardenPackets->ThePacket[12]);
										ptCurrentClient->MouseYPosition=*(WORD*)&(ptCurrentClient->pWardenPackets->ThePacket[8]);
										memcpy(ptCurrentClient->UIModes,ptCurrentClient->pWardenPackets->ThePacket+17,4*16);
										if(ptCurrentClient->MouseXPosition>800 || ptCurrentClient->MouseYPosition>600)
											Log("Hack: %s (*%s) has mouse in abnormal position (X='%d', Y='%d')!",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),ptCurrentClient->MouseXPosition,ptCurrentClient->MouseYPosition);
										ptCurrentClient->UIModesTime=GetTickCount();
									}  
									break; 			
									//case 2:
									//{
									//memcpy(ptCurrentClient->UIModes,ptCurrentClient->pWardenPackets->ThePacket+8,4*16);
									//ptCurrentClient->UIModesTime=GetTickCount();
									//}
									//break;
								case 1: 
									{
										ptCurrentClient->MyIp.assign((char*)&ptCurrentClient->pWardenPackets->ThePacket[8]);
										if(ptCurrentClient->MyIp == "127.0.0.1")
											Log("Hack: %s (*%s) is using RedVex (IP='%s')!",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),ptCurrentClient->MyIp.c_str());
									}
									break;
								case 2:
									{
										ptCurrentClient->AnimData=*(DWORD*)&(ptCurrentClient->pWardenPackets->ThePacket[8]);
									}
									break;
								case 3:
									{
										ptCurrentClient->AnimData2=*(DWORD*)&(ptCurrentClient->pWardenPackets->ThePacket[8]);
									}
									break;
								case 4:	
									{
										const BYTE TMCBP[]={0x36,0x00,0x00,0x00,0x30,0x49,0x4E,0x55,0x48,0x54,0x48,0x00,0x01,0x00,0x00,0x00};
										const BYTE TMCOK[]={0x36,0x00,0x00,0x00,0x4C,0x31,0x4F,0x50,0x48,0x54,0x48,0x00,0x02,0x00,0x00,0x00};

										if(memcmp(ptCurrentClient->pWardenPackets->ThePacket+8,TMCBP,16)==0)
										{
											char out[100];
											ConvertBytesToHexString(ptCurrentClient->pWardenPackets->ThePacket+8,16,out,50,',');
											Log("Hack: %s (*%s) is using TMC BP (%s)!",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),out);
										}
										//else 
										//if(memcmp(ptCurrentClient->pWardenPackets->ThePacket+8,TMCOK,16)!=0)
										//{
										//char out[100];
										//ConvertBytesToHexString(ptCurrentClient->pWardenPackets->ThePacket+8,16,out,50,',');
										//Log("Hack: %s (*%s) MAY use some private TMC (%s)!",ptCurrentClient->CharName,ptCurrentClient->AccountName,out);
										//}
										ptCurrentClient->TMCDetected=true;
									}
									break;
								case 5:
									{
										BYTE MH[]={0x90,0x90,0x90,0x90,0x90,0x90};
										if(memcmp(ptCurrentClient->pWardenPackets->ThePacket+8,MH,6)==0)
										{
											Log("Hack: %s (*%s) is using Sting's MapHack!",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str());
											ptCurrentClient->StingDetected=true;
										}
									}
									break;
								case 6:
									{
										BYTE WTF[]={0x5B,0x59,0xC2,0x04,0x00};
										if(memcmp(ptCurrentClient->pWardenPackets->ThePacket+8,WTF,5)!=0)
										{
											char out[50];
											ConvertBytesToHexString(ptCurrentClient->pWardenPackets->ThePacket+8,5,out,50,',');
											Log("Hack: %s (*%s) is using WtfPk/MH hack! (%s)",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),out);
											ptCurrentClient->WtfDetected=true;
										}
									}
									break;
								case 7:
									{
										BYTE RCV[]={0xE8,0xAA,0xF1,0xF4,0xFF};
										if(memcmp(ptCurrentClient->pWardenPackets->ThePacket+8,RCV,5)!=0)
										{
											char out[50];
											ConvertBytesToHexString(ptCurrentClient->pWardenPackets->ThePacket+8,5,out,50,',');
											Log("Hack: %s (*%s) is using Packet Intercepter hack! (%s)",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),out);
											ptCurrentClient->RCVDetected=true;
										}
									}
									break;
								case 8:
									{
										ptCurrentClient->ptSkillsTxt= *(DWORD*)(ptCurrentClient->pWardenPackets->ThePacket+8);
									}
									break;
								case 9:
								case 10:
								case 11:
								case 12:
									{
										SkillsTxt* pRec = (SkillsTxt*)(ptCurrentClient->pWardenPackets->ThePacket+8);
										SkillsTxt* pSrvTxt = *D2Vars::D2COMMON_SkillTxt;

										if(pSrvTxt[pRec->wSkillId].dwFlags.bLeftSkill != pRec->dwFlags.bLeftSkill ||
											pSrvTxt[pRec->wSkillId].dwFlags.bInTown != pRec->dwFlags.bInTown ||
											pSrvTxt[pRec->wSkillId].dwFlags.bSearchEnemyXY != pRec->dwFlags.bSearchEnemyXY ||
											pSrvTxt[pRec->wSkillId].dwFlags.bSearchEnemyNear != pRec->dwFlags.bSearchEnemyNear ||
											pSrvTxt[pRec->wSkillId].dwFlags.bSearchOpenXY != pRec->dwFlags.bSearchOpenXY)
											Log("Hack: %s (*%s) modification of Skills.Txt: Skill %s, Flags : LeftSkill %d InTown %d, bSearchEnemyXY %d, bSearchEnemyNear %d, bSearchOpenXY %d",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),ConvertSkill(pRec->wSkillId).c_str(),pRec->dwFlags.bLeftSkill,pRec->dwFlags.bInTown,pRec->dwFlags.bSearchEnemyXY, pRec->dwFlags.bSearchEnemyNear, pRec->dwFlags.bSearchOpenXY);
										if(ptCurrentClient->CheckCounter == 12) ptCurrentClient->GMDetected=true;
									}
									break;
								case 13:
									{
										//6FAE2B40  83 EC 20 89 4C 24 0C     ƒì ‰L$.
										BYTE LP[] = {0x83,0xEC,0x20,0x89,0x4C,0x24,0x0C};
										if(memcmp(ptCurrentClient->pWardenPackets->ThePacket+8,LP,7)!=0)
										{
											char out[100];
											ConvertBytesToHexString(ptCurrentClient->pWardenPackets->ThePacket+8,7,out,100,',');
											Log("Hack: %s (*%s) is using some private hack! (%s)",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),out);
											ptCurrentClient->LPDetected=true;
										}
									}
									break;
								}
begin:
								ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==1 && !ptCurrentClient->MyIp.empty()) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==2 && ptCurrentClient->AnimData) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==3 && ptCurrentClient->AnimData2) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==4 && ptCurrentClient->TMCDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==5 && ptCurrentClient->StingDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==6 && ptCurrentClient->WtfDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==7 && ptCurrentClient->RCVDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==8 && ptCurrentClient->ptSkillsTxt) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==9 && ptCurrentClient->GMDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==10 && ptCurrentClient->GMDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==11 && ptCurrentClient->GMDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==12 && ptCurrentClient->GMDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter==13 && ptCurrentClient->LPDetected) ptCurrentClient->CheckCounter++;
								if(ptCurrentClient->CheckCounter>13) ptCurrentClient->CheckCounter=0;
								if(ptCurrentClient->CheckCounter == 0 && !DetectTrick && !ptCurrentClient->MyIp.empty()
									&& ptCurrentClient->AnimData && ptCurrentClient->AnimData2 && ptCurrentClient->TMCDetected
									&& ptCurrentClient->StingDetected && ptCurrentClient->WtfDetected && ptCurrentClient->RCVDetected
									&& ptCurrentClient->ptSkillsTxt  && ptCurrentClient->GMDetected && ptCurrentClient->LPDetected)
								{
									ptCurrentClient->NextCheckTime = 0xFFFFFFFF;
									ptCurrentClient->WardenStatus = WARDEN_NOTHING;		
								}
								else if(ptCurrentClient->CheckCounter == 0 && !DetectTrick)
									goto begin;
								ptCurrentClient->NextCheckTime = GetTickCount() + random();
								Debug("Next check for %s in %.2f secs",ptCurrentClient->AccountName.c_str(),(float)(ptCurrentClient->NextCheckTime - GetTickCount()) / 1000);
								ptCurrentClient->WardenStatus=WARDEN_SEND_REQUEST;
								ptCurrentClient->ErrorCount=0;
							}
							else
							{
								ptCurrentClient->ErrorCount++;
								if(ptCurrentClient->ErrorCount>10) ptCurrentClient->WardenStatus=WARDEN_ERROR_RESPONSE; 
									else ptCurrentClient->WardenStatus=WARDEN_SEND_REQUEST;
								ptCurrentClient->NextCheckTime =GetTickCount() + random();
							}
						}
						else
						{
							if(CurrentTick - ptCurrentClient->pWardenPacket_SendTime < 4000)
								ptCurrentClient->NextCheckTime = GetTickCount() + 10;
							else
							{
								if(!ptCurrentClient->WardenBlocked && ptCurrentClient->ErrorCount > 5)
								{
									Log("Hack: %s (*%s) is blocking warden request!",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str());
									ptCurrentClient->WardenBlocked = true;
								}
								ptCurrentClient->ErrorCount++;
								if(ptCurrentClient->ErrorCount > 10) ptCurrentClient->WardenStatus=WARDEN_ERROR_RESPONSE; 
									else ptCurrentClient->WardenStatus = WARDEN_SEND_REQUEST;
								ptCurrentClient->NextCheckTime = GetTickCount() + random();
							}
						}					
					}
				break;
				case WARDEN_NOTHING:
					{
					Sleep(1);
					}
				break;
				case WARDEN_ERROR_RESPONSE:
					{
							Log("ERROR: %s (*%s) - error count has been exceeded, player is being removed from loop.(Counter = %d)",ptCurrentClient->CharName.c_str(),ptCurrentClient->AccountName.c_str(),ptCurrentClient->CheckCounter);
							ptCurrentClient->NextCheckTime = 0xFFFFFFFF;
							ptCurrentClient->WardenStatus = WARDEN_NOTHING;		
							RemoveWardenPacket(ptCurrentClient);
							KickPlayer(ptCurrentClient->ClientID);
							ptCurrentClient = hWarden.Clients.erase(ptCurrentClient);
							continue;
					}
				break;
		} // koniec switcha
	++ptCurrentClient;
	} // koniec for-a
	UNLOCK
} // koniec funkcji