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
class LevelRenderer;
class BSP;

class Engine
{
    public:

        Engine();

        vertexs_bounds_t getVertexsBounds();

        level_bounds_t getLevelBounds();

        LevelData::LevelData* getLevelData();

        int* getMinimapFovDistancePercent();

        int* getMinimapBorderPercentage();

        int* getMinimapContentPercentageOffset();

        int* getMinimapSize();

        int* getWindowWidth();

        int* getWindowHeight();

        int* getPlayerFOV();

        int* getPlayerHalfFOV();

        float* getDeltaSeconds();

        int* getPlayerSpeed();

        int* getPlayerRotationSpeed();

        int* getScreenDist();

        sf::RenderWindow* getWindow();

        void run();
    
    private:

        string TITLE = "CumManHunt";

        int WINDOW_WIDTH = 1800; // in pixels
        int WINDOW_HEIGHT = 1000; // in pixels
        int FRAMERATE_LIMIT = 60;  // frames per second
        bool VSYNC = false;

        int MINIMAP_SIZE = 50; // in percents
        int MINIMAP_CONTENT_PERCENTAGE_OFFSET = 5; // in percents
        int MINIMAP_BORFER_PERCENTAGE = 1; // in percents

        int FOV = 90;
        int H_FOV = FOV / 2;

        int MINIMAP_FOV_DISTANCE_PERCENT = 50;

        int PLAYER_SPEED = 300;
        int PLAYER_ROTATION_SPEED = 1;

        int H_WIDTH = WINDOW_WIDTH / 2;
        int H_HEIGHT = WINDOW_HEIGHT / 2;

        int SCREEN_DIST = (int) ((float) H_WIDTH / (float) tan(radians(H_FOV)));

        LevelData::LevelData level;
        level_bounds_t levelBounds;
        vertexs_bounds_t vertexsBounds;

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