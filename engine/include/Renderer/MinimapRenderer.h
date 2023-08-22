#ifndef MINIMAPRENDERER_H
#define MINIMAPRENDERER_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "../Utils/Math.h"
#include "LevelBuild.h"

#include <cmath>
#include <SFML/Graphics.hpp>

namespace MinimapRenderer
{

    class MinimapRenderer
    {
        public:

            MinimapRenderer(int lWidth, int lHeight, int lSize, int lOffset, int lBorder, int lFov, int lFovDistance, unique_ptr<LevelData::LevelData> *lLevel, LevelBuild::LevelBuild *lLevelBuild, vector<Player> *lPlayers)
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
                players = lPlayers;
                fov = lFov;
                fovDistance = static_cast<int>(((float) shortestLine * ( (float) lFovDistance / 100.0))) * 2;
                hFov = static_cast<int>((float) fov / 2.0);
                offset = static_cast<int>(((float) shortestLine * ( (float) lOffset / 100.0)));
                border = static_cast<int>(((float) shortestLine * ( (float) lBorder / 100.0)));

                vertexsBounds = levelBuild->getVertexsBounds();
                vertexs = level->get()->getVertexs();
                lines = level->get()->getLinedefs();

                setupDrawingBoard();  
                
            }


            void drawMinimap()
            {
                clearDrawingBoard();
                drawVertexs();  
                drawLines(); 
                drawOuter();
                drawNodes();

                drawPlayer(1);
                drawPlayer(2);
                drawPlayer(3);
                drawPlayer(4);

                drawPlayerFov(1);

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
            vector<Player>* players;

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


            void clearDrawingBoard()
            {
                for(int i = 0; i < width*height*4; i += 4) {
                    pixels[i] = backgroundColor.r;
                    pixels[i+1] = backgroundColor.g;
                    pixels[i+2] = backgroundColor.b;
                    pixels[i+3] = backgroundColor.a;
                }
            }

            void setupDrawingBoard()
            {
                pixels = new uint8_t[width*height*4];
                texture.create(width, height); 

                for(int i = 0; i < width*height*4; i += 4) {
                    pixels[i] = backgroundColor.r;
                    pixels[i+1] = backgroundColor.g;
                    pixels[i+2] = backgroundColor.b;
                    pixels[i+3] = backgroundColor.a;
                }

                texture.update(pixels);
                sprite.setTexture(texture);
            }

            void drawNodes()
            {
                vector<LevelData::Node> nodes = level->get()->getNodes();
                drawNode(nodes[nodes.size() - 1]);
                /*
                for(int i = 0; i < nodes.size(); i++)
                {

                    drawNode(nodes[i]);
                        
                }
                */
            }

            void drawNode(LevelData::Node node)
            {

                int x0;
                int x1;
                int y0;
                int y1;

                sf::Color frontColor = sf::Color::Green;
                sf::Color backColor = sf::Color::Red;
                sf::Color partitionColor = sf::Color::Blue;

                int thickness = 3;

                x0 = minimapRemap(node.leftBoxLeft, false);
                y0 = minimapRemap(node.leftBoxBottom, true);
                x1 = minimapRemap(node.leftBoxRight, false);
                y1 = minimapRemap(node.leftBoxTop, true);         

                drawSimpleSquare(x0, y0, x1, y1, thickness, frontColor);

                x0 = minimapRemap(node.rightBoxLeft, false);
                y0 = minimapRemap(node.rightBoxBottom, true);
                x1 = minimapRemap(node.rightBoxRight, false);
                y1 = minimapRemap(node.rightBoxTop, true);

                drawSimpleSquare(x0, y0, x1, y1, thickness, backColor);


                x0 = minimapRemap(node.xPartition, false);
                y0 = minimapRemap(node.yPartition, true);
                x1 = minimapRemap(node.xPartition + node.xPartitionDiff, false);
                y1 = minimapRemap(node.yPartition + node.yPartitionDiff, true);

                drawSimpleLine(x0, y0, x1, y1, thickness, partitionColor);

            }

            void drawPlayer(int number)
            {
                for(int i = 0; i < players->size(); i++)
                {
                   if((*players)[i].getNumber() == number)
                   {
                        int x = remap((*players)[i].getPosX(), vertexsBounds->xPosMin, vertexsBounds->xPosMax, 0, width);
                        int y = remap((*players)[i].getPosY(), vertexsBounds->yPosMin, vertexsBounds->yPosMax, 0, height);
                        drawSimpleCircle(x, y, 3, playerColor);
                   }
                }
            }

            void drawPlayerFov(int number)
            {
                int thickness = 2;
                sf::Color color = sf::Color::White;

                for(int i = 0; i < players->size(); i++)
                {
                   if((*players)[i].getNumber() == number)
                   {
                        int x = remap((*players)[i].getPosX(), vertexsBounds->xPosMin, vertexsBounds->xPosMax, 0, width);
                        int y = remap((*players)[i].getPosY(), vertexsBounds->yPosMin, vertexsBounds->yPosMax, 0, height);
                        int angle = -(*players)[i].getAngle() + 90;

                        int plusAngle = (float) angle + (float) hFov;
                        int minusAngle = (float) angle - (float) hFov;
                        
                        float minusRads = radians(minusAngle);
                        float plusRads = radians(plusAngle);

                        float sin_a1 = std::sin(minusRads);
                        float cos_a1 = std::cos(minusRads);
                        float sin_a2 = std::sin(plusRads);
                        float cos_a2 = std::cos(plusRads);

                        int x1 = minimapRemap(static_cast<int>((float) (*players)[i].getPosX() + (float) fovDistance * (float) sin_a1), false);
                        int y1 = minimapRemap(static_cast<int>((float) (*players)[i].getPosY() + (float) fovDistance * (float) cos_a1), true);
                        int x2 = minimapRemap(static_cast<int>((float) (*players)[i].getPosX() + (float) fovDistance * (float) sin_a2), false);
                        int y2 = minimapRemap(static_cast<int>((float) (*players)[i].getPosY() + (float) fovDistance * (float) cos_a2), true);

                        drawSimpleLine(x, y, x1, y1, thickness, color);
                        drawSimpleLine(x, y, x2, y2, thickness, color);
                   }
                }
            }


            void drawSimpleSquare(int x0, int y0, int x1, int y1, int thickness, sf::Color color)
            {
                drawSimpleLine(x0, y0, x1, y0, thickness, color);
                drawSimpleLine(x0, y1, x1, y1, thickness, color);
                drawSimpleLine(x1, y0, x1, y1, thickness, color);
                drawSimpleLine(x0, y0, x0, y1, thickness, color);
            }

            void drawSimpleLine(int x0, int y0, int x1, int y1, int thickness, sf::Color color)
            {

                 int dx = std::abs(x1 - x0);
                int dy = std::abs(y1 - y0);
                int steps = std::max(dx, dy); // Use the longer dimension for steps

                float xIncrement = static_cast<float>(x1 - x0) / steps;
                float yIncrement = static_cast<float>(y1 - y0) / steps;

                for (int t = 0; t < thickness; ++t)
                {
                    float x = static_cast<float>(x0);
                    float y = static_cast<float>(y0);

                    for (int i = 0; i <= steps; ++i)
                    {
                        drawPixel(static_cast<int>(x + 0.5), static_cast<int>(y + 0.5), color); // Round and draw pixel

                        x += xIncrement;
                        y += yIncrement;
                    }

                    // Offset the starting and ending points for the next line
                    x0 -= static_cast<int>(xIncrement);
                    y0 -= static_cast<int>(yIncrement);
                    x1 -= static_cast<int>(xIncrement);
                    y1 -= static_cast<int>(yIncrement);
                }

            }

            void drawSimpleCircle(int centerX, int centerY, int radius, sf::Color color) {
                for (int y = -radius; y <= radius; y++) {
                    for (int x = -radius; x <= radius; x++) {
                        if (x * x + y * y <= radius * radius) {
                            drawPixel(centerX + x, centerY + y, color);
                        }
                    }
                }
            }

            void drawOuter()
            {
                for(int i = 0; i < border; i++)
                {
                    for(int x = 0; x < width; x++)
                    {
                        drawPixel(x, 0 + i, borderColor);
                        drawPixel(x, height - i, borderColor);
                    }
                    for(int y = 0; y < height; y++)
                    {
                        drawPixel(0 + i, y, borderColor);
                        drawPixel(width - i, y, borderColor);
                    }
                }
            }

            void drawVertexs()
            {            
                for(LevelData::Vertex vertex: vertexs)
                {
                    drawVertex(&(vertex));
                }
            }

            void drawLines()
            {   
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

                int dx = std::abs(x1 - x0);
                int dy = std::abs(y1 - y0);
                int sx = (x0 < x1) ? 1 : -1;
                int sy = (y0 < y1) ? 1 : -1;
                int err = dx - dy;

                for (int t = 0; t < thickness; ++t) {
                    int x = x0;
                    int y = y0;

                    while (true) {
                        drawPixel(x, y, lineColor);

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

                drawCross(x_remapped, y_remapped, 3);
            }

            void drawCross(int x, int y, int size)
            {   
                 for (int i = -size; i <= size; ++i) {
                    drawPixel(x + i, y, vertexColor);
                    drawPixel(x, y + i, vertexColor);
                }      
            }

            void drawPixel(int x, int y, sf::Color color)
            {
                
                int place = (width*y*4)+(x*4);
                if(place >= width*height*4)
                {
                    return;
                }

                pixels[place] = color.r;
                pixels[place + 1] = color.g;
                pixels[place + 2] = color.b;
                pixels[place + 3] = color.a; 

            }

            int minimapRemap(int value, bool isVertical)
            {
                if(isVertical == true)
                {
                    return remap(value, vertexsBounds->yPosMin, vertexsBounds->yPosMax, 0, height);
                }
                else
                {
                    return remap(value, vertexsBounds->xPosMin, vertexsBounds->xPosMax, 0, width);
                }
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