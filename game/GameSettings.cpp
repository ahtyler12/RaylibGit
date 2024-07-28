#include "GameSettings.h"

GameSettings::GameSettings()
{
	_soundSettings = {};
	_matchSettings = {};
	_inputSettings = {};
	_videoSettings = {};

	ReadSettings();
	_videoSettings.InitVideo();
}

GameSettings::~GameSettings()
{
}

void GameSettings::WriteSettings()
{
	Json::Value root;
	Json::Value Sound;
	Json::Value Video;
	Json::Value Match;
	std::ofstream output(fileName, std::ifstream::binary);
	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	Video["Resolution X"] = _videoSettings.GetScreenWidth(); 
	Video["Resolution Y"] = _videoSettings.GetScreenHeight();
	Video["Screen Mode"] = _videoSettings.GetScreenMode(); 

	Sound["Master Volume"] = _soundSettings.GetMaster();

	Match["Round Number"] = _matchSettings.GetRoundCount();
	Match["Round Time"] = _matchSettings.GetRoundTime();

	root["Video Settings"] = Video;
	root["Sound Settings"] = Sound;
	root["Match Settings"] = Match;
	
	writer->write(root, &output);

}

bool GameSettings::ReadSettings()
{
	Json::Value root;
	std::ifstream input(fileName);
	JSONCPP_STRING errors;
	Json::Reader builder;

	builder.parse(input, root);

	/*Doing Explicitly for now. Should be more generic*/
	_videoSettings.SetScreenWidth(root["Video Settings"]["Resolution X"].asInt());
	_videoSettings.SetScreenHeight(root["Video Settings"]["Resolution Y"].asInt());
	_videoSettings.InitScreenMode(static_cast<ScreenMode>(root["Video Settings"]["Screen Mode"].asInt()));
	_soundSettings.SetMaster(root["Sound Settings"]["Master Volume"].asFloat());
	_matchSettings.SetRoundCount(root["Match Settings"]["Round Number"].asInt());
	_matchSettings.SetRoundTime(root["Match Settings"]["Round Time"].asFloat());
	return true;
}
