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

#ifndef D2WE_H__
#define D2WE_H__

bool WE_isKey(UnitAny* ptItem);
void WE_UpdateCounter(int Value);
void WE_GenerateNextDC();
void WE_CreateDCKey(UnitAny* pUnit);
void WE_Inform(Game* ptGame);
void WE_Inform2(Game* ptGame);
int __fastcall WE_Spawn(Game *pGame, UnitAny *pUnit, UnitAny *pScroll, UnitAny *ptItem, int a5, int a6, int SkillId);

#endif
