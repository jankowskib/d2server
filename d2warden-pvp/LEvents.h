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

#ifndef LEVENTS_H__
#define LEVENTS_H__

void __stdcall OnBroadcastEvent(Game* pGame, EventPacket * pEvent);
DWORD __fastcall OnResurrect(Game *pGame, UnitAny *pPlayer, BYTE *aPacket, int PacketSize);
void __stdcall OnDeath(UnitAny* ptKiller, UnitAny * ptVictim, Game * ptGame);
void __stdcall OnCreateCorpse(Game *pGame, UnitAny *pUnit, int xPos, int yPos, Room1 *pRoom);

void DoRoundEndStuff(Game* pGame, UnitAny* pUnit);
#endif