/*
 * Copyright (C) 2010-2011 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TRINITY_CALENDARMGR_H
#define TRINITY_CALENDARMGR_H

#include "Calendar.h"
#include "Player.h"

#include <ace/Singleton.h>

class CalendarMgr
{
	friend class ACE_Singleton<CalendarMgr, ACE_Null_Mutex>;

public:
	CalendarMgr();
	~CalendarMgr();
	
	void LoadHolidayData();
	
		
	CalendarInvite const* GetInvite(uint64 inviteID)
	{
		CalendarInviteMap::const_iterator itr = _inviteMap.find(inviteID);
		if(itr == _inviteMap.end())
			return NULL;
		return &itr->second;
	}

	void AddInvite(CalendarInvite invite) { _inviteMap[invite.id] = invite; }
	void RemoveInvite(uint64 inviteID) { _inviteMap.erase(inviteID); }

	CalendarEvent const* GetEvent(uint64 eventID)
	{
		CalendarEventMap::const_iterator itr = _eventMap.find(eventID);
		if(itr == _eventMap.end())
			return NULL;
		return &itr->second;
	}

	void AddEvent(CalendarEvent calendar_event) { _eventMap[calendar_event.id] = calendar_event; }
	void RemoveEvent(uint64 eventID) { _eventMap.erase(eventID); }

	void AppendInvitesToCalendarPacketForPlayer(WorldPacket &data, Player *player);
	void AppendEventsToCalendarPacketForPlayer(WorldPacket &data, Player *player);

	uint64 GetNextEventID() { return ++_currentEventID; }
	uint64 GetNextInviteID() { return ++_currentInviteID; }

	uint32 GetHolidayCount() {return _holidayCount;}

private:
	CalendarInviteMap _inviteMap;
	CalendarEventMap _eventMap;
	HolydaysEntryMap _holidaysEntryMap;
	HolydayNamesMap _holidayNameMap;
	uint32 _holidayCount;
	uint64 _currentEventID;
	uint64 _currentInviteID;
};

#define sCalendarMgr ACE_Singleton<CalendarMgr, ACE_Null_Mutex>::instance()

#endif