/* ==========================================================
* d2warden
* https://bitbucket.org/lolet/d2warden
* ==========================================================
*			Copyright 2008 marsgod
*			 2011-2015 Bartosz Jankowski
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
*/

#include "stdafx.h"
#include "WardenClient.h"
#include "RC4.h"


extern "C" void  __fastcall HashGameSeed(unsigned char *pt_0XAE_RC4_KEY, unsigned char *pt_0X66_RC4_KEY, unsigned char * TheGameSeed, unsigned int TheLength);
extern "C" void  __fastcall Double_MD5(DWORD *Mod_Length, DWORD unk, unsigned char *ptResult);

WardenClient::WardenClient(DWORD ClientId, DWORD sessionKey)
{
	DEBUGMSG("Creating a WardenClient: %d", ClientId)
	ClientID = ClientId;

	unsigned char temp0x66[16], temp0xAE[16];

	*(DWORD*)&SessionKey = sessionKey;

	HashGameSeed(temp0xAE, temp0x66, SessionKey, 4);
	rc4_setup(RC4_KEY_0XAE, temp0xAE, 16);
	rc4_setup(RC4_KEY_0X66, temp0x66, 16);
	

	ErrorCount = 0;
	ptPlayer = 0;

	ptGame = 0;
	ptClientData = 0;

	ClientLogonTime = 0;
	NextCheckTime = 0;

	WardenStatus = WARDEN_START;
	CheckCounter = 0;
	LocaleId = 0;
	NewPatch = 0;
	DebugTrick = 0;

	memset(&pWardenPacket, 0, sizeof(WardenPacket));
	MOD_Position = 0;


	MouseXPosition = -1;
	MouseYPosition = -1;

	memset(UIModes, 0, 16);
	UIModesTime = 0;

	AnimData = 0;
	AnimData2 = 0;

	ptSkillsTxt = 0;

	TMCDetected = 0;
	StingDetected = 0;
	WtfDetected = 0;
	RCVDetected = 0;
	GMDetected = 0;
	LPDetected = 0;
	WardenBlocked = 0;
	RedVexDetected = 0;
	DupeDetected = 0;

	VerCode = 0;
	bNeedUpdate = false;
	ready = false;
}

void WardenClient::setup(Game* pGame, ClientData* pClient)
{
	ASSERT(pGame);
	ASSERT(pClient);

	ClientID = pClient->ClientID;

	ClientLogonTime = GetTickCount();
	NextCheckTime = ClientLogonTime + 500;
	AccountName = pClient->AccountName;
	CharName = pClient->CharName;
	ptClientData = pClient;
	ptGame = pGame;
	ptPlayer = pClient->pPlayerUnit;
	ready = true;

	DEBUGMSG("Setup finished for %s!", pClient->CharName);
}

void WardenClient::removePacket()
{
	if (pWardenPacket.ThePacket)
	{
		delete[] pWardenPacket.ThePacket;
		pWardenPacket.PacketLen = 0;
		pWardenPacket.ReceiveTime = 0;
		pWardenPacket.ThePacket = 0;
		pWardenPacket.ClientID = 0;
	}
}


void WardenClient::request(char* DllName1, DWORD Addr, char* DllName2, DWORD Addr2)
{
	//BYTE Pack[] = {0x02,0x0C,'D','2','C','l','i','e','n','t','.','d','l','l',0x0c,'D','2','C','l','i','e','n','t','.','d','l','l',0x00,0xec,0x01,0xFF,0xFF,0xFF,0xFF,0x04,0xec,0x02,0xFF,0xFF,0xFF,0xFF,0x04,0xd9};
	unsigned char WardenCMD2_local[100] = { 0 };
	//DEBUGMSG("Sending Warden request...")
	BYTE s1 = strlen(DllName1);
	BYTE s2 = strlen(DllName2);
	WORD PacketSize = s1 + s2 + 19;
	BYTE * Packet = new BYTE[PacketSize];
	Packet[0] = 0x02;
	Packet[1] = s1;
	memcpy(&Packet[2], DllName1, s1);
	Packet[2 + s1] = s2;
	memcpy(&Packet[3 + s1], DllName2, s2);

	Packet[3 + s1 + s2] = 0x00;
	Packet[4 + s1 + s2] = 0xEC;
	Packet[5 + s1 + s2] = 0x01;
	*(DWORD*)&Packet[6 + s1 + s2] = Addr;
	Packet[10 + s1 + s2] = 0x04;
	Packet[11 + s1 + s2] = 0xEC;
	Packet[12 + s1 + s2] = 0x02;
	*(DWORD*)&Packet[13 + s1 + s2] = Addr2;
	Packet[17 + s1 + s2] = 0x04;
	Packet[18 + s1 + s2] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, ClientID, WardenCMD2_local, PacketSize + 3);

	delete[] Packet;
}


void WardenClient::request(char* DllName1, DWORD Addr, BYTE nBytes1, char* DllName2, DWORD Addr2, BYTE nBytes2)
{
	//BYTE Pack[] = {0x02,0x0C,'D','2','C','l','i','e','n','t','.','d','l','l',0x0c,'D','2','C','l','i','e','n','t','.','d','l','l',0x00,0xec,0x01,0xFF,0xFF,0xFF,0xFF,0x04,0xec,0x02,0xFF,0xFF,0xFF,0xFF,0x04,0xd9};
	unsigned char WardenCMD2_local[100] = { 0 };
	//DEBUGMSG("Sending Warden request...")
	BYTE s1 = strlen(DllName1);
	BYTE s2 = strlen(DllName2);
	WORD PacketSize = s1 + s2 + 19;
	BYTE * Packet = new BYTE[PacketSize];
	Packet[0] = 0x02;
	Packet[1] = s1;
	memcpy(&Packet[2], DllName1, s1);
	Packet[2 + s1] = s2;
	memcpy(&Packet[3 + s1], DllName2, s2);

	Packet[3 + s1 + s2] = 0x00;
	Packet[4 + s1 + s2] = 0xEC;
	Packet[5 + s1 + s2] = 0x01;
	*(DWORD*)&Packet[6 + s1 + s2] = Addr;
	Packet[10 + s1 + s2] = nBytes1;
	Packet[11 + s1 + s2] = 0xEC;
	Packet[12 + s1 + s2] = 0x02;
	*(DWORD*)&Packet[13 + s1 + s2] = Addr2;
	Packet[17 + s1 + s2] = nBytes2;
	Packet[18 + s1 + s2] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, ClientID, WardenCMD2_local, PacketSize + 3);

	delete[] Packet;
}


void WardenClient::request(char* DllName1, DWORD Addr, BYTE Bytes)
{
	//DEBUGMSG("Sending Warden request...")
	unsigned char WardenCMD2_local[100] = { 0 };
	//02 0c 62 6E 63 6C 69 65 6E 74 2E 64 6C 6C 00 ec 01 30 D3 01 00 04 d9
	BYTE s1 = strlen(DllName1);
	WORD PacketSize = s1 + 11;
	BYTE * Packet = new BYTE[PacketSize];
	Packet[0] = 0x02;
	Packet[1] = s1;
	memcpy(&Packet[2], DllName1, s1);

	Packet[2 + s1] = 0x00;
	Packet[3 + s1] = 0xEC;
	Packet[4 + s1] = 0x01;
	*(DWORD*)&Packet[5 + s1] = Addr;
	Packet[9 + s1] = Bytes;
	Packet[10 + s1] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, ClientID, WardenCMD2_local, PacketSize + 3);

	delete[] Packet;
}

void WardenClient::request(DWORD Addr, BYTE Bytes)
{
	unsigned char WardenCMD2_local[100] = { 0 };
	WORD PacketSize = 10;
	BYTE Packet[10];
	Packet[0] = 0x02;
	Packet[1] = 0x00;
	Packet[2] = 0xEC;
	Packet[3] = 0x00;
	*(DWORD*)&Packet[4] = Addr;
	Packet[8] = Bytes;
	Packet[9] = 0xD9;
	WardenCMD2_local[0] = 0xAE;
	*(WORD*)&WardenCMD2_local[1] = PacketSize;
	memcpy(&WardenCMD2_local[3], Packet, PacketSize);
	rc4_crypt(RC4_KEY_0XAE, &WardenCMD2_local[3], PacketSize);
	D2Funcs.D2NET_SendPacket(0, ClientID, WardenCMD2_local, PacketSize + 3);
}

WardenClient::~WardenClient()
{
	DEBUGMSG("Removing a WardenClient")
	removePacket();
}