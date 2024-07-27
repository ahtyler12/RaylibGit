#include "GameSettings.h"

GameSettings::GameSettings()
{
	_soundSettings = {};
	_matchSettings = {};
	_inputSettings = {};
	_videoSettings = {};

	ReadSettings();
	_videoSettings.initVideo();
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

	Video["Resolution X"] = _videoSettings.getScreenWidth(); //Gives 0 for some reason
	Video["Resolution Y"] = _videoSettings.getScreenHeight();
	Video["Screen Mode"] = _videoSettings.getScreenMode(); 

	Sound["Master Volume"] = _soundSettings._getMasterVolume();

	Match["Round Number"] = _matchSettings.getRoundCount();
	Match["Round Time"] = _matchSettings.getRoundTime();

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
	_videoSettings.setScreenWidth(root["Video Settings"]["Resolution X"].asInt());
	_videoSettings.setScreenHeight(root["Video Settings"]["Resolution Y"].asInt());
	_soundSettings._setMaster(root["Sound Settings"]["Master Volume"].asFloat());
	_matchSettings.setRoundCount(root["Match Settings"]["Round Number"].asInt());
	_matchSettings.setRoundTime(root["Match Settings"]["Round Time"].asFloat());
	return true;
}
