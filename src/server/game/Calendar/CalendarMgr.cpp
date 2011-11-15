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
#include "gamePCH.h"
#include "CalendarMgr.h"
#include "Calendar.h"
#include "QueryResult.h"
#include "LFGMgr.h"
#include "Player.h"

CalendarMgr::CalendarMgr() : _currentEventID(0), _currentInviteID(0)
{
}

CalendarMgr::~CalendarMgr()
{
}

void CalendarMgr::AppendInvitesToCalendarPacketForPlayer(WorldPacket &data, Player *player)
{
	size_t p_counter = data.wpos();
	data << uint32(0);
	uint32 counter = 0;
	for (CalendarInviteMap::iterator itr = _inviteMap.begin(); itr != _inviteMap.end(); ++itr)
	{
		CalendarInvite invite = itr->second;
		//"if (invite.target_guid == GetGUID())" was not correct getGUID was not identified
		if (invite.target_guid)
		{
			data << uint64(invite.id);                // Invite ID
			data << uint64(invite.c_event);           // Event ID
			data << uint8(invite.rank);               // rank
			data << uint8(0);                         // unk - TODO: Figure out what this is
			data << uint8(0);                         // unk
			data.appendPackGUID(invite.creator_guid); // creator's guid
			counter++;
		}
	}
	data.put<uint32>(p_counter, counter);             // update number of invites
}

void CalendarMgr::AppendEventsToCalendarPacketForPlayer(WorldPacket &data, Player *player)
{
	// TODO: There's gotta be a better way to do this
	size_t p_counter = data.wpos();
	data << uint32(0);
	uint32 counter = 0;
	std::set<uint64> alreadyAdded;
	for (CalendarInviteMap::iterator itr = _inviteMap.begin(); itr != _inviteMap.end(); ++itr)
	{
		CalendarInvite invite = itr->second;
		//"if (invite.target_guid == getGUID())" was not correct getGUID was not identified
		if (invite.target_guid)
		{
			if (alreadyAdded.find(invite.id) == alreadyAdded.end())
			{
				CalendarEvent const* calendar_event = GetEvent(invite.id);
				data << uint64(calendar_event->id);                // event ID
				data << calendar_event->name;                      // event title
				data << uint32(calendar_event->type);              // event type
				data << uint32(calendar_event->time);              // event time as time bit field
				data << uint32(calendar_event->flags);             // event flags
				data << uint32(calendar_event->dungeonID);         // dungeon ID
				data.appendPackGUID(calendar_event->creator_guid); // creator guid
				alreadyAdded.insert(invite.id);
				counter++;
			}
		}
	}
	data.put<uint32>(p_counter, counter);             // update number of invites
}


void CalendarMgr::LoadHolidayData()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;
	for (uint32 i = 0; i < sHolidaysStore.GetNumRows(); ++i)           // areaflag numbered from 0
    {
		 if (HolidaysEntry const* holidays = sHolidaysStore.LookupEntry(i))
			count++; 
	}
	_holidayCount = count;

	sLog->outString(">> Loaded %u Holidays definitions in %u ms", _holidayCount, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
	
}