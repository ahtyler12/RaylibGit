#include <string>
#include <raylib.h>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "json/writer.h"
#pragma once

struct SoundSettings
{
private:
	float masterVolume = 1.f; //Pulled from settings file and adjusted through settings menu
public:
	float _getMasterVolume()
	{
		return GetMasterVolume(); //could use variable but since these should match anyway this should be fine
	}

	void _setMaster(float _masterVolume)
	{
		SetMasterVolume(_masterVolume);
		masterVolume = _masterVolume;
	}

	/*Function to write to file here*/

	/*Function to read from file here*/

};

struct MatchSettings
{
	/*Placeholder data for now. Once base systems are done then this will be updated with real data*/
private:
	int numRounds = 2;
	float roundTime = 99.f;
public:
	void setRoundCount(int _newRounds)
	{
		numRounds = _newRounds;
	}
	void setRoundTime(float _newTime)
	{
		roundTime = _newTime;
	}

	int getRoundCount()
	{
		return numRounds;
	}

	float getRoundTime()
	{
		return roundTime;
	}

	/*Function to read from file here*/

};

struct InputSettings
{
	/*Empty for now until I figure out how to handle multiple controllers and types*/
	bool trashData = false;

	/*Function to write to file here*/

	/*Function to read from file here*/
};

enum ScreenMode
{
	WINDOWED = 0,
	BORDERLESS,
	FULLSCREEN
};

struct VideoSettings
{
	int _screenWidth = 800;
	int _screenHeight = 600;
	ScreenMode screenMode = BORDERLESS;

	void initVideo()
	{
		switch (screenMode)
		{
		case WINDOWED:
			SetConfigFlags(FLAG_MSAA_4X_HINT);
			break;
		case BORDERLESS:
			SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
			break;
		case FULLSCREEN:
			SetConfigFlags(FLAG_FULLSCREEN_MODE);
			break;
		default:
			break;
		}
		
		InitWindow(_screenWidth, _screenHeight, "My first RAYLIB program!");

	}

	void setScreenMode(ScreenMode _newMode)
	{
		switch (screenMode)
		{
		case WINDOWED:
			if (IsWindowFullscreen)
			{
				ToggleFullscreen();//Not sure if I need to call this but just in case
			}		
			SetConfigFlags(FLAG_MSAA_4X_HINT);
			break;
		case BORDERLESS:
			ToggleBorderlessWindowed();
			break;
		case FULLSCREEN:
			ToggleFullscreen();
			break;
		default:
			break;
		}
		

	}

	ScreenMode getScreenMode()
	{
		return screenMode;
	}

	void setScreenHeight(int _newHeight)
	{
		_screenHeight = _newHeight;
	}

	int getScreenHeight()
	{
		return _screenHeight;
	}

	void setScreenWidth(int _newWidth)
	{
		_screenWidth = _newWidth;
	}

	int getScreenWidth()
	{
		return _screenWidth;
	}
};


class GameSettings
{
public:
	GameSettings();
	~GameSettings();

	void WriteSettings();
	bool ReadSettings();

	SoundSettings _soundSettings;
	MatchSettings _matchSettings;
	InputSettings _inputSettings;
	VideoSettings _videoSettings;

private:
	std::string fileName = "./resources/GameSettings.json";
};

