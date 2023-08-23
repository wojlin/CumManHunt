#ifndef ENGINE_H
#define ENGINE_H

#include "include/GameData/GameData.h"

#include "include/GameData/LevelData.h"
#include "include/GameData/ColorMapData.h"
#include "include/GameData/PlayPalData.h"
#include "include/GameData/ResourcesData.h"
#include "include/GameData/EndoomData.h"
#include "include/GameData/AudioInfoData.h"
#include "include/GameData/SoundData.h"
#include "include/GameData/DemoData.h"

#include "include/Renderer/LevelBuild.h"
#include "include/Renderer/MinimapRenderer.h"
#include "include/Renderer/LevelRenderer.h"
#include "include/Renderer/BSP.h"

#include "include/Controller/Player.h"
#include "include/Controller/Input.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <chrono>


class Input;

class Engine
{
    public:

        Engine();

        float* getDeltaSeconds();

        int* getPlayerSpeed();

        int* getPlayerRotationSpeed();

        sf::RenderWindow* getWindow();

        void run();
    
    private:

        string TITLE = "CumManHunt";

        int WINDOW_WIDTH = 1800; // in pixels
        int WINDOW_HEIGHT = 1000; // in pixels
        int FRAMERATE_LIMIT = 60;  // frames per second
        bool VSYNC = false;

        int MINIMAP_SIZE = 100; // in percents
        int MINIMAP_CONTENT_PERCENTAGE_OFFSET = 5; // in percents
        int MINIMAP_BORFER_PERCENTAGE = 1; // in percents

        int FOV = 90;
        int H_FOV = FOV / 2;

        int MINIMAP_FOV_DISTANCE_PERCENT = 50;

        int PLAYER_SPEED = 300;
        int PLAYER_ROTATION_SPEED = 1;

        int H_WIDTH = WINDOW_WIDTH / 2;
        int H_HEIGHT = WINDOW_HEIGHT / 2;

        int SCREEN_DIST = H_WIDTH / tan(radians(H_FOV));

        float deltaSeconds;
        float fpsMeasure;

        sf::RenderWindow window;
        sf::Clock clock;

        sf::Time deltaTime;

        sf::Text fpsText;
        sf::Font fpsFont;

        void setupWindow();

        void setupFPS();

        void calculateFPS();

};

#endif