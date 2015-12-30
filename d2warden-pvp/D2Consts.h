/* ==========================================================
 * d2warden
 * https://github.com/lolet/d2warden
 * ==========================================================
 * Copyright 2011-2014 Bartosz Jankowski
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


enum DifficultyLevels
{
	DIFF_NORMAL = 0,
	DIFF_NIGHTMARE = 1,
	DIFF_HELL = 2,
};

enum D2Acts {
	D2ACT_I = 0,
	D2ACT_II,
	D2ACT_III,
	D2ACT_IV,
	D2ACT_V,
};

enum DrlgType
{
	DRLG_MAZE = 1,
	DRLG_PRESET = 2,
	DRLG_PRESETMAZE = 3,
};


/*
http://d2mods.info/forum/viewtopic.php?f=8&t=61497, Thanks Necrolis!
*/
enum CollisionMasks
{
	COLLIDE_NONE = 0x0000,
	COLLIDE_BLOCK_PLAYER = 0x0001,      // 'black space' in arcane sanctuary, cliff walls etc
	COLLIDE_BLOCK_MISSILE = 0x0002,      // tile based obstacles you can't shoot over
	COLLIDE_WALL = 0x0004,
	COLLIDE_BLOCK_LEAP = 0x0008,
	COLLIDE_ALTERNATE_FLOOR = 0x0010,      // some floors have this set, others don't
	COLLIDE_BLANK = 0x0020,      // returned if the subtile is invalid
	COLLIDE_MISSILE = 0x0040,
	COLLIDE_PLAYER = 0x0080,
	COLLIDE_MONSTER = 0x0100,
	COLLIDE_ITEM = 0x0200,
	COLLIDE_OBJECT = 0x0400,
	COLLIDE_DOOR = 0x0800,
	COLLIDE_UNIT_RELATED = 0x1000,      // set for units sometimes, but not always
	COLLIDE_PET = 0x2000,
	COLLIDE_4000 = 0x4000,
	COLLIDE_CORPSE = 0x8000,      // also used by portals, but dead monsters are mask 0x8000

	COLLIDE_BLOCK_UNITS = COLLIDE_BLOCK_PLAYER,

	COLLISION_INVALID = (COLLIDE_BLANK | COLLIDE_WALL | COLLIDE_BLOCK_MISSILE | COLLIDE_BLOCK_PLAYER), //0x27
	COLLISION_LOS_THREAT = (COLLIDE_MONSTER | COLLIDE_MONSTER), //0x180
	COLLISION_ITEMS = (COLLIDE_BLOCK_UNITS | COLLIDE_DOOR), // 0x801
	COLLISION_LOS_WALL = (COLLIDE_WALL | COLLIDE_DOOR), // 0x804
	COLLISION_LOS = (COLLIDE_BLOCK_UNITS | COLLIDE_WALL | COLLIDE_DOOR), // 0x805
	COLLISION_VISDOOR = (COLLIDE_BLOCK_MISSILE | COLLIDE_WALL | COLLIDE_DOOR), // 0x806
	PLAYER_COLLISION_WW = (COLLIDE_OBJECT | COLLIDE_BLOCK_UNITS | COLLIDE_DOOR), // 0xC01
	PLAYER_COLLISION_DEFAULT = (COLLIDE_BLOCK_UNITS | COLLIDE_BLOCK_LEAP | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED), // 0x1C09
	MONSTER_COLLISION_FLY = (COLLIDE_WALL | COLLIDE_DOOR | COLLIDE_UNIT_RELATED), // 0x1804
	MONSTER_COLLISION_DOORS = (COLLIDE_BLOCK_UNITS | COLLIDE_OBJECT | COLLIDE_UNIT_RELATED | COLLIDE_PET), // 0x3401
	MONSTER_COLLISION_DEFAULT = (COLLIDE_BLOCK_UNITS | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED | COLLIDE_PET), // 0x3C01
	SPAWN_ITEM_COLLISION = (COLLIDE_BLOCK_UNITS | COLLIDE_ITEM | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED | COLLIDE_PET), // 0x3E01
	SPAWN_UNIT_COLLISION = (COLLIDE_BLOCK_UNITS | COLLIDE_ALTERNATE_FLOOR | COLLIDE_MONSTER | COLLIDE_ITEM | COLLIDE_OBJECT | COLLIDE_DOOR | COLLIDE_UNIT_RELATED | COLLIDE_PET), // 0x3F11
	COLLISION_ALL_BUT_CORPSE = (COLLIDE_4000 | SPAWN_UNIT_COLLISION | COLLIDE_PLAYER | COLLIDE_MISSILE | COLLIDE_BLANK | COLLIDE_BLOCK_LEAP | COLLIDE_WALL | COLLIDE_BLOCK_MISSILE), //0x7FFF
	COLLISION_WARP = (COLLIDE_CORPSE | COLLIDE_PET | COLLIDE_UNIT_RELATED | COLLIDE_DOOR | COLLIDE_OBJECT | COLLIDE_ITEM | COLLIDE_ALTERNATE_FLOOR | COLLIDE_BLOCK_PLAYER), //0xBE11
	COLLISION_ALL = 0xFFFF,
};

enum UnitEvents
{
	UNITEVENT_MODECHANGE = 0,
	UNITEVENT_ENDANIM = 1,
	UNITEVENT_AITHINK = 2,
	UNITEVENT_STATREGEN = 3,
	UNITEVENT_TRAP = 4,
	UNITEVENT_ACTIVESTATE = 5,
	UNITEVENT_RESET = 5,
	UNITEVENT_FREEHOVER = 6,
	UNITEVENT_UMODCALLBACK = 7,
	UNITEVENT_QUESTCALLBACK = 7,
	UNITEVENT_PERIODICSKILLS = 8,
	UNITEVENT_PERIODICSTATS = 9,
	UNITEVENT_AIRESET = 10,
	UNITEVENT_DELAYEDPORTAL = 11,
	UNITEVENT_REMOVESTATE = 12,
	UNITEVENT_UPDATETRADE = 13,
	UNITEVENT_REFRESHVENDOR = 13,
	UNITEVENT_REMOVESKILLCOOLDOWN = 14,
	UNITEVENT_CUSTOM = 15,
};

enum TradeButtons
{
	TRADE_CANCEL = 2,				//ok
	TRADE_PERFORM = 3,				//ok
	TRADE_ACCEPT = 4,
	TRADE_GREEN_UNCHECK = 7,		//ok
	TRADE_SENDGOLD,					//ok	
	TRADE_CLOSE_STASH = 0x12,		//ok
	TRADE_STASH_WITHDRAW,			//ok
	TRADE_STASH_DEPOSIT,			//ok
	TRADE_CLOSE_CUBE = 0x17,		//ok
	TRADE_TRANSMUTE					//ok
};

enum TradeStates
{
	TRADE_STATE_OPENED = 3,
	TRADE_STATE_GREEN_PRESSED = 4,
	TRADE_RESETED = 5,
	TRADE_STATE_TRADE_DONE = 9,
	TRADE_STATE_FAIL = 12
};

enum UpdateClient
{
	UPDATE_ASK_TO_TRADE = 0,
	UPDATE_ASKED_TO_TRADE,
	UPDATE_ACCEPT_TRADE = 5,
	UPDATE_OPENTRADE = 6,
	UPDATE_SELLER_NOROOM = 9,
	UPDATE_BUYER_NOROOM = 0xA,
	UPDATE_CLOSE_TRADE = 0x0C,
	UPDATE_TRADE_DONE = 0xD,
	UPADTE_RED_CHECKBOX = 0xE,
	UPDATE_NORM_CHECKBOX = 0xF,
	UPADTE_OPEN_STASH = 0x10,
	UPDATE_OPEN_CUBE = 0x15
};

enum MessageResult
{
	MSG_OK = 0,
	MSG_UNK = 1,
	MSG_ERROR = 2,
	MSG_HACK = 3
};

enum PartyButtons
{
	PB_LOOT = 1,
	PB_MUTE = 2,
	PB_SQUELCH = 3,
	PB_HOSTILE = 4,
	PB_INVITE_PLAYER = 6,
	PB_INVITE_CANCEL = 7,
	PB_INVITE_ACCEPT = 8,
	PB_LEAVE_PARTY = 9,
	// -- my addtion below
	PB_SPECATE = 10,
};

enum ExEventMsgs
{
	EXEVENT_TEXTMSG = 1,
	EXEVENT_OVERHEAD = 2,
	EXEVENT_DOWNLOAD = 3,
	EXEVENT_MAPREVEAL = 4,
	EXEVENT_OPTIONS = 5,
	EXEVENT_SPECTATOR_START = 6,
	EXEVENT_SPECTATOR_END = 7,
};

enum ExEventOption
{
	EXOP_RESPAWNTIME = 1,
};

enum EventMsgType
{
	EVENT_TIMEOUT = 0,
	EVENT_DROPPED = 1,
	EVENT_JOINED = 2,
	EVENT_LEFT = 3,
	EVENT_NOT_IN_GAME = 4,
	EVENT_NOT_LOGGED = 5,
	EVENT_SLAIN = 6,
	EVENT_PARTY = 7,
	EVENT_BUSY = 8,
	EVENT_TRADE_WAIT = 9,
	EVENT_ITEMS_BOX = 10,
	EVENT_GUILD_BOOTED = 11,
	EVENT_GUILD_UNK = 12,
	EVENT_NOT_LISTEN = 13,
	EVENT_NO_MANA = 14,
	EVENT_REALM_SHUTDOWN = 15,
	EVENT_HOSTILE_WAIT = 16,
	EVENT_SOJ_SELL = 17,
	EVENT_DIABLO_WALKS = 18
};

enum D2Langs
{
	LOC_EN = 0,
	LOC_PL = 10
};

enum ExSounds
{
	STAND_SELECT = 2,
	STAND_CLICK = 4,
	STAND_WINOPEN = 5,
	STAND_LEVLEUP = 7,
	STAND_ARCANEPORTAL = 2556,
	D2EX_IMPRESSIVE = 4699,
	D2EX_DOUBLEKILL = 4700,
	D2EX_FIRSTBLOOD = 4701,
	D2EX_HEADSHOT = 4702,
	D2EX_HOLYSHIT = 4703,
	D2EX_HUMILATION = 4704,
	D2EX_KILLINGSPREE = 4705,
	D2EX_MONSTERKILL = 4706,
	D2EX_PERFECT = 4707,
	D2EX_PLAY = 4708,
	D2EX_PREPARE = 4709,
	D2EX_TRIPLEKILL = 4710,
	D2EX_UNSTOPPABLE = 4711,
	D2EX_DOMINATION = 4712,
	D2EX_ACHIVEMENT = 4713
};

enum UnitTypes
{
	UNIT_PLAYER = 0,
	UNIT_MONSTER = 1,
	UNIT_OBJECT = 2,
	UNIT_MISSILE = 3,
	UNIT_ITEM = 4,
	UNIT_TILE = 5
};

enum UNITFLAG
{
	UNITFLAG_OPERATED = 0x1,
	UNITFLAG_SELECTABLE = 0x2,
	UNITFLAG_ATTACKABLE = 0x4,
	UNITFLAG_VALIDTARGET = 0x8,
	UNITFLAG_SEEDSET = 0x10,
	UNITFLAG_DEATHVANISH = 0x20,
	UNITFLAG_SKILLEND = 0x40,
	UNITFLAG_NEWMODE = 0x80,
	UNITFLAG_HOVERMSG = 0x100,
	UNITFLAG_HASHOVERBAR = 0x200,
	UNITFLAG_CLIENTSOUND = 0x400,
	UNITFLAG_SUMMONER = 0x800,
	UNITFLAG_REFRESH = 0x1000,
	UNITFLAG_NONEUTRAL = 0x2000,
	UNITFLAG_CLIENTUPDATE = 0x8000,
	UNITFLAG_DEAD = 0x10000,
	UNITFLAG_NOITEMS = 0x20000,
	UNITFLAG_AISTARTED = 0x80000,
	UNITFLAG_NEUTRAL = 0x200000,
	UNITFLAG_UNITINIT = 0x1000000,
	UNITFLAG_OTHERINIT = 0x2000000,
	UNITFLAG_NOEXP = 0x4000000,
	UNITFLAG_PETIGNORE = 0x40000000,
	UNITFLAG_ISREVIVE = 0x80000000,
};


enum D2Colors
{
	COL_WHITE = 0x0,
	COL_RED = 0x1,
	COL_LIGHTGREEN = 0x2,
	COL_BLUE = 0x3,
	COL_DARK_GOLD = 0x4,
	COL_GREY = 0x5,
	COL_BLACK = 0x6,
	COL_GOLD = 0x7,
	COL_ORANGE = 0x8,
	COL_YELLOW = 0x9,
	COL_DARKGREEN = 0xA,
	COL_PURPLE = 0xB,
	COL_GREEN = 0xC,
	COL_WHITE2 = 0xD,
	COL_BLACK2 = 0xE,
	COL_DARKWHITE = 0xF,
};

enum ItemTypes
{
	ITEMTYPE_NONE = 0x0,
	ITEMTYPE_NONE2 = 0x1,
	ITEMTYPE_SHIELD = 0x2,
	ITEMTYPE_ARMOR = 0x3,
	ITEMTYPE_GOLD = 0x4,
	ITEMTYPE_BOW_QUIVER = 0x5,
	ITEMTYPE_CROSSBOW_QUIVER = 0x6,
	ITEMTYPE_PLAYER_BODY_PART = 0x7,
	ITEMTYPE_HERB = 0x8,
	ITEMTYPE_POTION = 0x9,
	ITEMTYPE_RING = 0xA,
	ITEMTYPE_ELIXIR = 0xB,
	ITEMTYPE_AMULET = 0xC,
	ITEMTYPE_CHARM = 0xD,
	ITEMTYPE_NOT_USED = 0xE,
	ITEMTYPE_BOOTS = 0xF,
	ITEMTYPE_GLOVES = 0x10,
	ITEMTYPE_NOT_USED2 = 0x11,
	ITEMTYPE_BOOK = 0x12,
	ITEMTYPE_BELT = 0x13,
	ITEMTYPE_GEM = 0x14,
	ITEMTYPE_TORCH = 0x15,
	ITEMTYPE_SCROLL = 0x16,
	ITEMTYPE_NOT_USED3 = 0x17,
	ITEMTYPE_SCEPTER = 0x18,
	ITEMTYPE_WAND = 0x19,
	ITEMTYPE_STAFF = 0x1A,
	ITEMTYPE_BOW = 0x1B,
	ITEMTYPE_AXE = 0x1C,
	ITEMTYPE_CLUB = 0x1D,
	ITEMTYPE_SWORD = 0x1E,
	ITEMTYPE_HAMMER = 0x1F,
	ITEMTYPE_KNIFE = 0x20,
	ITEMTYPE_SPEAR = 0x21,
	ITEMTYPE_POLEARM = 0x22,
	ITEMTYPE_CROSSBOW = 0x23,
	ITEMTYPE_MACE = 0x24,
	ITEMTYPE_HELM = 0x25,
	ITEMTYPE_MISSILE_POTION = 0x26,
	ITEMTYPE_QUEST = 0x27,
	ITEMTYPE_BODY_PART = 0x28,
	ITEMTYPE_KEY = 0x29,
	ITEMTYPE_THROWING_KNIFE = 0x2A,
	ITEMTYPE_THROWING_AXE = 0x2B,
	ITEMTYPE_JAVELIN = 0x2C,
	ITEMTYPE_WEAPON = 0x2D,
	ITEMTYPE_MELEE_WEAPON = 0x2E,
	ITEMTYPE_MISSILE_WEAPON = 0x2F,
	ITEMTYPE_THROWN_WEAPON = 0x30,
	ITEMTYPE_COMBO_WEAPON = 0x31,
	ITEMTYPE_ANY_ARMOR = 0x32,
	ITEMTYPE_ANY_SHIELD = 0x33,
	ITEMTYPE_MISCELLANEOUS = 0x34,
	ITEMTYPE_SOCKET_FILLER = 0x35,
	ITEMTYPE_SECOND_HAND = 0x36,
	ITEMTYPE_STAVES_AND_RODS = 0x37,
	ITEMTYPE_MISSILE = 0x38,
	ITEMTYPE_BLUNT = 0x39,
	ITEMTYPE_JEWEL = 0x3A,
	ITEMTYPE_CLASS_SPECIFIC = 0x3B,
	ITEMTYPE_AMAZON_ITEM = 0x3C,
	ITEMTYPE_BARBARIAN_ITEM = 0x3D,
	ITEMTYPE_NECROMANCER_ITEM = 0x3E,
	ITEMTYPE_PALADIN_ITEM = 0x3F,
	ITEMTYPE_SORCERESS_ITEM = 0x40,
	ITEMTYPE_ASSASSIN_ITEM = 0x41,
	ITEMTYPE_DRUID_ITEM = 0x42,
	ITEMTYPE_HAND_TO_HAND = 0x43,
	ITEMTYPE_ORB = 0x44,
	ITEMTYPE_VOODOO_HEADS = 0x45,
	ITEMTYPE_AURIC_SHIELDS = 0x46,
	ITEMTYPE_PRIMAL_HELM = 0x47,
	ITEMTYPE_PELT = 0x48,
	ITEMTYPE_CLOAK = 0x49,
	ITEMTYPE_RUNE = 0x4A,
	ITEMTYPE_CIRCLET = 0x4B,
	ITEMTYPE_HEALING_POTION = 0x4C,
	ITEMTYPE_MANA_POTION = 0x4D,
	ITEMTYPE_REJUV_POTION = 0x4E,
	ITEMTYPE_STAMINA_POTION = 0x4F,
	ITEMTYPE_ANTIDOTE_POTION = 0x50,
	ITEMTYPE_THAWING_POTION = 0x51,
	ITEMTYPE_SMALL_CHARM = 0x52,
	ITEMTYPE_MEDIUM_CHARM = 0x53,
	ITEMTYPE_LARGE_CHARM = 0x54,
	ITEMTYPE_AMAZON_BOW = 0x55,
	ITEMTYPE_AMAZON_SPEAR = 0x56,
	ITEMTYPE_AMAZON_JAVELIN = 0x57,
	ITEMTYPE_HAND_TO_HAND_2 = 0x58,
	ITEMTYPE_MAGIC_BOW_QUIV = 0x59,
	ITEMTYPE_MAGIC_XBOW_QUIV = 0x5A,
};


enum ItemLocation
{
	ITEMLOC_NONE = 0,			// Not equipped
	ITEMLOC_HEAD,				// Head
	ITEMLOC_AMULET,			// Amulet
	ITEMLOC_BODY,				// Body armor
	ITEMLOC_RIGHT_PRIMARY,	// Right primary slot
	ITEMLOC_LEFT_PRIMARY,		// Left primary slot
	ITEMLOC_RIGHT_RING,		// Right ring
	ITEMLOC_LEFT_RING,		// Left ring
	ITEMLOC_BELT,				// Belt
	ITEMLOC_FEET,				// Feet
	ITEMLOC_GLOVES,			// Gloves
	ITEMLOC_RIGHT_SECONDARY,	// Right secondary slot
	ITEMLOC_LEFT_SECONDARY,	// Left secondary slot
};


enum UnitModes
{
	PLAYER_MODE_DEATH = 0,				// death
	PLAYER_MODE_STAND_OUTTOWN,			// standing outside town
	PLAYER_MODE_WALK_OUTTOWN,			// walking outside town
	PLAYER_MODE_RUN,					// running
	PLAYER_MODE_BEING_HIT,				// being hit
	PLAYER_MODE_STAND_INTOWN,			// standing inside town
	PLAYER_MODE_WALK_INTOWN,			// walking outside town
	PLAYER_MODE_ATTACK1,				// attacking 1
	PLAYER_MODE_ATTACK2,				// attacking 2
	PLAYER_MODE_BLOCK,					// blocking
	PLAYER_MODE_CAST,					// casting spell
	PLAYER_MODE_THROW,					// throwing
	PLAYER_MODE_KICK,					// kicking (assassin)
	PLAYER_MODE_USESKILL1,				// using skill 1
	PLAYER_MODE_USESKILL2,				// using skill 2
	PLAYER_MODE_USESKILL3,				// using skill 3
	PLAYER_MODE_USESKILL4,				// using skill 4
	PLAYER_MODE_DEAD,					// dead
	PLAYER_MODE_SEQUENCE,				// sequence
	PLAYER_MODE_BEING_KNOCKBACK
};		// being knocked back

enum MonsterMode
{
	NPC_MODE_DEATH = 0,		// NPC death
	NPC_MODE_STAND,			// NPC standing still
	NPC_MODE_WALK,			// NPC walking
	NPC_MODE_BEING_HIT,		// NPC getting hit
	NPC_MODE_ATTACK1,		// NPC attacking 1
	NPC_MODE_ATTACK2,		// NPC attacking 2
	NPC_MODE_BLOCK,			// NPC blocking
	NPC_MODE_CAST,			// NPC casting spell skill
	NPC_MODE_USESKILL1,		// NPC using skill 1
	NPC_MODE_USESKILL2,		// NPC using skill 2
	NPC_MODE_USESKILL3,		// NPC using skill 3
	NPC_MODE_USESKILL4,		// NPC using skill 4
	NPC_MODE_DEAD,			// NPC dead
	NPC_MODE_BEING_KNOCKBACK,// NPC being knocked back
	NPC_MODE_SEQUENCE,		// NPC sequence
	NPC_MODE_RUN			// NPC running
};


enum ItemMode
{
	ITEM_MODE_INV_STASH_CUBE_STORE = 0,	// Item inven stash cube store
	ITEM_MODE_EQUIPPED = 1,					// Item equipped self or merc
	ITEM_MODE_IN_BELT = 2,					// Item in belt
	ITEM_MODE_ON_GROUND = 3,				// Item on ground
	ITEM_MODE_ON_CURSOR = 4,				// Item on cursor
	ITEM_MODE_BEING_DROPPED = 5,			// Item being dropped
	ITEM_MODE_SOCKETED_IN_ITEM = 6		// Item socketed in item
};


enum ObjectMode 
{
	OBJ_MODE_IDLE = 0,        // Object idle
	OBJ_MODE_OPERATING,        // Object operating
	OBJ_MODE_OPENED,        // Object opened
	OBJ_MODE_SPECIAL1,        // Object special 1
	OBJ_MODE_SPECIAL2,        // Object special 2
	OBJ_MODE_SPECIAL3,        // Object special 3
	OBJ_MODE_SPECIAL4,        // Object special 4
	OBJ_MODE_SPECIAL5		 // Object special 5
};   

enum ItemQuality
{
	ITEM_QUALITY_INVALID = 0,
	ITEM_QUALITY_LOW,
	ITEM_QUALITY_NORMAL,
	ITEM_QUALITY_SUPERIOR,
	ITEM_QUALITY_MAGIC,
	ITEM_QUALITY_SET,
	ITEM_QUALITY_RARE,
	ITEM_QUALITY_UNIQUE,
	ITEM_QUALITY_CRAFTED,
	ITEM_QUALITY_TEMPERED
};

enum ItemFlags
{
	ITEMFLAG_NEWITEM = 0x00000001,
	ITEMFLAG_TARGET = 0x00000002,
	ITEMFLAG_TARGETING = 0x00000004,
	ITEMFLAG_DELETED = 0x00000008,
	ITEMFLAG_IDENTIFIED = 0x00000010,
	ITEMFLAG_QUANTITY = 0x00000020,
	ITEMFLAG_WEAPONSET_IN = 0x00000040,
	ITEMFLAG_WEAPONSET_OUT = 0x00000080,
	ITEMFLAG_BROKEN = 0x00000100,
	ITEMFLAG_REPAIRED = 0x00000200,
	ITEMFLAG_UNKNOWN2 = 0x00000400,
	ITEMFLAG_SOCKETED = 0x00000800,
	ITEMFLAG_NON_SELLABLE = 0x00001000,
	ITEMFLAG_INSTORE = 0x00002000,
	ITEMFLAG_NOEQUIP = 0x00004000,
	ITEMFLAG_NAMED = 0x00008000,
	ITEMFLAG_ORGAN = 0x00010000,
	ITEMFLAG_SELLCHEAP = 0x00020000,
	ITEMFLAG_UNK5 = 0x00040000,
	ITEMFLAG_INIT = 0x00080000,
	ITEMFLAG_UNK6 = 0x00100000,
	ITEMFLAG_COMPACTSAVE = 0x00200000,
	ITEMFLAG_ETHEREAL = 0x00400000,
	ITEMFLAG_JUSTSAVED = 0x00800000,
	ITEMFLAG_PERSONALIZED = 0x01000000,
	ITEMFLAG_LOWQUALITY = 0x02000000,
	ITEMFLAG_RUNEWORD = 0x04000000,
	ITEMFLAG_ITEM = 0x08000000
};

enum QuestsId
{
	ACT1_INTRO = 0x00, //a1q0 		act introduction
	ACT1_DEN = 0x01, //a1q1   Den of Evil
	ACT1_RAVEN = 0x02, //a1q2   Sisters' Burial Grounds
	ACT1_IMBUE = 0x03, //a1q3   Tools of the Trade
	ACT1_CAIN = 0x04, //a1q4   Rescue Deckard Cain
	ACT1_COUNTESS = 0x05, //a1q5   The Forgotten Tower
	ACT1_ANDARIEL = 0x06, //a1q6   Sisters to the Slaughter
	ACT1_UNUSED = 0x07,   //       <<not used???>>
	ACT2_INTRO = 0x08, //a2q0 	act introduction
	ACT2_RADAMENT = 0x09, //a2q1   Radament's Lair
	ACT2_STAFF = 0x0A, //a2q2   The Horadric Staff
	ACT2_AMU = 0x0B, //a2q3   The Tainted Sun
	ACT2_ARCANE = 0x0C, //a2q4   The Arcane Sanctuary
	ACT2_SUMMONER = 0x0D, //a2q5   The Summoner
	ACT2_DURIEL = 0x0E, //a2q6   The Tomb of Tal Rasha
	ACT2_UNUSED = 0x0F,        //  <<not used???>>
	ACT3_INTRO = 0x10, //a3q0 	act introduction
	ACT3_BOOK = 0x11, //a3q1   Lam Esen's Tome
	ACT3_KHALIM = 0x12, //a3q2   Khalim's Will
	ACT3_GIDBIN = 0x13, //a3q3   Blade of The Old Religion
	ACT3_GOLDENBIRD = 0x14, //a3q4   The Golden Bird   
	ACT3_TRAVINCAL = 0x15, //a3q5   The Blackened Temple
	ACT3_MEPH = 0x16, //a3q6   The Lord of Hatred
	ACT3_UNUSED = 0x17,        //  <<not used???>>
	ACT4_IZUAL = 0x18, //a4q1   The Fallen Angel
	ACT4_DIABO = 0x19, //a4q2   Terror's End
	ACT4_FORGE = 0x1A, //a4q3   The Hellforge
	ACT4_FORGE_EX = 0x1B,         // (referred to) (also items "hfh" and "dss" -- "hfh" is Hellforge Hammer,   but "dss" not in item tables)
	ACT4_FORGE_EXX = 0x1C,       //   (referred to in Hellforge code)
	ACT4_UNUSED = 0x1D,        // <<not used???>>
	ACT2_Q7 = 0x1E, //a2q7   vestigial reference???
	ACT2_Q8 = 0x1F, //a2q8   vestigial reference???
	ACT3_Q7 = 0x20, //a3q7   vestigial reference???
	ACT_UNUSED = 0x21,         // <<not used???>>
	ACT_UNUSED2 = 0x22,       //   <<not used???>>
	ACT5_SHENK = 0x23, //a5q1   The Siege of Harrogath
	ACT5_RUNEQUEST = 0x24, //a5q2   Rescue on Mount Arreat
	ACT5_ANYA = 0x25, //a5q3   Prison of Ice
	ACT5_NITHALAK = 0x26, //a5q4   The Betrayal of Harrogath
	ACT5_ANCIENTS = 0x27, //a5q5   The Rite of Passage
	ACT5_BAAL = 0x28 //a5q6   Eve of Destruction
};

enum DmgResultFlag
{
	RESULTFLAG_HIT = 0x00000001,
	RESULTFLAG_DEATH = 0x00000002,
	RESULTFLAG_GETHIT = 0x00000004,
	RESULTFLAG_KNOCKBACK = 0x00000008,
	RESULTFLAG_BLOCK = 0x00000010,
	RESULTFLAG_DISALLOW_EVENTS = 0x00000020,
	RESULTFLAG_DODGE = 0x00000100,
	RESULTFLAG_AVOID = 0x00000200,
	RESULTFLAG_SOFTHIT = 0x00002000,
	RESULTFLAG_WEAPONBLOCK = 0x00008000
};

enum DmgHitFlag
{
	HITFLAG_SRCDAMAGE_MISSILE = 0x00000001,
	HITFLAG_SUCCESSFUL_CRITICAL_HIT = 0x00000002,
	HITFLAG_FRIENDLY_FIRE = 0x00000010,
	HITFLAG_SRCDAMAGE_MELEE = 0x00000020,
	HITFLAG_BYPASS_UNDEAD = 0x00000100,
	HITFLAG_BYPASS_BEASTS = 0x00000400,
	HITFLAG_BYPASS_DEMONS = 0x00000200,
};

enum Staty
{
	STAT_STRENGTH = 0		// str
	, STAT_ENERGY = 1		// energy
	, STAT_DEXTERITY = 2		// dexterity
	, STAT_VITALITY = 3		// vitality
	, STAT_HP = 6		// life
	, STAT_MAXHP = 7		// max life
	, STAT_MANA = 8		// mana
	, STAT_MAXMANA = 9		// max mana
	, STAT_STAMINA = 10		// stamina
	, STAT_MAXSTAMINA = 11		// max stamina
	, STAT_LEVEL = 12		// level
	, STAT_EXP = 13		// experience
	, STAT_GOLD = 14		// gold
	, STAT_GOLDBANK = 15		// stash gold
	, STAT_TOBLOCK = 20		// to block
	, STAT_DEFENSE = 30
	, STAT_DAMAGEREDUCTION = 36		// damage reduction
	, STAT_MAGICDAMAGEREDUCTION = 35		// magic damage reduction
	, STAT_MAGICRESIST = 37		// magic resist
	, STAT_MAXMAGICRESIST = 38		// max magic resist
	, STAT_FIRERESIST = 39		// fire resist
	, STAT_MAXFIRERESIST = 40		// max fire resist
	, STAT_LIGHTNINGRESIST = 41		// lightning resist
	, STAT_MAXLIGHTNINGRESIST = 42		// max lightning resist
	, STAT_COLDRESIST = 43		// cold resist
	, STAT_MAXCOLDRESIST = 44		// max cold resist
	, STAT_POISONRESIST = 45		// poison resist
	, STAT_MAXPOISONRESIST = 46		// max poison resist
	, STAT_LIFELEECH = 60		// Life Leech
	, STAT_MANALEECH = 62		// Mana Leech
	, STAT_VELOCITYPERCENT = 67		// effective run/walk
	, STAT_AMMOQUANTITY = 70		// ammo quantity(arrow/bolt/throwing)
	, STAT_DURABILITY = 72		// item durability
	, STAT_MAXDURABILITY = 73		// max item durability
	, STAT_HPREGEN = 74		// Life Replenish
	, STAT_GOLDFIND = 79		// Gold find (GF)
	, STAT_MAGICFIND = 80		// magic find (MF)
	, STAT_IAS = 93		// IAS
	, STAT_FASTERRUNWALK = 96		// faster run/walk
	, STAT_FASTERHITRECOVERY = 99		// faster hit recovery
	, STAT_FASTERBLOCK = 102		// faster block rate
	, STAT_FASTERCAST = 105		// faster cast rate
	, STAT_POISONLENGTHREDUCTION = 110		// Poison length reduction
	, STAT_OPENWOUNDS = 135		// Open Wounds
	, STAT_CRUSHINGBLOW = 136		// crushing blow
	, STAT_DEADLYSTRIKE = 141		// deadly strike
	, STAT_FIREABSORBPERCENT = 142		// fire absorb %
	, STAT_FIREABSORB = 143		// fire absorb
	, STAT_LIGHTNINGABSORBPERCENT = 144		// lightning absorb %
	, STAT_LIGHTNINGABSORB = 145		// lightning absorb
	, STAT_COLDABSORBPERCENT = 148		// cold absorb %
	, STAT_COLDABSORB = 149		// cold absorb
	, STAT_SLOW = 150		// slow %
	, STAT_SOCKETS = 194		// number of sockets
};



enum UIModes
{
	UI_INVENTORY = 0x01
	, UI_CHARACTER = 0x02
	, UI_MINISKILL = 0x03
	, UI_SKILL = 0x04
	, UI_CHAT = 0x05
	, UI_NEWSTATS = 0x06
	, UI_NEWSKILLS = 0x07
	, UI_NPCMENU = 0x08
	, UI_MAINMENU = 0x09
	, UI_AUTOMAP = 0x0a
	, UI_CONFIG = 0x0b
	, UI_NPCSHOP = 0x0c
	, UI_ALTDOWN = 0x0d
	, UI_ANVIL = 0x0e
	, UI_QUEST = 0x0f
	, UI_QUESTLOG = 0x11
	, UI_STATUSAREA = 0x12
	, UI_WPMENU = 0x14
	, UI_MINIPANEL = 0x15
	, UI_PARTY = 0x16
	, UI_TRADE = 0x17
	, UI_MSGS = 0x18
	, UI_STASH = 0x19
	, UI_CUBE = 0x1a
	, UI_BELT = 0x1f
	, UI_HELP = 0x21
	, UI_MERC = 0x24
	, UI_SCROLL = 0x25
};

///////////////////////////////////////////////////
// Common Spells
///////////////////////////////////////////////////

#define D2S_INVALID				0xffff // should never happen
#define D2S_ATTACK				0x0000
#define D2S_KICK				0x0001
#define D2S_THROW				0x0002
#define D2S_UNSUMMON			0x0003
#define D2S_LEFTTHROW			0x0004
#define D2S_LEFTSWING			0x0005
#define D2S_TOMEOFIDENTIFY		0x00da
#define D2S_SCROLLOFIDENTIFY	0x00d9
#define D2S_TOMEOFTOWNPORTAL	0x00dc
#define D2S_SCROLLOFTOWNPORTAL	0x00db

///////////////////////////////////////////////////////////////
// Amazon Spells
///////////////////////////////////////////////////////////////

// Javelin and Spear spells
#define D2S_JAB					0x000a
#define D2S_IMPALE				0x0013
#define D2S_FEND				0x001e
#define D2S_POWERSTRIKE			0x000e
#define D2S_CHARGEDSTRIKE		0x0018
#define D2S_LIGHTNINGSTRIKE		0x0022
#define D2S_POISONJAVELIN		0x000f
#define D2S_LIGHTNINGBOLT		0x0014
#define D2S_PLAGUEJAVELIN		0x0019
#define D2S_LIGHTNINGFURY		0x0023

// Passive and Magic spells
#define D2S_INNERSIGHT			0x0008
#define D2S_SLOWMISSILES		0x0011
#define D2S_DECOY				0x001c
#define D2S_VALKYRIE			0x0020
#define D2S_DODGE				0x000d
#define D2S_AVOID				0x0012
#define D2S_EVADE				0x001d
#define D2S_CRITICALSTRIKE		0x0009
#define D2S_PENETRATE			0x0017
#define D2S_PIERCE				0x0021

// Bow and Crossbow spells
#define D2S_COLDARROW			0x000b
#define D2S_ICEARROW			0x0015
#define D2S_FREEZINGARROW		0x001f
#define D2S_MAGICARROW			0x0006
#define D2S_MULTIPLESHOT		0x000c
#define D2S_GUIDEDARROW			0x0016
#define D2S_STRAFE				0x001a
#define D2S_FIREARROW			0x0007
#define D2S_EXPLODINGARROW		0x0010
#define D2S_IMMOLATIONARROW		0x001b


///////////////////////////////////////////////////////////////
// Assassin Spells
///////////////////////////////////////////////////////////////

// Martial Arts
#define D2S_FISTSOFFIRE			0x0103
#define D2S_CLAWSOFTHUNDER		0x010d
#define D2S_BLADESOFICE			0x0112
#define D2S_TIGERSTRIKE			0x00fe
#define D2S_COBRASTRIKE			0x0109
#define D2S_PHOENIXSTRIKE		0x0118
#define D2S_DRAGONTALON			0x00ff
#define D2S_DRAGONCLAW			0x0104
#define D2S_DRAGONTAIL			0x010e
#define D2S_DRAGONFLIGHT		0x0113

// Shadow Disciplines
#define D2S_BURSTOFSPEED		0x0102
#define D2S_FADE				0x010b
#define D2S_VENOM				0x0116
#define D2S_CLAWMASTERY			0x00fc
#define D2S_WEAPONBLOCK			0x0107
#define D2S_SHADOWWARRIOR		0x010c
#define D2S_SHADOWMASTER		0x0117
#define D2S_PSYCHICHAMMER		0x00fd
#define D2S_CLOAKOFSHADOWS		0x0108
#define D2S_MINDBLAST			0x0111

// Traps
#define D2S_SHOCKWEB			0x0100
#define D2S_CHARGEDBOLTSENTRY	0x0105
#define D2S_LIGHTNINGSENTRY		0x010f
#define D2S_DEATHSENTRY			0x0114
#define D2S_FIREBLAST			0x00fb
#define D2S_WAKEOFFIRE			0x0106
#define D2S_WAKEOFINFERNO		0x0110
#define D2S_BLADESENTINEL		0x0101
#define D2S_BLADEFURY			0x010a
#define D2S_BLADESHIELD			0x0115


///////////////////////////////////////////////////////////////
// Barbarian Spells
///////////////////////////////////////////////////////////////

// Warcries
#define D2S_HOWL				0x0082
#define D2S_TAUNT				0x0089
#define D2S_BATTLECRY			0x0092
#define D2S_WARCRY				0x009a
#define D2S_SHOUT				0x008a
#define D2S_BATTLEORDERS		0x0095
#define D2S_BATTLECOMMAND		0x009b
#define D2S_FINDPOTION			0x0083
#define D2S_FINDITEM			0x008e
#define D2S_GRIMWARD			0x0096

// Combat Masteries
#define D2S_SWORDMASTERY		0x007f
#define D2S_POLEARMMASTERY		0x0086
#define D2S_INCREASEDSTAMINA	0x008d
#define D2S_INCREASEDSPEED		0x0094
#define D2S_AXEMASTERY			0x0080
#define D2S_THROWINGMASTERY		0x0087
#define D2S_MACEMASTERY			0x0081
#define D2S_SPEARMASTERY		0x0088
#define D2S_IRONSKIN			0x0091
#define D2S_NATURALRESISTANCE	0x0099

// Combat spells
#define D2S_LEAP				0x0084
#define D2S_LEAPATTACK			0x008f
#define D2S_WHIRLWIND			0x0097
#define D2S_BASH				0x007e
#define D2S_STUN				0x008b
#define D2S_CONCENTRATE			0x0090
#define D2S_BERSERK				0x0098
#define D2S_DOUBLESWING			0x0085
#define D2S_DOUBLETHROW			0x008c
#define D2S_FRENZY				0x0093


///////////////////////////////////////////////////////////////
// Druid Spells
///////////////////////////////////////////////////////////////

// Elemental
#define D2S_FIRESTORM			0x00e1
#define D2S_MOLTENBOULDER		0x00e5
#define D2S_FISSURE				0x00ea
#define D2S_VOLCANO				0x00f4
#define D2S_ARMAGEDDON			0x00f9
#define D2S_TWISTER				0x00f0
#define D2S_TORNADO				0x00f5
#define D2S_HURRICANE			0x00fa
#define D2S_ARCTICBLAST			0x00e6
#define D2S_CYCLONEARMOR		0x00eb

// Shape Shifting
#define D2S_WEREWOLF			0x00df
#define D2S_FERALRAGE			0x00e8
#define D2S_RABIES				0x00ee
#define D2S_FURY				0x00f8
#define D2S_LYCANTHROPY			0x00e0
#define D2S_FIRECLAWS			0x00ef
#define D2S_HUNGER				0x00f2
#define D2S_WEREBEAR			0x00e4
#define D2S_MAUL				0x00e9
#define D2S_SHOCKWAVE			0x00f3

// Summoning
#define D2S_OAKSAGE				0x00e2
#define D2S_HEARTOFWOLVERINE	0x00ec
#define D2S_SPIRITOFBARBS		0x00f6
#define D2S_RAVEN				0x00dd
#define D2S_SUMMONSPIRITWOLF	0x00e3
#define D2S_SUMMONDIREWOLF		0x00ed
#define D2S_SUMMONGRIZZLY		0x00f7
#define D2S_POISONCREEPER		0x00de
#define D2S_CARRIONVINE			0x00e7
#define D2S_SOLARCREEPER		0x00f1


///////////////////////////////////////////////////////////////
// Necromancer Spells
///////////////////////////////////////////////////////////////

// Summoning Spells
#define D2S_SKELETONMASTERY		0x0045
#define D2S_GOLEMMASTERY		0x004f
#define D2S_SUMMONRESIST		0x0059
#define D2S_CLAYGOLEM			0x004b
#define D2S_BLOODGOLEM			0x0055
#define D2S_IRONGOLEM			0x005a
#define D2S_FIREGOLEM			0x005e
#define D2S_RAISESKELETON		0x0046
#define D2S_RAISESKELETALMAGE	0x0050
#define D2S_REVIVE				0x005f

// Poison and Bone Spells
#define D2S_POISONDAGGER		0x0049
#define D2S_POISONEXPLOSION		0x0053
#define D2S_POISONNOVA			0x005c
#define D2S_TEETH				0x0043
#define D2S_CORPOSEEXPLOSION	0x004a
#define D2S_BONESPEAR			0x0054
#define D2S_BONESPIRIT			0x005d
#define D2S_BONEARMOR			0x0044
#define D2S_BONEWALL			0x004e
#define D2S_BONEPRISON			0x0058

// Curses
#define D2S_DIMVISION			0x0047
#define D2S_CONFUSE				0x0051
#define D2S_ATTRACT				0x0056
#define D2S_AMPLIFYDAMAGE		0x0042
#define D2S_IRONMAIDEN			0x004c
#define D2S_LIFETAP				0x0052
#define D2S_LOWERRESIST			0x005b
#define D2S_WEAKEN				0x0048
#define D2S_TERROR				0x004d
#define D2S_DECREPIFY			0x0057


///////////////////////////////////////////////////////////////
// Paladin Spells
///////////////////////////////////////////////////////////////

// Defensive Auras
#define D2S_PRAYER				0x0063
#define D2S_CLEANSING			0x006d
#define D2S_MEDITATION			0x0078
#define D2S_DEFIANCE			0x0068
#define D2S_VIGOR				0x0073
#define D2S_REDEMPTION			0x007c
#define D2S_RESISTFIRE			0x0064
#define D2S_RESISTCOLD			0x0069
#define D2S_RESISTLIGHTNING		0x006e
#define D2S_SALVATION			0x007d

// Offensive Auras
#define D2S_MIGHT				0x0062
#define D2S_BLESSEDAIM			0x006c
#define D2S_CONCENTRATION		0x0071
#define D2S_FANATICISM			0x007a
#define D2S_HOLYFIRE			0x0066
#define D2S_HOLYFREEZE			0x0072
#define D2S_HOLYSHOCK			0x0076
#define D2S_THORNS				0x0067
#define D2S_SANCTUARY			0x0077
#define D2S_CONVICTION			0x007b

// Combat spells
#define D2S_SACRIFICE			0x0060
#define D2S_ZEAL				0x006a
#define D2S_VENGEANCE			0x006f
#define D2S_CONVERSION			0x0074
#define D2S_HOLYBOLT			0x0065
#define D2S_BLESSEDHAMMER		0x0070
#define D2S_FISTOFTHEHEAVENS	0x0079
#define D2S_SMITE				0x0061
#define D2S_CHARGE				0x006b
#define D2S_HOLYSHIELD			0x0075


///////////////////////////////////////////////////////////////
// Sorceress Spells
///////////////////////////////////////////////////////////////

// Cold Spells
#define D2S_FROSTNOVA			0x002c
#define D2S_BLIZZARD			0x003b
#define D2S_FROZENORB			0x0040
#define D2S_ICEBOLT				0x0027
#define D2S_ICEBLAST			0x002d
#define D2S_GLACIALSPIKE		0x0037
#define D2S_COLDMASTERY			0x0041
#define D2S_FROZENARMOR			0x0028
#define D2S_SHIVERARMOR			0x0032
#define D2S_CHILLINGARMOR		0x003c

// Lightning Spells
#define D2S_STATICFIELD			0x002a
#define D2S_NOVA				0x0030
#define D2S_THUNDERSTORM		0x0039
#define D2S_CHARGEDBOLT			0x0026
#define D2S_LIGHTNING			0x0031
#define D2S_CHAINLIGHTNING		0x0035
#define D2S_LIGHTNINGMASTERY	0x003f
#define D2S_TELEKINESIS			0x002b
#define D2S_TELEPORT			0x0036
#define D2S_ENERGYSHIELD		0x003a

// Fire Spells
#define D2S_INFERNO				0x0029
#define D2S_BLAZE				0x002e
#define D2S_FIREWALL			0x0033
#define D2S_FIREBOLT			0x0024
#define D2S_FIREBALL			0x002f
#define D2S_METEOR				0x0038
#define D2S_FIREMASTERY			0x003d
#define D2S_WARMTH				0x0025
#define D2S_ENCHANT				0x0034
#define D2S_HYDRA				0x003e


enum D2States
{
	freeze = 0x1,
	poison = 0x2,
	resistfire = 0x3,
	resistcold = 0x4,
	resistlight = 0x5,
	resistmagic = 0x6,
	playerbody = 0x7,
	resistall = 0x8,
	amplifydamage = 0x9,
	frozenarmor = 0xA,
	cold = 0xB,
	inferno = 0xC,
	blaze = 0xD,
	bonearmor = 0xE,
	concentrate = 0xF,
	enchant = 0x10,
	innersight = 0x11,
	skill_move = 0x12,
	weaken = 0x13,
	chillingarmor = 0x14,
	stunned = 0x15,
	spiderlay = 0x16,
	dimvision = 0x17,
	slowed = 0x18,
	fetishaura = 0x19,
	shout = 0x1A,
	taunt = 0x1B,
	conviction = 0x1C,
	convicted = 0x1D,
	energyshield = 0x1E,
	venomclaws = 0x1F,
	battleorders = 0x20,
	might = 0x21,
	prayer = 0x22,
	holyfire = 0x23,
	thorns = 0x24,
	defiance = 0x25,
	thunderstorm = 0x26,
	lightningbolt = 0x27,
	blessedaim = 0x28,
	stamina = 0x29,
	concentration = 0x2A,
	holywind = 0x2B,
	holywindcold = 0x2C,
	cleansing = 0x2D,
	holyshock = 0x2E,
	sanctuary = 0x2F,
	meditation = 0x30,
	fanaticism = 0x31,
	redemption = 0x32,
	battlecommand = 0x33,
	preventheal = 0x34,
	conversion = 0x35,
	uninterruptable = 0x36,
	ironmaiden = 0x37,
	terror = 0x38,
	attract = 0x39,
	lifetap = 0x3A,
	confuse = 0x3B,
	decrepify = 0x3C,
	lowerresist = 0x3D,
	openwounds = 0x3E,
	dopplezon = 0x3F,
	criticalstrike = 0x40,
	dodge = 0x41,
	avoid = 0x42,
	penetrate = 0x43,
	evade = 0x44,
	pierce = 0x45,
	warmth = 0x46,
	firemastery = 0x47,
	lightningmastery = 0x48,
	coldmastery = 0x49,
	swordmastery = 0x4A,
	axemastery = 0x4B,
	macemastery = 0x4C,
	polearmmastery = 0x4D,
	throwingmastery = 0x4E,
	spearmastery = 0x4F,
	increasedstamina = 0x50,
	ironskin = 0x51,
	increasedspeed = 0x52,
	naturalresistance = 0x53,
	fingermagecurse = 0x54,
	nomanaregen = 0x55,
	justhit = 0x56,
	slowmissiles = 0x57,
	shiverarmor = 0x58,
	battlecry = 0x59,
	blue = 0x5A,
	red = 0x5B,
	death_delay = 0x5C,
	valkyrie = 0x5D,
	frenzy = 0x5E,
	berserk = 0x5F,
	revive = 0x60,
	skel_mastery = 0x61,
	sourceunit = 0x62,
	redeemed = 0x63,
	healthpot = 0x64,
	holyshield = 0x65,
	just_portaled = 0x66,
	monfrenzy = 0x67,
	corpse_nodraw = 0x68,
	alignment = 0x69,
	manapot = 0x6A,
	shatter = 0x6B,
	sync_warped = 0x6C,
	conversion_save = 0x6D,
	pregnant = 0x6E,
	golem_mastery = 0x6F,
	rabies = 0x70,
	defense_curse = 0x71,
	blood_mana = 0x72,
	burning = 0x73,
	dragonflight = 0x74,
	maul = 0x75,
	corpse_noselect = 0x76,
	shadowwarrior = 0x77,
	feralrage = 0x78,
	skilldelay = 0x79,
	progressive_damage = 0x7A,
	progressive_steal = 0x7B,
	progressive_other = 0x7C,
	progressive_fire = 0x7D,
	progressive_cold = 0x7E,
	progressive_lightning = 0x7F,
	shrine_armor = 0x80,
	shrine_combat = 0x81,
	shrine_resist_lightning = 0x82,
	shrine_resist_fire = 0x83,
	shrine_resist_cold = 0x84,
	shrine_resist_poison = 0x85,
	shrine_skill = 0x86,
	shrine_mana_regen = 0x87,
	shrine_stamina = 0x88,
	shrine_experience = 0x89,
	fenris_rage = 0x8A,
	wolf = 0x8B,
	bear = 0x8C,
	bloodlust = 0x8D,
	changeclass = 0x8E,
	attached = 0x8F,
	hurricane = 0x90,
	armageddon = 0x91,
	invis = 0x92,
	barbs = 0x93,
	wolverine = 0x94,
	oaksage = 0x95,
	vine_beast = 0x96,
	cyclonearmor = 0x97,
	clawmastery = 0x98,
	cloak_of_shadows = 0x99,
	recycled = 0x9A,
	weaponblock = 0x9B,
	cloaked = 0x9C,
	quickness = 0x9D,
	bladeshield = 0x9E,
	fade = 0x9F,
	summonresist = 0xA0,
	oaksagecontrol = 0xA1,
	wolverinecontrol = 0xA2,
	barbscontrol = 0xA3,
	debugcontrol = 0xA4,
	itemset1 = 0xA5,
	itemset2 = 0xA6,
	itemset3 = 0xA7,
	itemset4 = 0xA8,
	itemset5 = 0xA9,
	itemset6 = 0xAA,
	runeword = 0xAB,
	restinpeace = 0xAC,
	corpseexp = 0xAD,
	whirlwind = 0xAE,
	fullsetgeneric = 0xAF,
	monsterset = 0xB0,
	delerium = 0xB1,
	antidote = 0xB2,
	thawing = 0xB3,
	staminapot = 0xB4,
	passive_resistfire = 0xB5,
	passive_resistcold = 0xB6,
	passive_resistltng = 0xB7,
	uberminion = 0xB8,
};

/*
	for 0xB4 packet
*/
enum BootReason {
	BOOT_BAD_CHARACTER_VERSION = 0,
	BOOT_BAD_QUEST_DATA,
	BOOT_BAD_WAYPOINT_DATA,
	BOOT_BAD_STATS_DATA,
	BOOT_BAD_SKILL_DATA,
	BOOT_UNABLE_TO_JOIN,
	BOOT_FAILED_TO_JOIN = 6,
	BOOT_CONNECTION_INTERRUPTED = 7,
	BOOT_HOST_LEFT = 8,
	BOOT_UNKNOWN_FAILURE = 9,
	BOOT_BAD_INVENTORY,
	BOOT_BAD_DEAD_BODY,
	BOOT_BAD_HEADER,
	BOOT_BAD_HIREABLE,
	BOOT_BAD_INTRO_DATA,
	BOOT_BAD_ITEM,
	BOOT_BAD_DEAD_BODY_ITEM,
	BOOT_BAD_GENERIC_BAD_FILE,
	BOOT_GAME_IS_FULL,
	BOOT_VERSION_MISMATCH,
	BOOT_CANNOT_INTO_NIGHTMARE,
	BOOT_CANNOT_INTO_HELL,
	BOOT_CANNOT_INTO_HARDCORE,
	BOOT_CANNOT_INTO_SOFTCORE,
	BOOT_DEAD_HARDCORE,
	BOOT_CANNOT_INTO_EXPANSION,
	BOOT_CANNOT_INTO_CLASSIC,
	BOOT_FAILED_TO_JOIN2,
	BOOT_UNABLE_TO_ENTER,
	//NOTE: you can safely add 3 more of custom msg
};

enum LoadingError {
	lOAD_FAILED_SC_TO_HC = 19,
	LOAD_FAILED_HC_TO_SC = 20,
	LOAD_FAILED_DEAD_HC = 21,
	
	LOAD_FAILED_EXPANSION = 23,
	LOAD_FAILED_NOT_EXPANSION = 24,
	LOAD_FAILED_LADDER = 25,
	LOAD_FAILED_NOT_LADDER,


};

enum D2ClientToServerMessages {
	D2SRVMSG_WALK_TO_LOC = 1,
	D2SRVMSG_WALK_TO_UNIT,
	D2SRVMSG_RUN_TO_LOC,
	D2SRVMSG_RUN_TO_UNIT,
	D2SRVMSG_LEFT_CLICK_ON_LOC,
	D2SRVMSG_LEFT_CLICK_ON_UNIT,
	D2SRVMSG_SHIFT_LEFT_CLICK_ON_UNIT,
	D2SRVMSG_HOLD_LEFT_CLICK_ON_LOC,
	D2SRVMSG_HOLD_LEFT_CLICK_ON_UNIT,
	D2SRVMSG_SHIFT_LEFT_CLICK_ON_UNIT_2,
	D2SRVMSG_UNKNOWN_0x0B,
	D2SRVMSG_RIGHT_CLICK_ON_LOC,
	D2SRVMSG_RIGHT_CLICK_ON_UNIT,
	D2SRVMSG_SHIFT_RIGHT_CLICK_ON_UNIT,
	D2SRVMSG_HOLD_RIGHT_CLICK_ON_LOC,
	D2SRVMSG_HOLD_RIGHT_CLICK_ON_UNIT,
	D2SRVMSG_SHIFT_RIGHT_CLICK_ON_UNIT_2,
	D2SRVMSG_UNKNOWN_0x12,
	D2SRVMSG_INTERACT,
	D2SRVMSG_OVERHEAD,
	D2SRVMSG_CHAT,
	D2SRVMSG_PICKUP_GROUND_ITEM,
	D2SRVMSG_DROP_ITEM_TO_GROUND,
	D2SRVMSG_ITEM_TO_BUFFER,
	D2SRVMSG_REMOVE_ITEM_FROM_BUFFER,
	D2SRVMSG_EQUIP_ITEM,
	D2SRVMSG_SWAP_2HANDED_ITEM,
	D2SRVMSG_REMOVE_ITEM_FROM_BODY,
	D2SRVMSG_SWAP_CURSOR_WITH_BODY,
	D2SRVMSG_SWAP_2HANDED_WITH_2HANDED,
	D2SRVMSG_SWAP_CURSOR_BODY_ITEMS,
	D2SRVMSG_ACTIVATE_BUFFER_ITEM,
	D2SRVMSG_STACK_ITEM,
	D2SRVMSG_UNSTACK_ITEM,
	D2SRVMSG_ITEM_TO_BELT,
	D2SRVMSG_ITEM_FROM_BELT,
	D2SRVMSG_SWITCH_BELT_ITEM,
	D2SRVMSG_USE_BELT_ITEM,
	D2SRVMSG_IDENTIFY_ITEM,
	D2SRVMSG_SOCKET_ITEM,
	D2SRVMSG_SCROLL_TO_BOOK,
	D2SRVMSG_ITEM_TO_CUBE,
	D2SRVMSG_UNKNOWN_0x2B,
	D2SRVMSG_AS_HACK_0x2C,
	D2SRVMSG_AS_HACK_0x2D,
	D2SRVMSG_UNKNOWN_0x2E,
	D2SRVMSG_NPC_HEAL,
	D2SRVMSG_NPC_CANCEL,
	D2SRVMSG_QUEST_MSG,
	D2SRVMSG_NPC_BUY,
	D2SRVMSG_NPC_SELL,
	D2SRVMSG_NPC_IDENTIFY,
	D2SRVMSG_REPAIR,
	D2SRVMSG_HIRE_MERC,
	D2SRVMSG_NPC_GAMBLE_IDENTIFY,
	D2SRVMSG_NPC_ACTION,
	D2SRVMSG_PURCHASE_LIFE,
	D2SRVMSG_ADD_STAT_POINT,
	D2SRVMSG_ADD_SKILL_POINT,
	D2SRVMSG_SELECT_SKILL,
	D2SRVMSG_HIGHLIST_ADOOR,
	D2SRVMSG_ACTIVATE_SCROLL_OF_INF,
	D2SRVMSG_PLAY_SOUND,
	D2SRVMSG_REQUEST_QUEST_DATA,
	D2SRVMSG_RESSURECT,
	D2SRVMSG_UNKNOWN_0x42,
	D2SRVMSG_UNKNOWN_0x43,
	D2SRVMSG_STAFF_IN_ORIFICE,
	D2SRVMSG_CHANGE_TP_LOCATION,
	D2SRVMSG_MERC_INTERACT,
	D2SRVMSG_MERC_MOVE,
	D2SRVMSG_UNBUSY,
	D2SRVMSG_USE_WAYPOINT,
	D2SRVMSG_UNKNOWN_0x4A,
	D2SRVMSG_REQUEST_UNIT_UPDATE,
	D2SRVMSG_TRANSMORGRIFY,
	D2SRVMSG_PLAY_NPC_SOUND,
	D2SRVMSG_UNKNOWN_0x4E,
	D2SRVMSG_TRADE_ACTION,
	D2SRVMSG_DROP_GOLD,
	D2SRVMSG_BIND_HOTKEY,
	D2SRVMSG_UNKNOWN_0x52,
	D2SRVMSG_TURN_STAMINA_ON,
	D2SRVMSG_TURN_STAMINA_OFF,
	D2SRVMSG_UNKNOWN_0x55,
	D2SRVMSG_UNKNOWN_0x56,
	D2SRVMSG_GUILD_1,
	D2SRVMSG_QUEST_END,
	D2SRVMSG_MOVE_UNIT,
	D2SRVMSG_GUILD_2,
	D2SRVMSG_GUILD_3,
	D2SRVMSG_GUILD_4,
	D2SRVMSG_PARTY_OPTIONS,
	D2SRVMSG_PARTY_OPTIONS_2,
	D2SRVMSG_UPDATE_PLAYER_POS,
	D2SRVMSG_SWAP_WEAPONS,
	D2SRVMSG_MERC_GIVE_ITEM,
	D2SRVMSG_MERC_RESSURECT,
	D2SRVMSG_ITEM_TO_BELT_QUICK,
	D2SRVMSG_HACK_DETECTION,
	D2SRVMSG_HACK_DETECTION_2,
	D2SRVMSG_WARDEN_RESPONSE,
	D2SRVMSG_CREATE_GAME,
	D2SRVMSG_JOIN_GAME,
	D2SRVMSG_UNKNOWN_0x69,
	D2SRVMSG_SEND_0XB2,
	D2SRVMSG_UNKNOWN_0x6B,
	D2SRVMSG_SEND_SAVE_DATA,
	D2SRVMSG_UNKNOWN_0x6D,
	D2SRVMSG_DROP_PLAYER,
	D2SRVMSG_UNKNOWN_0x6F,
	D2SRVMSG_SET_UNK_BOOL,
};

enum D2Levels
{
	ROGUE_ENCAMPMENT = 0x1,
	BLOOD_MOOR = 0x2,
	COLD_PLAINS = 0x3,
	STONY_FIELD = 0x4,
	DARK_WOOD = 0x5,
	BLACK_MARSH = 0x6,
	TAMOE_HIGHLAND = 0x7,
	DEN_OF_EVIL = 0x8,
	CAVE_LEVEL_1 = 0x9,
	UNDERGROUND_PASSAGE_LEVEL_1 = 0xA,
	HOLE_LEVEL_1 = 0xB,
	PIT_LEVEL_1 = 0xC,
	CAVE_LEVEL_2 = 0xD,
	UNDERGROUND_PASSAGE_LEVEL_2 = 0xE,
	HOLE_LEVEL_2 = 0xF,
	PIT_LEVEL_2 = 0x10,
	BURIAL_GROUNDS = 0x11,
	CRYPT = 0x12,
	MAUSOLEUM = 0x13,
	FORGOTTEN_TOWER = 0x14,
	TOWER_CELLAR_LEVEL_1 = 0x15,
	TOWER_CELLAR_LEVEL_2 = 0x16,
	TOWER_CELLAR_LEVEL_3 = 0x17,
	TOWER_CELLAR_LEVEL_4 = 0x18,
	TOWER_CELLAR_LEVEL_5 = 0x19,
	MONASTERY_GATE = 0x1A,
	OUTER_CLOISTER = 0x1B,
	BARRACKS = 0x1C,
	JAIL_LEVEL_1 = 0x1D,
	JAIL_LEVEL_2 = 0x1E,
	JAIL_LEVEL_3 = 0x1F,
	INNER_CLOISTER = 0x20,
	CATHEDRAL = 0x21,
	CATACOMBS_LEVEL_1 = 0x22,
	CATACOMBS_LEVEL_2 = 0x23,
	CATACOMBS_LEVEL_3 = 0x24,
	CATACOMBS_LEVEL_4 = 0x25,
	TRISTRAM = 0x26,
	MOO_MOO_FARM = 0x27,
	LUT_GHOLEIN = 0x28,
	ROCKY_WASTE = 0x29,
	DRY_HILLS = 0x2A,
	FAR_OASIS = 0x2B,
	LOST_CITY = 0x2C,
	VALLEY_OF_SNAKES = 0x2D,
	CANYON_OF_THE_MAGI = 0x2E,
	SEWERS_LEVEL_1 = 0x2F,
	SEWERS_LEVEL_2 = 0x30,
	SEWERS_LEVEL_3 = 0x31,
	HAREM_LEVEL_1 = 0x32,
	HAREM_LEVEL_2 = 0x33,
	PALACE_CELLAR_LEVEL_1 = 0x34,
	PALACE_CELLAR_LEVEL_2 = 0x35,
	PALACE_CELLAR_LEVEL_3 = 0x36,
	STONY_TOMB_LEVEL_1 = 0x37,
	HALLS_OF_THE_DEAD_LEVEL_1 = 0x38,
	HALLS_OF_THE_DEAD_LEVEL_2 = 0x39,
	CLAW_VIPER_TEMPLE_LEVEL_1 = 0x3A,
	STONY_TOMB_LEVEL_2 = 0x3B,
	HALLS_OF_THE_DEAD_LEVEL_3 = 0x3C,
	CLAW_VIPER_TEMPLE_LEVEL_2 = 0x3D,
	MAGGOT_LAIR_LEVEL_1 = 0x3E,
	MAGGOT_LAIR_LEVEL_2 = 0x3F,
	MAGGOT_LAIR_LEVEL_3 = 0x40,
	ANCIENT_TUNNELS = 0x41,
	TAL_RASHA_TOMB_1 = 0x42,
	TAL_RASHA_TOMB_2 = 0x43,
	TAL_RASHA_TOMB_3 = 0x44,
	TAL_RASHA_TOMB_4 = 0x45,
	TAL_RASHA_TOMB_5 = 0x46,
	TAL_RASHA_TOMB_6 = 0x47,
	TAL_RASHA_TOMB_7 = 0x48,
	DURIEL_LAIR = 0x49,
	ARCANE_SANCTUARY = 0x4A,
	KURAST_DOCKTOWN = 0x4B,
	SPIDER_FOREST = 0x4C,
	GREAT_MARSH = 0x4D,
	FLAYER_JUNGLE = 0x4E,
	LOWER_KURAST = 0x4F,
	KURAST_BAZAAR = 0x50,
	UPPER_KURAST = 0x51,
	KURAST_CAUSEWAY = 0x52,
	TRAVINCAL = 0x53,
	SPIDER_CAVE = 0x54,
	SPIDER_CAVERN = 0x55,
	SWAMPY_PIT_LEVEL_1 = 0x56,
	SWAMPY_PIT_LEVEL_2 = 0x57,
	FLAYER_DUNGEON_LEVEL_1 = 0x58,
	FLAYER_DUNGEON_LEVEL_2 = 0x59,
	SWAMPY_PIT_LEVEL_3 = 0x5A,
	FLAYER_DUNGEON_LEVEL_3 = 0x5B,
	A3_SEWERS_LEVEL_1 = 0x5C,
	A3_SEWERS_LEVEL_2 = 0x5D,
	RUINED_TEMPLE = 0x5E,
	DISUSED_FANE = 0x5F,
	FORGOTTEN_RELIQUARY = 0x60,
	FORGOTTEN_TEMPLE = 0x61,
	RUINED_FANE = 0x62,
	DISUSED_RELIQUARY = 0x63,
	DURANCE_OF_HATE_LEVEL_1 = 0x64,
	DURANCE_OF_HATE_LEVEL_2 = 0x65,
	DURANCE_OF_HATE_LEVEL_3 = 0x66,
	THE_PANDEMONIUM_FORTRESS = 0x67,
	OUTER_STEPPES = 0x68,
	PLAINS_OF_DESPAIR = 0x69,
	CITY_OF_THE_DAMNED = 0x6A,
	RIVER_OF_FLAME = 0x6B,
	CHAOS_SANCTUM = 0x6C,
	HARROGATH = 0x6D,
	BLOODY_FOOTHILLS = 0x6E,
	RIGID_HIGHLANDS = 0x6F,
	ARREAT_PLATEAU = 0x70,
	CRYSTALIZED_CAVERN_LEVEL_1 = 0x71,
	CELLAR_OF_PITY = 0x72,
	CRYSTALIZED_CAVERN_LEVEL_2 = 0x73,
	ECHO_CHAMBER = 0x74,
	TUNDRA_WASTELANDS = 0x75,
	GLACIAL_CAVES_LEVEL_1 = 0x76,
	GLACIAL_CAVES_LEVEL_2 = 0x77,
	ROCKY_SUMMIT = 0x78,
	NIHLATHAKS_TEMPLE = 0x79,
	HALLS_OF_ANGUISH = 0x7A,
	HALLS_OF_DEATH_CALLING = 0x7B,
	HALLS_OF_VAUGHT = 0x7C,
	HELL_1 = 0x7D,
	HELL_2 = 0x7E,
	HELL_3 = 0x7F,
	THE_WORLDSTONE_KEEP_LEVEL_1 = 0x80,
	THE_WORLDSTONE_KEEP_LEVEL_2 = 0x81,
	THE_WORLDSTONE_KEEP_LEVEL_3 = 0x82,
	THRONE_OF_DESTRUCTION = 0x83,
	THE_WORLDSTONE_CHAMBER = 0x84,
	PANDEMONIUM_RUN_1 = 0x85,
	PANDEMONIUM_RUN_2 = 0x86,
	PANDEMONIUM_RUN_3 = 0x87,
	UBER_TRISTRAM = 0x88,
};

enum D2Monsters
{
	SKELETON1 = 0x0,
	SKELETON2 = 0x1,
	SKELETON3 = 0x2,
	SKELETON4 = 0x3,
	SKELETON5 = 0x4,
	ZOMBIE1 = 0x5,
	ZOMBIE2 = 0x6,
	ZOMBIE3 = 0x7,
	ZOMBIE4 = 0x8,
	ZOMBIE5 = 0x9,
	BIGHEAD1 = 0xA,
	BIGHEAD2 = 0xB,
	BIGHEAD3 = 0xC,
	BIGHEAD4 = 0xD,
	BIGHEAD5 = 0xE,
	FOULCROW1 = 0xF,
	FOULCROW2 = 0x10,
	FOULCROW3 = 0x11,
	FOULCROW4 = 0x12,
	FALLEN1 = 0x13,
	FALLEN2 = 0x14,
	FALLEN3 = 0x15,
	FALLEN4 = 0x16,
	FALLEN5 = 0x17,
	BRUTE2 = 0x18,
	BRUTE3 = 0x19,
	BRUTE4 = 0x1A,
	BRUTE5 = 0x1B,
	BRUTE1 = 0x1C,
	SANDRAIDER1 = 0x1D,
	SANDRAIDER2 = 0x1E,
	SANDRAIDER3 = 0x1F,
	SANDRAIDER4 = 0x20,
	SANDRAIDER5 = 0x21,
	GORGON1 = 0x22,
	GORGON2 = 0x23,
	GORGON3 = 0x24,
	GORGON4 = 0x25,
	WRAITH1 = 0x26,
	WRAITH2 = 0x27,
	WRAITH3 = 0x28,
	WRAITH4 = 0x29,
	WRAITH5 = 0x2A,
	CORRUPTROGUE1 = 0x2B,
	CORRUPTROGUE2 = 0x2C,
	CORRUPTROGUE3 = 0x2D,
	CORRUPTROGUE4 = 0x2E,
	CORRUPTROGUE5 = 0x2F,
	BABOON1 = 0x30,
	BABOON2 = 0x31,
	BABOON3 = 0x32,
	BABOON4 = 0x33,
	BABOON5 = 0x34,
	GOATMAN1 = 0x35,
	GOATMAN2 = 0x36,
	GOATMAN3 = 0x37,
	GOATMAN4 = 0x38,
	GOATMAN5 = 0x39,
	FALLENSHAMAN1 = 0x3A,
	FALLENSHAMAN2 = 0x3B,
	FALLENSHAMAN3 = 0x3C,
	FALLENSHAMAN4 = 0x3D,
	FALLENSHAMAN5 = 0x3E,
	QUILLRAT1 = 0x3F,
	QUILLRAT2 = 0x40,
	QUILLRAT3 = 0x41,
	QUILLRAT4 = 0x42,
	QUILLRAT5 = 0x43,
	SANDMAGGOT1 = 0x44,
	SANDMAGGOT2 = 0x45,
	SANDMAGGOT3 = 0x46,
	SANDMAGGOT4 = 0x47,
	SANDMAGGOT5 = 0x48,
	CLAWVIPER1 = 0x49,
	CLAWVIPER2 = 0x4A,
	CLAWVIPER3 = 0x4B,
	CLAWVIPER4 = 0x4C,
	CLAWVIPER5 = 0x4D,
	SANDLEAPER1 = 0x4E,
	SANDLEAPER2 = 0x4F,
	SANDLEAPER3 = 0x50,
	SANDLEAPER4 = 0x51,
	SANDLEAPER5 = 0x52,
	PANTHERWOMAN1 = 0x53,
	PANTHERWOMAN2 = 0x54,
	PANTHERWOMAN3 = 0x55,
	PANTHERWOMAN4 = 0x56,
	SWARM1 = 0x57,
	SWARM2 = 0x58,
	SWARM3 = 0x59,
	SWARM4 = 0x5A,
	SCARAB1 = 0x5B,
	SCARAB2 = 0x5C,
	SCARAB3 = 0x5D,
	SCARAB4 = 0x5E,
	SCARAB5 = 0x5F,
	MUMMY1 = 0x60,
	MUMMY2 = 0x61,
	MUMMY3 = 0x62,
	MUMMY4 = 0x63,
	MUMMY5 = 0x64,
	UNRAVELER1 = 0x65,
	UNRAVELER2 = 0x66,
	UNRAVELER3 = 0x67,
	UNRAVELER4 = 0x68,
	UNRAVELER5 = 0x69,
	CHAOSHORDE1 = 0x6A,
	CHAOSHORDE2 = 0x6B,
	CHAOSHORDE3 = 0x6C,
	CHAOSHORDE4 = 0x6D,
	VULTURE1 = 0x6E,
	VULTURE2 = 0x6F,
	VULTURE3 = 0x70,
	VULTURE4 = 0x71,
	MOSQUITO1 = 0x72,
	MOSQUITO2 = 0x73,
	MOSQUITO3 = 0x74,
	MOSQUITO4 = 0x75,
	WILLOWISP1 = 0x76,
	WILLOWISP2 = 0x77,
	WILLOWISP3 = 0x78,
	WILLOWISP4 = 0x79,
	ARACH1 = 0x7A,
	ARACH2 = 0x7B,
	ARACH3 = 0x7C,
	ARACH4 = 0x7D,
	ARACH5 = 0x7E,
	THORNHULK1 = 0x7F,
	THORNHULK2 = 0x80,
	THORNHULK3 = 0x81,
	THORNHULK4 = 0x82,
	VAMPIRE1 = 0x83,
	VAMPIRE2 = 0x84,
	VAMPIRE3 = 0x85,
	VAMPIRE4 = 0x86,
	VAMPIRE5 = 0x87,
	BATDEMON1 = 0x88,
	BATDEMON2 = 0x89,
	BATDEMON3 = 0x8A,
	BATDEMON4 = 0x8B,
	BATDEMON5 = 0x8C,
	FETISH1 = 0x8D,
	FETISH2 = 0x8E,
	FETISH3 = 0x8F,
	FETISH4 = 0x90,
	FETISH5 = 0x91,
	CAIN1 = 0x92,
	GHEED = 0x93,
	AKARA = 0x94,
	CHICKEN = 0x95,
	KASHYA = 0x96,
	RAT = 0x97,
	ROGUE1 = 0x98,
	HELLMETEOR = 0x99,
	CHARSI = 0x9A,
	WARRIV1 = 0x9B,
	ANDARIEL = 0x9C,
	BIRD1 = 0x9D,
	BIRD2 = 0x9E,
	BAT = 0x9F,
	CR_ARCHER1 = 0xA0,
	CR_ARCHER2 = 0xA1,
	CR_ARCHER3 = 0xA2,
	CR_ARCHER4 = 0xA3,
	CR_ARCHER5 = 0xA4,
	CR_LANCER1 = 0xA5,
	CR_LANCER2 = 0xA6,
	CR_LANCER3 = 0xA7,
	CR_LANCER4 = 0xA8,
	CR_LANCER5 = 0xA9,
	SK_ARCHER1 = 0xAA,
	SK_ARCHER2 = 0xAB,
	SK_ARCHER3 = 0xAC,
	SK_ARCHER4 = 0xAD,
	SK_ARCHER5 = 0xAE,
	WARRIV2 = 0xAF,
	ATMA = 0xB0,
	DROGNAN = 0xB1,
	FARA = 0xB2,
	COW = 0xB3,
	MAGGOTBABY1 = 0xB4,
	MAGGOTBABY2 = 0xB5,
	MAGGOTBABY3 = 0xB6,
	MAGGOTBABY4 = 0xB7,
	MAGGOTBABY5 = 0xB8,
	CAMEL = 0xB9,
	BLUNDERBORE1 = 0xBA,
	BLUNDERBORE2 = 0xBB,
	BLUNDERBORE3 = 0xBC,
	BLUNDERBORE4 = 0xBD,
	MAGGOTEGG1 = 0xBE,
	MAGGOTEGG2 = 0xBF,
	MAGGOTEGG3 = 0xC0,
	MAGGOTEGG4 = 0xC1,
	MAGGOTEGG5 = 0xC2,
	ACT2MALE = 0xC3,
	ACT2FEMALE = 0xC4,
	ACT2CHILD = 0xC5,
	GREIZ = 0xC6,
	ELZIX = 0xC7,
	GEGLASH = 0xC8,
	JERHYN = 0xC9,
	LYSANDER = 0xCA,
	ACT2GUARD1 = 0xCB,
	ACT2VENDOR1 = 0xCC,
	ACT2VENDOR2 = 0xCD,
	CROWNEST1 = 0xCE,
	CROWNEST2 = 0xCF,
	CROWNEST3 = 0xD0,
	CROWNEST4 = 0xD1,
	MESHIF1 = 0xD2,
	DURIEL = 0xD3,
	BONEFETISH1 = 0xD4,
	BONEFETISH2 = 0xD5,
	BONEFETISH3 = 0xD6,
	BONEFETISH4 = 0xD7,
	BONEFETISH5 = 0xD8,
	DARKGUARD1 = 0xD9,
	DARKGUARD2 = 0xDA,
	DARKGUARD3 = 0xDB,
	DARKGUARD4 = 0xDC,
	DARKGUARD5 = 0xDD,
	BLOODMAGE1 = 0xDE,
	BLOODMAGE2 = 0xDF,
	BLOODMAGE3 = 0xE0,
	BLOODMAGE4 = 0xE1,
	BLOODMAGE5 = 0xE2,
	MAGGOT = 0xE3,
	SARCOPHAGUS = 0xE4,
	RADAMENT = 0xE5,
	FIREBEAST = 0xE6,
	ICEGLOBE = 0xE7,
	LIGHTNINGBEAST = 0xE8,
	POISONORB = 0xE9,
	FLYINGSCIMITAR = 0xEA,
	ZEALOT1 = 0xEB,
	ZEALOT2 = 0xEC,
	ZEALOT3 = 0xED,
	CANTOR1 = 0xEE,
	CANTOR2 = 0xEF,
	CANTOR3 = 0xF0,
	CANTOR4 = 0xF1,
	MEPHISTO = 0xF2,
	DIABLO = 0xF3,
	CAIN2 = 0xF4,
	CAIN3 = 0xF5,
	CAIN4 = 0xF6,
	FROGDEMON1 = 0xF7,
	FROGDEMON2 = 0xF8,
	FROGDEMON3 = 0xF9,
	SUMMONER = 0xFA,
	TYRAEL1 = 0xFB,
	ASHEARA = 0xFC,
	HRATLI = 0xFD,
	ALKOR = 0xFE,
	ORMUS = 0xFF,
	IZUAL = 0x100,
	HALBU = 0x101,
	TENTACLE1 = 0x102,
	TENTACLE2 = 0x103,
	TENTACLE3 = 0x104,
	TENTACLEHEAD1 = 0x105,
	TENTACLEHEAD2 = 0x106,
	TENTACLEHEAD3 = 0x107,
	MESHIF2 = 0x108,
	CAIN5 = 0x109,
	NAVI = 0x10A,
	BLOODRAVEN = 0x10B,
	BUG = 0x10C,
	SCORPION = 0x10D,
	ROGUE2 = 0x10E,
	ROGUEHIRE = 0x10F,
	ROGUE3 = 0x110,
	GARGOYLETRAP = 0x111,
	SKMAGE_POIS1 = 0x112,
	SKMAGE_POIS2 = 0x113,
	SKMAGE_POIS3 = 0x114,
	SKMAGE_POIS4 = 0x115,
	FETISHSHAMAN1 = 0x116,
	FETISHSHAMAN2 = 0x117,
	FETISHSHAMAN3 = 0x118,
	FETISHSHAMAN4 = 0x119,
	FETISHSHAMAN5 = 0x11A,
	LARVA = 0x11B,
	MAGGOTQUEEN1 = 0x11C,
	MAGGOTQUEEN2 = 0x11D,
	MAGGOTQUEEN3 = 0x11E,
	MAGGOTQUEEN4 = 0x11F,
	MAGGOTQUEEN5 = 0x120,
	CLAYGOLEM = 0x121,
	BLOODGOLEM = 0x122,
	IRONGOLEM = 0x123,
	FIREGOLEM = 0x124,
	FAMILIAR = 0x125,
	ACT3MALE = 0x126,
	BABOON6 = 0x127,
	ACT3FEMALE = 0x128,
	NATALYA = 0x129,
	VILEMOTHER1 = 0x12A,
	VILEMOTHER2 = 0x12B,
	VILEMOTHER3 = 0x12C,
	VILECHILD1 = 0x12D,
	VILECHILD2 = 0x12E,
	VILECHILD3 = 0x12F,
	FINGERMAGE1 = 0x130,
	FINGERMAGE2 = 0x131,
	FINGERMAGE3 = 0x132,
	REGURGITATOR1 = 0x133,
	REGURGITATOR2 = 0x134,
	REGURGITATOR3 = 0x135,
	DOOMKNIGHT1 = 0x136,
	DOOMKNIGHT2 = 0x137,
	DOOMKNIGHT3 = 0x138,
	QUILLBEAR1 = 0x139,
	QUILLBEAR2 = 0x13A,
	QUILLBEAR3 = 0x13B,
	QUILLBEAR4 = 0x13C,
	QUILLBEAR5 = 0x13D,
	SNAKE = 0x13E,
	PARROT = 0x13F,
	FISH = 0x140,
	EVILHOLE1 = 0x141,
	EVILHOLE2 = 0x142,
	EVILHOLE3 = 0x143,
	EVILHOLE4 = 0x144,
	EVILHOLE5 = 0x145,
	TRAP_FIREBOLT = 0x146,
	TRAP_HORZMISSILE = 0x147,
	TRAP_VERTMISSILE = 0x148,
	TRAP_POISONCLOUD = 0x149,
	TRAP_LIGHTNING = 0x14A,
	ACT2GUARD2 = 0x14B,
	INVISOSPAWNER = 0x14C,
	DIABLOCLONE = 0x14D,
	SUCKERNEST1 = 0x14E,
	SUCKERNEST2 = 0x14F,
	SUCKERNEST3 = 0x150,
	SUCKERNEST4 = 0x151,
	ACT2HIRE = 0x152,
	MINISPIDER = 0x153,
	BONEPRISON1 = 0x154,
	BONEPRISON2 = 0x155,
	BONEPRISON3 = 0x156,
	BONEPRISON4 = 0x157,
	BONEWALL = 0x158,
	COUNCILMEMBER1 = 0x159,
	COUNCILMEMBER2 = 0x15A,
	COUNCILMEMBER3 = 0x15B,
	TURRET1 = 0x15C,
	TURRET2 = 0x15D,
	TURRET3 = 0x15E,
	HYDRA1 = 0x15F,
	HYDRA2 = 0x160,
	HYDRA3 = 0x161,
	TRAP_MELEE = 0x162,
	SEVENTOMBS = 0x163,
	DOPPLEZON = 0x164,
	VALKYRIE = 0x165,
	ACT2GUARD3 = 0x166,
	ACT3HIRE = 0x167,
	MEGADEMON1 = 0x168,
	MEGADEMON2 = 0x169,
	MEGADEMON3 = 0x16A,
	NECROSKELETON = 0x16B,
	NECROMAGE = 0x16C,
	GRISWOLD = 0x16D,
	COMPELLINGORB = 0x16E,
	TYRAEL2 = 0x16F,
	DARKWANDERER = 0x170,
	TRAP_NOVA = 0x171,
	SPIRITMUMMY = 0x172,
	LIGHTNINGSPIRE = 0x173,
	FIRETOWER = 0x174,
	SLINGER1 = 0x175,
	SLINGER2 = 0x176,
	SLINGER3 = 0x177,
	SLINGER4 = 0x178,
	ACT2GUARD4 = 0x179,
	ACT2GUARD5 = 0x17A,
	SKMAGE_COLD1 = 0x17B,
	SKMAGE_COLD2 = 0x17C,
	SKMAGE_COLD3 = 0x17D,
	SKMAGE_COLD4 = 0x17E,
	SKMAGE_FIRE1 = 0x17F,
	SKMAGE_FIRE2 = 0x180,
	SKMAGE_FIRE3 = 0x181,
	SKMAGE_FIRE4 = 0x182,
	SKMAGE_LTNG1 = 0x183,
	SKMAGE_LTNG2 = 0x184,
	SKMAGE_LTNG3 = 0x185,
	SKMAGE_LTNG4 = 0x186,
	HELLBOVINE = 0x187,
	WINDOW1 = 0x188,
	WINDOW2 = 0x189,
	SLINGER5 = 0x18A,
	SLINGER6 = 0x18B,
	FETISHBLOW1 = 0x18C,
	FETISHBLOW2 = 0x18D,
	FETISHBLOW3 = 0x18E,
	FETISHBLOW4 = 0x18F,
	FETISHBLOW5 = 0x190,
	MEPHISTOSPIRIT = 0x191,
	SMITH = 0x192,
	TRAPPEDSOUL1 = 0x193,
	TRAPPEDSOUL2 = 0x194,
	JAMELLA = 0x195,
	IZUALGHOST = 0x196,
	FETISH11 = 0x197,
	MALACHAI = 0x198,
	HEPHASTO = 0x199,
	WAKEOFDESTRUCTION = 0x19A,
	CHARGEBOLTSENTRY = 0x19B,
	LIGHTNINGSENTRY = 0x19C,
	BLADECREEPER = 0x19D,
	INVISOPET = 0x19E,
	INFERNOSENTRY = 0x19F,
	DEATHSENTRY = 0x1A0,
	SHADOWWARRIOR = 0x1A1,
	SHADOWMASTER = 0x1A2,
	DRUIDHAWK = 0x1A3,
	SPIRITWOLF = 0x1A4,
	FENRIS = 0x1A5,
	SPIRITOFBARBS = 0x1A6,
	HEARTOFWOLVERINE = 0x1A7,
	OAKSAGE = 0x1A8,
	PLAGUEPOPPY = 0x1A9,
	CYCLEOFLIFE = 0x1AA,
	VINECREATURE = 0x1AB,
	DRUIDBEAR = 0x1AC,
	EAGLE = 0x1AD,
	WOLF = 0x1AE,
	BEAR = 0x1AF,
	BARRICADEDOOR1 = 0x1B0,
	BARRICADEDOOR2 = 0x1B1,
	PRISONDOOR = 0x1B2,
	BARRICADETOWER = 0x1B3,
	REANIMATEDHORDE1 = 0x1B4,
	REANIMATEDHORDE2 = 0x1B5,
	REANIMATEDHORDE3 = 0x1B6,
	REANIMATEDHORDE4 = 0x1B7,
	REANIMATEDHORDE5 = 0x1B8,
	SIEGEBEAST1 = 0x1B9,
	SIEGEBEAST2 = 0x1BA,
	SIEGEBEAST3 = 0x1BB,
	SIEGEBEAST4 = 0x1BC,
	SIEGEBEAST5 = 0x1BD,
	SNOWYETI1 = 0x1BE,
	SNOWYETI2 = 0x1BF,
	SNOWYETI3 = 0x1C0,
	SNOWYETI4 = 0x1C1,
	WOLFRIDER1 = 0x1C2,
	WOLFRIDER2 = 0x1C3,
	WOLFRIDER3 = 0x1C4,
	MINION1 = 0x1C5,
	MINION2 = 0x1C6,
	MINION3 = 0x1C7,
	MINION4 = 0x1C8,
	MINION5 = 0x1C9,
	MINION6 = 0x1CA,
	MINION7 = 0x1CB,
	MINION8 = 0x1CC,
	SUICIDEMINION1 = 0x1CD,
	SUICIDEMINION2 = 0x1CE,
	SUICIDEMINION3 = 0x1CF,
	SUICIDEMINION4 = 0x1D0,
	SUICIDEMINION5 = 0x1D1,
	SUICIDEMINION6 = 0x1D2,
	SUICIDEMINION7 = 0x1D3,
	SUICIDEMINION8 = 0x1D4,
	SUCCUBUS1 = 0x1D5,
	SUCCUBUS2 = 0x1D6,
	SUCCUBUS3 = 0x1D7,
	SUCCUBUS4 = 0x1D8,
	SUCCUBUS5 = 0x1D9,
	SUCCUBUSWITCH1 = 0x1DA,
	SUCCUBUSWITCH2 = 0x1DB,
	SUCCUBUSWITCH3 = 0x1DC,
	SUCCUBUSWITCH4 = 0x1DD,
	SUCCUBUSWITCH5 = 0x1DE,
	OVERSEER1 = 0x1DF,
	OVERSEER2 = 0x1E0,
	OVERSEER3 = 0x1E1,
	OVERSEER4 = 0x1E2,
	OVERSEER5 = 0x1E3,
	MINIONSPAWNER1 = 0x1E4,
	MINIONSPAWNER2 = 0x1E5,
	MINIONSPAWNER3 = 0x1E6,
	MINIONSPAWNER4 = 0x1E7,
	MINIONSPAWNER5 = 0x1E8,
	MINIONSPAWNER6 = 0x1E9,
	MINIONSPAWNER7 = 0x1EA,
	MINIONSPAWNER8 = 0x1EB,
	IMP1 = 0x1EC,
	IMP2 = 0x1ED,
	IMP3 = 0x1EE,
	IMP4 = 0x1EF,
	IMP5 = 0x1F0,
	CATAPULT1 = 0x1F1,
	CATAPULT2 = 0x1F2,
	CATAPULT3 = 0x1F3,
	CATAPULT4 = 0x1F4,
	FROZENHORROR1 = 0x1F5,
	FROZENHORROR2 = 0x1F6,
	FROZENHORROR3 = 0x1F7,
	FROZENHORROR4 = 0x1F8,
	FROZENHORROR5 = 0x1F9,
	BLOODLORD1 = 0x1FA,
	BLOODLORD2 = 0x1FB,
	BLOODLORD3 = 0x1FC,
	BLOODLORD4 = 0x1FD,
	BLOODLORD5 = 0x1FE,
	LARZUK = 0x1FF,
	DREHYA = 0x200,
	MALAH = 0x201,
	NIHLATHAK = 0x202,
	QUAL_KEHK = 0x203,
	CATAPULTSPOTTER1 = 0x204,
	CATAPULTSPOTTER2 = 0x205,
	CATAPULTSPOTTER3 = 0x206,
	CATAPULTSPOTTER4 = 0x207,
	CAIN6 = 0x208,
	TYRAEL3 = 0x209,
	ACT5BARB1 = 0x20A,
	ACT5BARB2 = 0x20B,
	BARRICADEWALL1 = 0x20C,
	BARRICADEWALL2 = 0x20D,
	NIHLATHAKBOSS = 0x20E,
	DREHYAICED = 0x20F,
	EVILHUT = 0x210,
	DEATHMAULER1 = 0x211,
	DEATHMAULER2 = 0x212,
	DEATHMAULER3 = 0x213,
	DEATHMAULER4 = 0x214,
	DEATHMAULER5 = 0x215,
	ACT5POW = 0x216,
	ACT5BARB3 = 0x217,
	ACT5BARB4 = 0x218,
	ANCIENTSTATUE1 = 0x219,
	ANCIENTSTATUE2 = 0x21A,
	ANCIENTSTATUE3 = 0x21B,
	ANCIENTBARB1 = 0x21C,
	ANCIENTBARB2 = 0x21D,
	ANCIENTBARB3 = 0x21E,
	BAALTHRONE = 0x21F,
	BAALCRAB = 0x220,
	BAALTAUNT = 0x221,
	PUTRIDDEFILER1 = 0x222,
	PUTRIDDEFILER2 = 0x223,
	PUTRIDDEFILER3 = 0x224,
	PUTRIDDEFILER4 = 0x225,
	PUTRIDDEFILER5 = 0x226,
	PAINWORM1 = 0x227,
	PAINWORM2 = 0x228,
	PAINWORM3 = 0x229,
	PAINWORM4 = 0x22A,
	PAINWORM5 = 0x22B,
	BUNNY = 0x22C,
	BAALHIGHPRIEST = 0x22D,
	VENOMLORD = 0x22E,
	BAALCRABSTAIRS = 0x22F,
	ACT5HIRE1 = 0x230,
	ACT5HIRE2 = 0x231,
	BAALTENTACLE1 = 0x232,
	BAALTENTACLE2 = 0x233,
	BAALTENTACLE3 = 0x234,
	BAALTENTACLE4 = 0x235,
	BAALTENTACLE5 = 0x236,
	INJUREDBARB1 = 0x237,
	INJUREDBARB2 = 0x238,
	INJUREDBARB3 = 0x239,
	BAALCLONE = 0x23A,
	BAALMINION1 = 0x23B,
	BAALMINION2 = 0x23C,
	BAALMINION3 = 0x23D,
	WORLDSTONEEFFECT = 0x23E,
	SK_ARCHER6 = 0x23F,
	SK_ARCHER7 = 0x240,
	SK_ARCHER8 = 0x241,
	SK_ARCHER9 = 0x242,
	SK_ARCHER10 = 0x243,
	BIGHEAD6 = 0x244,
	BIGHEAD7 = 0x245,
	BIGHEAD8 = 0x246,
	BIGHEAD9 = 0x247,
	BIGHEAD10 = 0x248,
	GOATMAN6 = 0x249,
	GOATMAN7 = 0x24A,
	GOATMAN8 = 0x24B,
	GOATMAN9 = 0x24C,
	GOATMAN10 = 0x24D,
	FOULCROW5 = 0x24E,
	FOULCROW6 = 0x24F,
	FOULCROW7 = 0x250,
	FOULCROW8 = 0x251,
	CLAWVIPER6 = 0x252,
	CLAWVIPER7 = 0x253,
	CLAWVIPER8 = 0x254,
	CLAWVIPER9 = 0x255,
	CLAWVIPER10 = 0x256,
	SANDRAIDER6 = 0x257,
	SANDRAIDER7 = 0x258,
	SANDRAIDER8 = 0x259,
	SANDRAIDER9 = 0x25A,
	SANDRAIDER10 = 0x25B,
	DEATHMAULER6 = 0x25C,
	QUILLRAT6 = 0x25D,
	QUILLRAT7 = 0x25E,
	QUILLRAT8 = 0x25F,
	VULTURE5 = 0x260,
	THORNHULK5 = 0x261,
	SLINGER7 = 0x262,
	SLINGER8 = 0x263,
	SLINGER9 = 0x264,
	CR_ARCHER6 = 0x265,
	CR_ARCHER7 = 0x266,
	CR_LANCER6 = 0x267,
	CR_LANCER7 = 0x268,
	CR_LANCER8 = 0x269,
	BLUNDERBORE5 = 0x26A,
	BLUNDERBORE6 = 0x26B,
	SKMAGE_FIRE5 = 0x26C,
	SKMAGE_FIRE6 = 0x26D,
	SKMAGE_LTNG5 = 0x26E,
	SKMAGE_LTNG6 = 0x26F,
	SKMAGE_COLD5 = 0x270,
	SKMAGE_POIS5 = 0x271,
	SKMAGE_POIS6 = 0x272,
	PANTHERWOMAN5 = 0x273,
	PANTHERWOMAN6 = 0x274,
	SANDLEAPER6 = 0x275,
	SANDLEAPER7 = 0x276,
	WRAITH6 = 0x277,
	WRAITH7 = 0x278,
	WRAITH8 = 0x279,
	SUCCUBUS6 = 0x27A,
	SUCCUBUS7 = 0x27B,
	SUCCUBUSWITCH6 = 0x27C,
	SUCCUBUSWITCH7 = 0x27D,
	SUCCUBUSWITCH8 = 0x27E,
	WILLOWISP5 = 0x27F,
	WILLOWISP6 = 0x280,
	WILLOWISP7 = 0x281,
	FALLEN6 = 0x282,
	FALLEN7 = 0x283,
	FALLEN8 = 0x284,
	FALLENSHAMAN6 = 0x285,
	FALLENSHAMAN7 = 0x286,
	FALLENSHAMAN8 = 0x287,
	SKELETON6 = 0x288,
	SKELETON7 = 0x289,
	BATDEMON6 = 0x28A,
	BATDEMON7 = 0x28B,
	BLOODLORD6 = 0x28C,
	BLOODLORD7 = 0x28D,
	SCARAB6 = 0x28E,
	SCARAB7 = 0x28F,
	FETISH6 = 0x290,
	FETISH7 = 0x291,
	FETISH8 = 0x292,
	FETISHBLOW6 = 0x293,
	FETISHBLOW7 = 0x294,
	FETISHBLOW8 = 0x295,
	FETISHSHAMAN6 = 0x296,
	FETISHSHAMAN7 = 0x297,
	FETISHSHAMAN8 = 0x298,
	BABOON7 = 0x299,
	BABOON8 = 0x29A,
	UNRAVELER6 = 0x29B,
	UNRAVELER7 = 0x29C,
	UNRAVELER8 = 0x29D,
	UNRAVELER9 = 0x29E,
	ZEALOT4 = 0x29F,
	ZEALOT5 = 0x2A0,
	CANTOR5 = 0x2A1,
	CANTOR6 = 0x2A2,
	VILEMOTHER4 = 0x2A3,
	VILEMOTHER5 = 0x2A4,
	VILECHILD4 = 0x2A5,
	VILECHILD5 = 0x2A6,
	SANDMAGGOT6 = 0x2A7,
	MAGGOTBABY6 = 0x2A8,
	MAGGOTEGG6 = 0x2A9,
	MINION9 = 0x2AA,
	MINION10 = 0x2AB,
	MINION11 = 0x2AC,
	ARACH6 = 0x2AD,
	MEGADEMON4 = 0x2AE,
	MEGADEMON5 = 0x2AF,
	IMP6 = 0x2B0,
	IMP7 = 0x2B1,
	BONEFETISH6 = 0x2B2,
	BONEFETISH7 = 0x2B3,
	FINGERMAGE4 = 0x2B4,
	FINGERMAGE5 = 0x2B5,
	REGURGITATOR4 = 0x2B6,
	VAMPIRE6 = 0x2B7,
	VAMPIRE7 = 0x2B8,
	VAMPIRE8 = 0x2B9,
	REANIMATEDHORDE6 = 0x2BA,
	DKFIG1 = 0x2BB,
	DKFIG2 = 0x2BC,
	DKMAG1 = 0x2BD,
	DKMAG2 = 0x2BE,
	MUMMY6 = 0x2BF,
	UBERMEPHISTO = 0x2C0,
	UBERDIABLO = 0x2C1,
	UBERIZUAL = 0x2C2,
	UBERANDARIEL = 0x2C3,
	UBERDURIEL = 0x2C4,
	UBERBAAL = 0x2C5,
	DEMONSPAWNER = 0x2C6,
	DEMONHOLE = 0x2C7,
	MEGADEMON6 = 0x2C8,
	DKMAG3 = 0x2C9,
	IMP8 = 0x2CA,
	SWARM5 = 0x2CB,
	SANDMAGGOT7 = 0x2CC,
	ARACH7 = 0x2CD,
	SCARAB8 = 0x2CE,
	SUCCUBUS8 = 0x2CF,
	SUCCUBUSWITCH9 = 0x2D0,
	CORRUPTROGUE6 = 0x2D1,
	CR_ARCHER8 = 0x2D2,
	CR_LANCER8_2 = 0x2D3,
	OVERSEER6 = 0x2D4,
	SKELETON8 = 0x2D5,
	SK_ARCHER11 = 0x2D6,
	SKMAGE_FIRE7 = 0x2D7,
	SKMAGE_LTNG7 = 0x2D8,
	SKMAGE_COLD6 = 0x2D9,
	SKMAGE_POIS7 = 0x2DA,
	VAMPIRE9 = 0x2DB,
	WRAITH9 = 0x2DC,
	WILLOWISP8 = 0x2DD,
};

enum D2Objects
{
	OBJ_TEST_DATA1 = 0,
	OBJ_CASKET_5 = 1,
	OBJ_SHRINE = 2,
	OBJ_CASKET_6 = 3,
	OBJ_URN_1 = 4,
	OBJ_LARGECHESTR = 5,
	OBJ_LARGECHESTL = 6,
	OBJ_BARREL = 7,
	OBJ_TOWER_TOME = 8,
	OBJ_URN_2 = 9,
	OBJ_BENCH = 10,
	OBJ_BARRELEXPLODING = 11,
	OBJ_ROGUEFOUNTAIN = 12,
	OBJ_DOOR_GATE_LEFT = 13,
	OBJ_DOOR_GATE_RIGHT = 14,
	OBJ_DOOR_WOODEN_LEFT = 15,
	OBJ_DOOR_WOODEN_RIGHT = 16,
	OBJ_STONEALPHA = 17,
	OBJ_STONEBETA = 18,
	OBJ_STONEGAMMA = 19,
	OBJ_STONEDELTA = 20,
	OBJ_STONELAMBDA = 21,
	OBJ_STONETHETA = 22,
	OBJ_DOOR_COURTYARD_LEFT = 23,
	OBJ_DOOR_COURTYARD_RIGHT = 24,
	OBJ_DOOR_CATHEDRAL_DOUBLE = 25,
	OBJ_CAIN_BEEN_CAPTURED = 26,
	OBJ_DOOR_MONASTERY_DOUBLE_RIGHT = 27,
	OBJ_HOLE_IN_GROUND = 28,
	OBJ_BRAZIER = 29,
	OBJ_INIFUSS_TREE = 30,
	OBJ_FOUNTAIN = 31,
	OBJ_CRUCIFIX = 32,
	OBJ_CANDLES1 = 33,
	OBJ_CANDLES2 = 34,
	OBJ_STANDARD1 = 35,
	OBJ_STANDARD2 = 36,
	OBJ_TORCH1_TIKI = 37,
	OBJ_TORCH2_WALL = 38,
	OBJ_ROGUEBONFIRE = 39,
	OBJ_RIVER1 = 40,
	OBJ_RIVER2 = 41,
	OBJ_RIVER3 = 42,
	OBJ_RIVER4 = 43,
	OBJ_RIVER5 = 44,
	OBJ_AMBIENT_SOUND_GENERATOR = 45,
	OBJ_CRATE = 46,
	OBJ_ANDARIEL_DOOR = 47,
	OBJ_ROGUETORCH1 = 48,
	OBJ_ROGUETORCH2 = 49,
	OBJ_CASKETR = 50,
	OBJ_CASKETL = 51,
	OBJ_URN_3 = 52,
	OBJ_CASKET = 53,
	OBJ_ROGUE_CORPSE_1 = 54,
	OBJ_ROGUE_CORPSE_2 = 55,
	OBJ_ROLLING_ROGUE_CORPSE = 56,
	OBJ_ROGUE_ON_A_STICK_1 = 57,
	OBJ_ROGUE_ON_A_STICK_2 = 58,
	OBJ_TOWN_PORTAL = 59,
	OBJ_PERMANENT_TOWN_PORTAL = 60,
	OBJ_INVISIBLE_OBJECT = 61,
	OBJ_DOOR_CATHEDRAL_LEFT = 62,
	OBJ_DOOR_CATHEDRAL_RIGHT = 63,
	OBJ_DOOR_WOODEN_LEFT_2 = 64,
	OBJ_INVISIBLE_RIVER_SOUND1 = 65,
	OBJ_INVISIBLE_RIVER_SOUND2 = 66,
	OBJ_RIPPLE1 = 67,
	OBJ_RIPPLE2 = 68,
	OBJ_RIPPLE3 = 69,
	OBJ_RIPPLE4 = 70,
	OBJ_FOREST_NIGHT_SOUND_1 = 71,
	OBJ_FOREST_NIGHT_SOUND_2 = 72,
	OBJ_YETI_DUNG = 73,
	OBJ_TRAP_DOOR = 74,
	OBJ_DOOR_BY_DOCK_ACT_2 = 75,
	OBJ_SEWER_DRIP = 76,
	OBJ_HEALTHORAMA = 77,
	OBJ_INVISIBLE_TOWN_SOUND = 78,
	OBJ_CASKET_3 = 79,
	OBJ_OBELISK = 80,
	OBJ_FOREST_ALTAR = 81,
	OBJ_BUBBLING_POOL_OF_BLOOD = 82,
	OBJ_HORN_SHRINE = 83,
	OBJ_HEALING_WELL = 84,
	OBJ_BULL_SHRINEHEALTH_TOMBS = 85,
	OBJ_STELEMAGIC_SHRINE_STONE_DESERT = 86,
	OBJ_TOMBCHEST_1_LARGECHESTL = 87,
	OBJ_TOMBCHEST_2_LARGECHESTR = 88,
	OBJ_MUMMY_COFFINL_TOMB = 89,
	OBJ_DESERT_OBELISK = 90,
	OBJ_TOMB_DOOR_LEFT = 91,
	OBJ_TOMB_DOOR_RIGHT = 92,
	OBJ_MANA_SHRINEFORINNERHELL = 93,
	OBJ_URN_4 = 94,
	OBJ_URN_5 = 95,
	OBJ_HEALTH_SHRINEFORINNERHELL = 96,
	OBJ_INNERSHRINEHELL = 97,
	OBJ_TOMB_DOOR_LEFT_2 = 98,
	OBJ_TOMB_DOOR_RIGHT_2 = 99,
	OBJ_PORTAL_TO_DURIEL_LAIR = 100,
	OBJ_BRAZIER3 = 101,
	OBJ_FLOOR_BRAZIER = 102,
	OBJ_FLIES = 103,
	OBJ_ARMOR_STAND_1R = 104,
	OBJ_ARMOR_STAND_2L = 105,
	OBJ_WEAPON_RACK_1R = 106,
	OBJ_WEAPON_RACK_2L = 107,
	OBJ_MALUS = 108,
	OBJ_PALACE_SHRINE_HEALTHR_HAROM_ARCANE_SANCTUARY = 109,
	OBJ_DRINKER = 110,
	OBJ_FOUNTAIN_1 = 111,
	OBJ_GESTURER = 112,
	OBJ_FOUNTAIN_2_WELL_DESERT_TOMB = 113,
	OBJ_TURNER = 114,
	OBJ_FOUNTAIN_3 = 115,
	OBJ_SNAKE_WOMAN_MAGIC_SHRINE_TOMB_ARCANE_SANCTUARY = 116,
	OBJ_JUNGLE_TORCH = 117,
	OBJ_FOUNTAIN_4 = 118,
	OBJ_WAYPOINT_PORTAL = 119,
	OBJ_HEALTHSHRINE_ACT_3_DUNGEUN = 120,
	OBJ_PLACEHOLDER_1 = 121,
	OBJ_PLACEHOLDER_2 = 122,
	OBJ_INNERSHRINEHELL2 = 123,
	OBJ_INNERSHRINEHELL3 = 124,
	OBJ_IHOBJECT3_INNER_HELL_1 = 125,
	OBJ_SKULLPILE_INNER_HELL = 126,
	OBJ_IHOBJECT5_INNER_HELL_2 = 127,
	OBJ_HOBJECT4_INNER_HELL = 128,
	OBJ_SECRET_DOOR_1 = 129,
	OBJ_POOL_ACT_1_WILDERNESS = 130,
	OBJ_VILE_DOG_AFTERGLOW = 131,
	OBJ_CATHEDRALWELL_ACT_1_INSIDE = 132,
	OBJ_SHRINE1_ARCANE_SANCTUARY = 133,
	OBJ_DSHRINE2_ACT_2_SHRINE = 134,
	OBJ_DESERTSHRINE3_ACT_2_SHRINE = 135,
	OBJ_DSHRINE1_ACT_2_SHRINE = 136,
	OBJ_DESERTWELL_ACT_2_WELL_DESERT_TOMB = 137,
	OBJ_CAVEWELL_ACT_1_CAVES_ = 138,
	OBJ_CHEST_R_LARGE_ACT_1 = 139,
	OBJ_CHEST_R_TALLSKINNEY_ACT_1 = 140,
	OBJ_CHEST_R_MED_ACT_1 = 141,
	OBJ_JUG1_ACT_2_DESERT_1 = 142,
	OBJ_JUG2_ACT_2_DESERT_2 = 143,
	OBJ_LCHEST1_ACT_1 = 144,
	OBJ_WAYPOINTI_INNER_HELL = 145,
	OBJ_DCHEST2R_ACT_2_DESERT_TOMB_CHEST_R_MED = 146,
	OBJ_DCHESTR_ACT_2_DESERT_TOMB_CHEST_R_LARGE = 147,
	OBJ_DCHESTL_ACT_2_DESERT_TOMB_CHEST_L_LARGE = 148,
	OBJ_TAINTED_SUN_ALTAR_QUEST = 149,
	OBJ_DSHRINE1_ACT_2_DESERT = 150,
	OBJ_DSHRINE4_ACT_2_DESERT = 151,
	OBJ_WHERE_YOU_PLACE_THE_HORADRIC_STAFF = 152,
	OBJ_TYRAEL_DOOR = 153,
	OBJ_GUARD_CORPSE = 154,
	OBJ_ROCK_ACT_1_WILDERNESS = 155,
	OBJ_WAYPOINT_ACT_2 = 156,
	OBJ_WAYPOINT_ACT_1_WILDERNESS = 157,
	OBJ_CORPSE = 158,
	OBJ_ROCKB_ACT_1_WILDERNESS = 159,
	OBJ_FIRE_SMALL = 160,
	OBJ_FIRE_MEDIUM = 161,
	OBJ_FIRE_LARGE = 162,
	OBJ_CLIFF_ACT_1_WILDERNESS = 163,
	OBJ_MANA_WELL1 = 164,
	OBJ_MANA_WELL2 = 165,
	OBJ_MANA_WELL3_ACT_2_TOMB_ = 166,
	OBJ_MANA_WELL4_ACT_2_HAROM = 167,
	OBJ_MANA_WELL5 = 168,
	OBJ_LOG = 169,
	OBJ_JUNGLE_HEALWELL_ACT_3 = 170,
	OBJ_CORPSEB = 171,
	OBJ_HEALTH_WELL_HEALTH_SHRINE_DESERT = 172,
	OBJ_MANA_WELL7_MANA_SHRINE_DESERT = 173,
	OBJ_ROCKC_ACT_1_WILDERNESS = 174,
	OBJ_ROCKD_ACT_1_WILDERNESS = 175,
	OBJ_CHEST_L_MED = 176,
	OBJ_CHEST_L_LARGE = 177,
	OBJ_GUARD_ON_A_STICK_DESERT_TOMB_HAROM = 178,
	OBJ_BOOKSHELF1 = 179,
	OBJ_BOOKSHELF2 = 180,
	OBJ_JUNGLE_CHEST_ACT_3 = 181,
	OBJ_TOMBCOFFIN = 182,
	OBJ_CHEST_L_MED_JUNGLE = 183,
	OBJ_JUNGLE_SHRINE2 = 184,
	OBJ_JUNGLE_OBJECT_ACT3_1 = 185,
	OBJ_JUNGLE_OBJECT_ACT3_2 = 186,
	OBJ_JUNGLE_OBJECT_ACT3_3 = 187,
	OBJ_JUNGLE_OBJECT_ACT3_4 = 188,
	OBJ_CAIN_PORTAL = 189,
	OBJ_JUNGLE_SHRINE3_ACT_3 = 190,
	OBJ_JUNGLE_SHRINE4_ACT_3 = 191,
	OBJ_TELEPORTATION_PAD1 = 192,
	OBJ_LAM_ESEN_TOME = 193,
	OBJ_STAIRSL = 194,
	OBJ_STAIRSR = 195,
	OBJ_TEST_DATA_FLOORTRAP = 196,
	OBJ_JUNGLESHRINE_ACT_3 = 197,
	OBJ_CHEST_L_TALLSKINNEY_GENERAL_CHEST_R = 198,
	OBJ_MAFISTOSHRINE1 = 199,
	OBJ_MAFISTOSHRINE2 = 200,
	OBJ_MAFISTOSHRINE3 = 201,
	OBJ_MAFISTOMANA = 202,
	OBJ_MAFISTOLAIR = 203,
	OBJ_BOX = 204,
	OBJ_ALTAR = 205,
	OBJ_MAFISTOHEALTH = 206,
	OBJ_WATER_ROCKS_IN_ACT_3_WROK = 207,
	OBJ_BASKET_1 = 208,
	OBJ_BASKET_2 = 209,
	OBJ_WATER_LOGS_IN_ACT_3_NE_LOGW = 210,
	OBJ_WATER_ROCKS_GIRL_IN_ACT_3_WROB = 211,
	OBJ_BUBBLES_IN_ACT3_WATER = 212,
	OBJ_WATER_LOGS_IN_ACT_3_LOGX = 213,
	OBJ_WATER_ROCKS_IN_ACT_3_ROKB = 214,
	OBJ_WATER_ROCKS_GIRL_IN_ACT_3_WATC = 215,
	OBJ_WATER_ROCKS_IN_ACT_3_WATY = 216,
	OBJ_WATER_LOGS_IN_ACT_3_LOGZ = 217,
	OBJ_WEB_COVERED_TREE_1 = 218,
	OBJ_WEB_COVERED_TREE_2 = 219,
	OBJ_WEB_COVERED_TREE_3 = 220,
	OBJ_WEB_COVERED_TREE_4 = 221,
	OBJ_HOBJECT1 = 222,
	OBJ_CACOON = 223,
	OBJ_CACOON_2 = 224,
	OBJ_HOBJECT1_2 = 225,
	OBJ_OUTERSHRINEHELL = 226,
	OBJ_WATER_ROCK_GIRL_ACT_3_NW_BLGB = 227,
	OBJ_BIG_LOG_ACT_3_SW_BLGA = 228,
	OBJ_SLIMEDOOR1 = 229,
	OBJ_SLIMEDOOR2 = 230,
	OBJ_OUTERSHRINEHELL2 = 231,
	OBJ_OUTERSHRINEHELL3 = 232,
	OBJ_HOBJECT2 = 233,
	OBJ_BIG_LOG_ACT_3_SE_BLGC = 234,
	OBJ_BIG_LOG_ACT_3_NW_BLGD = 235,
	OBJ_HEALTH_WELLFORHELL = 236,
	OBJ_ACT3WAYPOINT_TOWN = 237,
	OBJ_WAYPOINTH = 238,
	OBJ_BURNING_TOWN1 = 239,
	OBJ_GCHEST1L_GENERAL = 240,
	OBJ_GCHEST2R_GENERAL = 241,
	OBJ_GCHEST3R_GENERAL = 242,
	OBJ_GLCHEST3L_GENERAL = 243,
	OBJ_SEWERS1 = 244,
	OBJ_BURNING_TOWN2 = 245,
	OBJ_SEWERS2 = 246,
	OBJ_BED_ACT_1 = 247,
	OBJ_BED_ACT_1_2 = 248,
	OBJ_MANA_WELLFORHELL = 249,
	OBJ_EXPLODING_COW_FOR_TRISTAN_AND_ACT_3_VERY_RARE_1_OR_2 = 250,
	OBJ_GIDBINN_ALTAR = 251,
	OBJ_GIDBINN_DECOY = 252,
	OBJ_DIABLO_RIGHT_LIGHT = 253,
	OBJ_DIABLO_LEFT_LIGHT = 254,
	OBJ_DIABLO_START_POINT = 255,
	OBJ_STOOL_FOR_ACT_1_CABIN = 256,
	OBJ_WOOD_FOR_ACT_1_CABIN = 257,
	OBJ_MORE_WOOD_FOR_ACT_1_CABIN = 258,
	OBJ_SKELETON_SPAWN_FOR_HELL_FACING_NW = 259,
	OBJ_HOLYSHRINE_FOR_MONASTERYCATACOMBSJAIL = 260,
	OBJ_SPIKES_FOR_TOMBS_FLOORTRAP = 261,
	OBJ_ACT_1_CATHEDRAL = 262,
	OBJ_ACT_1_JAIL1 = 263,
	OBJ_ACT_1_JAIL2 = 264,
	OBJ_ACT_1_JAIL3 = 265,
	OBJ_GOO_PILE_FOR_SAND_MAGGOT_LAIR = 266,
	OBJ_BANK = 267,
	OBJ_WIRT_BODY = 268,
	OBJ_GOLD_PLACEHOLDER = 269,
	OBJ_GUARD_CORPSE_2 = 270,
	OBJ_DEAD_VILLAGER_1 = 271,
	OBJ_DEAD_VILLAGER_2 = 272,
	OBJ_YET_ANOTHER_FLAME_NO_DAMAGE = 273,
	OBJ_TINY_PIXEL_SHAPED_THINGIE = 274,
	OBJ_HEALTH_SHRINE_FOR_CAVES = 275,
	OBJ_MANA_SHRINE_FOR_CAVES = 276,
	OBJ_CAVE_MAGIC_SHRINE = 277,
	OBJ_MANASHRINE_ACT_3_DUNGEUN = 278,
	OBJ_MAGIC_SHRINE_ACT_3_SEWERS = 279,
	OBJ_HEALTHWELL_ACT_3_SEWERS = 280,
	OBJ_MANAWELL_ACT_3_SEWERS = 281,
	OBJ_MAGIC_SHRINE_ACT_3_SEWERS_DUNGEON = 282,
	OBJ_BRAZIER_CELLER_ACT_2 = 283,
	OBJ_ANUBIS_COFFIN_ACT2_TOMB = 284,
	OBJ_BRAZIER_GENERAL_ACT_2_SEWERS_TOMB_DESERT = 285,
	OBJ_BRAZIER_TALL_ACT_2_DESERT_TOWN_TOMBS = 286,
	OBJ_BRAZIER_SMALL_ACT_2_DESERT_TOWN_TOMBS = 287,
	OBJ_WAYPOINT_CELLER = 288,
	OBJ_BED_FOR_HARUM = 289,
	OBJ_IRON_GRATE_DOOR_LEFT = 290,
	OBJ_IRON_GRATE_DOOR_RIGHT = 291,
	OBJ_WOODEN_GRATE_DOOR_LEFT = 292,
	OBJ_WOODEN_GRATE_DOOR_RIGHT = 293,
	OBJ_WOODEN_DOOR_LEFT = 294,
	OBJ_WOODEN_DOOR_RIGHT = 295,
	OBJ_WALL_TORCH_LEFT_FOR_TOMBS = 296,
	OBJ_WALL_TORCH_RIGHT_FOR_TOMBS = 297,
	OBJ_ARCANE_SANCTUARY_PORTAL = 298,
	OBJ_MAGIC_SHRINE_ACT_2_HARAM_1 = 299,
	OBJ_MAGIC_SHRINE_ACT_2_HARAM_2 = 300,
	OBJ_MAGGOT_WELL_HEALTH = 301,
	OBJ_MAGGOT_WELL_MANA = 302,
	OBJ_MAGIC_SHRINE_ACT_3_ARCANE_SANCTUARY = 303,
	OBJ_TELEPORTATION_PAD2 = 304,
	OBJ_TELEPORTATION_PAD3 = 305,
	OBJ_TELEPORTATION_PAD4 = 306,
	OBJ_ARCANE_THING1 = 307,
	OBJ_ARCANE_THING2 = 308,
	OBJ_ARCANE_THING3 = 309,
	OBJ_ARCANE_THING4 = 310,
	OBJ_ARCANE_THING5 = 311,
	OBJ_ARCANE_THING6 = 312,
	OBJ_ARCANE_THING7 = 313,
	OBJ_HAREM_GUARD_1 = 314,
	OBJ_HAREM_GUARD_2 = 315,
	OBJ_HAREM_GUARD_3 = 316,
	OBJ_HAREM_GUARD_4 = 317,
	OBJ_HAREM_BLOCKER = 318,
	OBJ_HEALTHWELL_ACT_2_ARCANE = 319,
	OBJ_HEALTHWELL_ACT_2_ARCANE2 = 320,
	OBJ_TEST_DATA2 = 321,
	OBJ_TOMBWELL_ACT_2_WELL_TOMB = 322,
	OBJ_WAYPOINT_ACT2_SEWER = 323,
	OBJ_WAYPOINT_ACT3_TRAVINCAL = 324,
	OBJ_MAGIC_SHRINE_ACT_3_SEWER = 325,
	OBJ_ACT3_SEWER = 326,
	OBJ_TORCH_ACT_3_SEWER_STRA = 327,
	OBJ_TORCH_ACT_3_KURAST_STRB = 328,
	OBJ_MAFISTOCHESTLARGELEFT = 329,
	OBJ_MAFISTOCHESTLARGERIGHT = 330,
	OBJ_MAFISTOCHESTMEDLEFT = 331,
	OBJ_MAFISTOCHESTMEDRIGHT = 332,
	OBJ_SPIDERLAIRCHESTLARGELEFT = 333,
	OBJ_SPIDERLAIRCHESTTALLLEFT = 334,
	OBJ_SPIDERLAIRCHESTMEDRIGHT = 335,
	OBJ_SPIDERLAIRCHESTTALLRIGHT = 336,
	OBJ_STEEG_STONE = 337,
	OBJ_GUILD_VAULT = 338,
	OBJ_TROPHY_CASE = 339,
	OBJ_MESSAGE_BOARD = 340,
	OBJ_MEPHISTO_BRIDGE = 341,
	OBJ_HELLGATE = 342,
	OBJ_MANAWELL_ACT_3_KURAST = 343,
	OBJ_HEALTHWELL_ACT_3_KURAST = 344,
	OBJ_HELLFIRE1 = 345,
	OBJ_HELLFIRE2 = 346,
	OBJ_HELLFIRE3 = 347,
	OBJ_HELLLAVA1 = 348,
	OBJ_HELLLAVA2 = 349,
	OBJ_HELLLAVA3 = 350,
	OBJ_HELLLIGHTSOURCE1 = 351,
	OBJ_HELLLIGHTSOURCE2 = 352,
	OBJ_HELLLIGHTSOURCE3 = 353,
	OBJ_HORADRIC_CUBE_CHEST = 354,
	OBJ_HORADRIC_SCROLL_CHEST = 355,
	OBJ_STAFF_OF_KINGS_CHEST = 356,
	OBJ_YET_ANOTHER_TOME = 357,
	OBJ_HELL_BRAZIER1 = 358,
	OBJ_HELL_BRAZIER2 = 359,
	OBJ_DUNGEON1 = 360,
	OBJ_MAGIC_SHRINE_ACT_3DUNDEON = 361,
	OBJ_DUNGEON2 = 362,
	OBJ_OUTERHELL_SKELETON = 363,
	OBJ_GUY_FOR_DUNGEON = 364,
	OBJ_CASKET_FOR_ACT_3_DUNGEON = 365,
	OBJ_STAIRS_FOR_ACT_3_SEWER_QUEST = 366,
	OBJ_LEVER_FOR_ACT_3_SEWER_QUEST = 367,
	OBJ_START_POSITION = 368,
	OBJ_TRAPPED_SOUL_PLACEHOLDER = 369,
	OBJ_TORCH_FOR_ACT3_TOWN = 370,
	OBJ_LARGECHESTR2 = 371,
	OBJ_INNERHELLBONEPILE = 372,
	OBJ_SKELETON_SPAWN_FOR_HELL_FACING_NE = 373,
	OBJ_FOG_ACT_3_WATER_RFGA = 374,
	OBJ_NOT_USED = 375,
	OBJ_FORGE_HELL = 376,
	OBJ_PORTAL_TO_NEXT_GUILD_LEVEL = 377,
	OBJ_HRATLI_START = 378,
	OBJ_HRATLI_END = 379,
	OBJ_BURNING_GUY_FOR_OUTER_HELL1 = 380,
	OBJ_BURNING_GUY_FOR_OUTER_HELL2 = 381,
	OBJ_NATALYA_START = 382,
	OBJ_GUY_STUCK_IN_HELL1 = 383,
	OBJ_GUY_STUCK_IN_HELL2 = 384,
	OBJ_CAIN_START_POSITION = 385,
	OBJ_STAIRSR2 = 386,
	OBJ_ARCANESANCTUARYBIGCHESTLEFT = 387,
	OBJ_ARCANESANCTUARYCASKET = 388,
	OBJ_ARCANESANCTUARYBIGCHESTRIGHT = 389,
	OBJ_ARCANESANCTUARYCHESTSMALLLEFT = 390,
	OBJ_ARCANESANCTUARYCHESTSMALLRIGHT = 391,
	OBJ_DIABLO_SEAL1 = 392,
	OBJ_DIABLO_SEAL2 = 393,
	OBJ_DIABLO_SEAL3 = 394,
	OBJ_DIABLO_SEAL4 = 395,
	OBJ_DIABLO_SEAL5 = 396,
	OBJ_SPARKLYCHEST = 397,
	OBJ_WAYPOINT_PANDAMONIA_FORTRESS = 398,
	OBJ_FISSURE_FOR_ACT_4_INNER_HELL = 399,
	OBJ_BRAZIER_FOR_ACT_4_HELL_MESA = 400,
	OBJ_SMOKE = 401,
	OBJ_WAYPOINT_VALLEYWAYPOINT = 402,
	OBJ_HELL_BRAZIER3 = 403,
	OBJ_COMPELLING_ORB = 404,
	OBJ_KHALIM_CHEST1 = 405,
	OBJ_KHALIM_CHEST2 = 406,
	OBJ_KHALIM_CHEST3 = 407,
	OBJ_FORTRESS_BRAZIER_1 = 408,
	OBJ_FORTRESS_BRAZIER_2 = 409,
	OBJ_TO_CONTROL_SIEGE_MACHINES = 410,
	OBJ_POT_O_TORCH_LEVEL_1 = 411,
	OBJ_FIRE_PIT_LEVEL_1 = 412,
	OBJ_EXPANSION_NO_SNOW1 = 413,
	OBJ_EXPANSION_NO_SNOW2 = 414,
	OBJ_EXPANSION_NO_SNOW3 = 415,
	OBJ_EXPANSION_NO_SNOW4 = 416,
	OBJ_EXPANSION_NO_SNOW5 = 417,
	OBJ_EXPANSION_NO_SNOW6 = 418,
	OBJ_WILDERNESSHSIEGE = 419,
	OBJ_EXPANSION_NO_SNOW7 = 420,
	OBJ_EXPANSION_NO_SNOW8 = 421,
	OBJ_EXPANSION_NO_SNOW9 = 422,
	OBJ_EXPANSION_NO_SNOW10 = 423,
	OBJ_EXPANSION_NO_SNOW11 = 424,
	OBJ_EXPANSION_NO_SNOW12 = 425,
	OBJ_EXPANSION_NO_SNOW13 = 426,
	OBJ_EXPANSION_NO_SNOW14 = 427,
	OBJ_EXPANSION_NO_SNOW15 = 428,
	OBJ_EXPANSION_NO_SNOW16 = 429,
	OBJ_EXPANSION_NO_SNOW17 = 430,
	OBJ_EXPANSION_NO_SNOW18 = 431,
	OBJ_EXPANSION_NO_SNOW19 = 432,
	OBJ_EXPANSION_NO_SNOW20 = 433,
	OBJ_EXPANSION_NO_SNOW21 = 434,
	OBJ_CAMP_FIRE = 435,
	OBJ_TOWN_TORCH = 436,
	OBJ_EXPANSION_NO_SNOW22 = 437,
	OBJ_WILDERNESS_SIEGE1 = 438,
	OBJ_WILDERNESS_SIEGE2 = 439,
	OBJ_WILDERNESS_SIEGE3 = 440,
	OBJ_TOWN_FLAG = 441,
	OBJ_CHANDELEIR = 442,
	OBJ_WILDERNESS_SIEGE4 = 443,
	OBJ_WILDERNESS_SIEGE5 = 444,
	OBJ_WILDERNESS_SIEGE6 = 445,
	OBJ_WILDERNESS1 = 446,
	OBJ_WILDERNESS2 = 447,
	OBJ_EXPANSION_NO_SNOW23 = 448,
	OBJ_TOWN_MAIN_GATE = 449,
	OBJ_SEIGE1 = 450,
	OBJ_SEIGE2 = 451,
	OBJ_PRESET_IN_ENEMY_CAMP1 = 452,
	OBJ_PRESET_IN_ENEMY_CAMP2 = 453,
	OBJ_WILDERNESS_SIEGE7 = 454,
	OBJ_SPECIALCHEST = 455,
	OBJ_WILDERNESS3 = 456,
	OBJ_WILDERNESS4 = 457,
	OBJ_INSIDE_OF_TEMPLE = 458,
	OBJ_DREHYA_START_IN_TOWN = 459,
	OBJ_DREHYA_START_OUTSIDE_TOWN = 460,
	OBJ_NIHLATHAK_START_IN_TOWN = 461,
	OBJ_NIHLATHAK_START_OUTSIDE_TOWN = 462,
	OBJ_ICECAVE_2 = 463,
	OBJ_ICECAVE_3 = 464,
	OBJ_ICECAVE_4 = 465,
	OBJ_ICECAVE_5 = 466,
	OBJ_ICECAVE_6 = 467,
	OBJ_ICECAVE_7 = 468,
	OBJ_ICECAVE_8 = 469,
	OBJ_ICECAVE_9 = 470,
	OBJ_ICECAVE_10 = 471,
	OBJ_ICECAVE_11 = 472,
	OBJ_CAGED_FELLOW = 473,
	OBJ_STATUE1 = 474,
	OBJ_STATUE2 = 475,
	OBJ_STATUE3 = 476,
	OBJ_SEIGE_WILDERNESS = 477,
	OBJ_CLIENT_SMOKE = 478,
	OBJ_ICECAVE_12 = 479,
	OBJ_ICECAVE_13 = 480,
	OBJ_ICECAVE_14 = 481,
	OBJ_EXPANSION_TIKI_TORCH = 482,
	OBJ_BAALS1 = 483,
	OBJ_BAALS2 = 484,
	OBJ_BAAL_LAIR1 = 485,
	OBJ_BAAL_LAIR2 = 486,
	OBJ_BAAL_LAIR3 = 487,
	OBJ_BAAL_LAIR4 = 488,
	OBJ_BAAL_LAIR5 = 489,
	OBJ_BAAL_LAIR6 = 490,
	OBJ_SNOWY1 = 491,
	OBJ_SNOWY2 = 492,
	OBJ_SNOWY3 = 493,
	OBJ_BAALS_WAYPOINT = 494,
	OBJ_SNOWY_SHRINE3_1 = 495,
	OBJ_WILDERNESS_WAYPOINT = 496,
	OBJ_SNOWY_SHRINE3_2 = 497,
	OBJ_BAALSLAIR = 498,
	OBJ_BAAL_LAIR7 = 499,
	OBJ_SNOWY4 = 500,
	OBJ_SNOWY5 = 501,
	OBJ_SNOWY6 = 502,
	OBJ_BAALS_SHRINE3 = 503,
	OBJ_SNOWY7 = 504,
	OBJ_SNOWY8 = 505,
	OBJ_SNOWY9 = 506,
	OBJ_SNOWY10 = 507,
	OBJ_PEN_BREAKABLE_DOOR = 508,
	OBJ_TEMPLE1 = 509,
	OBJ_SNOWY11 = 510,
	OBJ_ICECAVE_15 = 511,
	OBJ_TEMPLE2 = 512,
	OBJ_TEMPLE3 = 513,
	OBJ_TEMPLE4 = 514,
	OBJ_TEMPLE5 = 515,
	OBJ_TEMPLE6 = 516,
	OBJ_TEMPLE7 = 517,
	OBJ_BAALS3 = 518,
	OBJ_ICECAVE_1 = 519,
	OBJ_TEMPLE8 = 520,
	OBJ_TEMPLE9 = 521,
	OBJ_TEMPLE10 = 522,
	OBJ_TOUCH_ME_FOR_BLACKSMITH = 523,
	OBJ_BAAL_LAIR8 = 524,
	OBJ_BAAL_LAIR9 = 525,
	OBJ_BAAL_LAIR10 = 526,
	OBJ_ICE_CAVE_BUBBLES_01 = 527,
	OBJ_ICE_CAVE_BUBBLES_02 = 528,
	OBJ_REDBAAL_LAIR1 = 529,
	OBJ_REDBAAL_LAIR2 = 530,
	OBJ_REDBAAL_LAIR3 = 531,
	OBJ_REDBAAL_LAIR4 = 532,
	OBJ_REDBAAL_LAIR5 = 533,
	OBJ_REDBAAL_LAIR6 = 534,
	OBJ_REDBAALS = 535,
	OBJ_REDBAAL_LAIR7 = 536,
	OBJ_REDBAAL_LAIR8 = 537,
	OBJ_TEMPLE11 = 538,
	OBJ_TEMPLE12 = 539,
	OBJ_EVERYWHERE1 = 540,
	OBJ_TEMPLE13 = 541,
	OBJ_LARZUK_GREETING = 542,
	OBJ_LARZUK_STANDARD = 543,
	OBJ_TEMPLE14 = 544,
	OBJ_EVERYWHERE2 = 545,
	OBJ_ANCIENTSALTAR = 546,
	OBJ_ANCIENTSDOOR = 547,
	OBJ_EVERYWHERE3 = 548,
	OBJ_EVERYWHERE4 = 549,
	OBJ_EVERYWHERE5 = 550,
	OBJ_EVERYWHERE6 = 551,
	OBJ_SUMMIT = 552,
	OBJ_OUTSIDE1 = 553,
	OBJ_OUTSIDE2 = 554,
	OBJ_ICE_CAVE_STEAM = 555,
	OBJ_EVERYWHERE7 = 556,
	OBJ_BAAL_LAIR11 = 557,
	OBJ_FROZEN_ANYA = 558,
	OBJ_BBQ_BUNNY = 559,
	OBJ_BAAL_TORCH_BIG = 560,
	OBJ_INVISIBLE_ANCIENT = 561,
	OBJ_INVISIBLE_BASE = 562,
	OBJ_BAALS_PORTAL1 = 563,
	OBJ_SUMMIT_DOOR = 564,
	OBJ_LAST_PORTAL = 565,
	OBJ_LAST_LAST_PORTAL = 566,
	OBJ_TEST_DATA3 = 567,
	OBJ_TEST_DATA4 = 568,
	OBJ_BAALS_PORTAL2 = 569,
	OBJ_FIRE_PLACE_GUY = 570,
	OBJ_DOOR_BLOCKER1 = 571,
	OBJ_DOOR_BLOCKER2 = 572,
};
/* End of file */
