/*#include "../../include/Renderer/SegmentHandler.h"

SegmentHandler::SegmentHandler(Engine& lEngine, Player *lPlayer) : engine(lEngine)
{
    LevelData::LevelData* level;
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
    if(screenRange.size())
    {
        std::set<int> currentWall;
        for (int i = x1; i < x2; ++i) 
        {
            screenRange.insert(i);
        }

        std::set<int> intersection;
        std::set_intersection(screenRange.begin(), screenRange.end(), currentWall.begin(), currentWall.end(), std::inserter(intersection, intersection.begin()));
        
        
    }
    else
    {

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

*/