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
#include "NodesEx.h"


void __fastcall NODES_AssignNode(Game *ptGame, UnitAny *ptUnit, UnitNode* pChild)  // 0xEDF00
{
	int NEU_NODE = wcfgMaxPlayers + 3;

	if (!ptGame) return;
	if (!ptUnit) return;
	if (ptUnit->dwType) return;
	if (ptUnit->dwNodeIdx != NEU_NODE) return;

	UnitNode** pNodes = ptGame->pNewNodes;


	for (int n = 0; n < wcfgMaxPlayers; ++n)
	{
		if (!pNodes[n]) {
			if (ptGame->nClients <= wcfgMaxPlayers && n < wcfgMaxPlayers)
			{
				UnitNode* ptUNode = (UnitNode *)D2Funcs.FOG_AllocServerMemory(ptGame->pMemPool, sizeof(UnitNode), __FILE__, __LINE__, 0);
				if (ptUNode)
				{
					ptUNode->ptUnit = 0;
					ptUNode->pNode = 0;
					ptUNode->pChildNode = 0;
					ptUNode->pParentNode = 0;
					ptUNode->ptUnit = ptUnit;
					ptUNode->pNode = pChild;
					pNodes[n] = ptUNode;
					ptUnit->dwNodeIdx = n;
				}
				else
					Log("No memory to allocate a node");
			}
			else
			{
				Log("NodesEX: nClients=%d <= wcfgMaxPlayers=%d || z=%d < MaxPlayers=%d", ptGame->nClients, wcfgMaxPlayers, n, wcfgMaxPlayers);
			}
			break;
		}
	}

}


void __fastcall NODES_FreeChildNode(Game *ptGame, UnitAny *ptUnit)
{
	int NEU_NODE = wcfgMaxPlayers + 3;

	if (!ptGame) return;
	if (!ptUnit) return;
	if (ptUnit->dwType > UNIT_MONSTER) return;

	int aNodeIdx = ptUnit->dwNodeIdx;
	if (aNodeIdx == NEU_NODE) return;

	UnitNode* pNode = ptGame->pNewNodes[aNodeIdx];
	if (!pNode) return;

	if (aNodeIdx >= wcfgMaxPlayers)
	{
		while (pNode->ptUnit != ptUnit)
		{
			pNode = pNode->pChildNode;
			if (!pNode)
			{
				ptUnit->dwNodeIdx = NEU_NODE;
				return;
			}
		}

		if (pNode == ptGame->pNewNodes[aNodeIdx])
		{
			ptGame->pNewNodes[aNodeIdx] = pNode->pChildNode;
			UnitNode* pChild = pNode->pChildNode;
			if (pChild) pChild->pParentNode = 0;
			D2Funcs.FOG_FreeServerMemory(ptGame->pMemPool, pNode, __FILE__, __LINE__, 0);
			ptUnit->dwNodeIdx = NEU_NODE;
			return;
		}
	}
	else
	{
		pNode = pNode->pChildNode;
		if (!pNode)
		{
			ptUnit->dwNodeIdx = NEU_NODE;
			return;
		}
		while (pNode->ptUnit != ptUnit)
		{
			pNode = pNode->pChildNode;
			if (!pNode)
			{
				ptUnit->dwNodeIdx = NEU_NODE;
				return;
			}
		}
	}

	pNode->pParentNode->pChildNode = pNode->pChildNode;
	UnitNode* pChild = pNode->pChildNode;
	if (pChild) pChild->pParentNode = pNode->pParentNode;
	D2Funcs.FOG_FreeServerMemory(ptGame->pMemPool, pNode, __FILE__, __LINE__, 0);
	ptUnit->dwNodeIdx = NEU_NODE;
}


void __fastcall NODES_FreeUnitNode(Game* ptGame, UnitAny* ptUnit)
{
	int NEU_NODE = wcfgMaxPlayers + 3;
	if (!ptGame) return;
	if (!ptUnit) return;
	if (ptUnit->dwType > UNIT_MONSTER) return;

	int aNodeIdx = ptUnit->dwNodeIdx;

	if (aNodeIdx == NEU_NODE) return;

	for (UnitNode* pNode = ptGame->pNewNodes[aNodeIdx]; pNode; pNode = pNode->pChildNode)
	{
		if (pNode->ptUnit)
			pNode->ptUnit->dwNodeIdx = NEU_NODE;
		D2Funcs.FOG_FreeServerMemory(ptGame->pMemPool, pNode, __FILE__, __LINE__, 0);
	}

	ptGame->pNewNodes[aNodeIdx] = 0;
}


UnitNode *__fastcall NODES_GetUnitNode(UnitAny *ptUnit, Game *ptGame)  //0x6FD0DC50
{
	int NEU_NODE = wcfgMaxPlayers + 3;
	if (!ptGame || !ptUnit) return 0;
	if (ptUnit->dwType > 1) return 0;
	if (ptUnit->dwNodeIdx == NEU_NODE) return 0;

	for (UnitNode* pNode = ptGame->pNewNodes[ptUnit->dwNodeIdx]; pNode; pNode = pNode->pChildNode)
	{
		if (pNode->ptUnit == ptUnit) return pNode;
	}
	return 0;
}

void __fastcall NODES_SetUnitNode(Game *ptGame, UnitAny *ptUnit, UnitNode* ptNode, int NodeIdx) ///6FD0DDE0
{
	int NEU_NODE = wcfgMaxPlayers + 3;
	UnitNode *pParentNode;
	UnitNode *pCurrentNode;
	if (!ptGame || !ptUnit || ptUnit->dwNodeIdx != NEU_NODE || ptUnit->dwType > UNIT_MONSTER)
		return;

	if (NodeIdx != wcfgMaxPlayers && NodeIdx != wcfgMaxPlayers + 1)
		return;

	pCurrentNode = ptGame->pNewNodes[NodeIdx];

	pParentNode = (UnitNode *)D2Funcs.FOG_AllocServerMemory(ptGame->pMemPool, sizeof(UnitNode), __FILE__, __LINE__, 0);
	if (pParentNode)
	{
		pParentNode->ptUnit = 0;
		pParentNode->pNode = 0;
		pParentNode->pChildNode = 0;
		pParentNode->pParentNode = 0;
		pParentNode->ptUnit = ptUnit;
		pParentNode->pNode = ptNode;
		ptGame->pNewNodes[NodeIdx] = pParentNode;
		if (pCurrentNode)
		{
			pParentNode->pChildNode = pCurrentNode;
			pCurrentNode->pParentNode = pParentNode;
		}
		ptUnit->dwNodeIdx = NodeIdx;
	}
	else
	{
		Log("NodesEx: No memory to allocate a node");
	}
}

void __fastcall NODES_AddAsParentNode(Game *ptGame, UnitAny *ptUnit, UnitNode *ptNode, int NodeIdx) //0x6FD0DE70
{
	int NEU_NODE = wcfgMaxPlayers + 3;
	if (!ptGame || !ptUnit) return;
	if (ptUnit->dwNodeIdx != NEU_NODE) return;
	if (NodeIdx >= wcfgMaxPlayers) return;
	if (ptUnit->dwType > UNIT_MONSTER) return;

	UnitNode * pCurrentNode = ptGame->pNewNodes[NodeIdx];
	if (pCurrentNode)
	{
		if (pCurrentNode->ptUnit)
		{
			UnitNode * pParentNode = (UnitNode *)D2Funcs.FOG_AllocServerMemory(ptGame->pMemPool, sizeof(UnitNode), __FILE__, __LINE__, 0);
			if (pParentNode)
			{
				pParentNode->ptUnit = 0;
				pParentNode->pNode = 0;
				pParentNode->pChildNode = 0;
				pParentNode->pParentNode = 0;
				pParentNode->ptUnit = ptUnit;
				pParentNode->pNode = ptNode;
				pParentNode->pChildNode = pCurrentNode->pChildNode;
				pParentNode->pParentNode = pCurrentNode;
				UnitNode * pChildNode = pCurrentNode->pChildNode;
				if (pChildNode)
					pChildNode->pParentNode = pParentNode;
				pCurrentNode->pChildNode = pParentNode;
				ptUnit->dwNodeIdx = NodeIdx;
			}
		}
	}
}

void __fastcall NODES_Free(Game *ptGame)
{
	if (!ptGame) 
		return;

	UnitNode** pNodes = ptGame->pNewNodes;

	for (int i = 0; i < wcfgMaxPlayers + 3; ++i)
	{
		if (pNodes[i])
		{
			UnitAny * pUnit = pNodes[i]->ptUnit;
			if (pUnit) NODES_FreeUnitNode(ptGame, pUnit);
		}
	}
}

__declspec(naked) UnitAny *__stdcall NODES_NormalCheck(Game *ptGame, UnitAny *ptUnit, int *xPos, int *yPos, int aZero, int(__fastcall *pfnCallback)(UnitAny*, UnitAny*)) //6FCA0AA0
{
	__asm
	{
		MOV EAX, DWORD PTR SS : [ESP + 0x18]
			SUB ESP, 0x18
			PUSH EBX
			XOR EBX, EBX
			CMP EAX, EBX
			PUSH EBP
			MOV EBP, DWORD PTR SS : [ESP + 0x28]
			JNZ L016
			PUSH __LINE__
			CALL D2Funcs.FOG_GetErrorData
			PUSH EAX
			PUSH 0
			CALL D2Funcs.FOG_Error
			ADD ESP, 0x0C
			PUSH - 1
			CALL exit
		L016 :
		MOV EAX, DWORD PTR SS : [ESP + 0x24]
			ADD EAX, 0x1E00 /// BYLO 0x10F8
			PUSH EDI
			MOV EDI, DWORD PTR DS : [EAX]
			MOV DWORD PTR SS : [ESP + 0x28], EAX
			MOV EAX, DWORD PTR SS : [EBP]
			CMP EAX, 2
			MOV DWORD PTR SS : [ESP + 0x14], EBX
			MOV DWORD PTR SS : [ESP + 0xC], EBX
			MOV DWORD PTR SS : [ESP + 0x10], EBX
			JE L040
			CMP EAX, 3
			JLE L031
			CMP EAX, 5
			JLE L040
		L031 :
		MOV EAX, [EBP + 0x2C]
			CMP EAX, EBX
			JE L038
			PUSH EAX
			CALL D2ASMFuncs::D2COMMON_GetUnitX
			MOV DWORD PTR SS : [ESP + 0x1C], EAX
			JMP L043
		L038 :
		MOV DWORD PTR SS : [ESP + 0x1C], EBX
			JMP L043
		L040 :
		MOV EAX, [EBP + 0x2C]
			MOV ECX, DWORD PTR DS : [EAX + 0xC]
			MOV DWORD PTR SS : [ESP + 0x1C], ECX
		L043 :
		MOV EAX, DWORD PTR SS : [EBP]
			CMP EAX, 2
			JE L058
			CMP EAX, 3
			JLE L050
			CMP EAX, 5
			JLE L058
		L050 :
		MOV EAX, [EBP + 0x2C]
			CMP EAX, EBX
			JE L056
			PUSH EAX
			CALL D2ASMFuncs::D2COMMON_GetUnitY
			JMP L060
		L056 :
		MOV DWORD PTR SS : [ESP + 0x18], EBX
			JMP L061
		L058 :
		MOV EDX, [EBP + 0x2C]
			MOV EAX, DWORD PTR DS : [EDX + 0x10]
		L060 :
			 MOV DWORD PTR SS : [ESP + 0x18], EAX
		 L061 :
		MOV CL, BYTE PTR SS : [EBP + 0x18]
			MOV BYTE PTR SS : [ESP + 0x2C], CL
			MOV DWORD PTR SS : [ESP + 0x20], EBX
			PUSH ESI
		L065 :
		CMP EDI, EBX
			JE L108
			MOV ESI, DWORD PTR DS : [EDI]
			CMP ESI, EBX
			JE L120
			CMP DWORD PTR DS : [ESI], EBX
			JNZ L120
			MOV EDX, ESI
			MOV ECX, EBP
			CALL DWORD PTR SS : [ESP + 0x40]
			TEST EAX, EAX
			JE L108
			CMP ESI, EBX
			JE L090
			MOV EAX, DWORD PTR DS : [ESI + 0x10]
			CMP EAX, 0x11
			JE L090
			CMP EAX, EBX
			JE L090
			MOV EBX, DWORD PTR SS : [ESP + 0x3C]
			PUSH EBP
			MOV EAX, EBX
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_II
			JMP L092
		L090 :
		MOV EBX, DWORD PTR SS : [ESP + 0x3C]
			XOR EAX, EAX
		L092 :
		CMP DWORD PTR SS : [ESP + 0x10], EAX
			JGE L096
			MOV DWORD PTR SS : [ESP + 0x10], EAX
			MOV DWORD PTR SS : [ESP + 0x18], ESI
		L096 :
		MOV ECX, DWORD PTR SS : [ESP + 0x14]
			MOV EDI, DWORD PTR DS : [EDI + 0x8]
			INC ECX
			TEST EDI, EDI
			MOV DWORD PTR SS : [ESP + 0x14], ECX
			JE L109
			MOV ESI, DWORD PTR DS : [EDI]
			PUSH EBP
			MOV EAX, EBX
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_II
			JMP L092
		L108 :
		MOV EBX, DWORD PTR SS : [ESP + 0x3C]
		L109 :
			 MOV EAX, DWORD PTR SS : [ESP + 0x2C]
			 MOV EDI, DWORD PTR DS : [EAX + 0x4]
			 ADD EAX, 0x4
			 MOV DWORD PTR SS : [ESP + 0x2C], EAX
			 MOV EAX, DWORD PTR SS : [ESP + 0x24]
			 INC EAX
			 CMP AL, BYTE PTR DS : [wcfgMaxPlayers]			// Zamiana z 8 do 16
			 MOV DWORD PTR SS : [ESP + 0x24], EAX
			 JGE L128
			 XOR EBX, EBX
			 JMP L065
		 L120 :
		PUSH __LINE__
			CALL D2Funcs.FOG_GetErrorData
			PUSH EAX
			PUSH 0
			CALL D2Funcs.FOG_Error
			ADD ESP, 0x0C
			PUSH - 1
			CALL exit
		L128 :
		TEST EDI, EDI
			JE L147
			MOV EDI, EDI
		L131 :
		MOV ESI, DWORD PTR DS : [EDI]
			MOV DL, BYTE PTR SS : [ESP + 0x30]
			CMP DL, BYTE PTR DS : [ESI + 0x18]
			JNZ L144
			PUSH EBP
			MOV EAX, EBX
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_II
			CMP DWORD PTR SS : [ESP + 0x10], EAX
			JGE L143
			MOV DWORD PTR SS : [ESP + 0x10], EAX
			MOV DWORD PTR SS : [ESP + 0x18], ESI
		L143 :
		INC DWORD PTR SS : [ESP + 0x14]
		L144 :
			 MOV EDI, DWORD PTR DS : [EDI + 0x8]
			 TEST EDI, EDI
			 JNZ L131
		 L147 :
		MOV EAX, DWORD PTR SS : [ESP + 0x2C]
			MOV EDI, DWORD PTR DS : [EAX + 0x4]
			XOR EBX, EBX
			TEST EDI, EDI
			MOV DWORD PTR SS : [ESP + 0x40], 0
			JE L203
		L153 :
		MOV ESI, DWORD PTR DS : [EDI]
			MOV CL, BYTE PTR SS : [ESP + 0x30]
			CMP CL, BYTE PTR DS : [ESI + 0x18]
			JNZ L165
			MOV EAX, DWORD PTR SS : [ESP + 0x3C]
			PUSH EBP
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_II
			CMP EBX, EAX
			JGE L165
			MOV EBX, EAX
			MOV DWORD PTR SS : [ESP + 0x40], ESI
		L165 :
		MOV EDI, DWORD PTR DS : [EDI + 0x8]
			TEST EDI, EDI
			JNZ L153
			MOV EAX, DWORD PTR SS : [ESP + 0x40]
			TEST EAX, EAX
			JE L203
			MOV EDX, DWORD PTR SS : [ESP + 0x1C]
			MOV ECX, DWORD PTR SS : [ESP + 0x20]
			PUSH EDX
			PUSH ECX
			CALL D2Ptrs.D2GAME_GetDistanceFromXY_I
			CMP EAX, 0x5
			JGE L203
			PUSH EDI
			MOV EDI, DWORD PTR SS : [ESP + 0x1C]
			PUSH EDI
			PUSH EBP
			CALL D2Funcs.D2COMMON_isUnitInMeleeRange
			TEST EAX, EAX
			JNZ L203
			MOV ESI, [EBP + 0x2C]
			PUSH EDI
			PUSH ESI
			CALL D2Funcs.D2COMMON_SetPathTarget
			PUSH 2
			PUSH ESI
			CALL D2Funcs.D2COMMON_SetPathType
			PUSH 0
			PUSH EBP
			PUSH ESI
			CALL D2Funcs.D2COMMON_AssignPath
			PUSH ESI
			CALL D2Funcs.D2COMMON_GetPathUNK
			TEST EAX, EAX
			JNZ L203
			MOV EDX, DWORD PTR SS : [ESP + 0x40]
			MOV DWORD PTR SS : [ESP + 0x18], EDX
			MOV DWORD PTR SS : [ESP + 0x10], EBX
		L203 :
		MOV EAX, DWORD PTR SS : [ESP + 0x38]
			MOV ECX, DWORD PTR SS : [ESP + 0x14]
			MOV EDX, DWORD PTR SS : [ESP + 0x34]
			POP ESI
			MOV DWORD PTR DS : [EAX], ECX
			MOV EAX, DWORD PTR SS : [ESP + 0xC]
			POP EDI
			POP EBP
			MOV DWORD PTR DS : [EDX], EAX
			MOV EAX, DWORD PTR SS : [ESP + 0xC]
			POP EBX
			ADD ESP, 0x18
			RETN 0x18
	}
}

__declspec(naked) UnitAny *__stdcall NODES_BaalCheck(Game *ptGame, UnitAny *ptUnit, int *xPos, int *yPos, int aZero, int(__fastcall *pfnCallback)(UnitAny*, UnitAny*)) //0x6FC4A790
{
	__asm
	{

		MOV EAX, DWORD PTR SS : [ESP + 0x18]
			SUB ESP, 0x18
			PUSH EBX
			XOR EBX, EBX
			CMP EAX, EBX
			PUSH EBP
			MOV EBP, DWORD PTR SS : [ESP + 0x28]
			JNZ L016
			PUSH __LINE__
			CALL D2Funcs.FOG_GetErrorData
			PUSH EAX
			PUSH 0
			CALL D2Funcs.FOG_Error
			ADD ESP, 0x0C
			PUSH - 1
			CALL exit
		L016 :
		MOV EAX, DWORD PTR SS : [ESP + 0x24]
			ADD EAX, 0x1E00 /// BYLO 0x10F8
			PUSH EDI
			MOV EDI, DWORD PTR DS : [EAX]
			MOV DWORD PTR SS : [ESP + 0x28], EAX
			MOV EAX, DWORD PTR SS : [EBP]
			CMP EAX, 2
			MOV DWORD PTR SS : [ESP + 0x14], EBX
			MOV DWORD PTR SS : [ESP + 0xC], EBX
			MOV DWORD PTR SS : [ESP + 0x10], EBX
			JE L040
			CMP EAX, 3
			JLE L031
			CMP EAX, 5
			JLE L040
		L031 :
		MOV EAX, [EBP + 0x2C]
			CMP EAX, EBX
			JE L038
			PUSH EAX
			CALL D2ASMFuncs::D2COMMON_GetUnitX
			MOV DWORD PTR SS : [ESP + 0x1C], EAX
			JMP L043
		L038 :
		MOV DWORD PTR SS : [ESP + 0x1C], EBX
			JMP L043
		L040 :
		MOV EAX, [EBP + 0x2C]
			MOV ECX, DWORD PTR DS : [EAX + 0xC]
			MOV DWORD PTR SS : [ESP + 0x1C], ECX
		L043 :
		MOV EAX, DWORD PTR SS : [EBP]
			CMP EAX, 2
			JE L058
			CMP EAX, 3
			JLE L050
			CMP EAX, 5
			JLE L058
		L050 :
		MOV EAX, [EBP + 0x2C]
			CMP EAX, EBX
			JE L056
			PUSH EAX
			CALL D2ASMFuncs::D2COMMON_GetUnitY
			JMP L060
		L056 :
		MOV DWORD PTR SS : [ESP + 0x18], EBX
			JMP L061
		L058 :
		MOV EDX, [EBP + 0x2C]
			MOV EAX, DWORD PTR DS : [EDX + 0x10]
		L060 :
			 MOV DWORD PTR SS : [ESP + 0x18], EAX
		 L061 :
		MOV CL, BYTE PTR SS : [EBP + 0x18]
			MOV BYTE PTR SS : [ESP + 0x2C], CL
			MOV DWORD PTR SS : [ESP + 0x20], EBX
			PUSH ESI
		L065 :
		CMP EDI, EBX
			JE L108
			MOV ESI, DWORD PTR DS : [EDI]
			CMP ESI, EBX
			JE L120
			CMP DWORD PTR DS : [ESI], EBX
			JNZ L120
			MOV EDX, ESI
			MOV ECX, EBP
			CALL DWORD PTR SS : [ESP + 0x40]
			TEST EAX, EAX
			JE L108
			CMP ESI, EBX
			JE L090
			MOV EAX, DWORD PTR DS : [ESI + 0x10]
			CMP EAX, 0x11
			JE L090
			CMP EAX, EBX
			JE L090
			MOV EBX, DWORD PTR SS : [ESP + 0x3C]
			PUSH EBP
			MOV EAX, EBX
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_I
			JMP L092
		L090 :
		MOV EBX, DWORD PTR SS : [ESP + 0x3C]
			XOR EAX, EAX
		L092 :
		CMP DWORD PTR SS : [ESP + 0x10], EAX
			JGE L096
			MOV DWORD PTR SS : [ESP + 0x10], EAX
			MOV DWORD PTR SS : [ESP + 0x18], ESI
		L096 :
		MOV ECX, DWORD PTR SS : [ESP + 0x14]
			MOV EDI, DWORD PTR DS : [EDI + 0x8]
			INC ECX
			TEST EDI, EDI
			MOV DWORD PTR SS : [ESP + 0x14], ECX
			JE L109
			MOV ESI, DWORD PTR DS : [EDI]
			PUSH EBP
			MOV EAX, EBX
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_I
			JMP L092
		L108 :
		MOV EBX, DWORD PTR SS : [ESP + 0x3C]
		L109 :
			 MOV EAX, DWORD PTR SS : [ESP + 0x2C]
			 MOV EDI, DWORD PTR DS : [EAX + 0x4]
			 ADD EAX, 0x4
			 MOV DWORD PTR SS : [ESP + 0x2C], EAX
			 MOV EAX, DWORD PTR SS : [ESP + 0x24]
			 INC EAX
			 CMP AL, BYTE PTR DS : [wcfgMaxPlayers]			// Zamiana z 8 do 16
			 MOV DWORD PTR SS : [ESP + 0x24], EAX
			 JGE L128
			 XOR EBX, EBX
			 JMP L065
		 L120 :
		PUSH __LINE__
			CALL D2Funcs.FOG_GetErrorData
			PUSH EAX
			PUSH 0
			CALL D2Funcs.FOG_Error
			ADD ESP, 0x0C
			PUSH - 1
			CALL exit
		L128 :
		TEST EDI, EDI
			JE L147
			MOV EDI, EDI
		L131 :
		MOV ESI, DWORD PTR DS : [EDI]
			MOV DL, BYTE PTR SS : [ESP + 0x30]
			CMP DL, BYTE PTR DS : [ESI + 0x18]
			JNZ L144
			PUSH EBP
			MOV EAX, EBX
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_I
			CMP DWORD PTR SS : [ESP + 0x10], EAX
			JGE L143
			MOV DWORD PTR SS : [ESP + 0x10], EAX
			MOV DWORD PTR SS : [ESP + 0x18], ESI
		L143 :
		INC DWORD PTR SS : [ESP + 0x14]
		L144 :
			 MOV EDI, DWORD PTR DS : [EDI + 0x8]
			 TEST EDI, EDI
			 JNZ L131
		 L147 :
		MOV EAX, DWORD PTR SS : [ESP + 0x2C]
			MOV EDI, DWORD PTR DS : [EAX + 0x4]
			XOR EBX, EBX
			TEST EDI, EDI
			MOV DWORD PTR SS : [ESP + 0x40], 0
			JE L203
		L153 :
		MOV ESI, DWORD PTR DS : [EDI]
			MOV CL, BYTE PTR SS : [ESP + 0x30]
			CMP CL, BYTE PTR DS : [ESI + 0x18]
			JNZ L165
			MOV EAX, DWORD PTR SS : [ESP + 0x3C]
			PUSH EBP
			MOV ECX, ESI
			CALL D2Ptrs.D2GAME_NodesUnk_I
			CMP EBX, EAX
			JGE L165
			MOV EBX, EAX
			MOV DWORD PTR SS : [ESP + 0x40], ESI
		L165 :
		MOV EDI, DWORD PTR DS : [EDI + 0x8]
			TEST EDI, EDI
			JNZ L153
			MOV EAX, DWORD PTR SS : [ESP + 0x40]
			TEST EAX, EAX
			JE L203
			MOV EDX, DWORD PTR SS : [ESP + 0x1C]
			MOV ECX, DWORD PTR SS : [ESP + 0x20]
			PUSH EDX
			PUSH ECX
			CALL D2Ptrs.D2GAME_GetDistanceFromXY_I
			CMP EAX, 0x5
			JGE L203
			PUSH EDI
			MOV EDI, DWORD PTR SS : [ESP + 0x1C]
			PUSH EDI
			PUSH EBP
			CALL D2Funcs.D2COMMON_isUnitInMeleeRange
			TEST EAX, EAX
			JNZ L203
			MOV ESI, [EBP + 0x2C]
			PUSH EDI
			PUSH ESI
			CALL D2Funcs.D2COMMON_SetPathTarget
			PUSH 2
			PUSH ESI
			CALL D2Funcs.D2COMMON_SetPathType
			PUSH 0
			PUSH EBP
			PUSH ESI
			CALL D2Funcs.D2COMMON_AssignPath
			PUSH ESI
			CALL D2Funcs.D2COMMON_GetPathUNK
			TEST EAX, EAX
			JNZ L203
			MOV EDX, DWORD PTR SS : [ESP + 0x40]
			MOV DWORD PTR SS : [ESP + 0x18], EDX
			MOV DWORD PTR SS : [ESP + 0x10], EBX
		L203 :
		MOV EAX, DWORD PTR SS : [ESP + 0x38]
			MOV ECX, DWORD PTR SS : [ESP + 0x14]
			MOV EDX, DWORD PTR SS : [ESP + 0x34]
			POP ESI
			MOV DWORD PTR DS : [EAX], ECX
			MOV EAX, DWORD PTR SS : [ESP + 0xC]
			POP EDI
			POP EBP
			MOV DWORD PTR DS : [EDX], EAX
			MOV EAX, DWORD PTR SS : [ESP + 0xC]
			POP EBX
			ADD ESP, 0x18
			RETN 0x18
	}
}


//
//UnitAny *__stdcall NODES_BaalCheck(Game *ptGame, UnitAny *ptUnit, int *xPos, int *yPos, int aZero, int (__fastcall *pfnCallback)(UnitAny*, UnitAny*))  //0x6FC4A790
//{
//  UnitNode *v6; // ebx@1
//  int v8; // eax@2
//  DWORD v9; // eax@4
//  DWORD v11; // eax@11
//  int UnitY; // eax@15
//  UnitAny *v13; // esi@21
//  DWORD v14; // eax@25
//  int v15; // eax@27
//  int v16; // ebx@27
//  unsigned __int8 v17; // sf@34
//  unsigned __int8 v18; // of@34
//  int v19; // eax@36
//  int v21; // eax@40
//  int v22; // ebx@44
//  UnitNode *v23; // edi@44
//  UnitAny *v24; // esi@45
//  int v25; // eax@46
//  Path *v26; // esi@52
//  int v28; // [sp+2Ch] [bp-18h]@4
//  int v29; // [sp+30h] [bp-14h]@4
//  UnitAny *v30; // [sp+34h] [bp-10h]@4
//  int v31; // [sp+38h] [bp-Ch]@16
//  DWORD UnitX; // [sp+3Ch] [bp-8h]@8
//  int v33; // [sp+40h] [bp-4h]@19
//  UnitNode **v34; // [sp+48h] [bp+4h]@4
//  char ActNo; // [sp+4Ch] [bp+8h]@19
//  UnitAny *v36; // [sp+5Ch] [bp+18h]@44
//
//  v6 = 0;
//
//  ASSERT(pfnCallback);
//
//  v34 = ptGame->pUnitNodes;
//
//  v30 = 0;
//  v28 = 0;
//  v29 = 0;
//
//	  if(ptUnit->pPath)
//	  {
//	  UnitX = D2Funcs.D2GAME_GetUnitX(ptUnit);
//	  UnitY = D2Funcs.D2GAME_GetUnitY(ptUnit);
//	  }
//	  else
//	  {
//	  UnitX = 0;
//	  UnitY = 0;
//	  }
//
//  *xPos=UnitX;
//  *yPos=UnitY;
//
//  ActNo = LOBYTE(ptUnit->dwAct);
//
//  UnitNode * pNode = ptGame->pUnitNodes[0];
//  for(int i = 0; i<8; i++)
//  {
//  if (!pNode) continue;
//
//   UnitAny* pNodeUnit = pNode->ptUnit;
//
//	ASSERT(pNodeUnit)
//	ASSERT(pNodeUnit->dwType == 0)
//
//    if ( pfnCallback(ptUnit, pNodeUnit) )
//    {
//	  if ( !pNodeUnit || (pNodeUnit->dwMode == PLAYER_MODE_DEAD || pNodeUnit->dwMode == PLAYER_MODE_DEATH))
//      {
//        v15 = 0;
//      }
//      else
//      {
//        v15 = D2Funcs.D2GAME_NodesUnk(pNodeUnit, aZero, ptUnit); //Something with Baal Ai
//      }
//
//      while ( 1 )
//      {
//        if ( v28 < v15 )
//        {
//          v28 = v15;
//          v30 = v13;
//        }
//        pNode = pNode->pChildNode;
//        ++v29;
//        if ( !pNode ) break;
//        v15 = D2Funcs.D2GAME_NodesUnk(pNode->ptUnit, aZero, ptUnit);
//      }
//    }
//    pNode = v34[1];
//    v6 = 0;
//  }
//
//
//  for (; pNode; pNode = pNode->pChildNode )
//  {
//    UnitAny* pNodeUnit = pNode->ptUnit;
//    if ( ActNo == LOBYTE(pNodeUnit->dwAct) )
//    {
//      v21 = D2Funcs.D2GAME_NodesUnk(pNodeUnit, aZero, ptUnit);
//      if (v21 > 0 )
//      {
//        v28 = v21;
//		v30 = pNode->ptUnit;
//      }
//      ++v29;
//    }
//  }
//  v23 = v34[1];
//  v22 = 0;
//  v36 = 0;
//  if ( v23 )
//  {
//    do
//    {
//      v24 = v23->ptUnit;
//      if ( ActNo == LOBYTE(v23->ptUnit->dwAct) )
//      {
//        v25 = D2Funcs.D2GAME_NodesUnk(v24, aZero, ptUnit);
//        if ( v22 < v25 )
//        {
//          v22 = v25;
//          v36 = v24;
//        }
//      }
//      v23 = v23->pChildNode;
//    }
//    while ( v23 );
//    if ( v36 )
//    {
//      if ( D2Funcs.D2GAME_GetDistanceFromXY(v36, UnitX, v31) < 5 )
//      {
//        if ( !D2Funcs.D2COMMON_isUnitInMeleeRange(ptUnit, v30, 0) )
//        {
//          v26 = ptUnit->pPath;
//          D2Funcs.D2COMMON_SetPathTarget(ptUnit->pPath, v30);
//          D2Funcs.D2COMMON_SetPathType(v26, 2);
//          D2Funcs.D2COMMON_AssignPath(v26, ptUnit, 0);
//          if ( !D2Funcs.D2COMMON_GetPathUNK(v26) )
//          {
//            v30 = v36;
//            v28 = v22;
//          }
//        }
//      }
//    }
//  }
//  *yPos = v29;
//  *xPos = v28;
//  return v30;
//}