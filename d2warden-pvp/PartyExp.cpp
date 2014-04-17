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

#include "stdafx.h"
#include "PartyExp.h"


//int __fastcall EXPERIENCE_CalculateDecay(int nAttackerLevel, int nDefenderLevel)
//{
//   if(nAttackerLevel == nDefenderLevel || nAttackerLevel == 1)
//      return 100;
//
//   int nDifferance = nAttackerLevel - nDefenderLevel;
//   if(nDifferance < EXPERIENCE_DECAY_START)
//      return 100;
//
//   return (nDifferance > 100 / EXPERIENCE_DECAY_AMOUNT) ? 0 : 100 - (nDifferance * EXPERIENCE_DECAY_AMOUNT);   
//}
//
//DWORD __fastcall EXPERIENCE_Calculate(Game* pGame, UnitAny* pAttacker, int nAttackerLevel, int nDefenderLevel, DWORD dwExp)
//{
//   if(pGame == NULL || pAttacker == NULL || nAttackerLevel < 1 || nDefenderLevel < 1 || dwExp == 0)
//      return EXPERIENCE_MIN_GAIN;
//
//   if(nAttackerLevel >= PLAYERS_GetMaxLevel(pAttacker))
//      return EXPERIENCE_MIN_GAIN;
//
//   int nRatio = PLAYERS_GetExperienceRatio(pAttacker,nAttackerLevel);
//   dwExp = INLINE_ApplyPercent(dwExp,100,EXPERIENCE_CalculateDecay(nAttackerLevel,nDefenderLevel));
//   return EXPERIENCE_Normalize((dwExp * nRatio) / 1024);
//}



SubParty* GetSubPartyById(Game *pGame, short PartyId)
{
	if(!pGame || PartyId == -1) return 0;

	Party * pParty = pGame->pPartyControl;
	if(!pParty) return 0;

	for(SubParty* pSub = pParty->ptLastParty; pSub; pSub = pSub->ptPrev)
	{
		if(pSub->PartyId == PartyId) return pSub;
	}
	return 0;
}

int __fastcall EXPFormula(int nPlayers)
{
	return 50 * (nPlayers - 1);
}

__declspec(naked) int nPlayersFormula()
{
	__asm
	{
		push ecx
		mov ecx, eax

		call EXPFormula

		pop ecx
		ret
	}
}

void __fastcall ExpCallback(Game *pGame, UnitAny *pPartyMember, PartyExp *pnPartyExp)
{
	if(pPartyMember && !((pPartyMember->dwFlags) & 0x00010000))
		if((pPartyMember->dwType == UNIT_MONSTER && pPartyMember->dwMode != NPC_MODE_DEATH && pPartyMember->dwMode != NPC_MODE_DEAD) ||
			(pPartyMember->dwType == UNIT_PLAYER && pPartyMember->dwMode != PLAYER_MODE_DEATH && pPartyMember->dwMode != PLAYER_MODE_DEAD))
		{

			UnitAny* pMonster = pnPartyExp->pMonster;
			int mX = D2Funcs.D2GAME_GetUnitX(pMonster);
			int mY = D2Funcs.D2GAME_GetUnitY(pMonster);

			int pX = D2Funcs.D2GAME_GetUnitX(pPartyMember);
			int pY = D2Funcs.D2GAME_GetUnitY(pPartyMember);

			if(GetRange(pX,pY,mX,mY)>wcfgExpRange && wcfgExpRange != -1) return;

			int nMembers = pnPartyExp->nMembers;
			if(nMembers>wcfgMaxPlayers)
			{
				D2ERROR("nMember>wcfgMaxPlayers")
			}
			pnPartyExp->pPartyMembers[nMembers] = pPartyMember;
			pnPartyExp->PartyLevels[nMembers] =  D2Funcs.D2COMMON_GetStatSigned(pPartyMember,STAT_LEVEL,0);
			pnPartyExp->LvlSum += pnPartyExp->PartyLevels[nMembers];
			pnPartyExp->nMembers++;
			return;
		}


}


void AddExp(Game *pGame, UnitAny* pMember, int ExpGained, int ExpLvl)
{
			int CurrentExp = D2Funcs.D2COMMON_GetBaseStatSigned(pMember, STAT_EXP,0);
			unsigned int NewExp = ExpGained + CurrentExp;
			int MaxLvl = D2Funcs.D2COMMON_GetMaxCLvl(pMember->dwClassId);
			unsigned int MaxExp = D2Funcs.D2COMMON_GetExpToAchiveLvl(pMember->dwClassId, MaxLvl - 1);

			if ( NewExp > MaxExp ) NewExp = MaxExp;
			D2Funcs.D2COMMON_SetStat(pMember, 0x1D, ExpGained, 0); // LAST EXP
			D2Funcs.D2COMMON_SetStat(pMember, STAT_EXP, NewExp, 0);
				if ( ExpLvl != D2Funcs.D2COMMON_GetNextCLvl(pMember->dwClassId, NewExp) )
				{
				D2ASMFuncs::D2GAME_LevelAwards(pMember, pGame);
				D2Funcs.D2GAME_ExecuteEvent(pGame, 12, 0, 0);
				}
}

void __stdcall ExpShare_NEW(UnitAny *pPlayer, Game *pGame, UnitAny *pMonster, int PlayerLvl, int MonsterLvl, int PlayerExp)
{
	PartyExp hPartyExp;
	::memset(&hPartyExp,0,sizeof(hPartyExp));
	hPartyExp.pMonster = pMonster;
	D2ASMFuncs::D2GAME_ForEachInParty(pPlayer, pGame, (void(__fastcall*)(Game*, UnitAny*, void*))&ExpCallback, &hPartyExp);
	if(hPartyExp.nMembers > 0)
	{
		if(hPartyExp.LvlSum > 0)
		{
			if(hPartyExp.nMembers == 1)
			{
				int ExpGained = D2ASMFuncs::D2GAME_GetExpGained(PlayerExp, pPlayer, PlayerLvl, pGame, MonsterLvl);
				AddExp(pGame,pPlayer,ExpGained,PlayerLvl);	
		//		SendMsgToClient(pPlayer->pPlayerData->pClientData,"ALONE: %s, EXP: %d",pPlayer->pPlayerData->szName,PlayerExp);
			}
			else
			{

				float PExp = float((abs((((int)hPartyExp.nMembers  - 1) * PlayerExp))) + PlayerExp) / float(hPartyExp.LvlSum);

				for(int i = 0; i<(signed int)hPartyExp.nMembers; ++i)
				{
				int PLvl = hPartyExp.PartyLevels[i];
				int nGain = int(float(PLvl) * PExp);
				UnitAny* pMember = hPartyExp.pPartyMembers[i]; 

				int ExpGained = D2ASMFuncs::D2GAME_GetExpGained(nGain, pMember, PLvl, pGame, MonsterLvl);
				AddExp(pGame,pMember,ExpGained,PLvl);
				//SendMsgToClient(pPlayer->pPlayerData->pClientData,"SHARE: %s, EXP: %d,Party EXP %d",pPlayer->pPlayerData->szName,PlayerExp, nGain);
				}
			}
		}
	}

}

//void __stdcall ExpShare(UnitAny *pPlayer, Game *pGame, UnitAny *pMonster, int PlayerLvl, int MonsterLvl, int PlayerExp)
//{
//	if(!pPlayer || !pGame || !pMonster) return;
//	if(pPlayer->dwType) return;
//	Room1 * aRoom = D2Funcs.D2COMMON_GetUnitRoom(pPlayer);
//	if(!aRoom) return;
//	int LevelNo = D2Funcs.D2COMMON_GetLevelNoByRoom(aRoom);
//	short TeamId = D2Funcs.D2GAME_GetPartyID(pPlayer,pGame);
//
//	int mX = D2Funcs.D2GAME_GetUnitX(pMonster);
//	int mY = D2Funcs.D2GAME_GetUnitY(pMonster);
//
//	if(TeamId!=-1)
//	{
//		int LevelSum = 0;
//		int nMembers = 0;
//		SubParty * pParty = GetSubPartyById(pGame,TeamId);
//		if(!pParty) return;
//		for(PartyMembers * i = pParty->ptMembers; i; i = i->ptNext)
//		{
//			UnitAny * pMember = D2ASMFuncs::D2GAME_FindUnit(pGame,i->dwUnitId,0);
//			if(!pMember) continue;
//			nMembers++;
//			LevelSum+= D2Funcs.D2COMMON_GetStatSigned(pMember,STAT_LEVEL,0);
//		}
//
//		if(nMembers == 0) return;
//		if(LevelSum == 0) return;
//
//		for(PartyMembers * i = pParty->ptMembers; i; i = i->ptNext)
//		{
//			UnitAny * pMember = D2ASMFuncs::D2GAME_FindUnit(pGame,i->dwUnitId,0);
//			if(!pMember) continue;
//			Room1 * aPartyRoom = D2Funcs.D2COMMON_GetUnitRoom(pMember);
//				if(!aPartyRoom) continue;
//			int PartyLevelNo = D2Funcs.D2COMMON_GetLevelNoByRoom(aPartyRoom);
//			if(PartyLevelNo != LevelNo) continue;
//				
//			int pX = D2Funcs.D2GAME_GetUnitX(pMember);
//			int pY = D2Funcs.D2GAME_GetUnitY(pMember);
//			if(GetRange(pX,pY,mX,mY)>wcfgExpRange) continue;	//Nasz zasieg expa
//			if(pMember->dwMode && pMember->dwMode!=PLAYER_MODE_DEAD)
//			{
//			int ExpLvl = D2Funcs.D2COMMON_GetStatSigned(pMember,STAT_LEVEL,0);
//			double PExp = (double)(PlayerExp + 89 * PlayerExp * (nMembers - 1)) / 256;
//			PExp = PExp / (double)LevelSum;
//			int ExpGained = D2Funcs.D2GAME_GetExpGained(PlayerExp+(int)(PExp * ExpLvl),pMember,ExpLvl,pGame,MonsterLvl); // Tu moze byc crash
//		//	Log("TEAM: PlayerExp = %d PExp = %f, LevelSum = %d, nMem = %d, Pexp* ExpLvl = %d, ExpGained = %d",PlayerExp,PExp,LevelSum,nMembers,(int)(PExp*ExpLvl),ExpGained);
//
//			int CurrentExp = D2Funcs.D2COMMON_GetBaseStatSigned(pMember, STAT_EXP,0);
//			unsigned int NewExp = ExpGained + CurrentExp;
//			int MaxLvl = D2Funcs.D2COMMON_GetMaxCLvl(pMember->dwClassId);
//			unsigned int MaxExp = D2Funcs.D2COMMON_GetExpToAchiveLvl(pMember->dwClassId, MaxLvl - 1);
//
//		//	Log("CurrentExp =%d, NewExp = %d, MaxLvl =%d, MaxExp =%u",CurrentExp,NewExp,MaxLvl,MaxExp);
//			if ( NewExp > MaxExp ) NewExp = MaxExp;
//			D2Funcs.D2COMMON_SetStat(pMember, 0x1D, ExpGained, 0); // LAST EXP
//			D2Funcs.D2COMMON_SetStat(pMember, STAT_EXP, NewExp, 0);
//				if ( ExpLvl != D2Funcs.D2COMMON_GetNextCLvl(pMember->dwClassId, NewExp) )
//				{
//				D2Funcs.D2GAME_LevelAwards(pMember, pGame);
//				D2Funcs.D2GAME_ExecuteEvent(pGame, 12, 0, 0);
//				}
//
//			}
//		
//		}
//	}
//	else
//	{
//			int ExpGained = D2Funcs.D2GAME_GetExpGained(PlayerExp, pPlayer,D2Funcs.D2COMMON_GetStatSigned(pPlayer,STAT_LEVEL,0),pGame,MonsterLvl); // Tu moze byc crash
//
//			int CurrentExp = D2Funcs.D2COMMON_GetBaseStatSigned(pPlayer, STAT_EXP,0);
//			unsigned int NewExp = ExpGained + CurrentExp;
//			int MaxLvl = D2Funcs.D2COMMON_GetMaxCLvl(pPlayer->dwClassId);
//			unsigned int NextLvlExp = D2Funcs.D2COMMON_GetExpToAchiveLvl(pPlayer->dwClassId, MaxLvl - 1);
//
//			if ( NewExp > NextLvlExp ) NewExp = NextLvlExp;
//			D2Funcs.D2COMMON_SetStat(pPlayer, 0x1D, ExpGained, 0); // LAST EXP
//			D2Funcs.D2COMMON_SetStat(pPlayer, STAT_EXP, NewExp, 0);
//				if ( PlayerLvl != D2Funcs.D2COMMON_GetNextCLvl(pPlayer->dwClassId, NewExp) )
//				{
//				D2Funcs.D2GAME_LevelAwards(pPlayer, pGame);
//				D2Funcs.D2GAME_ExecuteEvent(pGame, 12, 0, 0);
//				}
//	}
//
//}