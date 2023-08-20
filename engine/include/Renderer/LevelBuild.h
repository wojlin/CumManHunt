#ifndef LEVELBUILD_H
#define LEVELBUILD_H

#include "../GameData/LevelData.h"



namespace LevelBuild
{
    struct level_bounds_t
    {
        int xPos;
        int yPos;
        int width;
        int height;
    };

    struct vertexs_bounds_t
    {
        int xPosMin;
        int xPosMax;
        int yPosMin;
        int yPosMax;
    };

    class LevelBuild
    {
        public:

            LevelBuild(unique_ptr<LevelData::LevelData> *lLevel)
            {
                level = lLevel;
                levelBounds = calculateLevelBounds();
                vertexsBounds = calculateVertexsBounds();
            }

            level_bounds_t* getLevelBounds()
            {
                return &(levelBounds);
            }

            vertexs_bounds_t* getVertexsBounds()
            {
                return &(vertexsBounds);
            }

            

        private:
            unique_ptr<LevelData::LevelData>* level;
            level_bounds_t levelBounds;
            vertexs_bounds_t vertexsBounds;

            level_bounds_t calculateLevelBounds()
            {
                vector<LevelData::Vertex> vertexs = level->get()->getVertexs();
                int lowestX = 0;
                int highestX = 0;
                int lowestY = 0;
                int highestY = 0;
                for(LevelData::Vertex vertex: vertexs)
                {
                    if(vertex.x > highestX)
                    {
                        highestX = vertex.x;
                    }
                    if(vertex.x < lowestX)
                    {
                        lowestX = vertex.x;
                    }
                    if(vertex.y > highestY)
                    {
                        highestY = vertex.y;
                    }
                    if(vertex.y < lowestY)
                    {
                        lowestY = vertex.y;
                    }
                }

                level_bounds_t bounds;
                bounds.xPos = lowestX;
                bounds.yPos = highestY;
                bounds.height = highestY - lowestY;
                bounds.width = highestX - lowestX;
                return bounds;
            }


            vertexs_bounds_t calculateVertexsBounds()
            {
                vector<LevelData::Vertex> vertexs = level->get()->getVertexs();
                int lowestX = 0;
                int highestX = 0;
                int lowestY = 0;
                int highestY = 0;
                for(LevelData::Vertex vertex: vertexs)
                {
                    if(vertex.x > highestX)
                    {
                        highestX = vertex.x;
                    }
                    if(vertex.x < lowestX)
                    {
                        lowestX = vertex.x;
                    }
                    if(vertex.y > highestY)
                    {
                        highestY = vertex.y;
                    }
                    if(vertex.y < lowestY)
                    {
                        lowestY = vertex.y;
                    }
                }

                vertexs_bounds_t bounds;
                bounds.xPosMin= lowestX;
                bounds.yPosMax = highestY;
                bounds.xPosMax = highestX;
                bounds.yPosMin = lowestY;
                return bounds;
            }

    };

}

#endif