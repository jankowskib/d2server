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

#ifndef __LQUESTS_H__
#define __LQUESTS_H__

int __fastcall DYES_Prepare(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
int __fastcall DYES_Colorize(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);
BYTE * __stdcall DYES_DrawItem(UnitAny *ptPlayer, UnitAny* ptItem, BYTE* out, BOOL a4);

void QUESTS_UpdateUnit(UnitAny* pUnit, WORD UpdateType, UnitAny*pUpdateUnit);
BOOL __stdcall QUESTS_CowLevelOpenPortal(Game *pGame, UnitAny *pUnit);
bool __stdcall QUESTS_OnUseItem(Game* pGame, UnitAny* pUnit, UnitAny* pItem, DWORD dwItemCode);

#endif