#ifndef BSP_H
#define BSP_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "LevelBuild.h"
#include <utility>

class BSP
{
    public:

        BSP(LevelData::LevelData *lLevel, Player *lPlayer, int lFov)
        {
            player = lPlayer;
            level = lLevel;
            fov = lFov;
            hFov = (int) ((float) fov / 2.0);
            nodes = level->getNodes();
            subsectors = level->getSubSectors();
            segs = level->getSegs();
            vertexs = level->getVertexs();
            traverse = true;
        }

        void renderBsp()
        {
            nodesTree.clear();
            segsTree.clear();
            traverse = true;
            renderBspNode(nodes.size()-1);
        }     

        vector<int>* getNodesTree()
        {
            return &nodesTree;
        }

        vector<int>* getSegsTree()
        {
            return &segsTree;
        }
        
    private:
        int fov;
        int hFov;
        Player* player;
        LevelData::LevelData* level;
        vector<LevelData::Node> nodes;
        vector<LevelData::Vertex> vertexs;
        vector<LevelData::SSector> subsectors;
        vector<LevelData::Seg> segs;

        vector<int> segsTree;
        vector<int> nodesTree;

        bool traverse = true;

        int SUB_SECTOR_IDENTIFIER = 32768;

        bool addSegmentToFOV(pair<int, int> vertex1, pair<int, int> vertex2)
        {
            float angle1 = pointToAngle(vertex1);
            float angle2 = pointToAngle(vertex2);
            float span = norm(angle1 - angle2);
            if(span >= 180)
            {
                return false;
            }else
            {
                angle1 -= player->getAngle();
                angle2 -= player->getAngle();

                float span1 = norm(angle1 + hFov);
                if(span1 > fov)
                {
                    if(span1 >= span + fov)
                    {
                        return false;
                    }
                    angle1 = hFov;
                }

                float span2 = norm(hFov - angle2);
                if(span2 > fov)
                {
                    if(span2 >= span + fov)
                    {
                        return false;
                    }
                    angle2 = -hFov;
                }

                return true;
            }
        }

        void renderSubSector(int subSectorId)
        {
            LevelData::SSector subSector = subsectors[subSectorId];
            
            for(int i = 0; i < subSector.segCount; i++)
            {   
                LevelData::Seg seg = segs[subSector.firistSegNumber + i];
                int v1 = seg.startVertex;
                int v2 = seg.endVertex;
                LevelData::Vertex vertex1 = vertexs[v1];
                LevelData::Vertex vertex2 = vertexs[v2];

                if(addSegmentToFOV(pair<int,int>(vertex1.x, vertex1.y), pair<int,int>(vertex2.x, vertex2.y)))
                {
                    segsTree.push_back(subSector.firistSegNumber + i);
                }
                
            }
            
        }

        bool checkBbox(int boxBottom, int boxTop, int boxLeft, int boxRight)
        {
            std::pair<int, int> a = std::pair(boxLeft, boxBottom);
            std::pair<int, int> b = std::pair(boxLeft, boxTop);
            std::pair<int, int> c = std::pair(boxRight, boxTop);
            std::pair<int, int> d = std::pair(boxRight, boxBottom);
            
            vector<vector<std::pair<int, int>>> bboxSides;

            int px = player->getPosX();
            int py = player->getPosY();

            if(px < boxLeft)
            {
                if(py > boxTop)
                {
                    vector<std::pair<int, int>> v1;
                    v1.push_back(b);
                    v1.push_back(a);
                    bboxSides.push_back(v1);
                    vector<std::pair<int, int>> v2;
                    v2.push_back(c);
                    v2.push_back(b);
                    bboxSides.push_back(v2);
                    // (b, a), (c, b)
                }
                else if(py < boxBottom)
                {
                    vector<std::pair<int, int>> v1;
                    v1.push_back(b);
                    v1.push_back(a);
                    bboxSides.push_back(v1);
                    vector<std::pair<int, int>> v2;
                    v2.push_back(a);
                    v2.push_back(d);
                    bboxSides.push_back(v2);
                    // (b, a), (a, b)
                }else
                {   
                    vector<std::pair<int, int>> v1;
                    v1.push_back(b);
                    v1.push_back(a);
                    bboxSides.push_back(v1);
                    // (b, a)
                }
            }
            else if(px > boxRight)
            {
                if(py > boxTop)
                {
                    vector<std::pair<int, int>> v1;
                    v1.push_back(c);
                    v1.push_back(b);
                    bboxSides.push_back(v1);
                    vector<std::pair<int, int>> v2;
                    v2.push_back(d);
                    v2.push_back(c);
                    bboxSides.push_back(v2);
                    // (c, b), (d, c)
                }
                else if(py < boxBottom)
                {
                    vector<std::pair<int, int>> v1;
                    v1.push_back(a);
                    v1.push_back(d);
                    bboxSides.push_back(v1);
                    vector<std::pair<int, int>> v2;
                    v2.push_back(d);
                    v2.push_back(c);
                    bboxSides.push_back(v2);
                    // (a, d), (d, c)
                }else
                {
                    vector<std::pair<int, int>> v1;
                    v1.push_back(d);
                    v1.push_back(c);
                    bboxSides.push_back(v1);
                    // (d, c)
                }
            }
            else
            {
                if(py > boxTop)
                {
                    vector<std::pair<int, int>> v1;
                    v1.push_back(c);
                    v1.push_back(b);
                    bboxSides.push_back(v1);
                    // (c, b)
                }
                else if(py < boxBottom)
                {
                    vector<std::pair<int, int>> v1;
                    v1.push_back(a);
                    v1.push_back(d);
                    bboxSides.push_back(v1);
                    // (a, d)
                }else
                {
                    return true;
                }
            }

            for(int i = 0; i < bboxSides.size(); i++)
            {
                std::pair<int, int> v1 = bboxSides[i][0];
                std::pair<int, int> v2 = bboxSides[i][1];
                
                float angle1 = pointToAngle(v1);
                float angle2 = pointToAngle(v2);
                float span = norm(angle1 - angle2);
                angle1 -= player->getAngle() + 270;
                float span1 = norm(angle1 + hFov);
                if(span1 > fov)
                {
                    if(span1 >= span + (float) fov * 2.0)
                    {
                        continue;
                    }
                    return true;
                }
            }     
            //cout << "bounding box:" << endl;
            //cout << "p=(" << a.first << ","<<a.second << ")" << endl;
            //cout << "p=(" << b.first << ","<<b.second << ")" << endl;
            //cout << "p=(" << c.first << ","<<c.second << ")" << endl;
            //cout << "p=(" << d.first << ","<<d.second << ")" << endl;
            //cout << "is outside of player angle of view:"<< endl;
            //cout << "player=(" << player->getPosX() << "," << player->getPosY() << ")" << endl;
            //cout << "angle=(" << player->getAngle() << ")" << endl;
            return false;
        }

        float pointToAngle(std::pair<int, int> vertex)
        {
            //cout << "######" << endl;
            float deltaX = (float) vertex.first - (float) player->getPosX();
            float deltaY = (float) vertex.second - (float) player->getPosY();
            float radian_angle = atan2(deltaY, deltaX);
            float degree_angle = radian_angle * (180.0 / M_PI);
            //cout << "deltaX " << deltaX << endl;
            //cout << "deltaY " << deltaY << endl;
            //cout << "radian angle " << radian_angle << endl;
            //cout << "degree " << degree_angle << endl;
            return degree_angle;
        }

        float norm(float angle)
        {   
            angle = fmod(angle, 360.0);

            if(angle < 0)
            {
                return angle + 360;
            }else
            {
                return angle;
            }
        }

        void renderBspNode(int nodeId)
        { 
            if(traverse)
            {
                //cout << "traversing node: " << nodeId  << endl;
                if(nodeId < 0)
                {
                    //cout << "found subsector!" << endl;
                    int subSectorId = nodeId + SUB_SECTOR_IDENTIFIER;
                    //cout << nodeId << " " <<subSectorId << endl;
                    renderSubSector(subSectorId);
                    return;
                }

                nodesTree.push_back(nodeId);
                
                

                LevelData::Node* node = &nodes[nodeId];

                // front = right, back = left

                if(isOnBackSide(node) == true)
                {
                    //cout << "player is on back side!" << endl;
                    //cout << nodeId << endl;
                    renderBspNode(node->leftChild);
                    if(checkBbox(node->rightBoxBottom, node-> rightBoxTop, node->rightBoxLeft, node->rightBoxRight))
                    {
                        renderBspNode(node->rightChild);
                    }
                    
                    
                }
                else
                {
                    //cout << "player is on front side!" << endl;
                    //cout << nodeId << endl;
                    renderBspNode(node->rightChild);
                    
                    if(checkBbox(node->leftBoxBottom, node-> leftBoxTop, node->leftBoxLeft, node->leftBoxRight))
                    {
                        renderBspNode(node->leftChild);
                    }
                    
                }
            }       
            
        }

        bool isOnBackSide(LevelData::Node *node)
        {   
            int dx = player->getPosX() - node->xPartition;
            int dy = player->getPosY() - node->yPartition;
            return dx * node->yPartitionDiff - dy * node->xPartitionDiff <= 0;
        }

};


#endif