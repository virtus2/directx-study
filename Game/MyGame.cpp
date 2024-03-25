#include "Game.h"
#pragma comment(lib, "Engine.lib")
class MyGame : public Engine::Game
{
public:
    MyGame() {}
    ~MyGame() {}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    MyGame myGame;
    myGame.Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}