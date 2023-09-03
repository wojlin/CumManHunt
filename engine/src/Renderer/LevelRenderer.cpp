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

    for(int i = 0; i < *(engine.getWindowWidth()); i++)
    {
        upperClip.push_back(-1);
    }

    for(int i = 0; i < *(engine.getWindowWidth()); i++)
    {
        lowerClip.push_back(*(engine.getWindowHeight()));
    }
    

    setupDrawingBoard();
}     

void LevelRenderer::drawLevel()
{

}

sf::Sprite* LevelRenderer::getLevel()
{
    return &(sprite);
}

         
//private

int  LevelRenderer::angleToX(float angle)
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

vector<float> LevelRenderer::xtoAngle()
{
    vector<float> xToAngleTable;
    for(int i = 0; i < *(engine.getWindowWidth()) + 1; i++)
    {
        float angle = degrees(atan( ( ( (float) *(engine.getWindowWidth()) / 2.0 ) - i ) / (float) *(engine.getScreenDist() )));
        xToAngleTable.push_back(angle);
    }
    return xToAngleTable;
}

float LevelRenderer::scaleFromGlobalAngle(int x, float rwNormalAngle, float rwDistance)
{
    float xAngle = xToAngleTable[x];
    float num = (float) *(engine.getScreenDist()) * cos(radians(rwNormalAngle - (float) xAngle - (float) player->getAngle()));
    float den = rwDistance * cos(radians(xAngle));

    float scale = num / den;
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


    // checking conditions and choosing either to draw this segment or not:

    int worldFrontZ1 = static_cast<int>(frontSector->ceilingHeight) - static_cast<int>(player->getHeight());
    int worldBackZ1 = static_cast<int>(backSector->ceilingHeight) - static_cast<int>(player->getHeight());
    int worldFrontZ2 = static_cast<int>(frontSector->floorHeight) - static_cast<int>(player->getHeight());
    int worldBackZ2 = static_cast<int>(backSector->floorHeight) - static_cast<int>(player->getHeight());

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

    vector<int> localUpperClip = upperClip;
    vector<int> localLowerClip = lowerClip;

    // actual drawing calculations:

    float rwNormalAngle = segment.segment.angle + 90; 
    float offsetAngle = rwNormalAngle - segment.rwAngle;
    float hypotenuse = calculateDistance( (float) player->getPosX(), (float) player->getPosY(), (float) startVertex->x, (float) startVertex->y);
    float rwDistance = hypotenuse * cos(radians(offsetAngle));
    float rwScale1 = scaleFromGlobalAngle(x1, rwNormalAngle, rwDistance);
    float rwScaleStep;

    if(x1 < x2)
    {
        float scale2 = scaleFromGlobalAngle(x2, rwNormalAngle, rwDistance);
        rwScaleStep = (float) (scale2 - rwScale1) / (float) (x2 - x1);
    }else
    {
        rwScaleStep = 0.0;
    }

    // the y positions of the top / bottom edges of the wall on the screen
    float wallY1 = (float) (*(engine.getWindowHeight()) / 2.0) - (float) worldFrontZ1 * rwScale1;
    float wallY1Step = -rwScaleStep * worldFrontZ1;

    float wallY2 = (float) (*(engine.getWindowHeight()) / 2.0) - (float) worldFrontZ2 * rwScale1;
    float wallY2Step = -rwScaleStep * worldFrontZ2;

    
    float portalY1;
    float portalY1Step;
    float portalY2;
    float portalY2Step;
    

    if(bDrawUpperWall)
    {
        if(worldBackZ1 > worldFrontZ2)
        {
            portalY1 = (float) (*(engine.getWindowHeight()) / 2) - worldBackZ1 * rwScale1;
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
            portalY2 = (float) (*(engine.getWindowHeight()) / 2) - worldBackZ2 * rwScale1;
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
        float drawWallY1 = wallY1 - 1;
        float drawWallY2 = wallY2;

        if(bDrawUpperWall)
        {
            float drawUpperWallY1 = wallY1 - 1;
            float drawUpperWallY2 = portalY1;

            if(bDrawCeil)
            {
                int cy1 = (int) localUpperClip[x] + 1;
                int cy2 = min(static_cast<int>(drawWallY1 - 1), localLowerClip[x] - 1);
                drawVerticalLine(x, cy1, cy2, ceilTexture, lightLevel);
            }

            int wy1 = max(static_cast<int>(drawUpperWallY1), localUpperClip[x] + 1);
            int wy2 = min(static_cast<int>(drawUpperWallY2), localLowerClip[x] - 1);
            drawVerticalLine(x, wy1, wy2, upperWallTexture, lightLevel);

            if(localUpperClip[x] < wy2)
            {
                localUpperClip[x] = wy2;
            }

            portalY1 += portalY1Step;
        }

        if(bDrawCeil)
        {
            int cy1 = localUpperClip[x] + 1;
            int cy2 = min(static_cast<int>(drawWallY1 - 1), localLowerClip[x] - 1);
            drawVerticalLine(x, cy1, cy2, ceilTexture, lightLevel);

            if(localUpperClip[x] < cy2)
            {
                localUpperClip[x] = cy2;
            }
        }

        if(bDrawLowerWall)
        {
            if(bDrawFloor)
            {
                int fy1 = (int) max( (int) drawWallY2 +1, localUpperClip[x] + 1);
                int fy2 = localLowerClip[x] - 1;
                drawVerticalLine(x, fy1, fy2, floorTexture, lightLevel);
            }

            float drawLowerWallY1 = portalY2 - 1;
            float drawLowerWallY2 = wallY2;

            int wy1 = max(static_cast<int>(drawLowerWallY1), localUpperClip[x] + 1);
            int wy2 = min(static_cast<int>(drawLowerWallY2), localLowerClip[x] - 1);
            drawVerticalLine(x, wy1, wy2, lowerWallTexture, lightLevel);

            if(localLowerClip[x] > wy1)
            {
                localLowerClip[x] = wy1;
            }

            portalY2 += portalY2Step;
        }

        if(bDrawFloor)
        {
            int fy1 = max(static_cast<int>(drawWallY2 + 1), localUpperClip[x] + 1);
            int fy2 = localLowerClip[x] - 1;
            drawVerticalLine(x, fy1, fy2, floorTexture, lightLevel);

            if(localLowerClip[x] > drawWallY2 + 1)
            {
                localLowerClip[x] = fy2;
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

    //calculations
    int worldFrontZ1 = static_cast<int>(sector->ceilingHeight) - static_cast<int>(player->getHeight());
    int worldFrontZ2 = static_cast<int>(sector->floorHeight) - static_cast<int>(player->getHeight());

    bool drawWall = side->middleTextureName != "-"; // decide if drawing wall is needed
    bool drawCeiling = worldFrontZ1 > 0; // decide if drawing ceiling is needed
    bool drawFloor = worldFrontZ2 < 0; // decide if drawing floor is needed

    float rwNormalAngle = segment.segment.angle + 90; 
    float offsetAngle = rwNormalAngle - segment.rwAngle;
    float hypotenuse = calculateDistance( (float) player->getPosX(), (float) player->getPosY(), (float) startVertex->x, (float) startVertex->y);
    float rwDistance = hypotenuse * cos(radians(offsetAngle));
    float rwScale1 = scaleFromGlobalAngle(x1, rwNormalAngle, rwDistance);
    float rwScaleStep;

    if(x1 < x2)
    {
        float scale2 = scaleFromGlobalAngle(x2, rwNormalAngle, rwDistance);
        rwScaleStep = (float) (scale2 - rwScale1) / (float) (x2 - x1);
    }else
    {
        rwScaleStep = 0.0;
    }

    float wallY1 = (float) (*(engine.getWindowHeight()) / 2) - (float) worldFrontZ1 * rwScale1;
    float wallY1Step = (float) -rwScaleStep * (float) worldFrontZ1;

    float wallY2 = (float) (*(engine.getWindowHeight()) / 2) - (float) worldFrontZ2 * rwScale1;
    float wallY2Step = (float) -rwScaleStep * (float) worldFrontZ2;

    vector<int> localUpperClip = upperClip;
    vector<int> localLowerClip = lowerClip;

    for(int i = x1; i < x2 + 1; i ++)
    {
        int drawWallY1 = wallY1 - 1;
        int drawWallY2 = wallY2;

        if(drawCeiling)
        {
            int cy1 = localUpperClip[i] + 1;
            int cy2 = min(static_cast<int>(drawWallY1 - 1), localLowerClip[i] - 1);
            drawVerticalLine(i, cy1, cy2, ceilTexture, lightLevel);
        }

        if(drawWall)
        {
            int wy1 = max(static_cast<int>(drawWallY1), localUpperClip[i] + 1);
            int wy2 = min(static_cast<int>(drawWallY2), localLowerClip[i] - 1); 
            drawVerticalLine(i, wy1, wy2, wallTexture, lightLevel);                   
        }

        if(drawFloor)
        {
            int fy1 = max(static_cast<int>(drawWallY2 + 1), localUpperClip[i] + 1);
            int fy2 = localLowerClip[i] - 1;
            drawVerticalLine(i, fy1, fy2, floorTexture, lightLevel);
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

        drawVerticalLine(x1, 0, *(engine.getWindowHeight()), "XD", 1);
        drawVerticalLine(x2, 0, *(engine.getWindowHeight()), "XD", 1);

    }
}  

void LevelRenderer::drawVerticalLine(int posX, int bottomOffset, int topOffset, string textureName, int lightLevel)
{
    sf::Color color = getRandomColor(textureName, lightLevel);
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

    if(y > *(engine.getWindowHeight()))
    {
        return;
    }

    if(y < 0)
    {
        return;
    }

    if(x < 0)
    {
        return;
    }

    if(x > *(engine.getWindowWidth()))
    {
        return;
    }
    
    int place = (width*y*4)+(x*4);
    if(place >= width*height*4)
    {
        return;
    }

    pixels[place] = color.r;
    pixels[place + 1] = color.g;
    pixels[place + 2] = color.b;
    pixels[place + 3] = color.a; 

}

void LevelRenderer::update()
{
    texture.update(pixels);
    sprite.setTexture(texture);
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