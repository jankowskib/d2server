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

#include "stdafx.h"

#include "LEvents.h"
#include "LRoster.h"
#include "D2Warden.h"

/*
DWORD __fastcall OnResurrect(Game *pGame, UnitAny *pPlayer, BYTE *aPacket, int PacketSize) // Replacement for 0x41 parse (1.11b: D2Game.6FC51F60)
{
	UnitAny *pUnit; // ebx@1
	Game *v5; // edi@1
	Skill *v8; // eax@10
	Skill *v9; // esi@10
	Room1 *pRoom; // eax@10
	int LevelNo; // eax@10
	int TownLvl; // eax@10
	int v16; // ST5C_4@11
	int v17; // eax@11
	Skill *RightSkill; // eax@12
	Skill *v19; // esi@12
	int v20; // ST5C_4@13
	int SkillId; // eax@13
	Game *a2; // [sp+6Ch] [bp-4h]@1

	ClientData *pClientData = 0;

	v5 = pGame;
	pUnit = pPlayer;
	a2 = pGame;
	if (PacketSize != 1)
		return 3;
	if (pPlayer && pPlayer->dwMode == PLAYER_MODE_DEAD)
	{
		if (pPlayer->dwType == UNIT_PLAYER)
			pClientData = pPlayer->pPlayerData->pClientData;
		if (!pClientData)
			return 3;
		if (pClientData->PlayerStatus & 4)
		{
			D2Funcs.(pClientData, v5, 3u);
			return 0;
		}
		sub_6FC2D440(pUnit);                        // Something with skill and changes state
		int dwMaxLife = D2Common_GetMaxLife_10983(pUnit);
		int dwMaxMana = D2Common_GetMaxMana(pUnit);
		int dwMaxStamina = D2Common_GetMaxStamina_11022(pUnit);
		D2Common_SetStat_10590(pUnit, STAT_HITPOINTS, dwMaxLife, 0);
		D2Common_SetStat_10590(pUnit, STAT_MANA, dwMaxMana, 0);
		D2Common_SetStat_10590(pUnit, STAT_STAMINA, dwMaxStamina, 0);
		UpdatePlayerStats_6FC4D2E0(pUnit, 6, dwMaxLife, pUnit);
		UpdatePlayerStats_6FC4D2E0(pUnit, 8, dwMaxMana, pUnit);
		UpdatePlayerStats_6FC4D2E0(pUnit, 10, dwMaxStamina, pUnit);
		pUnit->dwFlags |= 2u;                       // Make unit selectable
		D2Common_SetGfxState_10702(pUnit, 54, 1);   // Uninterrupable
		D2Common_SetGfxState_10702(pUnit, 54, 0);
		D2Common_GetActNoByLvl_10026(0);
		pRoom = D2Common_GetUnitRoom_10933(pUnit);
		LevelNo = D2Common_GetLvlNoByRoom_11021(pRoom);
		LOBYTE(PacketSize) = D2Common_GetActNoByLvl_10026(LevelNo);
		TownLvl = D2Common_GetTownLvl_10394(PacketSize);
		TeleportUnitToLevel_6FC822C0(pUnit, TownLvl, a2, 0);
		SetUnitMode_6FC61990(a2, pUnit, 0, PLAYER_MODE_STAND_OUTTOWN, 0, 0, 1);
		v8 = D2Common_GetLeftSkill_10064(pUnit);
		v9 = v8;
		if (v8)
		{
			v16 = D2Common_GetSkillFlags_10427(v8);
			v17 = D2Common_GetSkillId_10170(v9, "..\\Source\\D2Game\\PLAYER\\PlrMsg.cpp", 4509);
			SetMonsterSkill_6FC31C20(pUnit, 1, v17, v16);
		}
		RightSkill = D2Common_GetRightSkill_11036(pUnit);
		v19 = RightSkill;
		if (RightSkill)
		{
			v20 = D2Common_GetSkillFlags_10427(RightSkill);
			SkillId = D2Common_GetSkillId_10170(v19, "..\\Source\\D2Game\\PLAYER\\PlrMsg.cpp", 4514);
			SetMonsterSkill_6FC31C20(pUnit, 0, SkillId, v20);
		}
	}
	return 0;
}
*/

/*
0x00 - "%Name1(%Name2) dropped due to time out."
0x01 - "%Name1(%Name2) dropped due to errors."
0x02 - "%Name1(%Name2) joined our world. Diablo's minions grow stronger."
0x03 - "%Name1(%Name2) left our world. Diablo's minions weaken."
0x04 - "%Name1 is not in the game."
0x05 - "%Name1 is not logged in."
0x06 - "%Name1 was Slain by %Name2" 
0x07 - Player Relations (Bottom Left Text)
0x08 - "%Name1 is busy."
0x09 - "You must wait a short time to trade with that player."
0x0d - "%Name1 is not listening to you."
0x0e - Received on 'Not enough mana' speech.
0x0f - "Realm going down in %Param1 minutes."
0x10 - "You must wait a short time to declare hostility with that player."
0x11 - "%Param1 Stones of Jordan Sold to Merchants"
0x12 - "Diablo Walks the Earth"
*/

void __stdcall OnBroadcastEvent(Game* pGame, EventPacket * pEvent)
{
	DEBUGMSG("OnBroadcastEvent %d",pEvent->MsgType);
	if(pGame) 
	{
		if(pGame->bFestivalMode == 1)
			if(pEvent->MsgType == 0 || pEvent->MsgType == 1 || pEvent->MsgType == 3) // Leave msgs 
			{
				DEBUGMSG("DEBUG: Szukam struktury ClientData z nazwa %s",pEvent->Name1);

				ClientData* pClient = FindClientDataByName(pGame,pEvent->Name1);
				if(pClient)
				{
					if(pClient->pPlayerUnit->pPlayerData->isPlaying == 0)  return;
					DoRoundEndStuff(pGame, pClient->pPlayerUnit);
				}
				else 
					DEBUGMSG("Nie znalazlem struktury WardenClient w %s",__FUNCTION__);
			}
			if(pEvent->MsgType == 2) // Join packet
			{
				ClientData* pClient = FindClientDataByName(pGame,pEvent->Name1);
				if(pClient) 
				{
					LRost::SyncClient(pGame,pClient);
					LRoster* pRoster = LRost::Find(pGame,pEvent->Name1);
					if(pRoster && pClient->pPlayerUnit) {
						LRost::SyncClient(pGame,pClient->pPlayerUnit->dwUnitId,pRoster);
					}
				}
			}
	BroadcastPacket(pGame,(BYTE*)pEvent,40);
	}
	switch(pEvent->MsgType)
	{
		case EVENT_TIMEOUT:
		case EVENT_LEFT: 
		case EVENT_DROPPED:
		{
			DEBUGMSG("[REMOVECLIENT] Removing (*%s) %s from WardenQueue", pEvent->Name2, pEvent->Name1);
			WardenClient_i pWardenClient = GetClientByName(pEvent->Name1);
			if(pWardenClient == hWarden.Clients.end()) 
			{
				DEBUGMSG("[REMOVECLIENT] Failed to find WardenClient!");
				return;
			}
			RemoveWardenPacket(pWardenClient);
			pWardenClient = hWarden.Clients.erase(pWardenClient);
			UNLOCK
		}
		break;
	}

}


void DoRoundEndStuff(Game* pGame, UnitAny* pUnit) //Sprawdz czy wskaznik jest ok przed wywolaniem, WardenClient = Victim
{
	DEBUGMSG("DEBUG: Robie DoRoundEndStuff");
	if(!pGame || !pUnit) return;

	ostringstream str; 
	string wynik;

	short P1Out = 0,P2Out = 0;
	int P1ID = 0, P2ID = 0;
	int vPID = D2ASMFuncs::D2GAME_GetPartyID(pUnit);

	if(pGame->nPlaying == 0) return;

	for(ClientData* pClient = pGame->pClientList; pClient; pClient = pClient->ptPrevious) {
		if(!pClient->pPlayerUnit) continue;
		if(pClient->InitStatus != 4) continue;
		
		if(pClient->pPlayerUnit->pPlayerData->isPlaying == 1) {
			Room1* pRoom = D2Funcs.D2COMMON_GetUnitRoom(pClient->pPlayerUnit);
			if(!pRoom) continue;
			if(D2Funcs.D2COMMON_GetLevelNoByRoom(pRoom) != D2Funcs.D2COMMON_GetTownLevel(pClient->pPlayerUnit->dwAct))
			{
			int pId = D2ASMFuncs::D2GAME_GetPartyID(pClient->pPlayerUnit);
			if(pId != -1 && P1ID == 0) P1ID = pId;
			else if(pId != -1 && P1ID && pId != P1ID && P2ID == 0) P2ID = pId;

			if(pId != -1 && pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEAD && pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEATH) {
				if(pId == P1ID) P1Out++;
				else if(pId == P2ID) P2Out++;
			}
		}
		}
	}

	DEBUGMSG("Party %d Out = %d, Party %d Out = %d,  Victim Party Id = %d ", P1ID, P1Out,  P2ID, P2Out,  vPID);

	if((P1ID == 0) || (P1Out == 0 || P2Out == 0)) {
		for(ClientData* pClient = pGame->pClientList; pClient; pClient = pClient->ptPrevious)
		{
			if(!pClient->pPlayerUnit) continue;
			if(pClient->InitStatus != 4) continue;

			if(pClient->pPlayerUnit->pPlayerData->isPlaying == 1)
			if(pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEAD && pClient->pPlayerUnit->dwMode != PLAYER_MODE_DEATH)
			{
				int aLevel = D2Funcs.D2COMMON_GetTownLevel(pClient->pPlayerUnit->dwAct);
				int aCurrLevel = D2Funcs.D2COMMON_GetLevelNoByRoom(pClient->pPlayerUnit->pPath->pRoom1);
				if(aCurrLevel!=aLevel)
					D2ASMFuncs::D2GAME_MoveUnitToLevelId(pClient->pPlayerUnit,aLevel,pGame);
			}

		}
		pGame->nPlaying = 0;
		ClearSayGoFlag(pGame);
		ClearCanAttackFlag(pGame);
		ClearIsPlayingFlag(pGame);
		BroadcastExEvent(pGame,1,0,3,-1,200, "Runda zakonczona!", "Round end!");
	}

}