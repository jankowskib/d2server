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

#ifndef __WARDENCLIENT_H__
#define __WARDENCLIENT_H__

#include <string>
#include <memory>
#include "CommonStructs.h"
#include "Windows.h"
#include <list>

enum WardenStatus;

struct WardenPacket
{
	//#ifdef _DEBUG
	DWORD SendTime;
	DWORD ReceiveTime;
	//#endif
	DWORD ClientID;
	DWORD PacketLen;
	BYTE *ThePacket;
};

class WardenClient
{
public:
	WardenClient(DWORD ClientId, DWORD sessionKey);
	WardenClient& operator=(const WardenClient&) = default;
	WardenClient(const WardenClient&) = default;
	~WardenClient();
	void setup(Game* pGame, ClientData* pClient);
	void removePacket();

	//Memory request funcs
	void request(char* DllName1, DWORD Addr, char* DllName2, DWORD Addr2);
	void request(char* DllName1, DWORD Addr, BYTE nBytes1, char* DllName2, DWORD Addr2, BYTE nBytes2);
	void request(char* DllName1, DWORD Addr, BYTE Bytes);
	void request(DWORD Addr, BYTE Bytes);

	DWORD ClientID;
	DWORD ErrorCount;

	UnitAny* ptPlayer;
	Game* ptGame;
	ClientData* ptClientData;

	unsigned char SessionKey[16];

	std::string CharName;
	std::string AccountName;
	std::string MyIp;

	DWORD ClientLogonTime;
	DWORD NextCheckTime;

	WardenStatus WardenStatus;
	BYTE CheckCounter;
	BYTE LocaleId;
	BYTE NewPatch;
	BYTE DebugTrick;

	WardenPacket pWardenPacket;
	DWORD MOD_Position;

	unsigned char RC4_KEY_0X66[258];
	unsigned char RC4_KEY_0XAE[258];

	signed short MouseXPosition;
	signed short MouseYPosition;

	DWORD UIModes[16];
	DWORD UIModesTime;

	DWORD AnimData;
	DWORD AnimData2;

	DWORD ptSkillsTxt;

	BYTE TMCDetected;
	BYTE StingDetected;
	BYTE WtfDetected;
	BYTE RCVDetected;
	BYTE GMDetected;
	BYTE LPDetected;
	BYTE WardenBlocked;
	BYTE RedVexDetected;
	BYTE DupeDetected;

	BYTE VerCode;
	bool bNeedUpdate;
	bool ready;

private:
	WardenClient() = delete;
};

typedef std::list<WardenClient>::iterator WardenClient_i;

#endif