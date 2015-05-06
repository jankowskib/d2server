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

#ifndef D2LITEMS_H__
#define D2LITEMS_H__

bool FG_WaitForMySQLServer(unsigned int msTimeout);
void FG_ConnectToSQL(std::string szServer, std::string szUser, std::string szPass, std::string szDatabase);

BOOL __fastcall ITEMS_BuyItem(UnitAny *pPlayer, int nCost);
void __fastcall ITEMS_RollbackStat(int nStat, UnitAny *pUnit, int nValue);
int __stdcall GetGoldBankLimit(UnitAny *pUnit);
signed int GetStatSigned(UnitAny *pUnit, int nStat, int nLayer);
int __stdcall ITEMS_OnStatFetch(UnitAny *pUnit, int nStat, WORD nLayer);
int __stdcall ITEMS_GetItemCost(UnitAny *pPlayer, UnitAny *ptItem, int DiffLvl, QuestFlags *pQuestFlags, int NpcClassId, int InvPage);

//-------- don't break it accidently
UnitAny*
#ifdef VER_111B
__stdcall
#else
__fastcall
#endif
ITEMS_AddKillerId(Game *pGame, PresetItem  *srCreateItem, int a5);
//-----------
void CreateFFAItems(UnitAny* pUnit);
bool ParseItemsCmds(UnitAny* pUnit, char* str, char *t);

#endif