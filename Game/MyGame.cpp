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
    int desiredWidth = 1280;
    int desiredHeight = 720;
    myGame.Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow, desiredWidth, desiredHeight);
    return 0;
}