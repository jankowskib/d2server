#include "stdafx.h"

#include "WardenMisc.h"
#include "Vars.h"
#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "RC4.h"
//#include <math.h>
#include <boost/algorithm/string.hpp>


int round(double a) {
return int(a + 0.5);
}


BYTE GetColorNameByAcc(string szAcc)
{
	static string File;
	if(File.empty()) {
	char filename[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH,filename);
	File.assign(filename);
	File+="\\Colors.ini";
	}
	return GetPrivateProfileInt("Colors",szAcc.c_str(),COL_DARK_GOLD,File.c_str());
}

short CalculateDistance(short x1, short y1, short x2, short y2)
{
	return (short)::sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}


bool isAnAdmin(string szAcc)
{
 boost::to_lower(szAcc);

 for(list<std::string>::iterator i = Admins.begin(); i != Admins.end(); ++i) if(szAcc == *i) return true;

 return false;
}


BOOL isItemBelongToUnit(UnitAny* ptUnit, DWORD iCode)
{
Inventory* ptInv = ptUnit->pInventory;
if(!ptInv) return 0;
UnitAny* ptItem = D2COMMON_GetFirstItem(ptInv);
if(!ptItem) return 0;
if(D2COMMON_GetItemCode(ptItem) == iCode) return TRUE;
if(ptItem == D2COMMON_GetCursorItem(ptInv) && D2COMMON_GetItemCode(ptItem) == iCode) return TRUE;

for(UnitAny* i = ptItem; i; i = D2COMMON_GetNextItem(i))
{
if(!D2COMMON_UnitIsItem(i)) continue;
if(D2COMMON_GetItemCode(i) == iCode) return TRUE;
}
return FALSE;
}

BOOL isItemBelongToUnit(UnitAny* ptUnit, DWORD iCode, ItemQuality iQuality)
{
Inventory* ptInv = ptUnit->pInventory;
if(!ptInv) return 0;
UnitAny* ptItem = D2COMMON_GetFirstItem(ptInv);
if(!ptItem) return 0;
if(D2COMMON_GetItemCode(ptItem) == iCode && ptItem->pItemData->QualityNo==iQuality) return TRUE;
if(ptItem == D2COMMON_GetCursorItem(ptInv) && D2COMMON_GetItemCode(ptItem) == iCode && ptItem->pItemData->QualityNo==iQuality) return TRUE;

for(UnitAny* i = ptItem; i; i = D2COMMON_GetNextItem(i))
{
if(!D2COMMON_UnitIsItem(i)) continue;
if(D2COMMON_GetItemCode(i) == iCode && ptItem->pItemData->QualityNo==iQuality) return TRUE;
}
return FALSE;
}

UnitAny* FindUnitByClassId(Game* pGame, DWORD UnitType, DWORD dwClassId)
{
if(!pGame) return 0;

for(int i = 0; i<128; i++)
{
if(!pGame->pUnitList[UnitType][i]) continue;
if(pGame->pUnitList[UnitType][i]->dwClassId==dwClassId) return pGame->pUnitList[UnitType][i];
}
return 0;
}


ClientData* FindClientDataByName(Game* pGame, char* szName)
{
if(!pGame) return 0;

for(ClientData* pClient = pGame->pClientList; pClient; pClient = pClient->ptPrevious)
{
if(_stricmp(pClient->CharName, szName) == 0) return pClient;
}

return 0;
}

ClientData* FindClientDataById(Game* pGame, DWORD ClientID)
{
if(!pGame) return 0;

for(ClientData* pClient = pGame->pClientList; pClient; pClient = pClient->ptPrevious)
{
if(pClient->ClientID == ClientID) return pClient;
}

return 0;
}

int TransQuality(string str)
{
	if(str == "whatever") return 0;
	else if (str == "low") return 1;
	else if (str == "normal") return 2;
	else if (str == "superior") return 3;
	else if (str == "magic") return 4;
	else if (str == "set") return 5;
	else if (str == "rare") return 6;
	else if (str == "unique") return 7;
	else if (str == "crafted") return 8;
	else if (str == "tempered") return 9;
	return 0;
}


DWORD TransCode(const char* ptCode) // taken from afj666
{
   DWORD ItemCode = 0;

   if(strlen(ptCode)==3)
   {
	  char NewStr[5];
	  _snprintf_s(NewStr,5,5,"%s ",ptCode);
      ItemCode =TransCode(NewStr);
   }
   else
   {
	ItemCode = (DWORD)(ptCode[3]<<24) + (DWORD)(ptCode[2]<<16) +
                  (DWORD)(ptCode[1]<<8) + (DWORD)(ptCode[0]); 
   }

   return ItemCode;
}

char* TransCode(DWORD dwCode)
{
	BYTE* aCode = (BYTE*)&dwCode;
	static char ItemCode[5] = {0};

	ItemCode[0] = aCode[0];
	ItemCode[1] = aCode[1];
	ItemCode[2] = aCode[2];
	ItemCode[3] = aCode[3];
	ItemCode[4] = 0;

   return ItemCode;
}

void KickPlayer(ClientData* pClient)
{
		BYTE Packet = 0x06;
		D2GAME_SendPacket(pClient,&Packet,1);
//		D2NET_SendPacket(1,ClientID,&Packet,1);
}

void KickPlayer(DWORD ClientID)
{
	BYTE Packet = 0x06;
	D2NET_SendPacket(0,ClientID,&Packet,1);
}

void UpdateStats(UnitAny* ptUnit, int StatNo, int StatValue)
{
ClientData * pClientList = ptUnit->pGame->pClientList;
	while(pClientList)
	{
	if(pClientList->ClientID!=ptUnit->pPlayerData->pClientData->ClientID && pClientList->InitStatus==4) D2GAME_UpdatePlayerStats(ptUnit,StatNo,StatValue,pClientList->pPlayerUnit);
	if(!pClientList->ptPrevious) break;
	pClientList=pClientList->ptPrevious;
	}

}

void BroadcastMsg(Game* pGame,char *Msg...)
{
/*
00000000 px26            struc ; (sizeof=0x11A)
00000000 px26            db ?
00000001 ChatType        db ?  CHAT_NORMAL = 1, CHAT_WHISPERFROM = 2, CHAT_3, CHAT_NONAME = 4, CHAT_OVERHEAD =5 , CHAT_WHISPERTO = 6, CHAT_SCROLL =7 };
00000002 MsgLang         db ?
00000003 MsgStyle        db ?
00000004 UnitId          dd ?
00000008 MsgColor        db ?
00000009 MsgFlags        db ?
0000000A CharName        db 16 dup(?)
0000001A szMsg           db 256 dup(?)
0000011A px26            ends
*/
	if(!Msg) return;

	va_list arguments;
	va_start(arguments, Msg);

	int len = _vscprintf(Msg, arguments ) + 1;
	if(len == 0) return; 
	char * text = new char[len];

	vsnprintf_s(text,len,255,Msg,arguments);
	va_end(arguments);
	 
	WORD MsgLen = 21+strlen(text);
	BYTE * aPacket = new BYTE[MsgLen];
	memset(aPacket,0,MsgLen);
	aPacket[0] = 0x26;
	aPacket[1] = 0x01;
	aPacket[3] = 0x02; //MsgStyle
	aPacket[9] = 0x53; //MsgFlags
	strcpy_s((char*)aPacket+10,10,"ÿc1WARDEN");
	strcpy_s((char*)aPacket+20,MsgLen-20,text);

	ClientData * pClientList = pGame->pClientList;
	while(pClientList)
	{
	if(pClientList->InitStatus==4) D2GAME_SendPacket(pClientList,aPacket,MsgLen);
	if(!pClientList->ptPrevious) break;
	pClientList=pClientList->ptPrevious;
	}
	delete[] aPacket;
	delete[] text;
}

void BroadcastPacket(Game* pGame, BYTE * aPacket, int aLen)
{
	if(!pGame) return;
	ClientData * pClientList = pGame->pClientList;
	while(pClientList)
	{
	if(pClientList->InitStatus == 4) D2GAME_SendPacket(pClientList,aPacket,aLen);
	if(!pClientList->ptPrevious) break;
	pClientList=pClientList->ptPrevious;
	}
}

void BroadcastExEvent(Game* pGame, int Color, int Sound, int Font, short X, short Y, string polMsg, string engMsg)
{

	ExEvent hEvent = {0};
	hEvent.MsgType = 1;
	hEvent.Color = Color;
	hEvent.wX = X;
	hEvent.wY = Y;
	hEvent.Argument = Font;
	hEvent.Sound = Sound;
	hEvent.P_A6 = 0xA6;

	if(!pGame) return;
	ClientData * pClientList = pGame->pClientList;
	while(pClientList)
	{
	if(pClientList->InitStatus==4) {
		_snprintf_s(hEvent.szMsg,255,255,"%s",(pClientList->LocaleID == 10 ? polMsg.c_str() : engMsg.c_str()));
		hEvent.PacketLen = 0xE + strlen(hEvent.szMsg) +1;
		D2GAME_SendPacket(pClientList,(BYTE*)&hEvent,hEvent.PacketLen);
		}
	if(!pClientList->ptPrevious) break;
	pClientList=pClientList->ptPrevious;
	}
}

void BroadcastExEvent(Game* pGame, int Color, DWORD UnitId, int nCell, string szPath)
{

	ExEvent hEvent = {0};
	hEvent.MsgType = 2;
	hEvent.Color = Color;
	hEvent.UnitId = UnitId;
	hEvent.Sound = nCell;
	hEvent.P_A6 = 0xA6;

	_snprintf_s(hEvent.szMsg,255,255,szPath.c_str());
	hEvent.PacketLen = 0xE + strlen(hEvent.szMsg) +1;

	if(!pGame) return;
	ClientData * pClientList = pGame->pClientList;
	while(pClientList)
	{
		if(pClientList->InitStatus==4)	D2GAME_SendPacket(pClientList,(BYTE*)&hEvent,hEvent.PacketLen);
		if(!pClientList->ptPrevious) break;
		pClientList=pClientList->ptPrevious;
	}
}


void SendExEvent(ClientData* pClient, int Color, int Sound, int Font, short X, short Y, string polMsg, string engMsg)
{
	if(!pClient) return;
	ExEvent hEvent = {0};
	hEvent.MsgType = 1;
	hEvent.Color = Color;
	hEvent.wX = X;
	hEvent.wY = Y;
	hEvent.Argument = Font;
	hEvent.Sound = Sound;
	hEvent.P_A6 = 0xA6;

	_snprintf_s(hEvent.szMsg,255,255,"%s",(pClient->LocaleID == 10 ? polMsg.c_str() : engMsg.c_str()));
	hEvent.PacketLen = 0xE + strlen(hEvent.szMsg) +1;

	if(pClient->InitStatus==4) D2GAME_SendPacket(pClient,(BYTE*)&hEvent,hEvent.PacketLen);
}


void BroadcastEventMsg(Game* pGame, int Color, char *Msg...)
{
	va_list arguments;
	va_start(arguments, Msg);

	int len = _vscprintf(Msg, arguments ) + 1;
	char * text = new char[len];

	vsnprintf_s(text,len,255,Msg,arguments);
	va_end(arguments);

	WORD MsgLen = 27+strlen(text);
	BYTE * aPacket = new BYTE[MsgLen];
	memset(aPacket,0,MsgLen);
	aPacket[0] = 0x26;
	aPacket[1] = 0x04;
	aPacket[3] = 0x02;
	aPacket[8] = (BYTE)Color;
	strcpy_s((char*)aPacket+10,16,"[administrator]");
	strcpy_s((char*)aPacket+26,MsgLen-26,text);

	ClientData * pClientList = pGame->pClientList;
	while(pClientList)
	{
	if(pClientList->InitStatus>=4) D2GAME_SendPacket(pClientList,aPacket,MsgLen);
	if(!pClientList->ptPrevious) break;
	pClientList=pClientList->ptPrevious;
	}
	delete[] aPacket;
	delete[] text;
}


void BroadcastEventMsgEx(Game* pGame, int Color, string EngMsg, string PolMsg)
{
	ClientData * pClientList = pGame->pClientList;
	PolMsg.resize(255);
	EngMsg.resize(255);
	while(pClientList)
	{
	if(pClientList->InitStatus!=4) continue;

	WORD MsgLen = 27 + (pClientList->LocaleID == 10 ? strlen(PolMsg.c_str()) : strlen(EngMsg.c_str()));
	BYTE * aPacket = new BYTE[MsgLen];
	memset(aPacket,0,MsgLen);
	aPacket[0] = 0x26;
	aPacket[1] = 0x04;
	aPacket[3] = 0x02;
	aPacket[8] = (BYTE)Color;
	strcpy_s((char*)aPacket+10,16,"[administrator]");
	strcpy_s((char*)aPacket+26,MsgLen-26, pClientList->LocaleID == 10 ? PolMsg.c_str() : EngMsg.c_str());


	D2GAME_SendPacket(pClientList,aPacket,MsgLen);
	delete[] aPacket;
	if(!pClientList->ptPrevious) break;
	pClientList=pClientList->ptPrevious;
	}

}


void ClearSayGoFlag(Game* pGame)
{
for(ClientData* i = pGame->pClientList; i; i = i->ptPrevious) 
{
	if(i->InitStatus == 4)	i->pPlayerUnit->pPlayerData->SaidGO = 0;
}
}

void ClearIsPlayingFlag(Game* pGame)
{
for(ClientData* i = pGame->pClientList; i; i = i->ptPrevious) 
{
	if(i->InitStatus == 4)	i->pPlayerUnit->pPlayerData->isPlaying = 0;
}
}


void ClearCanAttackFlag(Game* pGame)
{
for(ClientData* i = pGame->pClientList; i; i = i->ptPrevious) 
{
	if(i->InitStatus == 4)	i->pPlayerUnit->pPlayerData->CanAttack = 0;
}
}

list<WardenClient>::iterator GetClientByID(DWORD CID)
{
LOCK
list<WardenClient>::iterator ptWardenClient = hWarden.Clients.end();

for(list<WardenClient>::iterator i = hWarden.Clients.begin(); i!=hWarden.Clients.end(); ++i)
{
	if(i->ClientID==CID) 
	{
	ptWardenClient = i;
	break;
	}
}

if(ptWardenClient != hWarden.Clients.end()) return ptWardenClient;

ptWardenClient = hWarden.Clients.end(); 
UNLOCK
return ptWardenClient;

}

list<WardenClient>::iterator GetClientByAcc(char *szAcc)
{
if(!szAcc) return hWarden.Clients.end();
if(!szAcc[0]) return hWarden.Clients.end();
LOCK

list<WardenClient>::iterator ptWardenClient = hWarden.Clients.end();

for(list<WardenClient>::iterator i = hWarden.Clients.begin(); i!=hWarden.Clients.end(); ++i)
{
	if(_stricmp(i->AccountName.c_str(),szAcc) == 0)
	{
	ptWardenClient = i;
	break;
	}
}

if(ptWardenClient != hWarden.Clients.end()) return ptWardenClient;

ptWardenClient = hWarden.Clients.end(); 
UNLOCK
return ptWardenClient;
}


list<WardenClient>::iterator GetClientByName(char *szName)
{
if(!szName) return hWarden.Clients.end();
if(!szName[0]) return hWarden.Clients.end();
LOCK

list<WardenClient>::iterator ptWardenClient = hWarden.Clients.end();

for(list<WardenClient>::iterator i = hWarden.Clients.begin(); i!=hWarden.Clients.end(); ++i)
{
	if(_stricmp(i->CharName.c_str(),szName) == 0)
	{
	ptWardenClient = i;
	break;
	}
}

if(ptWardenClient != hWarden.Clients.end()) return ptWardenClient;

ptWardenClient = hWarden.Clients.end(); 
UNLOCK
return ptWardenClient;
}

list<WardenClient>::iterator GetClientByName(Game* pGame, char *szName)
{
if(!pGame) return hWarden.Clients.end();
if(!szName) return hWarden.Clients.end();
if(!szName[0]) return hWarden.Clients.end();

LOCK

list<WardenClient>::iterator ptWardenClient = hWarden.Clients.end();

for(list<WardenClient>::iterator i = hWarden.Clients.begin(); i!=hWarden.Clients.end(); ++i)
{
	if(_stricmp(i->CharName.c_str(),szName) == 0) 
	{
	ptWardenClient = i;
	break;
	}
}

if(ptWardenClient != hWarden.Clients.end())
if(ptWardenClient->ptGame)
if(ptWardenClient->ptGame == pGame) return ptWardenClient;

ptWardenClient = hWarden.Clients.end(); 
UNLOCK
return ptWardenClient;
}

void SendPtrRequest(list<WardenClient>::iterator ptCurrentClient,char* DllName1,DWORD Addr, char* DllName2, DWORD Addr2)
{
//BYTE Pack[] = {0x02,0x0C,'D','2','C','l','i','e','n','t','.','d','l','l',0x0c,'D','2','C','l','i','e','n','t','.','d','l','l',0x00,0xec,0x01,0xFF,0xFF,0xFF,0xFF,0x04,0xec,0x02,0xFF,0xFF,0xFF,0xFF,0x04,0xd9};
unsigned char WardenCMD2_local[100];
memset(WardenCMD2_local,0,100);

BYTE s1 = strlen(DllName1);
BYTE s2 = strlen(DllName2);
WORD PacketSize =s1+s2+19;
BYTE * Packet = new BYTE[PacketSize];
Packet[0]=0x02;
Packet[1]=s1;
memcpy(&Packet[2],DllName1,s1);
Packet[2+s1]=s2;
memcpy(&Packet[3+s1],DllName2,s2);

Packet[3+s1+s2]=0x00;
Packet[4+s1+s2]=0xEC;
Packet[5+s1+s2]=0x01;
*(DWORD*)&Packet[6+s1+s2]=Addr;
Packet[10+s1+s2]=0x04;
Packet[11+s1+s2]=0xEC;
Packet[12+s1+s2]=0x02;
*(DWORD*)&Packet[13+s1+s2]=Addr2;
Packet[17+s1+s2]=0x04;
Packet[18+s1+s2]=0xD9;
WardenCMD2_local[0] = 0xAE;
*(WORD*)&WardenCMD2_local[1] = PacketSize;
memcpy(&WardenCMD2_local[3],Packet,PacketSize);
rc4_crypt(ptCurrentClient->RC4_KEY_0XAE,&WardenCMD2_local[3],PacketSize);
D2NET_SendPacket(0,ptCurrentClient->ClientID,WardenCMD2_local,PacketSize+3);
//if(D2NET_GetClient(ptCurrentClient->ClientID))
//D2GAME_SendPacket(ptCurrentClient->ptClientData,WardenCMD2_local,PacketSize+3);
delete[] Packet;
}


void SendPtrRequest(list<WardenClient>::iterator ptCurrentClient,char* DllName1,DWORD Addr, BYTE nBytes1, char* DllName2, DWORD Addr2, BYTE nBytes2)
{
//BYTE Pack[] = {0x02,0x0C,'D','2','C','l','i','e','n','t','.','d','l','l',0x0c,'D','2','C','l','i','e','n','t','.','d','l','l',0x00,0xec,0x01,0xFF,0xFF,0xFF,0xFF,0x04,0xec,0x02,0xFF,0xFF,0xFF,0xFF,0x04,0xd9};
unsigned char WardenCMD2_local[100];
memset(WardenCMD2_local,0,100);

BYTE s1 = strlen(DllName1);
BYTE s2 = strlen(DllName2);
WORD PacketSize =s1+s2+19;
BYTE * Packet = new BYTE[PacketSize];
Packet[0]=0x02;
Packet[1]=s1;
memcpy(&Packet[2],DllName1,s1);
Packet[2+s1]=s2;
memcpy(&Packet[3+s1],DllName2,s2);

Packet[3+s1+s2]=0x00;
Packet[4+s1+s2]=0xEC;
Packet[5+s1+s2]=0x01;
*(DWORD*)&Packet[6+s1+s2]=Addr;
Packet[10+s1+s2]=nBytes1;
Packet[11+s1+s2]=0xEC;
Packet[12+s1+s2]=0x02;
*(DWORD*)&Packet[13+s1+s2]=Addr2;
Packet[17+s1+s2]=nBytes2;
Packet[18+s1+s2]=0xD9;
WardenCMD2_local[0] = 0xAE;
*(WORD*)&WardenCMD2_local[1] = PacketSize;
memcpy(&WardenCMD2_local[3],Packet,PacketSize);
rc4_crypt(ptCurrentClient->RC4_KEY_0XAE,&WardenCMD2_local[3],PacketSize);
D2NET_SendPacket(0,ptCurrentClient->ClientID,WardenCMD2_local,PacketSize+3);
//if(D2NET_GetClient(ptCurrentClient->ClientID))
//D2GAME_SendPacket(ptCurrentClient->ptClientData,WardenCMD2_local,PacketSize+3);
delete[] Packet;
}


void SendPtrRequest(list<WardenClient>::iterator ptCurrentClient,char* DllName1,DWORD Addr, BYTE Bytes)
{
unsigned char WardenCMD2_local[100];
memset(WardenCMD2_local,0,100);
//02 0c 62 6E 63 6C 69 65 6E 74 2E 64 6C 6C 00 ec 01 30 D3 01 00 04 d9
BYTE s1 = strlen(DllName1);
WORD PacketSize =s1+11;
BYTE * Packet = new BYTE[PacketSize];
Packet[0]=0x02;
Packet[1]=s1;
memcpy(&Packet[2],DllName1,s1);

Packet[2+s1]=0x00;
Packet[3+s1]=0xEC;
Packet[4+s1]=0x01;
*(DWORD*)&Packet[5+s1]=Addr;
Packet[9+s1]=Bytes;
Packet[10+s1]=0xD9;
WardenCMD2_local[0] = 0xAE;
*(WORD*)&WardenCMD2_local[1] = PacketSize;
memcpy(&WardenCMD2_local[3],Packet,PacketSize);
rc4_crypt(ptCurrentClient->RC4_KEY_0XAE,&WardenCMD2_local[3],PacketSize);
D2NET_SendPacket(0,ptCurrentClient->ClientID,WardenCMD2_local,PacketSize+3);
//if(D2NET_GetClient(ptCurrentClient->ClientID))
//D2GAME_SendPacket(ptCurrentClient->ptClientData,WardenCMD2_local,PacketSize+3);
delete[] Packet;
}

void SendPtrRequest(list<WardenClient>::iterator ptCurrentClient,DWORD Addr,BYTE Bytes)
{
//{0x02,0x00,0xec,0x00,0xff,0xff,0xff,0xff,0x40,0xd9 };
unsigned char WardenCMD2_local[100];
memset(WardenCMD2_local,0,100);
WORD PacketSize = 10;
BYTE * Packet = new BYTE[10];
Packet[0]=0x02;
Packet[1]=0x00;
Packet[2]=0xEC;
Packet[3]=0x00;
*(DWORD*)&Packet[4]=Addr;
Packet[8]=Bytes;
Packet[9]=0xD9;
WardenCMD2_local[0] = 0xAE;
*(WORD*)&WardenCMD2_local[1] = PacketSize;
memcpy(&WardenCMD2_local[3],Packet,PacketSize);
rc4_crypt(ptCurrentClient->RC4_KEY_0XAE,&WardenCMD2_local[3],PacketSize);
D2NET_SendPacket(0,ptCurrentClient->ClientID,WardenCMD2_local,PacketSize+3);
//if(D2NET_GetClient(ptCurrentClient->ClientID))
//D2GAME_SendPacket(ptCurrentClient->ptClientData,WardenCMD2_local,PacketSize+3);
delete[] Packet;
}

BOOL WriteBytes(void* lpAddr, void* lpBuffer, DWORD dwLen)
{
	DWORD dwOldProtect;
	if(!VirtualProtect(lpAddr, dwLen, PAGE_READWRITE, &dwOldProtect))
		return FALSE;

	memcpy(lpAddr, lpBuffer, dwLen);

	if(!VirtualProtect(lpAddr, dwLen, dwOldProtect, &dwOldProtect))
		return FALSE;

	return TRUE;
}

void PatchGS(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen, char* Type)
{
	BYTE *bCode = new BYTE[dwLen];
	if(bInst)
	{
	::memset(bCode, 0x90, dwLen);
	bCode[0] = bInst;
	if(pFunc)	
	{	
		if(bInst==0xE8 || bInst==0xE9)
		{
		DWORD dwFunc = pFunc - (pAddr + 5);
		*(DWORD*)&bCode[1] = dwFunc;
		}
		else
		if(bInst==0x68 || bInst==0x05)
		{
		*(LPDWORD)&bCode[1]=pFunc;
		}
		else
		if(bInst==0x83)
		{
		*(WORD*)&bCode[1]=(WORD)pFunc;
		}
		else
		{
		bCode[1] = (BYTE)pFunc;
		}
	}
	}
	else
	{
	if(dwLen==6)
	{
	::memset(bCode, 0x00, dwLen);
	*(DWORD*)&bCode[0]=pFunc;
	}
	else if(dwLen==4)
	*(DWORD*)&bCode[0]=pFunc;
	else if (dwLen==2)
	*(WORD*)&bCode[0]=(WORD)pFunc;
	else if (dwLen==1)
	*(BYTE*)&bCode[0]=(BYTE)pFunc;
	}
	if(!WriteBytes((void*)pAddr, bCode, dwLen))
	{
		Log("Error while patching GS %s with %d byte(s)",Type,dwLen);
		Warden_Enable=false;
	}
	delete[] bCode;
}

void SendMsgToClient(ClientData* ptClient,char *Msg...)
{
	va_list arguments;

	va_start(arguments, Msg);

	int len = _vscprintf(Msg, arguments ) + 1;
	char * text = new char[len];

	vsnprintf_s(text,len,255,Msg,arguments);
	va_end(arguments);

	WORD MsgLen = 21+strlen(text);
	BYTE * aPacket = new BYTE[MsgLen];
	memset(aPacket,0,MsgLen);
	aPacket[0] = 0x26;
	aPacket[1] = 0x01;
	aPacket[3] = 0x02;
	aPacket[9] = 0x53; //0x05
	strcpy_s((char*)aPacket+10,10,"ÿc3WARDEN");
	strcpy_s((char*)aPacket+20,MsgLen-20,text);

//	D2NET_SendPacket(0,ptClient->ClientID,aPacket,MsgLen);

	D2GAME_SendPacket(ptClient,aPacket,MsgLen);
	delete[] aPacket;
	delete[] text;
}

void UpdatePlayerXY(UnitAny* ptUnit)
{
if(!ptUnit) return;
BYTE * Packet = new BYTE[11];
memset(Packet,0,11);
Packet[0]=0x15;
*(DWORD*)&Packet[2]=ptUnit->dwUnitId;
*(WORD*)&Packet[6]=ptUnit->pPath->xPos;
*(WORD*)&Packet[8]=ptUnit->pPath->yPos;
Packet[10]=0x01;
ClientData * pClientList = ptUnit->pGame->pClientList;
	while(pClientList)
	{
	if(pClientList->ClientID!=ptUnit->pPlayerData->pClientData->ClientID && pClientList->InitStatus>=4) D2GAME_SendPacket(pClientList,Packet,11);
	if(!pClientList->ptPrevious) break;
	pClientList=pClientList->ptPrevious;
	}
delete[] Packet;
return;
}


void WideToChar(CHAR* Dest, const WCHAR* Source)
{
WideCharToMultiByte(CP_ACP, 0, Source, -1, Dest, 255, NULL, NULL);
}

string ConvertSkill(WORD SkillID)
{
BYTE* Tbl = (*p_D2COMMON_sgptDataTables)->pSkillDescTxt;
if(SkillID> (*p_D2COMMON_sgptDataTables)->dwSkillsRecs) return "?";
SkillsTxt* pTxt = (*p_D2COMMON_sgptDataTables)->pSkillsTxt;
int nRow = pTxt[SkillID].wSkillDesc;
if(!nRow) return "?";

Tbl+= (nRow*0x120);
WORD LocId = *(WORD*)(Tbl+8);
if(LocId == 0) return "? - tell lolet";
wstring wText (D2LANG_GetLocaleText(LocId));
string szText;
szText.assign(wText.begin(),wText.end());
return szText;
}

char * ConvertLevel(int LevelId)
{
switch(LevelId)
{
case 1: return "Rogue Encampment";
case 2: return "Blood Moor";
case 3: return "Cold Plains";
case 4: return "Stony Field";
case 39: return "Secret Cow Level";
}
return "Not defined";
}

char * ConvertClass(int ClassId)
{
switch (ClassId)
{
case 0: return "Amazon";
case 1:	return "Sorceress";
case 2: return "Necromancer";
case 3: return "Paladin";
case 4: return "Barbarian";
case 5: return "Druid";
case 6: return "Assassin";
}
	return "???";
}

bool isSafeSkill(WORD SkillId)
{
switch(SkillId)
{
case D2S_UNSUMMON: return true;
case D2S_DECOY: return true;
case D2S_VALKYRIE: return true;

case D2S_BURSTOFSPEED: return true;
case D2S_VENOM: return true;
case D2S_FADE: return true;
case D2S_SHADOWMASTER: return true;
case D2S_SHADOWWARRIOR: return true;

case D2S_LEAP: return true;
case D2S_BATTLEORDERS: return true;
case D2S_BATTLECOMMAND: return true;
case D2S_SHOUT: return true;

case D2S_OAKSAGE: return true;
case D2S_CYCLONEARMOR: return true;

case D2S_BONEARMOR: return true;

case D2S_TELEPORT: return true;
case D2S_CHILLINGARMOR: return true;
case D2S_FROZENARMOR: return true;
case D2S_SHIVERARMOR: return true;
case D2S_TELEKINESIS: return true;
case D2S_ENERGYSHIELD: return true;
case D2S_ENCHANT: return true;

case D2S_HOLYSHIELD: return true;

}
return false;
}

bool isMeleeSkill(int SkillID)
{
switch(SkillID)
{
//ALL SKILLS
case D2S_ATTACK: return true;
case D2S_KICK: return true;
case D2S_THROW: return true;
case D2S_LEFTTHROW: return true;
case D2S_LEFTSWING: return true;

//AMA SKILLS
case D2S_JAB: return true;
case D2S_POWERSTRIKE: return true;
case D2S_CHARGEDSTRIKE: return true;
case D2S_FEND: return true;
case D2S_STRAFE: return true; // TEMP

//SORC SKILLS
case D2S_TELEKINESIS: return true;
case D2S_STATICFIELD: return true;

//NEC SKILLS
case D2S_POISONDAGGER: return true;

//PAL SKILLS
case D2S_SACRIFICE: return true;
case D2S_SMITE: return true;
case D2S_HOLYFIRE: return true;
case D2S_ZEAL: return true;
//case D2S_CHARGE: return true;
case D2S_VENGEANCE: return true;
case D2S_HOLYFREEZE: return true;
case D2S_HOLYSHOCK: return true;
//BARB SKILLS
case D2S_BASH: return true;
case D2S_LEAP: return true;
case D2S_STUN: return true;
case D2S_LEAPATTACK: return true;
case D2S_CONCENTRATE: return true;
case D2S_FRENZY: return true;
case D2S_WHIRLWIND: return true;
case D2S_BERSERK: return true;
//DRU SKILLS
case D2S_FERALRAGE: return true;
case D2S_MAUL: return true;
case D2S_RABIES: return true;
case D2S_FIRECLAWS: return true;
case D2S_HUNGER: return true;
case D2S_FURY: return true;
case D2S_HURRICANE: return true;
//ASSA SKILLS
case D2S_PSYCHICHAMMER: return true;
case D2S_MINDBLAST: return true;
case D2S_TIGERSTRIKE: return true;
case D2S_DRAGONTALON: return true;
case D2S_FISTSOFFIRE: return true;
case D2S_DRAGONCLAW: return true;
case D2S_COBRASTRIKE: return true;
case D2S_CLAWSOFTHUNDER: return true;
case D2S_DRAGONTAIL: return true;
case D2S_DRAGONFLIGHT: return true;
case D2S_PHOENIXSTRIKE: return true;
}
return false;
}

int isGoodSkill(WORD SkillID)
{
return 1;

//switch(SkillID)
//{
//case 0x16:
//return 1;
//case 0x0C:
//return 1;
//case 0x36:
//return 1;
//case 0x3B:
//return 1;
//case 0x42:
//return 1;
//case 0x5D:
//return 1;
//case 0x5B:
//return 1;
//case 0x10F:
//return 1;
//case 0x106:
//return 1;
//case 0x00:
//return 0;
//}
//return 0;
}

void upcase(char * q)
{
unsigned char c;
while (*q) { c = *q; *q = toupper(c); q++; }
}

void lowcase(char * q)
{
unsigned char c;
while (*q) { c = *q; *q = tolower(c); q++; }
}

void Log(char *format,...)
{
	va_list arguments;
	va_start(arguments, format);
	int len = _vscprintf(format, arguments ) + 1;
	char * text = new char[len];
	vsprintf_s(text, len, format, arguments);
	va_end(arguments);

	SYSTEMTIME t;
	char timebuf[256];
	GetLocalTime(&t);
	sprintf_s(timebuf,256,"[%04d-%02d-%02d %02d:%02d:%02d]",t.wYear,t.wMonth,t.wDay,t.wHour, t.wMinute, t.wSecond );

	EnterCriticalSection(&LOG_CS);
	FILE *fp = 0;
	fopen_s(&fp,"d2warden.log","a+");
	if(!fp) { 	LeaveCriticalSection(&LOG_CS); return; }
	fseek(fp,0,SEEK_END);
	fwrite(timebuf,strlen(timebuf),1,fp);
	fwrite(text,strlen(text),1,fp);
	fwrite("\n",1,1,fp);
	fclose(fp);

	delete[] text;
	LeaveCriticalSection(&LOG_CS);
};

void LogNoLock(char *format,...)
{
	va_list arguments;
	va_start(arguments, format);
	int len = _vscprintf(format, arguments ) + 1;
	char * text = new char[len];
	vsprintf_s(text, len, format, arguments);
	va_end(arguments);

	SYSTEMTIME t;
	char timebuf[256];
	GetLocalTime(&t);
	sprintf_s(timebuf,256,"[%04d-%02d-%02d %02d:%02d:%02d]",t.wYear,t.wMonth,t.wDay,t.wHour, t.wMinute, t.wSecond );

	FILE *fp = 0;
	fopen_s(&fp,"d2warden.log","a+");
	if(!fp) { 	LeaveCriticalSection(&LOG_CS); return; }
	fseek(fp,0,SEEK_END);
	fwrite(timebuf,strlen(timebuf),1,fp);
	fwrite(text,strlen(text),1,fp);
	fwrite("\n",1,1,fp);
	fclose(fp);

	delete[] text;
};

void Debug(char *format,...)
{
	va_list arguments;
	va_start(arguments, format);
	int len = _vscprintf(format, arguments ) + 1;
	char * text = new char[len];
	vsprintf_s(text, len, format, arguments);
	va_end(arguments);

	OutputDebugString(text);
	OutputDebugString("\n");

	delete[] text;
};



void LogToFile(char *FileName, bool PutTime, char *format,...)
{	
	va_list arguments;
	va_start(arguments, format);
	int len = _vscprintf(format, arguments ) + 1;
	char * text = new char[len];
	vsprintf_s(text, len, format, arguments);
	va_end(arguments);
	EnterCriticalSection(&LOG_CS);
	FILE *fp = 0;
	fopen_s(&fp,FileName,"a+");
	if(!fp) { LeaveCriticalSection(&LOG_CS); return;}
	fseek(fp,0,SEEK_END);

if(PutTime)
{
	SYSTEMTIME t;
	char timebuf[256];
	GetLocalTime(&t);
	sprintf_s(timebuf,256,"[%04d-%02d-%02d %02d:%02d:%02d]",t.wYear,t.wMonth,t.wDay,t.wHour, t.wMinute, t.wSecond );
	fwrite(timebuf,strlen(timebuf),1,fp);
}

	fwrite(text,len,1,fp);
	fwrite("\n",1,1,fp);
	fclose(fp);

	delete[] text;
	LeaveCriticalSection(&LOG_CS);
};

void LogError(unsigned char log_level_in,char *format,...)
{
	va_list arguments;
	va_start(arguments, format);
	int len = _vscprintf(format, arguments ) + 1;
	char * text = new char[len];
	vsprintf_s(text, len, format, arguments);
	va_end(arguments);

	SYSTEMTIME t;
	char timebuf[256];
	GetLocalTime(&t);
	sprintf_s(timebuf,256,"[%04d-%02d-%02d %02d:%02d:%02d]",t.wYear,t.wMonth,t.wDay,t.wHour, t.wMinute, t.wSecond );

	FILE *fp = 0;
	fopen_s(&fp,"d2warden-errors.log","a+");
	if(!fp) return;
	fseek(fp,0,SEEK_END);
	fwrite(timebuf,strlen(timebuf),1,fp);
	fwrite(text,strlen(text),1,fp);
	fclose(fp);

	delete[] text;
};

void LogMsg(char * GameName, char *format,...)
{
	va_list arguments;
	va_start(arguments, format);

	int len = _vscprintf(format, arguments ) + 1;
	char * text = new char[len];

	vsprintf_s(text, len, format, arguments);
	va_end(arguments);

	SYSTEMTIME t;
	char timebuf[256];
	GetLocalTime(&t);
	sprintf_s(timebuf,256,"[%04d-%02d-%02d %02d:%02d:%02d]",t.wYear,t.wMonth,t.wDay,t.wHour, t.wMinute, t.wSecond );
	char fn[40];
	sprintf_s(fn,40,"Msgs-%s.log",GameName);
	FILE *fp = 0;
	fopen_s(&fp,fn,"a+");
	if(!fp) return;
	fseek(fp,0,SEEK_END);
	fwrite(timebuf,strlen(timebuf),1,fp);
	fwrite(text,strlen(text),1,fp);
	fclose(fp);

	delete[] text;
};

int GetHexValue(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}

	if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}

	if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}

	return -1;
}


unsigned char Convert2HEX(unsigned char *tt)
{
	unsigned char tt0;
	
	tt0 = 0;
	tt0 = GetHexValue(tt[0]) * 16 + GetHexValue(tt[1]);
	return tt0;
}


int GetHexSpan(const char* string)
{
	int i = 0;
	for (; string[i] != '\0' && GetHexValue(string[i]) >= 0; i++);
	return i;
}

int ConvertHexStringToBytes(const char* string, unsigned char* bytes, int length)
{

	int span = GetHexSpan(string); 
	int offset = span / 2 + (span & 1) - 1;
	int end = span - 1;

	if (offset >= length)
	{
		offset = length - 1;
		end = length * 2 - 1;
	}

	int count = offset + 1;

	for (int i = end, j = 0; i >= 0 && offset < length; i--, j++)
	{
		int value = GetHexValue(string[i]);

		if ((j & 1) != 0)
		{
			bytes[offset--] |= (value << 4);
		}

		else
		{
			bytes[offset] = value;
		}
	}

	return count;
}

int ConvertBytesToHexString(const unsigned char* bytes, int dataSize, char* string, int stringSize, char delimiter)
{
	int count = 0;

	for (int i = 0; i < dataSize && stringSize - count > 2; i++)
	{
		unsigned char byte = bytes[i];
		
		if (delimiter != 0 && stringSize - count > 3 && i < dataSize - 1)
		{
			count += sprintf_s(string + count,stringSize-count, "%.2x%c", byte, delimiter);
		}

		else
		{
			count += sprintf_s(string + count,stringSize-count, "%.2x", byte);
		}
	}

	return count;
}

