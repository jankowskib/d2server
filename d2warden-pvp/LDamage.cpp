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
#include "LDamage.h"
#include "Warden.h"



/**
signed int __usercall sub_6FCA87B0<eax>(ResRecord *pRecord<ebx>, Attack *pAttack<edi>)
@param pAttack	Damage structure
@param pRecord	current calculated resistance type
@return a resist penalty
*/
signed int __stdcall DAMAGE_ApplyResistPenalty(Attack* pAttack, ResRecord *pRecord)
{
	DWORD nStat;
	int nPenalty;
	DWORD nPierceStat;
	DWORD nBaseStat;
	DWORD nMaxResStat;
	int nMaxResVal;

	nStat = pRecord->nBaseStat;
	nPenalty = 0;
	if (nStat != -1)
		nPenalty = D2Funcs.D2COMMON_GetStatSigned(pAttack->pDefender, nStat, 0);
	nPierceStat = pRecord->nPierceStat;
	if (nPierceStat != -1 && (nPenalty < 100 || !pAttack->dwDefenderType))
	{
		int nPierceVal = D2Funcs.D2COMMON_GetStatSigned(pAttack->pAttacker, nPierceStat, 0);
		if (nPierceVal)
			nPenalty -= nPierceVal;
	}
	if (pAttack->dwDefenderType == UNIT_PLAYER)
	{
		nBaseStat = pRecord->nBaseStat;
		if (nBaseStat != STAT_DAMAGEREDUCTION)
		{
			if (nBaseStat != STAT_MAGICRESIST)
			{
				if (pAttack->pGame->bExpansion)
				{
					nPenalty += pAttack->pDifficultyLevelsTxt->ResistPenalty;
				}
				else // Handle classic
				{
					if (pAttack->pGame->DifficultyLevel == DIFF_NIGHTMARE)
					{
						nPenalty -= 20;
					}
					else
					{
						if (pAttack->pGame->DifficultyLevel == DIFF_HELL)
							nPenalty -= 50;
					}
				}
			}
		}
	}
	if (nPenalty > 0)
	{
		if (pAttack->dwDefenderType == UNIT_PLAYER)
		{
			nMaxResStat = pRecord->nMaxResStat;
			nMaxResVal = 75;
			if (nMaxResStat == -1)
			{
				if (pRecord->nBaseStat == STAT_DAMAGEREDUCTION)
					nMaxResVal = Warden::getInstance().wcfgMaxDmgRes;
			}
			else
			{
				nMaxResVal = D2Funcs.D2COMMON_GetStatSigned(pAttack->pDefender, nMaxResStat, 0) + 75;
				int maxCap = 95;
				switch (nMaxResStat)
				{
				case STAT_MAXFIRERESIST: maxCap = Warden::getInstance().wcfgMaxFireRes; break;
				case STAT_MAXCOLDRESIST: maxCap = Warden::getInstance().wcfgMaxColdRes; break;
				case STAT_MAXPOISONRESIST: maxCap = Warden::getInstance().wcfgMaxPsnRes; break;
				case STAT_MAXLIGHTNINGRESIST: maxCap = Warden::getInstance().wcfgMaxLightRes; break;
				}
				if (nMaxResVal >= maxCap)
					nMaxResVal = maxCap;
			}
			if (nPenalty <= -100)
				nPenalty = -100;
			if (nPenalty >= nMaxResVal)
				nPenalty = nMaxResVal;
		}
		if (pRecord->nBaseStat == STAT_DAMAGEREDUCTION && D2Funcs.D2COMMON_GetUnitState(pAttack->pAttacker, sanctuary))
		{
			if (D2Funcs.D2COMMON_IsMonsterUndead(pAttack->pDefender))
				nPenalty = 0;
		}
	}
	else
	{
		if (nPenalty <= -100)
			return -100;
	}
	return nPenalty;
}


/**
Patch location: D2GAME.0x89187
*/
void __declspec(naked) DAMAGE_AppylResistPenalty_STUB()
{
	__asm {

		push ebx
		push edi

		call DAMAGE_ApplyResistPenalty

		ret
	}
}