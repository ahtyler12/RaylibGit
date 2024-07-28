#include "MatchData.h"

/*Functions for ggpo callbacks*/

bool MatchData::AdvanceFrame(int flags)
{
	int inputs[GGPO_MAX_PLAYERS]; //just a test for now. should actually be the inputs from the players
	int disconnect_flags;
	ggpo_synchronize_input(ggpo, (void*)inputs, sizeof(int), &disconnect_flags);
	return true;
}

bool MatchData::Event_callback(GGPOEvent* info)
{
	/*Do stuff with these messages. should call events*/
	switch (info->code) {
	case GGPO_EVENTCODE_CONNECTED_TO_PEER:
		break;
	case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
		break;
	case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
		break;
	case GGPO_EVENTCODE_RUNNING:
		break;
	case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
		break;
	case GGPO_EVENTCODE_CONNECTION_RESUMED:
		break;
	case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
		break;
	case GGPO_EVENTCODE_TIMESYNC:
		break;
	}
	return true;
}

void MatchData::Free_buffer(void* buffer)
{
	free(buffer);
}

bool MatchData::Log_game_state(char* filename, unsigned char* buffer, int)
{
	/*Add logging data here to keep track of important information*/
	return true;
}

bool MatchData::Load_game_state_callback(unsigned char* buffer, int len)
{
	_memccpy(&ms, buffer, len, sizeof(len));
	return false;
}

bool MatchData::Save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int)
{
	*len = sizeof(ms);
	*buffer = (unsigned char *)malloc(*len);

	if (!*buffer)
	{
		return false;
	}

	_memccpy(*buffer, &ms, *len, sizeof(len));
	
	/*Update Checksum here*/

	return true;
}

bool MatchData::Begin_game_callback(const char*)
{
	return true;
}

bool MatchData::StartMatch(MatchType _type)
{
	
	switch (_type)
	{
	case LOCAL:
		break;
	case ONLINE:
		GGPOPlayer Players[GGPO_MAX_SPECTATORS + GGPO_MAX_PLAYERS];
		InitGGPOSession(2,Players, false);		
		break;
	case REPLAY:
		break;
	default:
		break;
	}
    return false;
}

int MatchData::EndMatch(MatchEndReason _endReason)
{
    return 0;
}

void MatchData::UpdateMatchState()
{
}

GGPOSessionCallbacks  MatchData::CreateCallbacks()
{
	GGPOSessionCallbacks cb = { 0 };
	cb.advance_frame = std::bind(&MatchData::AdvanceFrame, this, std::placeholders::_1);
	cb.begin_game = std::bind(&MatchData::Begin_game_callback, this, std::placeholders::_1);
	cb.free_buffer = std::bind(&MatchData::Free_buffer, this, std::placeholders::_1);;
	cb.load_game_state = std::bind(&MatchData::Load_game_state_callback, this, std::placeholders::_1, std::placeholders::_2);;
	cb.log_game_state = std::bind(&MatchData::Log_game_state, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
	cb.on_event = std::bind(&MatchData::Event_callback, this, std::placeholders::_1);;
	cb.save_game_state = std::bind(&MatchData::Save_game_state_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	return cb;
}

void MatchData::InitGGPOSession(int _numPlayers, GGPOPlayer *players,bool _syncTest)
{
	GGPOSessionCallbacks cb = CreateCallbacks();

	GGPOErrorCode result;

	int numPlayers = 2;
	int numSpectators = 1;


	if (_syncTest)
		result = ggpo_start_synctest(&ggpo, &cb, "GAME", 2, sizeof(int), 1);
	else
		result = ggpo_start_session(&ggpo, &cb, "GAME", 2, sizeof(int), 7000);

	ggpo_set_disconnect_timeout(ggpo, 3000);
	ggpo_set_disconnect_notify_start(ggpo, 1000);

	for (int i = 0; i < numPlayers + numSpectators; i++)
	{
		GGPOPlayerHandle handle;
		result = ggpo_add_player(ggpo, players + i, &handle); //add players to the session
		if (players[i].type == GGPO_PLAYERTYPE_LOCAL)
		{
			ggpo_set_frame_delay(ggpo, handle, 2);
		}
	}
}



