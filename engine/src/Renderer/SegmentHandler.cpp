#include "../../include/Renderer/SegmentHandler.h"
#include "../../include/Controller/Player.h"

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
    sectors = level->getSectors();
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

void SegmentHandler::clipPortalWalls(LevelData::Seg segment, int x_start, int x_end, float rwAngle)
{
    std::set<int> currentWall;
    if(x_start < x_end)
    {
        for (int i = x_start; i < x_end; ++i) 
        {
            currentWall.insert(i);
        }
    }else
    {
        for (int i = x_end; i < x_start; ++i) 
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
                data.segment = segment;
                data.x1 = x_start;
                data.x2 = x_end - 1;
                data.rwAngle = rwAngle;
                data.isPortal = true;
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
                        data.segment = segment;
                        data.x1 = x;
                        data.x2 = x1;
                        data.rwAngle = rwAngle;
                        data.isPortal = true;
                        drawData.push_back(data);

                        x = x2;
                    }
                }

                segmentDrawData data;
                data.segment = segment;
                data.x1 = x;
                data.x2 = arr[arr.size() -1];
                data.rwAngle = rwAngle;
                data.isPortal = true;
                drawData.push_back(data);
            }
        }
}

void SegmentHandler::clipSolidWalls(LevelData::Seg segment, int x_start, int x_end, float rwAngle)
{   
    if(!screenRange.empty())
    {   
        std::set<int> currentWall;
        if(x_start < x_end)
        {
            for (int i = x_start; i < x_end; ++i) 
            {
                currentWall.insert(i);
            }
        }else
        {
            for (int i = x_end; i < x_start; ++i) 
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
                data.segment = segment;
                data.x1 = x_start;
                data.x2 = x_end - 1;
                data.rwAngle = rwAngle;
                data.isPortal = false;
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
                        data.segment = segment;
                        data.x1 = x;
                        data.x2 = x1;
                        data.rwAngle = rwAngle;
                        data.isPortal = false;
                        drawData.push_back(data);


                        x = x2;
                    }
                }

                segmentDrawData data;
                data.segment = segment;
                data.x1 = x;
                data.x2 = x2;
                data.rwAngle = rwAngle;
                data.isPortal = false;
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

    if(!twoSided)
    {
        int frontSidedefId = linedef.frontSidedef;
        clipSolidWalls(segment, x1, x2, rwAngle);
        return;
    }

    LevelData::Sidedef front = sidedefs[linedef.frontSidedef];
    LevelData::Sidedef back = sidedefs[linedef.backSidedef];

    LevelData::Sector frontSector = sectors[front.sectorNumber];
    LevelData::Sector backSector = sectors[back.sectorNumber];
    

    if((frontSector.ceilingHeight != backSector.ceilingHeight) || (frontSector.floorHeight != backSector.floorHeight))
    {
        clipPortalWalls(segment, x1, x2, rwAngle);
        return;
    }

    if(backSector.ceilingTextureName == frontSector.ceilingTextureName && 
    backSector.floorTextureName == frontSector.floorTextureName &&
    backSector.lightLevel == frontSector.lightLevel &&
    front.middleTextureName == "-")
    {
        return;
    }

    clipPortalWalls(segment, x1, x2, rwAngle);
}
