#include "ScreenManager.h"

ScreenManager::ScreenManager(std::shared_ptr<GameSettings> _newSettings)
{
	_currentScreen = LOGO;
	_settings = _newSettings;
}

ScreenManager::~ScreenManager()
{
}

Screen ScreenManager::ChangeScreen(Screen _newScreen)
{ 
	/*Need to do logic to make sure that we cannot go to certain screens from other screens*/
	if(_currentScreen != _newScreen)
	{
		switch (_newScreen)
		{
		case LOGO:
			if (_currentScreen == MAINMENU)
			{
				return LOGO;
			}
			break;
		case MAINMENU:
			if (_currentScreen != CHARACTERSELECT)
			{
				return MAINMENU;
			}
			break;
		case SETTINGS:
			if (_currentScreen != ONLINE)
			{
				return SETTINGS;
			}
			break;
		case ONLINE:
			return ONLINE;
			break;
		case CHARACTERSELECT:
			return CHARACTERSELECT;
			break;
		default:
			break;
		}
	}
	else
	{
		return _currentScreen;
	}
}

void ScreenManager::DrawScreen()
{
	switch (_currentScreen)
	{
	case LOGO:
		DrawLogo();
		break;
	case MAINMENU:
		DrawMainMenu();
		break;
	case SETTINGS:
		DrawSettings();
		break;
	case ONLINE:
		DrawOnline();
		break;
	case CHARACTERSELECT:
		DrawCharacterSelect();
		break;
	case MATCH:
		DrawMatchScreen();
	default:
		DrawLogo();
		break;
	}
}

void ScreenManager::DrawLogo()
{
	DrawText("Insert Logo Here", ((GetScreenWidth() - 36) / 2), ((GetScreenHeight()- 36) / 2), 36, BLACK);

	if (GetKeyPressed() != 0) //Check if any keyboard key was pressed and go to the main menu.
	{
		_currentScreen = ChangeScreen(MAINMENU);
	}

	if (GetGamepadButtonPressed() != 0)
	{
		_currentScreen = ChangeScreen(MAINMENU);
	}
}

void ScreenManager::DrawMainMenu()
{
	DrawText("Main Menu Screen", ((GetScreenWidth() - 36) / 2), ((GetScreenHeight() - 36) / 2), 36, BLACK);
}

void ScreenManager::DrawSettings()
{
	DrawText("Main Menu Screen", ((GetScreenWidth() - 36) / 2), ((GetScreenHeight() - 36) / 2), 36, BLACK);
}

void ScreenManager::DrawOnline()
{
	DrawText("Main Menu Screen", ((GetScreenWidth() - 36) / 2), ((GetScreenHeight() - 36) / 2), 36, BLACK);
}

void ScreenManager::DrawCharacterSelect()
{
	DrawText("Main Menu Screen", ((GetScreenWidth() - 36) / 2), ((GetScreenHeight() - 36) / 2), 36, BLACK);
}

void ScreenManager::DrawMatchScreen()
{

}
