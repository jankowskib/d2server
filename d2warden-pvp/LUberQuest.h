/* ==========================================================
* d2warden
* https://github.com/lolet/d2warden
* ==========================================================
* Copyright 2011-2015 Bartosz Jankowski
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

#ifndef LUBERQUEST_H__
#define LUBERQUEST_H__

void __stdcall UBERQUEST_SpawnMonsters(Game* pGame);
BOOL UBERQUEST_OpenPandemoniumPortal(Game *pGame, UnitAny *pPlayer);
BOOL UBERQUEST_OpenPandemoniumFinalPortal(Game *pGame, UnitAny *pPlayer);

#endif