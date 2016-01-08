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
#include "LSpectator.h"

void EVENTS_SendAccountInfo(ClientData * pClient)
{
	ASSERT(pClient)

	// Don't bother if we are the only player in a game
	if (pClient->pGame->nClients == 1)
		return;

	ExEventAccountInfo hEvent;
	hEvent.MsgType = EXEVENT_ACCOUNT_INFO;
	hEvent.P_A6 = 0xA6;
	hEvent.PacketLen = sizeof(ExEventAccountInfo);
	for (ClientData* pClientList = pClient->pGame->pClientList; pClientList; pClientList = pClientList->ptPrevious)
	{
		if (!(pClientList->InitStatus & 4))
			continue;
		if (pClientList == pClient)
			continue;

		hEvent.UnitId = pClientList->UnitId;
		strcpy_s<16>(hEvent.szAccount, pClientList->AccountName);
		D2ASMFuncs::D2GAME_SendPacket(pClient, (BYTE*)&hEvent, sizeof(ExEventAccountInfo));
	}

}

void EVENTS_OnGameJoin(Game* pGame, ClientData* pClient)
{
	WardenClient_i client = Warden::getInstance().findClientById(pClient->ClientID);
	if (client != Warden::getInstance().getInvalidClient())
	{
		DEBUGMSG("NEWCLIENT: Setup for '%s' !", pClient->CharName);
		client->setup(pGame, pClient);

		if (client->bNeedUpdate && !Warden::getInstance().wcfgUpdateURL.empty())
		{
			SendMsgToClient(pClient, "Trying to download patch....");
			ExEventDownload pEvent;
			::memset(&pEvent, 0, sizeof(ExEventDownload));
			pEvent.P_A6 = 0xA6;
			pEvent.MsgType = EXEVENT_DOWNLOAD;
			pEvent.bExec = 1;
			strcpy_s(pEvent.szURL, 255, Warden::getInstance().wcfgUpdateURL.c_str());
			if (pEvent.szURL[0])
				pEvent.PacketLen = 14 + strlen(pEvent.szURL) + 1;
			else
				pEvent.PacketLen = 15;

			D2ASMFuncs::D2GAME_SendPacket(pClient, (BYTE*)&pEvent, pEvent.PacketLen);
		}
	}
	else
	{
		DEBUGMSG("Booting %s", pClient->AccountName)
		Log("NEWCLIENT: Failed to find a client! Dropping player %s !", pClient->AccountName);
		BootPlayer(pClient->ClientID, BOOT_UNKNOWN_FAILURE);
		return;
	}

	// Download the clan list only if URL isn't empty
	if (!Warden::getInstance().wcfgClansURL.empty()) {
		ExEventDownload pEvent;
		pEvent.P_A6 = 0xA6;
		pEvent.bExec = 0;
		pEvent.MsgType = EXEVENT_DOWNLOAD;
		strcpy_s(pEvent.szURL, 255, Warden::getInstance().wcfgClansURL.c_str());
		if (pEvent.szURL[0])
			pEvent.PacketLen = 14 + strlen(pEvent.szURL) + 1;
		else
			pEvent.PacketLen = 15;
		D2ASMFuncs::D2GAME_SendPacket(pClient, (BYTE*)&pEvent, pEvent.PacketLen);
	}

	if (strlen(pGame->GameDesc) > 0)
	{
		char tk[32];
		strcpy_s(tk, 32, pGame->GameDesc);
		char *nt = 0;
		for (char * ret = strtok_s(tk, " -", &nt); ret; ret = strtok_s(NULL, " -", &nt))
		{
			if (_strnicmp(ret, "ffa", 3) == 0) { SendMsgToClient(pClient, pClient->LocaleID == 10 ? "Tryb FFA jest w³¹czony na tej grze!" : "Free For All Mode Is Enabled!"); continue; }
			if (_strnicmp(ret, "m", 1) == 0 && strlen(ret) > 1 && Warden::getInstance().wcfgAllowTourMode) { SendMsgToClient(pClient, pClient->LocaleID == 10 ? "Ustawiono identyfikator mapy na '%d'" : "Custom Map Id : '%d'", atoi(ret + 1)); continue; }
			if (_strnicmp(ret, "t", 1) == 0 && strlen(ret) == 1) { SendMsgToClient(pClient, pClient->LocaleID == 10 ? "ÿc;Tryb turniejowy!" : "ÿc;Tournament Mode!"); continue; }
		}
	}

	SendExEvent(pClient, EXOP_DISABLESPECTATOR, !Warden::getInstance().wcfgSpectator);

	EVENTS_SendAccountInfo(pClient);

}

void __stdcall OnDeath(UnitAny* ptKiller, UnitAny * ptVictim, Game * ptGame)
{
	DEBUGMSG(__FUNCTION__)
	if (!ptVictim || !ptKiller || ptVictim->dwType) return;

	EventPacket aPacket = { 0 };
	aPacket.P_5A = 0x5A;
	aPacket.Color = COL_DARK_GOLD;
	aPacket.MsgType = EVENT_SLAIN;

	if (ptKiller->dwType == UNIT_MONSTER)
	{
		UnitAny* ptParent = D2ASMFuncs::D2GAME_FindUnit(ptGame, ptKiller->pMonsterData->pAiGeneral->OwnerGUIDEq, ptKiller->pMonsterData->pAiGeneral->eOwnerTypeEq);
		if (ptParent)
			if (ptParent->dwType == UNIT_PLAYER)
				ptKiller = ptParent;
	}

	if (ptKiller->dwType == UNIT_PLAYER)
		LRoster::UpdateRoster(ptGame, ptKiller->pPlayerData->szName, 1);
	LRoster::UpdateRoster(ptGame, ptVictim->pPlayerData->szName, 2);

	*(DWORD*)&aPacket.Param1 = ptKiller->dwClassId;

	strcpy_s(aPacket.Name1, 16, ptVictim->pPlayerData->szName);
	if (ptKiller->dwType == UNIT_PLAYER)
		strcpy_s(aPacket.Name2, 16, ptKiller->pPlayerData->szName);
	else if (ptKiller->dwType == UNIT_MONSTER)
	{
		aPacket.Param2 = 1;
		if (ptKiller->pMonsterData->fSuperUniq) *(WORD*)&aPacket.Name2 = ptKiller->pMonsterData->wUniqueNo;
	}

	ptVictim->pPlayerData->tDeathTime = GetTickCount();
	SendExEvent(ptVictim->pPlayerData->pClientData, EXOP_RESPAWNTIME, Warden::getInstance().wcfgRespawnTimer);

	SPECTATOR_RemoveFromQueue(ptGame, ptVictim->dwUnitId);

	//Invisible hit
	if (ptKiller->dwType == UNIT_PLAYER)
	{
		int KillCount = 0;
		bool check = false;
		if (!ptKiller->pPlayerData->FirstKillTick) { ptKiller->pPlayerData->FirstKillTick = GetTickCount(); }
		if (GetTickCount() - ptKiller->pPlayerData->FirstKillTick < 1500) { ptKiller->pPlayerData->KillCount++; KillCount = ptKiller->pPlayerData->KillCount; }
		if (KillCount>1) { ptKiller->pPlayerData->FirstKillTick = 0; ptKiller->pPlayerData->KillCount = 0; check = true; }

		if (check)
		{
			switch (KillCount)
			{
			case 2: {SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_DOUBLEKILL, 3, -1, 150, "PODWOJNE ZABÓJSTWO!", "DOUBLE KILL!"); 	BroadcastExEvent(ptKiller->pGame, COL_WHITE, ptKiller->dwUnitId, 1, "data\\D2Ex\\Blobs"); } break;
			case 3: {SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_TRIPLEKILL, 3, -1, 150, "POTROJNE ZABÓJSTWO!", "TRIPPLE KILL!"); BroadcastExEvent(ptKiller->pGame, COL_GOLD, ptKiller->dwUnitId, 1, "data\\D2Ex\\Blobs"); } break;
			case 5:
			case 4: {SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_DOMINATION, 3, -1, 150, "ZABOJCA DRU¯YNY!", "TEAM KILL!"); BroadcastExEvent(ptKiller->pGame, COL_RED, ptKiller->dwUnitId, 1, "data\\D2Ex\\Blobs"); } break;
			}
		}

		int InRange = D2ASMFuncs::D2GAME_isUnitInRange(ptGame, ptVictim->dwUnitId, UNIT_PLAYER, ptKiller, 51);

		if (InRange) {
			SendExEvent(ptKiller->pPlayerData->pClientData, COL_YELLOW, D2EX_IMPRESSIVE, 3, -1, 150, "SNAJPER!", "SNIPER!");
			BroadcastExEvent(ptKiller->pGame, COL_WHITE, ptKiller->dwUnitId, 4, "data\\D2Ex\\Blobs");
			//BroadcastMsg(ptKiller->pGame,"%s zdobywa osiagniecie 'Snajper'",ptKiller->pPlayerData->pClientData->AccountName);
		}
		int SkillId = 0;
		Skill* pSkill = D2Funcs.D2COMMON_GetCurrentSkill(ptKiller);
		if (pSkill) SkillId = isMeleeSkill(pSkill->pTxt->wSkillId) ? pSkill->pTxt->wSkillId : 0;
		if (SkillId)
		{
			if (SkillId == D2S_JAB || SkillId == D2S_STRAFE)
			{
				SendExEvent(ptKiller->pPlayerData->pClientData, COL_PURPLE, D2EX_HUMILATION, 3, -1, 150, "LOL!", "LOL!");
				BroadcastExEvent(ptKiller->pGame, COL_WHITE, ptKiller->dwUnitId, 3, "data\\D2Ex\\Blobs");
			}
		}
	}


	if (ptKiller->dwType == UNIT_PLAYER)
	{
		if (ptGame->bFestivalMode == 1) {
			ptVictim->pPlayerData->CanAttack = 0;
			ptVictim->pPlayerData->SaidGO = 0;

			DoRoundEndStuff(ptGame, ptVictim);

			ptVictim->pPlayerData->isPlaying = 0;
			ptVictim->pGame->nPlaying--;
			if (ptVictim->pGame->nPlaying<0) ptVictim->pGame->nPlaying = 0;
		}

		//int SkillId = 0;
		//Skill* pSkill = D2Funcs.D2COMMON_GetCurrentSkill(ptKiller);
		//if(pSkill) SkillId = isMeleeSkill(pSkill->pTxt->wSkillId) ? pSkill->pTxt->wSkillId : 0;

		//if(!SkillId)
		//   { 
		//SkillId = ptVictim->pPlayerData->LastHitSkillId;
		//ptVictim->pPlayerData->LastHitSkillId = 0;
		//}
		//if(!SkillId)
		//{
		//Skill* pSkill = D2Funcs.D2COMMON_GetCurrentSkill(ptKiller);
		//if(pSkill) SkillId =  pSkill->pTxt->wSkillId;
		//}
		//ostringstream MsgEng; MsgEng << ptVictim->pPlayerData->szName  << " was slain by " << ptKiller->pPlayerData->szName << " (" << ConvertSkill(SkillId).c_str() << ')';
		//ostringstream MsgPol; MsgPol << ptVictim->pPlayerData->szName  << " - zabija cie " << ptKiller->pPlayerData->szName << " (" << ConvertSkill(SkillId).c_str() << ')';
		//BroadcastEventMsgEx(ptGame,COL_DARK_GOLD,MsgEng.str(),MsgPol.str());

		LRosterData * VictimRoster = LRoster::Find(ptGame, ptVictim->pPlayerData->szName);
		LRosterData * KillerRoster = LRoster::Find(ptGame, ptKiller->pPlayerData->szName);
		if (VictimRoster) LRoster::SyncClient(ptGame, ptVictim->dwUnitId, VictimRoster);
		if (KillerRoster) LRoster::SyncClient(ptGame, ptKiller->dwUnitId, KillerRoster);
		ptGame->LastKiller = ptKiller->pPlayerData->pClientData->ClientID;
		ptGame->LastVictim = ptVictim->pPlayerData->pClientData->ClientID;
	}
	BroadcastPacket(ptGame, (BYTE*)&aPacket, 40);
}

void __stdcall OnCreateCorpse(Game *pGame, UnitAny *pUnit, int xPos, int yPos, Room1 *pRoom)
{
	int aX, aY, aLevel;
	Room1* aRoom;

	aLevel = D2Funcs.D2COMMON_GetTownLevel(pUnit->dwAct);
	aRoom = D2Funcs.D2COMMON_GetRoomXYByLevel(pRoom->pAct, aLevel, 0, &aX, &aY, 2);
	D2Funcs.D2GAME_CreateCorpse(pGame, pUnit, aX, aY, aRoom);

}


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
	if ((pGame->bFestivalMode == 1) && (pEvent->MsgType == 0 || pEvent->MsgType == 1 || pEvent->MsgType == 3)) // Leave msgs 
	{
		DEBUGMSG("Szukam struktury ClientData z nazwa %s",pEvent->Name1);

		ClientData* pClient = FindClientDataByName(pGame,pEvent->Name1);
		if(pClient)
		{
			if(pClient->pPlayerUnit->pPlayerData->isPlaying == 0)  
				return;
			DoRoundEndStuff(pGame, pClient->pPlayerUnit);
		} else {
			DEBUGMSG("Nie znalazlem struktury WardenClient w %s", __FUNCTION__);
		}
	}

	if(pEvent->MsgType == EVENT_JOINED) // Join packet
	{
		ClientData* pClient = FindClientDataByName(pGame, pEvent->Name1);
		if(pClient) 
		{
			EVENTS_OnGameJoin(pGame, pClient);
			LRoster::SyncClient(pGame, pClient);
			LRosterData* pRoster = LRoster::Find(pGame, pEvent->Name1);
			if (pRoster && pClient->pPlayerUnit) {
				LRoster::SyncClient(pGame, pClient->pPlayerUnit->dwUnitId, pRoster);
			}
		}
		else {
			Log("OnGameJoin: Didn't find a unit!! report to lolet pls");
		}
	}
	BroadcastPacket(pGame,(BYTE*)pEvent,40);

	switch(pEvent->MsgType)
	{
		case EVENT_TIMEOUT:
		case EVENT_LEFT: 
		case EVENT_DROPPED:
		{

			DEBUGMSG("[REMOVECLIENT] Removing (*%s) %s from WardenQueue", pEvent->Name2, pEvent->Name1);
			WardenClient_i pWardenClient = Warden::getInstance().findClientByName(pEvent->Name1);
			if(pWardenClient == Warden::getInstance().getInvalidClient()) 
			{
				DEBUGMSG("[REMOVECLIENT] Failed to find WardenClient!");
				return;
			}
			SPECTATOR_RemoveFromQueue(pGame, pWardenClient->ptClientData->UnitId);
			Warden::getInstance().onRemoveClient(pWardenClient);

		}
		break;
	}

}



void DoRoundEndStuff(Game* pGame, UnitAny* pUnit) //Sprawdz czy wskaznik jest ok przed wywolaniem, WardenClient = Victim
{
	DEBUGMSG("DEBUG: Doing DoRoundEndStuff");
	if(!pGame || !pUnit) return;

	std::ostringstream str;
	std::string wynik;

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