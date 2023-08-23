#ifndef MINIMAPRENDERER_H
#define MINIMAPRENDERER_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "../Utils/Math.h"

#include "../../engine.h"

#include <cmath>
#include <SFML/Graphics.hpp>

class Engine;


class MinimapRenderer
{
    public:

        MinimapRenderer(Engine& lEngine, Player *lPlayer);


        void drawMinimap();

        void update();

        void drawNodeById(int id);

        void drawSegById(int id);

        sf::Sprite* getMinimap();

        int getMinimapSize();

        

    private:
        Engine& engine;
        Player* player;
        
        LevelData::LevelData level;
        vertexs_bounds_t vertexsBounds;
        vector<LevelData::Vertex> vertexs;
        vector<LevelData::Linedef> lines;
        vector<LevelData::Node> nodes;
        vector<LevelData::Seg> segs;
        

        sf::Color vertexColor = sf::Color::Cyan;
        sf::Color lineColor = sf::Color::Yellow;
        sf::Color borderColor = sf::Color::White;
        sf::Color backgroundColor = sf::Color::Black;
        sf::Color playerColor = sf::Color::Magenta;

        uint8_t* pixels;

        int minimapSize;
        int width;
        int height;
        int offset;
        int border;
        int fov;
        int hFov;
        int fovDistance;
        sf::Texture texture;
        sf::Sprite sprite;

        void drawSeg(LevelData::Seg seg);

        void clearDrawingBoard();

        void setupDrawingBoard();

        void drawNode(LevelData::Node node);

        void drawPlayer();

        void drawPlayerFov();


        void drawSimpleSquare(int x0, int y0, int x1, int y1, int thickness, sf::Color color);

        void drawSimpleLine(int x0, int y0, int x1, int y1, int thickness, sf::Color color);

        void drawSimpleCircle(int centerX, int centerY, int radius, sf::Color color);

        void drawOuter();

        void drawVertexs();

        void drawLines();

        void drawLine(LevelData::Linedef* line);

        void drawVertex(LevelData::Vertex* vertex);

        void drawCross(int x, int y, int size);

        void drawPixel(int x, int y, sf::Color color);

        int minimapRemap(int value, bool isVertical);
        
        int remap(int x, int x_min, int x_max, int y_min, int y_max);

};



#endif