/*
 * Copyright (C) 2010-2011 Project StarGate
 */

#ifndef TRINITY_CALENDAR_H
#define TRINITY_CALENDAR_H

#include "Common.h"

enum CalendarEventType
{
	CALENDARTYPE_RAID = 0,
    CALENDARTYPE_DUNGEON,
    CALENDARTYPE_PVP,
    CALENDARTYPE_MEETING,
    CALENDARTYPE_OTHER,
};

enum CalendarInviteStatus

{
    CALENDARSTATUS_INVITED = 0,
    CALENDARSTATUS_ACCEPTED,
    CALENDARSTATUS_DECLINED,
    CALENDARSTATUS_TENTATIVE,
    CALENDARSTATUS_OUT,
    CALENDARSTATUS_STANDBY,
    CALENDARSTATUS_CONFIRMED,
};


struct CalendarEvent

{
    uint64 id;
    uint64 creator_guid;
    std::string name;
    std::string description;
    uint8 type;
    uint8 unk;
    uint32 dungeonID;
    uint32 unkTime;
    uint32 time;
    uint32 flags;
    uint32 guildID;
};

struct CalendarInvite

{
    uint64 id;
    uint64 c_event;
    uint8 status;
    uint8 rank;
    uint8 unk1;
    uint8 unk2;
    uint8 unk3;
    std::string text;
    uint64 creator_guid;
    uint32 time;
    uint64 target_guid;
};

typedef UNORDERED_MAP<uint64, CalendarInvite> CalendarInviteMap;
typedef UNORDERED_MAP<uint64, CalendarEvent> CalendarEventMap;

#endif