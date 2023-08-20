#ifndef MINIMAPRENDERER_H
#define MINIMAPRENDERER_H

#include "../GameData/LevelData.h"
#include "LevelBuild.h"

#include <SFML/Graphics.hpp>

namespace MinimapRenderer
{

    class MinimapRenderer
    {
        public:

            MinimapRenderer(int lWidth, int lHeight, int lSize, int lOffset, unique_ptr<LevelData::LevelData> *lLevel, LevelBuild::LevelBuild *lLevelBuild)
            {
                int shortestLine = lWidth;
                if(lHeight < shortestLine)
                {
                    shortestLine = lHeight;
                }

                int size = static_cast<int>(((float) shortestLine * ( (float) lSize / 100.0)));
                width = size;
                height = size;
                minimapSize = size;
                level = lLevel;
                levelBuild = lLevelBuild;
                offset = static_cast<int>(((float) shortestLine * ( (float) lOffset / 100.0)));

                vertexsBounds = levelBuild->getVertexsBounds();
                vertexs = level->get()->getVertexs();
                lines = level->get()->getLinedefs();

                setupDrawingBoard();  
                drawVertexs();  
                drawLines(); 
            }

            void setupDrawingBoard()
            {
                pixels = new uint8_t[width*height*4];
                texture.create(width, height); 

                for(int i = 0; i < width*height*4; i += 4) {
                    pixels[i] = 255;
                    pixels[i+1] = 0;
                    pixels[i+2] = 0;
                    pixels[i+3] = 255;
                }

                texture.update(pixels);
                sprite.setTexture(texture);
            }

            sf::Sprite* getMinimap()
            {
                return &(sprite);
            }

            int getMinimapSize()
            {
                return minimapSize;
            }

            

        private:
            unique_ptr<LevelData::LevelData>* level;
            LevelBuild::LevelBuild* levelBuild;
            LevelBuild::vertexs_bounds_t* vertexsBounds;
            vector<LevelData::Vertex> vertexs;
            vector<LevelData::Linedef> lines;

            uint8_t* pixels;

            int minimapSize;
            int width;
            int height;
            int offset;
            sf::Texture texture;
            sf::Sprite sprite;

            void drawVertexs()
            {            
                for(LevelData::Vertex vertex: vertexs)
                {
                    drawVertex(&(vertex));
                }
            }

            void drawLines()
            {   
                cout << "drawing lines" << endl;
                for(LevelData::Linedef line: lines)
                {
                    drawLine(&(line));
                }
            }

            void drawLine(LevelData::Linedef* line)
            {
                int start = line->startVertex;
                int end = line->endVertex;
                LevelData::Vertex vertexStart = vertexs[start];
                LevelData::Vertex vertexEnd = vertexs[end];

                int thickness = 1;

                int x0 = remap(vertexStart.x, vertexsBounds->xPosMin, vertexsBounds->xPosMax, 0, width);
                int x1 = remap(vertexEnd.x, vertexsBounds->xPosMin, vertexsBounds->xPosMax, 0, width);
                int y0 = remap(vertexStart.y, vertexsBounds->yPosMin, vertexsBounds->yPosMax, 0, width);
                int y1 = remap(vertexEnd.y, vertexsBounds->yPosMin, vertexsBounds->yPosMax, 0, width);
                
                cout << "drawing line between " << start << "  and " << end << endl;

                int dx = std::abs(x1 - x0);
                int dy = std::abs(y1 - y0);
                int sx = (x0 < x1) ? 1 : -1;
                int sy = (y0 < y1) ? 1 : -1;
                int err = dx - dy;

                for (int t = 0; t < thickness; ++t) {
                    int x = x0;
                    int y = y0;

                    while (true) {
                        drawPixel(x, y);

                        if (x == x1 && y == y1)
                            break;

                        int e2 = 2 * err;
                        if (e2 > -dy) {
                            err -= dy;
                            x += sx;
                        }
                        if (e2 < dx) {
                            err += dx;
                            y += sy;
                        }
                    }

                    // Offset points for the next thickness level
                    x0 -= sx;
                    y0 -= sy;
                    x1 -= sx;
                    y1 -= sy;
                }

            }



            void drawVertex(LevelData::Vertex* vertex)
            {
                int x = vertex->x;
                int y = vertex->y;
                
                int x_remapped = remap(x, vertexsBounds->xPosMin, vertexsBounds->xPosMax, 0, width);
                int y_remapped = remap(y, vertexsBounds->yPosMin, vertexsBounds->yPosMax, 0, height);

                drawCross(x_remapped, y_remapped, 1);
            }

            void drawCross(int x, int y, int size)
            {   
                 for (int i = -size; i <= size; ++i) {
                    drawPixel(x + i, y);
                    drawPixel(x, y + i);
                }      
            }

            void drawPixel(int x, int y)
            {
                
                int place = (width*y*4)+(x*4);
                if(place >= width*height*4)
                {
                    return;
                }
                cout << "vertex at x:" << x << "   y:" << y << endl;
                pixels[place] = 255;
                pixels[place + 1] = 255;
                pixels[place + 2] = 255;
                pixels[place + 3] = 255;

                texture.update(pixels);
                sprite.setTexture(texture);

            }
            
            int remap(int x, int x_min, int x_max, int y_min, int y_max) {
                x_min += offset;
                x_max -= offset;
                y_min += offset;
                y_max -= offset;
                double scaled = static_cast<double>(x - x_min) / (x_max - x_min) * (y_max - y_min) + y_min;
                return static_cast<int>(scaled + 0.5);
            }

    };

}

#endif