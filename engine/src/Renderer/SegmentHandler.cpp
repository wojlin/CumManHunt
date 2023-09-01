#include "../../include/Renderer/SegmentHandler.h"


SegmentHandler::SegmentHandler(Engine& lEngine, Player *lPlayer) : engine(lEngine)
{
    LevelData::LevelData* level;
    level = engine.getLevelData();
    nodes = level->getNodes();
    vertexs = level->getVertexs();
    subsectors = level->getSubSectors();
    segs = level->getSegs();
    linedefs = level->getLinedefs();
    sidedefs = level->getSidedefs();
}


void SegmentHandler::initScreenRange()
{
    screenRange.clear();
    for (int i = 0; i < (*engine.getWindowWidth()); ++i) 
    {
        screenRange.insert(i);
    }
    drawData.clear();
}

void SegmentHandler::clipSolidWalls(int x1, int x2)
{   
    if(!screenRange.empty())
    {   
        std::set<int> currentWall;
        if(x1 < x2)
        {
            for (int i = x1; i < x2; ++i) 
            {
                currentWall.insert(i);
            }
        }else
        {
            for (int i = x2; i < x1; ++i) 
            {
                currentWall.insert(i);
            }
        }

        std::set<int> intersection;
        
        
        for (const int &num : currentWall) {
            if (currentWall.count(num) > 0) {
                intersection.insert(num);
            }
        }

        if (!intersection.empty()) 
        {
            if (intersection.size() == currentWall.size()) 
            {
                segmentDrawData data;
                data.x1 = x1;
                data.x2 = x2 - 1;
                drawData.push_back(data);
            } 
            else 
            {
                std::vector<int> arr(intersection.begin(), intersection.end());
                std::sort(arr.begin(), arr.end());
                int x = arr[0];
                int x2 = arr.back();
                for (size_t i = 1; i < arr.size(); ++i) {
                    int x1 = arr[i - 1];
                    x2 = arr[i];
                    if (x2 - x1 > 1) {

                        segmentDrawData data;
                        data.x1 = x;
                        data.x2 = x1;
                        drawData.push_back(data);


                        x = x2;
                    }
                }

                segmentDrawData data;
                data.x1 = x;
                data.x2 = x2;
                drawData.push_back(data);
            }
            
            for (int x : intersection) {
                screenRange.erase(x);
            }
        }
    }
    else 
    {
        //BSP * bsp = *(engine.getBSP());
        //bsp->setTraverse(false);
    }
    
}

void SegmentHandler::classifySegment(LevelData::Seg segment, int x1, int x2, float rwAngle)
{
    if(x1 == x2)
    {
        return;
    }

    int linedefId = segment.lindefNumber;

    LevelData::Linedef linedef = linedefs[linedefId];

    bool twoSided = (linedef.flags >> 2) & 1;

    if(twoSided)
    {
        int frontSidedefId = linedef.frontSidedef;
        int backSidedefId = linedef.backSidedef;
        
    }else
    {
        int frontSidedefId = linedef.frontSidedef;
        clipSolidWalls(x1, x2);
        return;
    }
}
