#ifndef BSP_H
#define BSP_H

#include "../GameData/LevelData.h"
#include "../Controller/Player.h"
#include "LevelBuild.h"

class BSP
{
    public:

        BSP(unique_ptr<LevelData::LevelData> *lLevel, Player *lPlayer)
        {
            player = lPlayer;
            level = lLevel;
            nodes = level->get()->getNodes();
            subsectors = level->get()->getSubSectors();
            segs = level->get()->getSegs();
        }

        void renderBsp()
        {
            renderBspNode(nodes.size()-1);
        }
        
    private:
        Player* player;
        unique_ptr<LevelData::LevelData>* level;
        vector<LevelData::Node> nodes;
        vector<LevelData::SSector> subsectors;
        vector<LevelData::Seg> segs;

        int SUB_SECTOR_IDENTIFIER = 32768;

        void renderSubSector(int subSectorId)
        {
            LevelData::SSector subSector = subsectors[subSectorId];
            vector<LevelData::Seg> segArray;
            
            for(int i = 0; i < subSector.segCount; i++)
            {   
                cout << subSector.firistSegNumber + i << endl;
                LevelData::Seg seg = segs[subSector.firistSegNumber + i];
                //segArray.push_back(seg);
            }
            //return segArray;
        }

        void renderBspNode(int nodeId)
        { 
            cout << "traversing node: " << nodeId  << endl;
            if(nodeId <= 0)
            {
                cout << "found subsector!" << endl;
                int subSectorId = nodeId + SUB_SECTOR_IDENTIFIER;
                cout << subSectorId << endl;
                renderSubSector(subSectorId);
                return;
                //vector<LevelData::Seg> segArray = renderSubSector(subSectorId);
                //return segArray;
            }

            LevelData::Node* node = &nodes[nodeId];

            if(isOnBackSide(node) == true)
            {
                cout << "player is on back side!" << endl;

                renderBspNode(node->leftChild);
                renderBspNode(node->rightChild);
            }
            else
            {
                cout << "player is on front side!" << endl;

                renderBspNode(node->rightChild);
                renderBspNode(node->leftChild);
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