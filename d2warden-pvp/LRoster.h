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

#ifndef LRosterData_H__
#define LRosterData_H__

void __stdcall OnCreateDamage(UnitAny* pDefender, Damage* pDamage, UnitAny* pMissile);
void __fastcall OnGameDestroy(Game* ptGame);
void __fastcall PLAYERMODES_0_Death(Game *pGame, UnitAny *pVictim, int nMode, UnitAny *pKiller);
void COMBAT_Free(Game* pGame, UnitAny* pUnit);

namespace LRoster
{
	void SyncClientRunaways(Game *ptGame, DWORD UnitId, LRosterData* pRoster);
	void SyncClient(Game *ptGame, ClientData* ptClient);
	void SyncClient(Game *ptGame, DWORD UnitId, LRosterData* pRoster);
	LRosterData* Find(Game * ptGame, char* szName);
	void UpdateRoster(Game * ptGame,char * szName, BYTE Type);
	void Clear(Game * ptGame);
}

#endif