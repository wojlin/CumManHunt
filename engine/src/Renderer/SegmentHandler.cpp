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
}

void SegmentHandler::clipSolidWalls(int x1, int x2)
{
    if(!screenRange.empty())
    {
        std::set<int> currentWall;
        for (int i = x1; i < x2; ++i) 
        {
            screenRange.insert(i);
        }

        std::set<int> intersection;
        for (int x : currentWall) {
            if (screenRange.count(x)) {
                intersection.insert(x);
            }
        }

        if (!intersection.empty()) 
        {
            if (intersection.size() == currentWall.size()) 
            {
                //draw_solid_wall_range(x_start, x_end - 1);
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
                        //draw_solid_wall_range(x, x1);
                        x = x2;
                    }
                }
                //draw_solid_wall_range(x, x2);
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
