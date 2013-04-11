/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2013 lolet
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

#ifndef D2PARTYEXP_H__
#define D2PARTYEXP_H__

int nPlayersFormula();
int __inline GetRange(int X1, int Y1, int X2, int Y2) { return ((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2)); }
SubParty* GetSubPartyById(Game *pGame, short PartyId);
void __fastcall ExpCallback(Game *pGame, UnitAny *pPartyMember, PartyExp *pnPartyExp); // oryginalna funkcja 0x6FC9A280
void __stdcall ExpShare(UnitAny *pPlayer, Game *pGame, UnitAny *pMonster, int PlayerLvl, int MonsterLvl, int PlayerExp); // orginalna funkcja 0x6FC9DFD0
void __stdcall ExpShare_NEW(UnitAny *pPlayer, Game *pGame, UnitAny *pMonster, int PlayerLvl, int MonsterLvl, int PlayerExp);

//
//
//void __userpurge sub_6FC9DDB0(UnitAny *pPlayer<eax>, UnitAny *pePlayer<ebx>, Game *pGame<ebp>, Game *peGame, int PlayerLvl, int ExpGained)
//{
//  UnitAny *ppPlayer; // esi@1
//  int NextLvlExp; // eax@3
//  int v8; // ebx@3
//  int v9; // ebp@3
//  int NewExp; // edi@3
//  int CurrentExp; // eax@3
//  int v12; // eax@3
//  double v13; // st7@6
//  int v14; // ST2C_4@6
//  int v15; // [sp+2Ch] [bp-10h]@5
//
//  ppPlayer = pPlayer;
//  if ( pPlayer )
//  {
//    if ( !pPlayer->dwType )
//    {
//      CurrentExp = D2Common_GetBaseStatSigned_10550(pPlayer, STAT_EXPERIENCE, 0);
//      v8 = ppPlayer->dwClassId;
//      v9 = CurrentExp;
//      NewExp = ExpGained + CurrentExp;
//      v12 = D2Common_GetMaxLvl_11123(ppPlayer->dwClassId);
//      NextLvlExp = D2Common_GetExpToAchiveLvl_10152(v8, v12 - 1);
//      if ( NewExp > (unsigned int)NextLvlExp )
//        NewExp = NextLvlExp;
//      D2Common_SetStat_10590(ppPlayer, STAT_LASTEXP, NewExp - v9, 0);
//      D2Common_SetStat_10590(ppPlayer, STAT_EXPERIENCE, NewExp, 0);
//      if ( PlayerLvl != D2Common_GetNextCLvl_10988(v8, NewExp) )
//      {
//        sub_6FC99540(ppPlayer, v13, (int)peGame, v15);
//        ExcecuteEvent(peGame, 12, 0, 0, v14);
//      }
//    }
//  }
//}

#endif