#ifndef BSP_H
#define BSP_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "../Utils/Math.h"
#include "LevelBuild.h"
#include <utility>

#include "../../engine.h"

class Engine;

struct bspFov
{
    bool visible;
    int x1;
    int x2;
    int rw_angle1;
};

class BSP
{
    public:

        BSP(Engine& lEngine, Player *lPlayer);

        void renderBsp();

        vector<int>* getNodesTree();

        vector<int>* getSegsTree();

        void setTraverse(bool state);
        
    private:
        Engine& engine;

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

        bspFov addSegmentToFOV(pair<int, int> vertex1, pair<int, int> vertex2);

        void renderSubSector(int subSectorId);

        bool checkBbox(int boxBottom, int boxTop, int boxLeft, int boxRight);

        int angleToX(float angle);

        float pointToAngle(std::pair<int, int> vertex);

        float norm(float angle);

        void renderBspNode(int nodeId);

        bool isOnBackSide(LevelData::Node *node);

};


#endif