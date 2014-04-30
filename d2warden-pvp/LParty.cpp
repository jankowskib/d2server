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
#include "LParty.h"
#include "LSpectator.h"

#include "process.h"

/*
	Wrapper for 0x5E
*/
DWORD __fastcall OnPartyRelationChange(Game* pGame, UnitAny* pPlayer, BYTE *pPacket, int nLen)
{
	px5e * p = (px5e*)pPacket;

	DEBUGMSG("Entered %s, Button %d", __FUNCTION__, p->nButton)

	if (nLen != 6)
		return 3;

	ASSERT(pGame)
	Arena* pArena = pGame->pArenaControl;
	ASSERT(pArena);

	if (pArena->ArenaFlags & 0x10000)
		return 3;
	 
	if (p->nButton > 10)
		return 2;

	UnitAny* pDestUnit = D2ASMFuncs::D2GAME_FindUnit(pGame, p->dwUnitId, UNIT_PLAYER);
	if (!pDestUnit)
		return 3;

	typedef void* (__fastcall * fnPartyButtons_t)(Game*, UnitAny* pRequester, UnitAny* pDestUnit, int state, int* result);
	fnPartyButtons_t* fnPartyButtons = (fnPartyButtons_t*)D2Vars.D2GAME_PartyButtonCallbacks;

	if (p->nButton < 10)
	{
		if (fnPartyButtons[p->nButton])
			fnPartyButtons[p->nButton](pGame, pPlayer, pDestUnit, 0, 0);
	}
	else if (p->nButton == PB_SPECATE)
	{
		return OnClickSpecate(pGame, pPlayer, pDestUnit);
	}
	return 0;
}