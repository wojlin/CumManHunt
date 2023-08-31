#ifndef LEVELRENDERER_H
#define LEVELRENDERER_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "../Utils/Math.h"
#include "LevelBuild.h"

#include "../../engine.h"

#include <cmath>
#include <SFML/Graphics.hpp>

#include "SegmentHandler.h"

class Engine;

#include "../Utils/Common.h"


class LevelRenderer
{
    public:

        LevelRenderer(Engine& lEngine, Player *lPlayer);    

        void drawLevel();

        void drawSegmentsById(vector<int>* segmentsIds);

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
        vector<LevelData::Node> nodes;
        vector<LevelData::Seg> segs;

        sf::Color backgroundColor = sf::Color::Black;

        int angleToX(float angle);
        
        void drawVerticalLine(int posX, int bottomOffset, int topOffset, sf::Color color);

        void drawPixel(int x, int y, sf::Color color);

};



#endif