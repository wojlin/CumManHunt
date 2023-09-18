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

        void drawScene();

        sf::Sprite* getScene();

        void drawSegmentsById(vector<int>* segmentsIds);

        void drawSolidWall(segmentDrawData segment);

        void drawPortalWall(segmentDrawData segment);

        void drawData(vector<segmentDrawData>* drawData);

        void update();

        void clearDrawingBoard();

        void setupDrawingBoard();

        
         

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

        vector<double> xToAngleTable;

        vector<double> lowerClip;
        vector<double> upperClip;

        sf::Color backgroundColor = sf::Color::Black;

        double MAX_SEG_SCALE = 64.0;
        double MIN_SEG_SCALE = 0.00001;//0.00390625;

        int angleToX(double angle);
        
        void drawVerticalLine(int posX, int bottomOffset, int topOffset, sf::Color color);

        double scaleFromGlobalAngle(int x, double rwNormalAngle, double rwDistance);

        vector<double> xtoAngle();

        void drawPixel(int x, int y, sf::Color color);

        sf::Color getRandomColor(string textureName, int lightLevel);

        size_t hashString(const std::string& str);

        void createClips();

        void drawFlat(ResourcesData::Image* texture, int lightLevel, int x, int y1, int y2, int worldZ);

        void drawFlatColumn(ResourcesData::Image* texture, int x, int y1, int y2, int lightLevel, int worldZ, int playerAngle, float playerX, float playerY);

        void drawWallColumn(ResourcesData::Image* wallTexture, float textureColumn, int i, int wy1, int wy2, float middleTexAlt, float invScale, int lightLevel);

};



#endif