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

#ifndef D2NODES_H__
#define D2NODES_H__

void __fastcall NODES_Free(Game *ptGame);
void __fastcall NODES_AddAsParentNode(Game *ptGame, UnitAny *ptUnit, UnitNode *ptNode, int NodeIdx);
void __fastcall NODES_SetUnitNode(Game *ptGame, UnitAny *ptUnit, UnitNode* ptNode, int NodeIdx);
UnitNode *__fastcall NODES_GetUnitNode(UnitAny *ptUnit, Game *ptGame);
void __fastcall NODES_AssignNode(Game *ptGame, UnitAny *ptUnit, UnitNode* pChild);
void __fastcall NODES_FreeChildNode(Game *ptGame, UnitAny *ptUnit);
void __fastcall NODES_FreeUnitNode(Game* ptGame, UnitAny* ptUnit);
UnitAny *__stdcall NODES_BaalCheck(Game *ptGame, UnitAny *ptUnit, int *xPos, int *yPos, int aZero, int (__fastcall *pfnCallback)(UnitAny*, UnitAny*));  //0x6FC4A790
UnitAny *__stdcall NODES_NormalCheck(Game *ptGame, UnitAny *ptUnit, int *xPos, int *yPos, int aZero, int (__fastcall *pfnCallback)(UnitAny*, UnitAny*));  //0x6FC4A790


#endif