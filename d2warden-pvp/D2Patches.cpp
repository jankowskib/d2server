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

#include "stdafx.h"

#ifdef VER_111B
#include "D2Patches_111B.h"
#elif defined VER_113D
#include "D2Patches_113D.h"
#else
#error "Unsupported version" 
#endif

#include "D2Handlers.h"
#include "LQuests.h"
#include "PartyExp.h"
#include "LSpectator.h"
#include "LWorldEvent.h"
#include "LEvents.h"
#include "LMonsters.h"
#include "LItems.h"
#include "NodesEx.h"
#include "LParty.h"
#include "LPackets.h"
#include "LCube.h"
#include "MainLoop.h"

void Warden::patchD2()
{

	int NEU_NODE = wcfgMaxPlayers + 3;

	#define JUMP 0xE9
	#define CALL 0xE8
	#define NOP 0x90
	#define CUSTOM 0x0
	/*
	UNUSED (1.11b)
	PatchGS(CALL,(DWORD)D2Ptrs::D2GAME_LifeLeech_P,(DWORD)D2Stubs::D2GAME_OnLifeLeech_STUB,6,"Life Leech");
	PatchGS(CALL,(DWORD)D2Stubs::D2GAME_ManaLeech_P,(DWORD)D2Ptrs::D2GAME_OnManaLeech_STUB,6,"Mana Leech");

	PatchGS(NOP,(DWORD)D2Ptrs::D2GAME_Corpse_P,0,10,"Disable Corpse");
	PatchGS(CALL,(DWORD)D2Ptrs::D2GAME_GetUnitX_P,(DWORD)D2Stubs::D2COMMON_GetUnitX,5,"TargetX fix");
	PatchGS(CALL,(DWORD)D2Ptrs::D2GAME_GetUnitY_P,(DWORD)D2Stubs::D2COMMON_GetUnitY,5,"TargetY fix");
	*/
	//        HDR    ADDR                         FUNC                          SIZE    DESC
	//PatchGS(0,(DWORD)& D2Vars.D2GAME_PacketTable[0x66].Callback,(DWORD)d2warden_0X66Handler,4,"On Warden Packet");
	//PatchGS(JUMP,GetDllOffset("D2Game.dll",0x703D0),(DWORD)D2Stubs::D2GAME_OnJoinGame_STUB,5,"Join Game Handler");
	//PatchGS(CALL,GetDllOffset("D2Game.dll",0x7A6CE),(DWORD)D2Stubs::D2GAME_SetDRCap_STUB,5,"DR Cap");
	//#ifdef _DEBUG
	//	PatchGS(0x68,GetDllOffset("D2Client.dll",0x5E7CD),1,5,"TCP/IP Delay fix");
	//	PatchGS(CALL,GetDllOffset("D2Game.dll",0xE832E),(DWORD)D2Stubs::D2GAME_ParseCreatePackets_STUB,5,"ParseCreatePackets");
	//	PatchGS(0,GetDllOffset("D2Client.dll",0xBE919),0xEB,1,"AE Packet Fixture");
	//#endif


//	PatchGS(NOP, GetDllOffset("D2Game.dll", D2GAME_DISABLE_ROOM_CACHE), 0x90, 6, "Disable Room Cache");
	
//	PatchGS(CUSTOM, GetDllOffset("D2Game.dll", D2GAME_TIMER_EXPAND), 16, 1, "Expand Timer List");

#ifdef VER_113D
	
	if (wcfgClassicWW) {
		PatchGS(NOP, GetDllOffset("D2Game.dll", 0xC14AB), 0x90, 4, "Bring back the classic ww style");
	}

	if (wcfgDmgRedCap) {
		PatchGS(CUSTOM, GetDllOffset("D2Game.dll", 0x8889F), (BYTE)wcfgDmgRedCap, 1, "Change DR cap");
	}

	if (wcfgMagicPierceStat) {
		PatchGS(CUSTOM, GetDllOffset("D2Game.dll", 0x100ECC), (DWORD)wcfgMagicPierceStat, 4, "Set a stat for magic pierce");
	}
	
	if (wcfgPhysPierceStat) {
		PatchGS(CUSTOM, GetDllOffset("D2Game.dll", 0x100ECC - 0xB0), (DWORD)wcfgPhysPierceStat, 4, "Set a stat for physical pierce");
	}

	if (wcfgAllowPierceBreakImmunites) {
		PatchGS(CUSTOM, GetDllOffset("D2Game.dll", 0x887D0), 0xEB909090, 4, "Allow pierce to break immunities");
	}

	//PatchGS(CUSTOM, GetDllOffset("D2Game.dll", 0xC0150), 0x8518768B, 4, "Undo crit section patch [TEST]");

	PatchGS(CUSTOM, GetDllOffset("Fog.dll", 0x202C1), 0xEB, 1, "Disable reconnect spam hacklisting");

	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_PARSE_CREATE_PACKETS), (DWORD)OnCreatePacketReceive, 5, "Create packet parser");
	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_GAME_LOOP), (DWORD)MainLoop_ASM, 5, "Game loop");

	PatchGS(CALL, GetDllOffset("D2Game.dll", 0xBFFC0), (DWORD)D2Stubs::UBERQUEST_SpawnMonsters_STUB, 5, "Spawn uber monsters");
		

	// I'd rather to avoid using __asm stubs
	PatchGS(CALL, GetDllOffset("D2Common.dll", 0x4F330), (DWORD)LEVELS_GetActByLevel, 22, "Replace inline GetActByLevel"); //  Ordinal11007
	PatchGS(CALL, GetDllOffset("D2Common.dll", 0x4F3C7), (DWORD)LEVELS_GetActByLevel, 26, "Replace inline GetActByLevel"); //  Ordinal11051
	PatchGS(CALL, GetDllOffset("D2Common.dll", 0x4F160), (DWORD)LEVELS_GetActByRoom2, 22, "Replace inline GetActByLevel"); //  Ordinal10301

	PatchGS(CALL, GetDllOffset("D2Common.dll", 0x2AD76), (DWORD)D2Stubs::GetActByLevelNo_STUB1, 20, "Replace inline GetActByLevel"); //  sub_6FD7AD60
	PatchGS(CALL, GetDllOffset("D2Common.dll", 0x56F40), (DWORD)D2Stubs::GetActByLevelNo_STUB1, 20, "Replace inline GetActByLevel"); //  sub_6FDA6EA0
	PatchGS(CALL, GetDllOffset("D2Common.dll", 0x510A0), (DWORD)D2Stubs::GetActByLevelNo_STUB2, 20, "Replace inline GetActByLevel"); //  sub_6FDA1070

	PatchGS(JUMP, GetDllOffset("D2Common.dll", -10864), (DWORD)LEVELS_GetActByLevelNo, 5, "Replace original GetActByLevelNo");

	PatchGS(CALL, GetDllOffset("D2Game.dll", 0x970FC), (DWORD)D2Stubs::D2GAME_OnCustomFunc_STUB, 49, "CubeMain.Txt custom functions replacement");
#endif

	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_ON_DEBUG_PACKET), (DWORD)D2Stubs::OnDebugPacketReceive_STUB, 5, "Fix Debug Packet Parser");

	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_QUESTINIT_I), (DWORD)QUEST_AllocQuestControl, 5, "Quest Init I");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_QUESTINIT_II), (DWORD)QUEST_AllocQuestControl, 5, "Quest Init II");

#ifdef VER_113D
	PatchGS(0, GetDllOffset("D2Game.dll", 0x3C707), 0xEB, 1, "Assassin's Cloak Crash Fix");
	//PatchGS(CALL, GetDllOffset("D2Game.dll", 0x1602A), (DWORD)D2GAME_PortalCrashFix, 5, "TP Crash Fix");
#endif

	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_ISDEADOVERRIDE), (DWORD)D2Stubs::D2GAME_IsUnitDead_STUB, 5, "Is Unit Dead Override");

	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_MONSTERSPAWN_I), (DWORD)OnMonsterSpawn, 5, "On Create Monster");

	if (wcfgHostileLevel)
	{
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_HOSTILELVL_I + 2), wcfgHostileLevel, 1, "Hostile Level I");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_HOSTILELVL_II + 2), wcfgHostileLevel, 1, "Hostile Level II");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_HOSTILELVL_III + 2), wcfgHostileLevel, 1, "Hostile Level III");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_HOSTILELVL_IV + 2), wcfgHostileLevel, 1, "Hostile Level IV");
	}

	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_LASTHITINTERCEPT), (DWORD)D2Stubs::D2GAME_LastHitIntercept_STUB, 5, "LastHitIntercept");
	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_ONDAMAGECREATION), (DWORD)D2Stubs::D2GAME_OnCreateDamage_STUB, 6, "OnDamageCreation");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_CLIENT_PACKET_WRAPPER_NEW), (DWORD)D2Stubs::D2GAME_OnPacketReceive_STUB, 5, "Client Packet Wrapper NEW ");


	if (wcfgAllowD2Ex)
	{
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_D2EX_CONNECT), (DWORD)OnD2ExPacket, 5, "D2Ex Connect");
		D2Vars.D2NET_ToSrvPacketSizes[0x0B] = 5;
	}
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_ACT_LOAD), (DWORD)OnActLoad, 5, "Act Load");
	if (wcfgMoveToTown)
		PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_MOVE_PLAYER_CORPSE_TO_TOWN), (DWORD)OnCreateCorpse, 5, "Move Player Corpse To Town"); // 1.13d = 	0x93BCB 

	if (wcfgDisableHostileDelay)
	{
		PatchGS(0x90, GetDllOffset("D2Game.dll", D2GAME_HOSTILE_WP_DELAY), 0, 19, "Hostile WP Delay");
		PatchGS(0x33, GetDllOffset("D2Game.dll", D2GAME_HOSTILE_BUTTON_DELAY), 0xF6, 2, "Hostile Button Delay"); //0x33F6 xor esi, esi
	}

	if (wcfgDropEarGold)
	{
		PatchGS(0x90, GetDllOffset("D2Game.dll", D2GAME_GOLD_DROP_ON_DEATH), 0, 7, "Gold drop on death");
		PatchGS(0xEB, GetDllOffset("D2Game.dll", D2GAME_EAR_DROP_ON_DEATH), 0, 1, "Ear drop on death");
	}

	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_FINDUNIT_FIX), (DWORD)D2Stubs::D2GAME_FindUnit_STUB, 9, "FindUnit Fix");

	//PatchGS(JUMP,GetDllOffset("D2Game.dll",D2GAME_PLAYER_MODE_INTERCEPT),(DWORD)D2Stubs::D2GAME_OnPlayerModeChange_I,5,"Player mode intercept");

	if (wcfgStrBugFix)
		PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_STRDEX_BUG_FIX), (DWORD)D2Stubs::D2GAME_SendStatToOther_STUB, 5, "Str/dex bug fix");

	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_CHAT_WRAPPER), (DWORD)D2Stubs::D2GAME_Chat_STUB, 6, "Chat Wrapper");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_NPC_SLOW_MISSILE_HEAL), (DWORD)D2Stubs::D2GAME_NPCHeal_STUB, 61, "NPC Slow Missile Heal");
	//PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_RESSURECT_FIX), (DWORD)D2Stubs::D2GAME_Ressurect_STUB, 39, "Ressurect Fix");
	//PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_0X41_WRAPPER), (DWORD)OnResurrect, 6, "0x41 Wrapper");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_ROSTER_WRAPPER), (DWORD)D2Stubs::D2GAME_DeathMsg_STUB, 5, "Roster Wrapper");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_ON_MONSTER_DEATH), (DWORD)D2Stubs::D2GAME_OnMonsterDeath_STUB, 9, "On Monster Death");

	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NULL_ORIGINAL_SC_0X66), (DWORD)D2Stubs::D2GAME_SendKillsNULL_STUB, 6, "Null Original S->C 0x66");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_GAME_DESTROY_WRAPPER), (DWORD)D2Stubs::D2GAME_GameDestroy_STUB, 5, "Game Destroy Wrapper");

	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_ADD_CLIENTTOGAME_WRAPPER), (DWORD)D2Stubs::D2GAME_GameEnter_STUB, 6, "Add ClientToGame Wrapper"); //0ld = 0xE76D6, The newest = 0xE76E4
	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_PLAYER_EVENTS_WRAPPER), (DWORD)D2Stubs::D2GAME_OnEventSend_STUB, 5, "Player Events Wrapper");



	if (wcfgSpectator)
	{
		WriteDword((DWORD*)&D2Vars.D2GAME_ClientPacketTable[0x5E].Callback, (DWORD)&OnPartyRelationChange); // TODO: move to lpackets
	}

	//PatchGS(JUMP,GetDllOffset("D2Game.dll",D2GAME_LOCK_HACK_WRAPPER),(DWORD)D2Stubs::D2GAME_LogHack_STUB,5,"Lock Hack Wrapper");

	if (wcfgFFAMode)
	{
		PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_PERM_STORE_STUB_I), (DWORD)D2Stubs::D2GAME_PermStore_STUB, 5, "Perm Store Stub");
		PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_PERM_STORE_STUB_II), (DWORD)D2Stubs::D2GAME_PermStore_STUB, 5, "Perm Store Stub");
	}
#ifdef D2EX_MYSQL
	PatchGS(JUMP, GetDllOffset("D2Common.dll", -10550), (DWORD)ITEMS_OnStatFetch, 6, "Gold value override");

#ifdef VER_113D
	PatchGS(JUMP, GetDllOffset("D2Common.dll", -10186) + 34, (DWORD)D2Stubs::D2COMMON_GetItemCost_STUB, 5, "Remove item cost"); // Set 'gamble cost' column to 0 to remove item cost
	PatchGS(JUMP, GetDllOffset("D2Common.dll", -10243), (DWORD)D2Stubs::D2COMMON_GetMercCost, 5, "Remove merc cost");
#endif

	PatchGS(JUMP, GetDllOffset("D2Common.dll", D2COMMON_GETBANKGOLDLIMIT_J), (DWORD)GetGoldBankLimit, 5, "Nullify stash gold");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_ADDGOLD_I), (DWORD)D2Stubs::D2GAME_AddStat_STUB, 5, "Fetch forum gold value instead of gold stat I");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_ADDGOLD_II), (DWORD)D2Stubs::D2GAME_AddStat_STUB, 5, "Fetch forum gold value instead of gold stat II");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_GOLDTRANS_I), (DWORD)D2Stubs::D2GAME_GoldTransaction_STUB, 5, "Fetch forum gold value instead of gold stat I");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_GOLDTRANS_II), (DWORD)D2Stubs::D2GAME_GoldTransaction_STUB, 5, "Fetch forum gold value instead of gold stat II");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_GOLDTRANS_III), (DWORD)D2Stubs::D2GAME_GoldTransaction_STUB, 5, "Fetch forum gold value instead of gold stat III");
#endif

	if (wcfgDisableRepairCost)
	{
		PatchGS(0, GetDllOffset("D2Common.dll", D2COMMON_REPAIR_I), 0xD231, 2, "Disable repair costs");
		PatchGS(0x90, GetDllOffset("D2Game.dll", D2COMMON_REPAIR_II), 0x90909090, 8, "Disable repair costs II");
	}



	PatchGS(0x05, GetDllOffset("D2Client.dll", D2CLIENT_EXTEND_GAME_STRUCT), sizeof(Game), 5, "Extend Game Struct"); //6FAB2589  |.  05 F41D0000   ADD EAX,1DF4

	PatchGS(0x00, GetDllOffset("D2Common.dll", D2COMMON_EXTEND_PLAYERDATA_STRUCT_I), sizeof(PlayerData), 4, "Extend PlayerData Struct");
	PatchGS(0x00, GetDllOffset("D2Common.dll", D2COMMON_EXTEND_PLAYERDATA_STRUCT_II), (sizeof(PlayerData) / 4), 4, "Extend PlayerData Struct");

	//EXP FIX
	PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_PARTY_SHARE_FIX_PREPARE), 0x5690, 2, "Party Share Fix Prepare");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_PARTY_SHARE_FIX), (DWORD)ExpShare_NEW, 5, "Party Share Fix");
	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_EXP_INCREASE_FIX), (DWORD)nPlayersFormula, 5, "Exp Increase Fix");
	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_EXP_INCREASE_FIX_2), (DWORD)HpGainFormula, 5, "Hp Increase Fix");

	//PatchGS(JUMP,GetDllOffset("D2Game.dll",D2GAME_MAX_PLAYERS_CHANGE),(DWORD)D2Stubs::D2GAME_GetPlayers_STUB,5,"Max Players Change");

	//MISC FIXES
	PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_DECRESAE_CORPSE_NUMBER), 7, 1, "Decresae Corpse number");  //NEW
	PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NULL_IRON_MAIDEN), 0, 1, "Null Iron Maiden");
	PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NULL_LOWER_RESIST), 0, 1, "Null Lower Resist");
	PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_COWS_DROP_FIX), 0xEB, 1, "Cows' drop fix");
//	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_COW_LEVEL_QUEST_OVERRIDE), (DWORD)QUESTS_CowLevelOpenPortal, 5, "Cow Level Quest Override");
#ifdef VER_111B
	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_RESPEC), (DWORD)D2Stubs::D2GAME_OnUseItem_STUB, 5, "Respec");
#endif
	PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_FIRE_ENCHANTED_FIX), (DWORD)DAMAGE_FireEnchanted, 6, "Fire Enchanted Fix");
	PatchGS(CALL, GetDllOffset("D2Common.dll", D2COMMON_COLOUR_ITEM_INTERCEPT_IV), (DWORD)DYES_DrawItem, 5, "Colour Item Intercept IV");
	PatchGS(CALL, GetDllOffset("D2Common.dll", D2COMMON_COLOUR_ITEM_INTERCEPT_V), (DWORD)DYES_DrawItem, 5, "Colour Item Intercept V");

	D2Vars.D2GAME_pSpell[12].SpellPrepareFunc = &DYES_Prepare;
	D2Vars.D2GAME_pSpell[12].SpellDoFunc = &DYES_Colorize;
	D2Vars.D2GAME_pSpell[13].SpellDoFunc = &WE_Spawn;

	PatchGS(CALL, GetDllOffset("D2Game.dll", D2GAME_TRESURE_CLASS_CREATE_HNDLR), (DWORD)ITEMS_AddKillerId, 5, "Tresure Class Create Hndlr");


	//	PatchGS(JUMP,GetDllOffset("Fog.dll",FOG_MEM_ALLOC_OVERRIDE),(DWORD)ExMemory::Alloc_STUB,5,"Mem Alloc Override");
	//	PatchGS(JUMP,GetDllOffset("Fog.dll",FOG_MEM_FREE_OVERRIDE),(DWORD)ExMemory::Free_STUB,5,"Mem Free Override");
	//	PatchGS(JUMP,GetDllOffset("Fog.dll",FOG_MEM_REALLOC_OVERRIDE),(DWORD)ExMemory::Realloc,5,"Mem Realloc Override");

	//PatchGS(0x90, GetDllOffset("D2Game.dll", D2GAME_NODESEX_AI_TEMP_FIX), 0x90909090, 9, "NodesEX: Ai Temp Fix"); // This temp fix is no more needed
	if (wcfgMaxPlayers > 8) {
		DEBUGMSG("Patching the nodes system")
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_BAAL_AI), (DWORD)NODES_BaalCheck, 7, "NodesEX: Baal Ai"); // bylo 0x2BB75
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_DIABLO_AI), (DWORD)NODES_NormalCheck, 7, "NodesEX: Diablo Ai");

		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_ALLOC_16PLAYERS_NODES), (DWORD)D2Stubs::D2GAME_AssignNode_STUB, 6, "NodesEX: Alloc 16-players nodes");
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_FREE_16PLAYERS_NODES), (DWORD)D2Stubs::D2GAME_FreeNode_STUB, 6, "NodesEX: Free 16-players nodes");
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_FREE_CHILD_OF_16PLAYERS_NODES), (DWORD)D2Stubs::D2GAME_FreeChildNode_STUB, 8, "NodesEX: Free Child of 16-players nodes");
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_GET_UNIT_NODE), (DWORD)NODES_GetUnitNode, 6, "NodesEX: Get Unit Node");
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_SET_UNIT_NODE_AS_PARENT_MONSTERS), (DWORD)D2Stubs::D2GAME_SetUnitsNode_STUB, 6, "NodesEX: Set Unit Node As Parent (Monsters)");
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_SET_UNIT_NODE_AS_PARENT_PLAYERS), (DWORD)D2Stubs::D2GAME_SetNodeParented_STUB, 6, "NodesEX: Set Unit Node As Parent (Players)");
		PatchGS(JUMP, GetDllOffset("D2Game.dll", D2GAME_NODESEX_FREE_ALL_NODES), (DWORD)D2Stubs::D2GAME_FreeNodes_STUB, 5, "NodesEX: Free all nodes");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NODE_OFFSET_I), 0x1E00, 4, "NodesEX: Change node offset I");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NODE_OFFSET_II), 0x1E00, 4, "NodesEX: Change node offset II");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NODE_OFFSET_III), 0x1E00, 4, "NodesEX: Change node offset III");

		//Basic fixes
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_GAMEENTER_CHECK_NUMBER_OF_PLAYERS), wcfgMaxPlayers, 1, "GameEnter: Check number of players");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_GAMEENTER_CHECK_NUMBER_OF_PLAYERS_II), wcfgMaxPlayers, 1, "GameEnter: Check number of players II");
#ifndef _SINGLEPLAYER
		PatchGS(0, GetDllOffset("D2GS.exe", 0x240C), wcfgMaxPlayers, 1, "GameEnter: Check number of players III");
		PatchGS(0, GetDllOffset("D2GS.exe", 0x4FD4), wcfgMaxPlayers, 1, "GameEnter: Check number of players IV");
#endif

		//TREASURE Class
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_FIX_TREASURE_CLASS_DROP), wcfgMaxPlayers, 1, "NodesEX: Fix Treasure Class Drop");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_FIX_TREASURE_CLASS_DROP_II), wcfgMaxPlayers, 1, "NodesEX: Fix Treasure Class Drop II");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CREATE_UNITANY_CHANGE_NEUTRAL_NODE), NEU_NODE, 4, "NodesEX: Create UnitAny: Change neutral Node");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_ITEM_CREATE_SUB), NEU_NODE, 4, "NodesEX: Create Item: Change neutral Node");

		//UNIT_Player

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEPLAYER_I_F1), NEU_NODE, 4, "NodesEX: Change neutral Node: CreatePlayer I -f1 ");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEPLAYER_II_F2), NEU_NODE, 4, "NodesEX: Change neutral Node: CreatePlayer II -f2");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEPLAYER_III_F3), NEU_NODE, 4, "NodesEX: Change neutral Node: CreatePlayer III -f3");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEPLAYER_IV_F4), NEU_NODE, 4, "NodesEX: Change neutral Node CreatePlayer IV -f4");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATECORPSE_I), NEU_NODE, 4, "NodesEX: Change neutral Node CreateCorpse I");

		//UNIT_Object 

		//CreateObject Patches
		//po chuj blizzowi tyle funkcji do tworzenia obiektow...
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_I), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject I");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_II), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject II");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_III_F2), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject III - f2");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_IV_F3), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject IV - f3");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_V_F3), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject V - f3");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_VI_F4_HUGE_ONE), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject VI - f4 - huge one");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_VII_F4_HUGE_ONE), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject VII - f4 - huge one");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_VIII_F5_SMALL_ONE), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject VIII - f5 - small one");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_IX_F6), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject IX - f6");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_X_MOVED_ONE_F6), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject X - moved one f6");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XI_F7), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XI - f7");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XII_F8), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XII - f8");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XIII_F9), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XIII - f9");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XIV_F10), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XIV - f10");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XV_F11), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XV - f11");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XVI_F12), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XVI - f12");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XVII_F13), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XVII - f13");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEPORTAL_I), NEU_NODE, 4, "NodesEX: Change neutral Node: CreatePortal I");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XVIII_F14), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XVIII - f14");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XIX_F15), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XIX - f15");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XX_F16), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XX - f16");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XXI_F17), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XXI - f17");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XXII_F17), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XXII - f17");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XXIII_F18), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XXIII - f18");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XXIV_F19), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XXIV - f19");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XXV_F20), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XXV - f20");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XXVI_F20), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XXVI - f20");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_XXVII_F21), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject XXVII - f21");

		//Teraz potwory

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_PLAYERS_NODES_FROM_8), wcfgMaxPlayers, 1, "NodesEX: Change Players Nodes from 8");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE), NEU_NODE, 1, "NodesEX: Change neutral Node");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEMONSTER_I_F1), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateMonster I - f1");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_REMOVEMONSTERCORPSE_CHANGE_ITERATION_TO_16), wcfgMaxPlayers, 1, "NodesEX: RemoveMonsterCorpse: Change iteration to 16");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_MONSTERS_SKILL_NECRO), NEU_NODE, 1, "NodesEX: Change neutral Node: Monsters Skill : Necro");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_MONSTER_NODE_TO_17_SAME_FUNC_AS_ABOVE), NEU_NODE - 2, 4, "NodesEX: Change Monster Node to 17 same func as above");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_OFFSET_TO_NEWNODES17_SAME_FUNC_AS_ABOVE_I), 0x1E00 + (4 * (NEU_NODE - 2)), 4, "NodesEX: Change offset to NewNodes[17]: same func as above");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_OFFSET_TO_NEWNODES17_SAME_FUNC_AS_ABOVE_II), 0x1E00 + (4 * (NEU_NODE - 2)), 4, "NodesEX: Change offset to NewNodes[17]: same func as above");


		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_MONSTERS), NEU_NODE, 1, "NodesEX: Change neutral Node: Monsters");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_ITERATION_TO_16_SAME_FUNC_AS_ABOVE), wcfgMaxPlayers, 4, "NodesEX: Change iteration to 16: same func as above");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_OFFSET_TO_NEWNODES16_SAME_FUNC_AS_ABOVE_I), 0x1E00 + (4 * wcfgMaxPlayers), 4, "NodesEX: Change offset to NewNodes[16]: same func as above");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_OFFSET_TO_NEWNODES16_SAME_FUNC_AS_ABOVE_II), 0x1E00 + (4 * wcfgMaxPlayers), 4, "NodesEX: Change offset to NewNodes[16]: same func as above");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_ITERATION_TO_16), wcfgMaxPlayers, 1, "NodesEX: Change iteration to 16");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_MONSTER_NODE_TO_16), wcfgMaxPlayers, 4, "NodesEX: Change Monster Node to 16");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_MONSTER_NODE_TO_17), NEU_NODE - 2, 4, "NodesEX: Change Monster Node to 17");

		//Missiles
#ifdef VER_111B
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_CREATEOBJECT_IB), NEU_NODE, 4, "NodesEX: Change neutral Node: CreateObject I");
#pragma message ("Warning: Look at the patches below - they're not ported to 1.11b so game will crash randomly!")
#endif

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_NEUTRAL_NODE_PLAYER_EVENT_CONTROLLER), NEU_NODE, 1, "NodesEX: MONAI_WUSSIE: Change neutral Node");
#ifdef VER_113D
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_WUSSIE_AI_NODE_TO_16), wcfgMaxPlayers, 1, "NodesEX: MONAI_WUSSIE: Change iteration to 16");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_ROGUE_SCOUT_SPAWN_NODE_TO_16), wcfgMaxPlayers, 1, "NodesEX: Rogue Scout spawn: Change iteration to 16");

		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_BONEWALL_NODE_TO_17), NEU_NODE - 2, 1, "NodesEX: SRVDOFUNC:BONEWALLMAKER: Change iteration to 17");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_BONEWALL_NODE_TO_17_SKILL), NEU_NODE - 2, 1, "NodesEX: SRVDOFUNC:BONEWALL: Change iteration to 17");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_ATTRACT_NODE_TO_17), NEU_NODE - 2, 1, "NodesEX: SRVDOFUNC:ATTRACT: Change iteration to 17");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_BONEPRISON_NODE_TO_17), NEU_NODE - 2, 1, "NodesEX: SRVDOFUNC:BONEPRISON: Change iteration to 17");
		PatchGS(0, GetDllOffset("D2Game.dll", D2GAME_NODESEX_CHANGE_ITERATION_TO_16_MISSING_I), wcfgMaxPlayers, 1, "NodesEX: Change iteration to 17 [unknown]");

	}
	PatchGS(JUMP, GetDllOffset("D2Game.dll", 0xB2255), (DWORD)D2Stubs::D2GAME_TestCrash_STUB, 5, "NodesEX: Test why the game keeps crashing");
	
#endif



}