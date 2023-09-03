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
    //create set filled with all the segment width on the screen
    std::set<int> currentWall;
    for (int i = x_start; i < x_end; i++) 
    {
        currentWall.insert(i);
    }

    // create intersection of segment and whole screen
    std::set<int> intersection;
    std::set_intersection(screenRange.begin(), screenRange.end(), currentWall.begin(), currentWall.end(), std::inserter(intersection, intersection.begin()));

    if (!intersection.empty())  // check if intersection occured
    {
        if (intersection.size() == currentWall.size()) 
        {
            // paste whole segment to the screen
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
            // split the segment and paste it to screen
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
            data.x2 = arr.back();
            data.rwAngle = rwAngle;
            data.isPortal = true;
            drawData.push_back(data);
        }
    }
}

void SegmentHandler::clipSolidWalls(LevelData::Seg segment, int x_start, int x_end, float rwAngle)
{   
    // no need to calculate futher if all the screen is covered
    if(!screenRange.empty())
    {   

        //create set filled with all the segment width on the screen
        std::set<int> currentWall;
        for (int i = x_start; i < x_end; ++i) 
        {
            currentWall.insert(i);
        }

        // create intersection of segment and whole screen
        std::set<int> intersection;
        std::set_intersection(screenRange.begin(), screenRange.end(), currentWall.begin(), currentWall.end(), std::inserter(intersection, intersection.begin()));

        if (!intersection.empty()) // check if intersection occured
        {
            if (intersection.size() == currentWall.size()) 
            {
                // paste whole segment to the screen
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
                // split the segment and paste it to screen
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
        // disabling futher traversal of BSP tree
        BSP* bsp = engine.getBSP();
        bsp->setTraverse(false);
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
