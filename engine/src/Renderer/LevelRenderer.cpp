#include "../../include/Renderer/LevelRenderer.h"

LevelRenderer::LevelRenderer(Engine& lEngine, Player *lPlayer): engine(lEngine)
{
    player = lPlayer;
    level = engine.getLevelData();
    vertexs = level->getVertexs();
    lines = level->getLinedefs();
    sides = level->getSidedefs();
    nodes = level->getNodes();
    sectors = level->getSectors();
    segs = level->getSegs();
    width = (*engine.getWindowWidth());
    height = (*engine.getWindowHeight());

    xToAngleTable = xtoAngle();

    createClips();
    

    setupDrawingBoard();
}     

void LevelRenderer::createClips()
{
    upperClip.clear();
    lowerClip.clear();

    for(int i = 0; i < *(engine.getWindowWidth()); i++)
    {
        upperClip.push_back(-1);
    }

    for(int i = 0; i < *(engine.getWindowWidth()); i++)
    {
        lowerClip.push_back(*(engine.getWindowHeight()));
    }
}

void LevelRenderer::drawScene()
{
    BSP* bsp = engine.getBSP();
    vector<segmentDrawData>* draw = bsp->getDrawData();
    drawData(draw);
}

sf::Sprite* LevelRenderer::getScene()
{
    return &(sprite);
}

         
//private

int  LevelRenderer::angleToX(double angle)
{
    float x;

    if(angle > 0)
    {
        x = (float) *(engine.getScreenDist()) - tan(radians(angle)) * ( (float) *(engine.getWindowWidth()) / 2.0);
    }
    else
    {
        x = -tan(radians(angle)) * ( (float) *(engine.getWindowWidth()) / 2.0) + (float) *(engine.getScreenDist());
    }

    return (int) x;
}

vector<double> LevelRenderer::xtoAngle()
{
    vector<double> xToAngleTable;
    for(int i = 0; i < *(engine.getWindowWidth()) + 1; i++)
    {
        double angle = degrees(atan( ( ( (double) *(engine.getWindowWidth()) / 2.0 ) - i ) / (double) *(engine.getScreenDist() )));
        xToAngleTable.push_back(angle);
    }
    return xToAngleTable;
}

double LevelRenderer::scaleFromGlobalAngle(int x, double rwNormalAngle, double rwDistance)
{
    double xAngle = xToAngleTable[x];
    double num = (double) *(engine.getScreenDist()) * cos(radians(rwNormalAngle - (double) xAngle - (double) player->getAngle()));
    double den = rwDistance * cos(radians(xAngle));

    double scale = num / den;
    scale = min(MAX_SEG_SCALE, max(MIN_SEG_SCALE, scale));

    return scale;
}

void LevelRenderer::drawPortalWall(segmentDrawData segment)
{

    // initial setup

    int x1 = segment.x1;
    int x2 = segment.x2;
    int lineId = segment.segment.lindefNumber;

    LevelData::Linedef* line = &lines[lineId];
    LevelData::Sidedef* frontSide = &sides[line->frontSidedef];
    LevelData::Sidedef* backSide = &sides[line->backSidedef];
    LevelData::Sector* frontSector = &sectors[frontSide->sectorNumber];
    LevelData::Sector* backSector = &sectors[backSide->sectorNumber];
    LevelData::Vertex* startVertex = &vertexs[segment.segment.startVertex];
    LevelData::Vertex* endVertex = &vertexs[segment.segment.endVertex];

    string upperWallTexture = frontSide->upperTextureName;
    string lowerWallTexture = frontSide->lowerTextureName;
    string ceilTexture = frontSector->ceilingTextureName;
    string floorTexture = frontSector->floorTextureName;
    int lightLevel = frontSector->lightLevel;

    sf::Color upperWallColor = getRandomColor(upperWallTexture, lightLevel);
    sf::Color lowerWallColor = getRandomColor(lowerWallTexture, lightLevel);
    sf::Color ceilColor = getRandomColor(ceilTexture, lightLevel);
    sf::Color florColor = getRandomColor(floorTexture, lightLevel);

    // checking conditions and choosing either to draw this segment or not:

    double worldFrontZ1 = (double) (frontSector->ceilingHeight) - (double) (player->getHeight());
    double worldBackZ1 = (double) (backSector->ceilingHeight) - (double) (player->getHeight());
    double worldFrontZ2 = (double) (frontSector->floorHeight) - (double) (player->getHeight());
    double worldBackZ2 = (double) (backSector->floorHeight) - (double) (player->getHeight());

    bool bDrawUpperWall = false;
    bool bDrawCeil = false;

    bool bDrawLowerWall = false;
    bool bDrawFloor = false;

    if(worldFrontZ1 != worldBackZ1 ||
        frontSector->lightLevel != backSector->lightLevel ||
        frontSector->ceilingTextureName != backSector->ceilingTextureName)
    {
        bDrawUpperWall = frontSide->upperTextureName != "-" && worldBackZ1 < worldFrontZ1;
        bDrawCeil = worldFrontZ1 >= 0;
    }

    if(worldFrontZ2 != worldBackZ2 ||
        frontSector->lightLevel != backSector->lightLevel ||
        frontSector->floorTextureName != backSector->floorTextureName)
    {
        bDrawLowerWall = frontSide->lowerTextureName != "-" && worldBackZ2 > worldFrontZ2;
        bDrawFloor = worldFrontZ2 <= 0;
    }

    if(!bDrawUpperWall && !bDrawCeil && !bDrawLowerWall && !bDrawFloor)
    {
        return;
    }

    // actual drawing calculations:

    double rwNormalAngle = segment.segment.angle + 90.0; 
    double offsetAngle = rwNormalAngle - (double) segment.rwAngle;
    double hypotenuse = calculateDistance( (double) player->getPosX(), (double) player->getPosY(), (double) startVertex->x, (double) startVertex->y);
    double rwDistance = hypotenuse * cos(radians(offsetAngle));
    double rwScale1 = scaleFromGlobalAngle(x1, rwNormalAngle, rwDistance);
    double rwScaleStep;

    if(x1 < x2)
    {
        double scale2 = scaleFromGlobalAngle(x2, rwNormalAngle, rwDistance);
        rwScaleStep = (double) (scale2 - rwScale1) / (double) (x2 - x1);
    }else
    {
        rwScaleStep = 0.0;
    }

    // the y positions of the top / bottom edges of the wall on the screen
    double wallY1 = (double) ( (double) *(engine.getWindowHeight()) / 2.0) - (double) worldFrontZ1 * rwScale1;
    double wallY1Step = -rwScaleStep * worldFrontZ1;

    double wallY2 = (double) ( (double) *(engine.getWindowHeight()) / 2.0) - (double) worldFrontZ2 * rwScale1;
    double wallY2Step = -rwScaleStep * worldFrontZ2;

    
    double portalY1;
    double portalY1Step;
    double portalY2;
    double portalY2Step;
    

    if(bDrawUpperWall)
    {
        if(worldBackZ1 > worldFrontZ2)
        {
            portalY1 = (double) ( (double)*(engine.getWindowHeight()) / 2.0) - worldBackZ1 * rwScale1;
            portalY1Step = -rwScaleStep * worldBackZ1;
        }
        else
        {
            portalY1 = wallY2;
            portalY1Step = wallY2Step;
        }
    }

    if(bDrawLowerWall)
    {
        if(worldBackZ2 < worldFrontZ1)
        {
            portalY2 = (double) ( (double) *(engine.getWindowHeight()) / 2.0) - worldBackZ2 * rwScale1;
            portalY2Step = -rwScaleStep * worldBackZ2;
        }
        else
        {
            portalY2 = wallY1;
            portalY2Step = wallY1Step;
        }
    }

    //cout << bDrawFloor << bDrawCeil << bDrawLowerWall << bDrawUpperWall << endl;

    for(int x = x1; x < x2 + 1; x++)
    {
        double drawWallY1 = wallY1 - 1;
        double drawWallY2 = wallY2;

        if(bDrawUpperWall)
        {
            double drawUpperWallY1 = wallY1 - 1;
            double drawUpperWallY2 = portalY1;

            if(bDrawCeil)
            {
                int cy1 = (int) upperClip[x] + 1;
                int cy2 = (int) min((double) (drawWallY1 - 1), (double) (lowerClip[x] - 1.0));
                drawVerticalLine(x, cy1, cy2, ceilColor);
            }

            int wy1 = max((double) (drawUpperWallY1), upperClip[x] + 1);
            int wy2 = min((double) (drawUpperWallY2), lowerClip[x] - 1);
            drawVerticalLine(x, wy1, wy2, upperWallColor);

            if(upperClip[x] < wy2)
            {
                upperClip[x] = wy2;
            }

            portalY1 += portalY1Step;
        }

        if(bDrawCeil)
        {
            int cy1 = (int) upperClip[x] + 1;
            int cy2 = (int) min( (double) (drawWallY1 - 1), (double) (lowerClip[x] - 1.0));
            drawVerticalLine(x, cy1, cy2, ceilColor);

            if(upperClip[x] < cy2)
            {
                upperClip[x] = cy2;
            }
        }

        if(bDrawLowerWall)
        {
            if(bDrawFloor)
            {
                int fy1 = (int) max( (double) drawWallY2 +1, (double) upperClip[x] + 1);
                int fy2 = (int) (lowerClip[x] - 1.0);
                drawVerticalLine(x, fy1, fy2, florColor);
            }

            float drawLowerWallY1 = portalY2 - 1;
            float drawLowerWallY2 = wallY2;

            int wy1 = (int) max( (double) (drawLowerWallY1), (double) upperClip[x] + 1);
            int wy2 = (int) min( (double) (drawLowerWallY2), (double) lowerClip[x] - 1);
            drawVerticalLine(x, wy1, wy2, lowerWallColor);

            if(lowerClip[x] > wy1)
            {
                lowerClip[x] = wy1;
            }

            portalY2 += portalY2Step;
        }

        if(bDrawFloor)
        {
            int fy1 = max( (double) (drawWallY2 + 1), (double) (upperClip[x] + 1.0));
            int fy2 = lowerClip[x] - 1;
            drawVerticalLine(x, fy1, fy2, florColor);

            if(lowerClip[x] > drawWallY2 + 1)
            {
                lowerClip[x] = fy2;
            }

        }

        wallY1 += wallY1Step;
        wallY2 += wallY2Step;

    }


}

void LevelRenderer::drawSolidWall(segmentDrawData segment)
{
    //setup for ease of coding
    int x1 = segment.x1;
    int x2 = segment.x2;
    int lineId = segment.segment.lindefNumber;

    LevelData::Linedef* line = &lines[lineId];
    LevelData::Sidedef* side = &sides[line->frontSidedef];
    LevelData::Sector* sector = &sectors[side->sectorNumber];
    LevelData::Vertex* startVertex = &vertexs[segment.segment.startVertex];
    LevelData::Vertex* endVertex = &vertexs[segment.segment.endVertex];

    string wallTexture = side->middleTextureName;
    string ceilTexture = sector->ceilingTextureName;
    string floorTexture = sector->floorTextureName;


    int lightLevel = sector->lightLevel;

    sf::Color wallColor = getRandomColor(wallTexture, lightLevel);
    sf::Color ceilColor = getRandomColor(ceilTexture, lightLevel);
    sf::Color florColor = getRandomColor(floorTexture, lightLevel);

    //calculations
    double worldFrontZ1 = (double) (sector->ceilingHeight) - (double) (player->getHeight());
    double worldFrontZ2 = (double) (sector->floorHeight) - (double) (player->getHeight());

    bool drawWall = side->middleTextureName != "-"; // decide if drawing wall is needed
    bool drawCeiling = worldFrontZ1 > 0; // decide if drawing ceiling is needed
    bool drawFloor = worldFrontZ2 < 0; // decide if drawing floor is needed

    double rwNormalAngle = (double) segment.segment.angle + 90.0; 
    double offsetAngle = rwNormalAngle - (double) segment.rwAngle;
    double hypotenuse = calculateDistance( (double) player->getPosX(), (double) player->getPosY(), (double) startVertex->x, (double) startVertex->y);
    double rwDistance = hypotenuse * cos(radians(offsetAngle));
    double rwScale1 = scaleFromGlobalAngle(x1, rwNormalAngle, rwDistance);
    double rwScaleStep;

    if(x1 < x2)
    {
        double scale2 = scaleFromGlobalAngle(x2, rwNormalAngle, rwDistance);
        rwScaleStep = (double) (scale2 - rwScale1) / (double) (x2 - x1);
    }else
    {
        rwScaleStep = 0.0;
    }

    double wallY1 = (double) (*(engine.getWindowHeight()) / 2.0) - (double) worldFrontZ1 * (double) rwScale1;
    double wallY1Step = (double) -rwScaleStep * (double) worldFrontZ1;

    double wallY2 = (double) (*(engine.getWindowHeight()) / 2.0) - (double) worldFrontZ2 * (double) rwScale1;
    double wallY2Step = (double) -rwScaleStep * (double) worldFrontZ2;

    for(int i = x1; i < x2 + 1; i ++)
    {
        double drawWallY1 = wallY1 - 1.0;
        double drawWallY2 = wallY2;

        if(drawCeiling)
        {
            int cy1 = (int) std::round(upperClip[i] + 1.0);
            int cy2 = (int) min( (double) (drawWallY1 - 1.0), (double) (lowerClip[i] - 1.0) );
            drawVerticalLine(i, cy1, cy2, ceilColor);
        }

        if(drawWall)
        {
            int wy1 = (int) max((double)(drawWallY1), (double) (upperClip[i] + 1.0));
            int wy2 = (int) min((double)(drawWallY2), (double) (lowerClip[i] - 1.0)); 
            drawVerticalLine(i, wy1, wy2, wallColor);                   
        }

        if(drawFloor)
        {
            int fy1 = (int) max((double)(drawWallY2 + 1.0), (double) (upperClip[i] + 1.0));
            int fy2 = (int) std::round(lowerClip[i] - 1.0);
            drawVerticalLine(i, fy1, fy2, florColor);
        }

        wallY1 += wallY1Step;
        wallY2 += wallY2Step;

    } 

}

void LevelRenderer::drawData(vector<segmentDrawData>* drawData)
{
    for(int i = 0; i < drawData->size(); i++)
    {
        
        segmentDrawData segment = (*drawData)[i];
        
        if(segment.isPortal)
        {
            drawPortalWall(segment);
        }
        else
        {
            drawSolidWall(segment);
        }

    }
}

size_t LevelRenderer::hashString(const std::string& str) {
    std::hash<std::string> hasher;
    return hasher(str);
}

sf::Color LevelRenderer::getRandomColor(string textureName, int lightLevel)
{
    size_t seed = hashString(textureName);
    std::mt19937 mt(seed);

    std::uniform_int_distribution<int> distribution(40, 255);

    sf::Color color;
    color.r = distribution(mt) - lightLevel * 1;
    color.g = distribution(mt) - lightLevel * 1;
    color.b = distribution(mt) - lightLevel * 1;
    color.a = 255;
    return color;
}

void LevelRenderer::drawSegmentsById(vector<int>* segmentsIds)
{
    for(int i = 0; i < segmentsIds->size(); i++)
    {
        LevelData::Seg segment = segs[(*segmentsIds)[i]];
        LevelData::Vertex v1 = vertexs[segment.startVertex];
        LevelData::Vertex v2 = vertexs[segment.endVertex];
        
        std::pair<int, int> vertex1(v1.x, v1.y);
        std::pair<int, int> vertex2(v2.x, v2.y);

        float angle1 = pointToAngle(vertex1, player->getPosX(), player->getPosY());
        float angle2 = pointToAngle(vertex2, player->getPosX(), player->getPosY());
        float span = norm(angle1 - angle2);

        if(span >= 180.0)
        {
            continue;
        }

        float rwAngle = static_cast<int>(angle1);
   
        angle1 -= (float) player->getAngle();
        angle2 -= (float) player->getAngle();

        int hFov = (*engine.getPlayerHalfFOV());
        int fov = (*engine.getPlayerFOV());

        float span1 = norm(angle1 + hFov);
        if(span1 > fov)
        {
            if(span1 >= span + fov)
            {
                continue;
            }
            angle1 = hFov;
        }

        float span2 = norm(hFov - angle2);
        if(span2 > fov)
        {
            if(span2 >= span + fov)
            {
                continue;
            }
            angle2 = -hFov;
        }

        int x1 = angleToX(angle1);
        int x2 = angleToX(angle2);
        float angle = rwAngle;

        sf::Color color = getRandomColor("XD", 1);

        drawVerticalLine(x1, 0, *(engine.getWindowHeight()), color);
        drawVerticalLine(x2, 0, *(engine.getWindowHeight()), color);

    }
}  

void LevelRenderer::drawVerticalLine(int posX, int bottomOffset, int topOffset, sf::Color color)
{
    if(bottomOffset < topOffset)
    {
        for(int i = bottomOffset; i < topOffset; i++)
        {   
            //cout << i << endl;
            drawPixel(posX, i, color);
        }
    }
    
}

void LevelRenderer::drawPixel(int x, int y, sf::Color color)
{
    int place = (width*y*4)+(x*4);
    pixels[place] = color.r;
    pixels[place + 1] = color.g;
    pixels[place + 2] = color.b;
    pixels[place + 3] = color.a; 
}

void LevelRenderer::update()
{
    texture.update(pixels);
    sprite.setTexture(texture);
    createClips();
}


void LevelRenderer::clearDrawingBoard()
{
    for(int i = 0; i < width*height*4; i += 4) {
        pixels[i] = backgroundColor.r;
        pixels[i+1] = backgroundColor.g;
        pixels[i+2] = backgroundColor.b;
        pixels[i+3] = backgroundColor.a;
    }
}

void LevelRenderer::setupDrawingBoard()
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