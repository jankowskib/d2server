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

#ifndef SPECTATOR_H__
#define SPECTATOR_H__

DWORD OnClickSpecate(Game *pGame, UnitAny* pWatcher, UnitAny* pUnit);

DWORD __fastcall OnRunToLocation(Game* ptGame, UnitAny* ptPlayer, SkillPacket *ptPacket, DWORD PacketLen);
DWORD __fastcall OnClickLocation(Game* ptGame, UnitAny* ptPlayer, SkillPacket *ptPacket, DWORD PacketLen);
DWORD __fastcall OnClickUnit(Game* ptGame, UnitAny* ptPlayer, SkillTargetPacket *ptPacket, DWORD PacketLen);

#endif