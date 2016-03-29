#ifndef main_H_
#define main_H_

#include "gameplay.h"


using namespace gameplay;

class Table;
class Opponent;
class Player;
class MyCamera;
struct lua_State;

//class Overmap;

class Screen;

// screens
class StartScreen;
class OverMap;
class FoosballScreen;
class MainMenu;
class AboutScreen;
class DifficultySet;

// global, mmkay
lua_State *L;
int gameStatus;

int panic(lua_State* state){
	std::string error = lua_tostring(state, -1);
	GP_ERROR(error.c_str());
	return 0;
}


/**
 * Main game class.
 */
class main: public Game
{
public:
	enum STATUS{
		TITLE_SCREEN,
		OVERMAP,
		MAIN_MENU,
		GAME_RUNNING,
		ABOUT_SCREEN,
		DIFFICULTY_SET,
		PAUSED
	};

	Screen* currentScreen = NULL;
	Screen* creditsScreen = NULL;
	Screen* removeScreen = NULL;

	Table* table;
	Opponent* opponent;
	Player* player;

	//std::vector<Node*> p1Handles;
	//std::vector<Node*> p2Handles;
	//std::vector<Node*> poles;
	//Vector2 screenTouchPos;
	//Vector2 worldTouchPos;
    /**
     * Constructor.
     */
    main();

	

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	FoosballScreen* StartGame();
	void Wait(float milliseconds);
	void ResetTimer(){ eventTimer = 0; }
	void WaitForTime(float milliseconds);
//	void StartRound();
//	void SwoopLogo(int milliseconds);
	void PlayMusic(std::string title);
	void PlaySound(std::string file);
	StartScreen* ShowLogo();
	//void ShowClickToStart();
	OverMap* ShowOverMap();
	MainMenu* ShowMainMenu();
	AboutScreen* ShowAboutScreen();
	DifficultySet* ShowDifficultySet();

	void SaveProgress(int moveAheadLevel, int currentLevel);




	//Node* getInterceptingBar();
	//Node* getDefendingBar(std::vector<Node*> &characterBars);
	//Node* getClosestMan(Node* bar);
	//Vector3 getDifference(Node* man);
	std::string to_string(int number);



protected:
	//effects for "buildMaterial"
	
    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

	//float ANG_VELOCITY_LOCK = 0.5f;

	//Node* ballNode;
    /**F
     * Draws the scene each frame.
     */
  //  bool drawScene(Node* node);
//	bool drawShadow(Node* node);

 //   Scene* _scene;
	

	
//    bool _wireframe;


	//Vector2 veloctiy;
//	FrameBuffer* defaultFB;
//	FrameBuffer* shadowFB;
//	RenderTarget* renderTarget;
//	DepthStencilTarget* depthTarget;
//	Effect* depthEffect;
//	Material* shadowMat;



	AudioSource* currentMusic = NULL;



	

//	Overmap* overmap = NULL;
	
	int progress;

//	float timer;
	float eventTimer;
	float wakeUpTime;

//	float tTarget;
	void checkForLuaWakeUp(const float& elapsedTime);

	
};

#endif
