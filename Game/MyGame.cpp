#include "Game.h"

class MyGame : public Engine::Game
{
public:
	MyGame() {}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    MyGame myGame;

    return 0;
}