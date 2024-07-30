#include "GameSettings.h"
#include "include\ggponet.h"
#include "Entity.h"
#include <vector>
#include <memory>
#pragma once

enum MatchType
{
	LOCAL = 0,
	ONLINE,
	REPLAY
};

enum MatchEndReason
{
	Success,
	INTERRUPTED
};

struct MatchState 
{
	int _frameNumber;
	int entityNumber;
};

class MatchData
{
public:
	float _roundTime;
	int _numRounds;
	int _numPlayers;
	MatchType _matchType;
	MatchState ms;
	std::shared_ptr<GameSettings> _settings;
	std::vector<std::shared_ptr<Entity>> entities;
	GGPOSession *ggpo = NULL;

	MatchData();
	MatchData(MatchType _type, std::shared_ptr<GameSettings> _newSettings);
	~MatchData();
	bool StartMatch(MatchType _type);
	bool EndMatch(MatchEndReason _endReason);
	void UpdateMatchState();
	void DrawMatchState(Camera3D camera);
	

private:
	bool AdvanceFrame(int);
	bool Event_callback(GGPOEvent* info);
	void Free_buffer(void* buffer);
	bool Log_game_state(char* filename, unsigned char* buffer, int);
	bool Load_game_state_callback(unsigned char* buffer, int len);
	bool Save_game_state_callback(unsigned char** buffer, int* len, int* checksum, int);
	bool Begin_game_callback(const char*);
	GGPOSessionCallbacks  CreateCallbacks();
	void InitGGPOSession(int _numPlayers, GGPOPlayer* players, bool _syncTest);
};

