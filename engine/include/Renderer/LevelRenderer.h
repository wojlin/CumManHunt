#ifndef LEVELRENDERER_H
#define LEVELRENDERER_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "../Utils/Math.h"
#include "LevelBuild.h"

#include "../../engine.h"

#include <cmath>
#include <SFML/Graphics.hpp>
#include <random>

#include "SegmentHandler.h"

class Engine;

#include "../Utils/Common.h"


class LevelRenderer
{
    public:

        LevelRenderer(Engine& lEngine, Player *lPlayer);    

        void drawLevel();

        void drawSegmentsById(vector<int>* segmentsIds);

        void drawSolidWall(segmentDrawData segment);

        void drawPortalWall(segmentDrawData segment);

        void drawData(vector<segmentDrawData>* drawData);

        void update();

        void clearDrawingBoard();

        void setupDrawingBoard();

        sf::Sprite* getLevel();
         

    private:
        Engine& engine;
        Player* player;

        int width;
        int height;

        uint8_t* pixels;
        sf::Texture texture;
        sf::Sprite sprite;

        LevelData::LevelData* level;

        vector<LevelData::Vertex> vertexs;
        vector<LevelData::Linedef> lines;
        vector<LevelData::Sidedef> sides;
        vector<LevelData::Node> nodes;
        vector<LevelData::Seg> segs;
        vector<LevelData::Sector> sectors;

        vector<float> xToAngleTable;

        vector<int> lowerClip;
        vector<int> upperClip;

        sf::Color backgroundColor = sf::Color::Black;

        float MAX_SEG_SCALE = 64.0;
        float MIN_SEG_SCALE = 0.00390625;

        int angleToX(float angle);

        int remapValue(int value, int oldMin, int oldMax, int newMin, int newMax);
        
        void drawVerticalLine(int posX, int bottomOffset, int topOffset, string textureName, int lightLevel);

        float scaleFromGlobalAngle(int x, float rwNormalAngle, float rwDistance);

        vector<float> xtoAngle();

        void drawPixel(int x, int y, sf::Color color);

        sf::Color getRandomColor(string textureName, int lightLevel);

        size_t hashString(const std::string& str);

};



#endif