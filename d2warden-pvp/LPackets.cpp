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
* ========================================================== */

#include "stdafx.h"
#include "LPackets.h"
#include "LSpectator.h"

/*
	Replacement for D2GAME.0x673A0
	(BYTE *pPacket<ebx>, UnitAny *pUnit<esi>, Game *pGame, int nPacketLen)
*/
int __stdcall OnPacketReceive(BYTE *pPacket, UnitAny *pUnit, Game *pGame, int nPacketLen)
{
	BYTE pType = pPacket[0];
	if (pType > 0x66)
		return 3;
	PacketTbl* cbCallback = (PacketTbl*)&D2Vars.D2GAME_ClientPacketTable[pType];

	if (!cbCallback->Callback)
	{
		Log("WARNING: No action taken for received packet id (0x%x). Reason: No callback", pType);
		return 3;
	}

	switch (pType)
	{
		case 0x14: // Overhead chat
		case 0x15: // Chat
		case 0x3C: // Select skill
		case 0x41: // Ressurect
		case 0x43: // Unknown
		case 0x66: // Warden packet
		{
			if (pType == 0x41)
				if (!pUnit || pUnit->dwMode != PLAYER_MODE_DEAD)
					return 0;

			if (pType == 0x20 || pType == 0x26)
			{
			}

			if (pGame->nSyncTimer > 1)
				pGame->nSyncTimer = D2Funcs.FOG_GetTime();

			return cbCallback->Callback(pGame, pUnit, pPacket, nPacketLen);
		}
		case 0x5E: // Party relation
		{
			px5e * p = (px5e*)pPacket;
			if (nPacketLen != sizeof(px5e))
			{
				if (pUnit)
					Log("HACK: Malformed packet 0x5E. **VERY SUSPECT**. Received from *%s", pUnit->pPlayerData->pClientData->AccountName);
				return 3;
			}
			if (p->nButton != PB_SPECATE)
			{
				if (!pUnit || pUnit->dwMode == PLAYER_MODE_DEAD || pUnit->dwMode == PLAYER_MODE_DEATH || D2Funcs.D2COMMON_GetUnitState(pUnit, uninterruptable))
					return 0;
			}
			else
			{
				if (!pUnit || pUnit->dwMode == PLAYER_MODE_DEAD || pUnit->dwMode == PLAYER_MODE_DEATH)
					return 0;
			}

			if (pGame->nSyncTimer > 1)
				pGame->nSyncTimer = D2Funcs.FOG_GetTime();

			return cbCallback->Callback(pGame, pUnit, pPacket, nPacketLen);
		}
		default:
		{
			if (!pUnit || pUnit->dwMode == PLAYER_MODE_DEAD || pUnit->dwMode == PLAYER_MODE_DEATH || D2Funcs.D2COMMON_GetUnitState(pUnit, uninterruptable))
				return 0;

			if (pGame->nSyncTimer > 1)
				pGame->nSyncTimer = D2Funcs.FOG_GetTime();

			return cbCallback->Callback(pGame, pUnit, pPacket, nPacketLen);
		}
	}

	return 3;
}


//Valid for 0x06*, 0x07, 0x09*, 0x0A,  || 0x0D*, 0x0E, 0x10*, 0x11
DWORD __fastcall OnClickUnit(Game* pGame, UnitAny* pPlayer, SkillTargetPacket *ptPacket, DWORD PacketLen)
{
	int InRange = 0;
	if (PacketLen != 9)
		return 3;
	if (!pGame)
		return 3;
	if (pPlayer->dwType != UNIT_PLAYER)
		return 3;
	if (ptPacket->UnitType > 5)
		return 3;

	InRange = D2ASMFuncs::D2GAME_isUnitInRange(pGame, ptPacket->UnitId, ptPacket->UnitType, pPlayer, 50);
	if (InRange == 2)
	{
		DEBUGMSG("OnClickUnit: Over the range (2)")
			return MSG_ERROR;
	}
	if (InRange == 3)
	{
		DEBUGMSG("OnClickUnit: Over the range (3)");
		return MSG_HACK;
	}
	Skill * ptSkill = (ptPacket->Header == 6 || ptPacket->Header == 7 || ptPacket->Header == 9 || ptPacket->Header == 0xA) ? D2Funcs.D2COMMON_GetLeftSkill(pPlayer) : D2Funcs.D2COMMON_GetRightSkill(pPlayer);
	if (!ptSkill)
	{
		DEBUGMSG("%s: ptSkill not found!. Packet id is = %d", __FUNCTION__, ptPacket->Header);
		return 3;
	}
	int SkillId = D2Funcs.D2COMMON_GetSkillId(ptSkill, __FILE__, __LINE__);

	PlayerData* pPlayerData = pPlayer->pPlayerData;
	if (!pPlayerData) return 2;

	if (TeleChars[pPlayer->dwClassId] == FALSE && pPlayer->pGame->dwGameState == 0 && SkillId == 0x36)
	{
		SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Teleport nie jest dozwolony dla tej klasy!" : "Teleport Is Not Allowed For This Character");
		return 0;
	}

	if (SkillId == 0x65 && !wcfgAllowHB)
	{
		SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Swiety pocisk jest zabroniony na tym serwerze" : "Holy Bolt Is Not Allowed On This Server");
		return 0;
	}
	if (SkillId == 151 && !wcfgAllowNLWW)
	{
		SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "NLWW jest zabronione na tym serwerze" : "NLWW Is Not Allowed On This Server");
		return 0;
	}

	static int AttackCount;

	if (pGame->bFestivalMode == 1 && pPlayerData->CanAttack == 0 && (!isSafeSkill(SkillId) && SkillId != D2S_CHARGE))
	{
		if (AttackCount == 0)
		{
			SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Wpisz #go aby rozpoczac runde!" : "Type #go to start round");
		}
		AttackCount++;
		if (AttackCount > 4) AttackCount = 0;
		return 0;
	}


	int nPierceIdx = D2Funcs.D2COMMON_GetBaseStatSigned(pPlayer, 328, 0);
	D2Funcs.D2COMMON_SetStat(pPlayer, 328, nPierceIdx + 1, 0);

	D2ASMFuncs::D2GAME_CastSkillOnUnit(pPlayer, ptSkill, pGame, ptPacket->UnitType, ptPacket->UnitId, (ptPacket->Header == 6 || ptPacket->Header == 9 || ptPacket->Header == 0xD || ptPacket->Header == 0x10) ? 1 : 0);

	UnitAny * pDest = D2ASMFuncs::D2GAME_FindUnit(pGame, ptPacket->UnitId, ptPacket->UnitType);
	if (pDest)
	{
		int mX = D2Funcs.D2COMMON_GetPathX(pDest->pPath);
		int mY = D2Funcs.D2COMMON_GetPathY(pDest->pPath);

		SPECTATOR_UpdatePositions(pGame, pPlayer, mX, mY);
	}
	else
	{
		DEBUGMSG("WARNING: pDest in %s was not found!", __FUNCTION__)
	}
	return 0;
}


DWORD __fastcall OnClickLocation(Game* pGame, UnitAny* pPlayer, SkillPacket *ptPacket, DWORD PacketLen)
{
	bool InRange = false;
	static int AttackCount;
	if (PacketLen != 5) return MSG_HACK; // zwroc hack
	if (!pGame) return MSG_HACK;
	if (pPlayer->dwType != UNIT_PLAYER) return MSG_HACK;

	WORD UnitX = D2Funcs.D2GAME_GetUnitX(pPlayer);
	WORD UnitY = D2Funcs.D2GAME_GetUnitY(pPlayer);

	int xOffset = UnitX - ptPacket->xPos;
	if (xOffset < 0) xOffset = -xOffset;
	int yOffset = UnitY - ptPacket->yPos;
	if (yOffset < 0) yOffset = -yOffset;

	if (yOffset < 50 && xOffset < 50) InRange = true;

	PlayerData* pPlayerData = pPlayer->pPlayerData;
	if (!pPlayerData)
	{
		DEBUGMSG("Didn't find a PlayerData, function %s, %d", __FUNCTION__, __LINE__);
		return MSG_ERROR;
	}

	if (InRange)
	{
		pPlayerData->GameFrame = pGame->GameFrame;
		Skill * ptSkill = (ptPacket->Header == 5 || ptPacket->Header == 8) ? D2Funcs.D2COMMON_GetLeftSkill(pPlayer) : D2Funcs.D2COMMON_GetRightSkill(pPlayer);
		if (!ptSkill) return 3;
		int SkillId = D2Funcs.D2COMMON_GetSkillId(ptSkill, __FILE__, __LINE__);

		if (TeleChars[pPlayer->dwClassId] == FALSE && pPlayer->pGame->dwGameState == 0 && SkillId == D2S_TELEPORT)
		{
			SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Teleport nie jest dozwolony dla tej klasy!" : "Teleport Is Not Allowed For This Character");
			return 0;
		}

		if (SkillId == D2S_HOLYBOLT && !wcfgAllowHB) {
			SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Swiety pocisk jest zabroniony na tym serwerze" : "Holy Bolt Is Not Allowed On This Server");
			return 0;
		}

		int nPierceIdx = D2Funcs.D2COMMON_GetBaseStatSigned(pPlayer, 328, 0);
		D2Funcs.D2COMMON_SetStat(pPlayer, 328, nPierceIdx + 1, 0);

		if (pGame->bFestivalMode && !pPlayerData->CanAttack && !isSafeSkill(SkillId))
		{
			if (AttackCount == 0)
			{
				SendMsgToClient(pPlayerData->pClientData, pPlayerData->pClientData->LocaleID == 10 ? "Wpisz #go aby rozpoczac runde!" : "Type #go to start round");
			}
			AttackCount++;
			if (AttackCount > 4)
				AttackCount = 0;
			return 0;
		}

		D2ASMFuncs::D2GAME_CastSkill(pPlayer, ptSkill, pGame, ptPacket->xPos, ptPacket->yPos);
		if (SkillId == D2S_TELEPORT || SkillId == D2S_CHARGE || SkillId == D2S_WHIRLWIND)
		SPECTATOR_UpdatePositions(pGame, pPlayer, ptPacket->xPos, ptPacket->yPos);
		else
		SPECTATOR_UpdatePositions(pGame, pPlayer, UnitX, UnitY);

		if (!wcfgDetectTrick)
			return 0;

		WardenClient_i ptWardenClient = GetClientByID(pPlayerData->pClientData->ClientID);
		if (ptWardenClient == hWarden.Clients.end()) return 0;

		if (GetTickCount() > ptWardenClient->UIModesTime + 500) { UNLOCK return 0; }

		if ((ptWardenClient->UIModes[UI_CHARACTER] || ptWardenClient->UIModes[UI_QUEST]) && (ptWardenClient->MouseXPosition >= 0 && ptWardenClient->MouseXPosition <= 200) && (ptWardenClient->MouseYPosition >= 0 && ptWardenClient->MouseYPosition <= 550))
		{
			if (ptWardenClient->DebugTrick)
				SendMsgToClient(ptWardenClient->ptClientData, "Trick (Left window) X=%d Y=%d, LAG= %d ms", ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition, GetTickCount() - ptWardenClient->UIModesTime);
			Log("HACK: %s (*%s) used Polish GA Trick [%s]!, skill : %s XY=[%d,%d]", ptWardenClient->CharName.c_str(), ptWardenClient->AccountName.c_str(), ptWardenClient->UIModes[UI_CHARACTER] ? "Character Stats" : "Quests", ConvertSkill(SkillId).c_str(), ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition);
		}
		else
			if ((ptWardenClient->UIModes[UI_INVENTORY] || ptWardenClient->UIModes[UI_SKILL]) && (ptWardenClient->MouseXPosition >= 600 && ptWardenClient->MouseXPosition <= 800) && (ptWardenClient->MouseYPosition >= 0 && ptWardenClient->MouseYPosition <= 550))
			{
				if (ptWardenClient->DebugTrick)
					SendMsgToClient(ptWardenClient->ptClientData, "Trick (Right window) X=%d Y=%d, LAG = %d ms", ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition, GetTickCount() - ptWardenClient->UIModesTime);
				Log("HACK: %s (*%s) used Polish GA Trick [%s]!, skill : %s XY=[%d,%d]", ptWardenClient->CharName.c_str(), ptWardenClient->AccountName.c_str(), ptWardenClient->UIModes[UI_INVENTORY] ? "Inventory" : "Skill Tree", ConvertSkill(SkillId).c_str(), ptWardenClient->MouseXPosition, ptWardenClient->MouseYPosition);
			}
		UNLOCK
			return 0;
	}
	else
	{
		if ((signed int)(pGame->GameFrame - pPlayerData->GameFrame) > 25)
		{
			ClientData * pClient = pPlayerData->pClientData;
			if (pClient)
			{
				ReassignPacket hReassign = { 0 };
				hReassign.Header = 0x15;
				hReassign.UnitId = pPlayer->dwUnitId;
				hReassign.xPos = UnitX;
				hReassign.yPos = UnitY;
				hReassign.Reassign = 1;

				D2ASMFuncs::D2GAME_SendPacket(pClient, (BYTE*)&hReassign, 11);
			}
		}
		return 1;
	}
	return MSG_HACK;
}

DWORD __fastcall OnRunToLocation(Game* pGame, UnitAny* pPlayer, SkillPacket *ptPacket, DWORD PacketLen)
{
	bool InRange = false;
	if (PacketLen != 5) return MSG_HACK; // zwroc hack
	if (!pGame) return MSG_HACK;
	if (!pPlayer || pPlayer->dwType != UNIT_PLAYER) return MSG_HACK;

	WORD UnitX = D2Funcs.D2GAME_GetUnitX(pPlayer);
	WORD UnitY = D2Funcs.D2GAME_GetUnitY(pPlayer);

	int xOffset = UnitX - ptPacket->xPos;
	if (xOffset < 0) xOffset = -xOffset;
	int yOffset = UnitY - ptPacket->yPos;
	if (yOffset < 0) yOffset = -yOffset;

	if (yOffset < 50 && xOffset < 50) InRange = true;

	PlayerData* pPlayerData = pPlayer->pPlayerData;
	if (!pPlayerData)
	{
		Log("Didn't find a PlayerData, function %s, %d", __FUNCTION__, __LINE__);
		return MSG_ERROR;
	}
	if (pPlayerData->pTrade)
	{
		WardenClient_i ptWardenClient = GetClientByID(pPlayerData->pClientData->ClientID);
		if (ptWardenClient == hWarden.Clients.end()) return MSG_OK;
		if (!ptWardenClient->DupeDetected)
		{
			Log("HACK: %s (*%s) tryed to move while in trade [Probably *nice* dupe try]", ptWardenClient->CharName.c_str(), ptWardenClient->AccountName.c_str());
			ptWardenClient->DupeDetected = 1;
		}
		UNLOCK
		return MSG_HACK;
	}
	if (InRange)
	{
		SPECTATOR_UpdatePositions(pGame, pPlayer, UnitX, UnitY);
		pPlayerData->GameFrame = pGame->GameFrame;
		D2Funcs.D2GAME_SetUnitModeXY(pGame, pPlayer, 0, PLAYER_MODE_RUN, ptPacket->xPos, ptPacket->yPos, 0);
		return 0;
	}
	else
	{
		if ((signed int)(pGame->GameFrame - pPlayerData->GameFrame) > 25)
		{
			ClientData * pClient = pPlayerData->pClientData;
			if (pClient)
			{
				ReassignPacket hReassign = { 0 };
				hReassign.Header = 0x15;
				hReassign.UnitId = pPlayer->dwUnitId;
				hReassign.xPos = UnitX;
				hReassign.yPos = UnitY;
				hReassign.Reassign = 1;

				D2ASMFuncs::D2GAME_SendPacket(pClient, (BYTE*)&hReassign, 11);
			}
		}
		return MSG_UNK;
	}
	return MSG_HACK;
}
