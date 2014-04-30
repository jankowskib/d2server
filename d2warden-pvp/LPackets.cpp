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