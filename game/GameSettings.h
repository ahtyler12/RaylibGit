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
	float GetMaster()
	{
		return GetMasterVolume(); //could use variable but since these should match anyway this should be fine
	}

	void SetMaster(float _masterVolume)
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
	void SetRoundCount(int _newRounds)
	{
		numRounds = _newRounds;
	}
	void SetRoundTime(float _newTime)
	{
		roundTime = _newTime;
	}

	int GetRoundCount()
	{
		return numRounds;
	}

	float GetRoundTime()
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
	ScreenMode screenMode = WINDOWED;

	void InitVideo()
	{
		InitWindow(_screenWidth, _screenHeight, "My first RAYLIB program!");
		UpdateScreenMode(screenMode);

	}

	void InitScreenMode(ScreenMode _newMode)
	{
		screenMode = _newMode;
	}

	void UpdateScreenMode(ScreenMode _newMode)
	{
		switch (_newMode)
		{
		case WINDOWED:
			std::cout << "Starting in Windowed Mode!\n";
			if (IsWindowFullscreen())
			{
				ToggleFullscreen();//Not sure if I need to call this but just in case
			}		
			SetConfigFlags(FLAG_MSAA_4X_HINT);
			break;
		case BORDERLESS:
			std::cout << "Starting in Borderless Windowed Mode!\n";
			ToggleBorderlessWindowed();
			break;
		case FULLSCREEN:
			std::cout << "Starting in Fullscreen Mode!\n";
			ToggleFullscreen();
			break;
		default:
			break;
		}
		screenMode = _newMode;
	}

	ScreenMode GetScreenMode()
	{
		return screenMode;
	}

	void SetScreenHeight(int _newHeight)
	{
		_screenHeight = _newHeight;
	}

	int GetScreenHeight()
	{
		return _screenHeight;
	}

	void SetScreenWidth(int _newWidth)
	{
		_screenWidth = _newWidth;
	}

	int GetScreenWidth()
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
	bool LoadSettings();

	SoundSettings _soundSettings;
	MatchSettings _matchSettings;
	InputSettings _inputSettings;
	VideoSettings _videoSettings;

private:
	std::string fileName = "./resources/GameSettings.json";
};

