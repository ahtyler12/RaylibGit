#include "raygui.h"
#include "GameSettings.h"



enum Screen
{
	LOGO = 0,
	MAINMENU,
	SETTINGS,
	ONLINE,
	CHARACTERSELECT,
	MATCH
};
class ScreenManager
{
public:

	ScreenManager(std::shared_ptr<GameSettings> _newSettings); //Takes in a settings pointer so that we have access to the game settings for menu handling
	~ScreenManager();
	
	Screen _currentScreen;
	std::shared_ptr<GameSettings> _settings;


	Screen ChangeScreen(Screen _newScreen);
	void DrawScreen();
	void DrawLogo();
	void DrawMainMenu();
	void DrawSettings();
	void DrawOnline();
	void DrawCharacterSelect();
	void DrawMatchScreen();

};