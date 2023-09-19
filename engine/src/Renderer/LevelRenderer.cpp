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
    float halfWindowWidth = static_cast<float>(*(engine.getHalfWindowWidth()));
    float screenDist = static_cast<float>(*(engine.getScreenDist()));
    float tangent = tan(radians(angle));
    float x;

    if (angle > 0)
    {
        x = screenDist - tangent * halfWindowWidth;
    }
    else
    {
        x = screenDist + tangent * halfWindowWidth;
    }

    return static_cast<int>(x);
}

vector<double> LevelRenderer::xtoAngle()
{
    int windowWidth = *(engine.getWindowWidth());
    double halfWindowWidth = *(engine.getHalfWindowWidth());
    double screenDist = static_cast<double>(*(engine.getScreenDist()));

    vector<double> xToAngleTable;
    xToAngleTable.reserve(windowWidth + 1); // Reserve space for efficiency

    for (int i = 0; i <= windowWidth; ++i)
    {
        double angle = degrees(atan((halfWindowWidth - (float) i) / screenDist));
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

    // some aliases to shorten the following code
    int x1 = segment.x1;
    int x2 = segment.x2;
    int lineId = segment.segment.lindefNumber;
    LevelData::Linedef* line = &lines[lineId];

    LevelData::Sidedef* frontSide; 
    LevelData::Sidedef* backSide;

    if(segment.segment.direction == 0)
    {
        frontSide = &sides[line->frontSidedef];
        backSide = &sides[line->backSidedef];
    }
    else
    {
        frontSide = &sides[line->backSidedef];
        backSide = &sides[line->frontSidedef];
    }
    
    LevelData::Sector* frontSector = &sectors[frontSide->sectorNumber];
    LevelData::Sector* backSector = &sectors[backSide->sectorNumber];

    LevelData::Vertex* startVertex = &vertexs[segment.segment.startVertex];
    LevelData::Vertex* endVertex = &vertexs[segment.segment.endVertex];
    int lightLevel = frontSector->lightLevel;

    //textures
    string upperWallTextureName = frontSide->upperTextureName;
    string lowerWallTextureName = frontSide->lowerTextureName;
    string ceilTextureName = frontSector->ceilingTextureName;
    string floorTextureName = frontSector->floorTextureName;
    sf::Color upperWallColor = getRandomColor(upperWallTextureName, lightLevel);
    sf::Color lowerWallColor = getRandomColor(lowerWallTextureName, lightLevel);
    sf::Color ceilColor = getRandomColor(ceilTextureName, lightLevel);
    sf::Color florColor = getRandomColor(floorTextureName, lightLevel);
    ResourcesData::Image* upperWallTexture = level->getTexture(upperWallTextureName);
    ResourcesData::Image* lowerWallTexture = level->getTexture(lowerWallTextureName);
    ResourcesData::Image* ceilTexture = level->getTexture(ceilTextureName);
    ResourcesData::Image* floorTexture = level->getTexture(floorTextureName);
    

    //calculate the relative plane heights of front and back sector
    double worldFrontZ1 = (double) (frontSector->ceilingHeight) - (double) (player->getHeight());
    double worldBackZ1 = (double) (backSector->ceilingHeight) - (double) (player->getHeight());
    double worldFrontZ2 = (double) (frontSector->floorHeight) - (double) (player->getHeight());
    double worldBackZ2 = (double) (backSector->floorHeight) - (double) (player->getHeight());

    // sky hack
    if(frontSector->ceilingTextureName == *(engine.getSkyId()))
    {
        if(backSector->ceilingTextureName == *(engine.getSkyId()))
        {
            worldFrontZ1 = worldBackZ1;
        }      
    }
            
    // check which parts must be rendered
    bool bDrawUpperWall = false;
    bool bDrawCeil = false;
    bool bDrawLowerWall = false;
    bool bDrawFloor = false;

    if( (int) worldFrontZ1 != (int) worldBackZ1 ||
        frontSector->lightLevel != backSector->lightLevel ||
        frontSector->ceilingTextureName != backSector->ceilingTextureName)
    {
        bDrawUpperWall = frontSide->upperTextureName != "-" && worldBackZ1 < worldFrontZ1;
        bDrawCeil = worldFrontZ1 >= 0 || frontSector->ceilingTextureName == *(engine.getSkyId());
    }

    if((int) worldFrontZ2 != (int) worldBackZ2 ||
        frontSector->lightLevel != backSector->lightLevel ||
        frontSector->floorTextureName != backSector->floorTextureName)
    {
        bDrawLowerWall = frontSide->lowerTextureName != "-" && worldBackZ2 > worldFrontZ2;
        bDrawFloor = worldFrontZ2 <= 0;
    }

    // if nothing must be rendered, we can skip this seg
    if(!bDrawUpperWall && !bDrawCeil && !bDrawLowerWall && !bDrawFloor)
    {
        return;
    }

    // calculate the scaling factors of the left and right edges of the wall range
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

    // determine how the wall textures are vertically aligned
    float upperTexAlt;
    float lowerTexAlt;
    float vTop;
    if(bDrawUpperWall)
    {
        if(line->flags.unpeggedUpperTexture == true)
        {
            upperTexAlt = worldFrontZ1;
        }
        else
        {
            vTop = backSector->ceilingHeight + upperWallTexture->getHeight();
            upperTexAlt = vTop - player->getHeight();
        }
        upperTexAlt += frontSide->yOffset;
    }
    if(bDrawLowerWall)
    {
        if(line->flags.unpeggedLowerTexture == true)
        {
            lowerTexAlt = worldFrontZ1;
        }
        else
        {
            lowerTexAlt = worldBackZ2;
        }
            
        lowerTexAlt += frontSide->yOffset;
    }
        
    // determine how the wall textures are horizontally aligned
    bool segTextured;
    if(bDrawUpperWall || bDrawLowerWall)
    {
        segTextured = true;
    }
    else
    {
        segTextured = false;
    }
    
    float rwOffset;
    float rwCenterAngle;
    if (segTextured)
    {
        rwOffset = hypotenuse * sin(radians(offsetAngle));
        rwOffset += segment.segment.offset + frontSide->xOffset;
        rwCenterAngle = rwNormalAngle - player->getAngle();
    }
            

    // the y positions of the top / bottom edges of the wall on the screen
    double wallY1 = (double) ( (double) *(engine.getHalfWindowHeight())) - (double) worldFrontZ1 * rwScale1;
    double wallY1Step = -rwScaleStep * worldFrontZ1;
    double wallY2 = (double) ( (double) *(engine.getHalfWindowHeight())) - (double) worldFrontZ2 * rwScale1;
    double wallY2Step = -rwScaleStep * worldFrontZ2;

    // the y position of the top edge of the portal
    double portalY1;
    double portalY1Step;
    double portalY2;
    double portalY2Step;
    if(bDrawUpperWall)
    {
        if(worldBackZ1 > worldFrontZ2)
        {
            portalY1 = (double) ( (double)*(engine.getHalfWindowHeight())) - worldBackZ1 * rwScale1;
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
            portalY2 = (double) ( (double) *(engine.getHalfWindowHeight())) - worldBackZ2 * rwScale1;
            portalY2Step = -rwScaleStep * worldBackZ2;
        }
        else
        {
            portalY2 = wallY1;
            portalY2Step = wallY1Step;
        }
    }


    // now the rendering is carried out
    for(int x = x1; x < x2 + 1; x++)
    {
        double drawWallY1 = wallY1 - 1;
        double drawWallY2 = wallY2;

        double angle;
        double textureColumn;
        double invScale;

        if(segTextured)
        {
            angle = rwCenterAngle - xToAngleTable[x];
            textureColumn = rwDistance * tan(radians(angle)) - rwOffset;
            invScale = 1.0 / rwScale1;
        }         

        if(bDrawUpperWall)
        {
            double drawUpperWallY1 = wallY1 - 1;
            double drawUpperWallY2 = portalY1;

            if(bDrawCeil)
            {
                int cy1 = (int) upperClip[x] + 1;
                int cy2 = (int) min((double) (drawWallY1 - 1), (double) (lowerClip[x] - 1.0));
                //drawVerticalLine(x, cy1, cy2, ceilColor);
                drawFlat(ceilTexture, lightLevel, x, cy1, cy2, worldFrontZ1);
            }

            int wy1 = (int) max((double) (drawUpperWallY1), upperClip[x] + 1);
            int wy2 = min((double) (drawUpperWallY2), lowerClip[x] - 1);
            //drawVerticalLine(x, wy1, wy2, upperWallColor);
            drawWallColumn(upperWallTexture, textureColumn, x, wy1, wy2, upperTexAlt, invScale, lightLevel);

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
            //drawVerticalLine(x, cy1, cy2, ceilColor);
            drawFlat(ceilTexture, lightLevel, x, cy1, cy2, worldFrontZ1);

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
                //drawVerticalLine(x, fy1, fy2, florColor);
                drawFlat(floorTexture, lightLevel, x, fy1, fy2, worldFrontZ2);
            }

            float drawLowerWallY1 = portalY2 - 1;
            float drawLowerWallY2 = wallY2;

            int wy1 = (int) max( (double) (drawLowerWallY1), (double) upperClip[x] + 1);
            int wy2 = (int) min( (double) (drawLowerWallY2), (double) lowerClip[x] - 1);
            //drawVerticalLine(x, wy1, wy2, lowerWallColor);
            drawWallColumn(lowerWallTexture, textureColumn, x, wy1, wy2, lowerTexAlt, invScale, lightLevel);

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
            //drawVerticalLine(x, fy1, fy2, florColor);
            drawFlat(floorTexture, lightLevel, x, fy1, fy2, worldFrontZ2);

            if(lowerClip[x] > drawWallY2 + 1)
            {
                lowerClip[x] = fy1;
            }

        }

        rwScale1 += rwScaleStep;
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


    LevelData::Sidedef* side;

    if(segment.segment.direction == 0)
    {
        side = &sides[line->frontSidedef];
    }
    else
    {
        side = &sides[line->backSidedef];
    }

    LevelData::Sector* sector = &sectors[side->sectorNumber];
    LevelData::Vertex* startVertex = &vertexs[segment.segment.startVertex];
    LevelData::Vertex* endVertex = &vertexs[segment.segment.endVertex];

    string wallTextureName = toUpper(side->middleTextureName);
    string ceilTextureName = sector->ceilingTextureName;
    string floorTextureName = sector->floorTextureName;

    

    int lightLevel = sector->lightLevel;

    sf::Color wallColor = getRandomColor(wallTextureName, lightLevel);
    sf::Color ceilColor = getRandomColor(ceilTextureName, lightLevel);
    sf::Color florColor = getRandomColor(floorTextureName, lightLevel);

    //calculations
    double worldFrontZ1 = (double) (sector->ceilingHeight) - (double) (player->getHeight());
    double worldFrontZ2 = (double) (sector->floorHeight) - (double) (player->getHeight());

    bool drawWall = side->middleTextureName != "-"; // decide if drawing wall is needed
    bool drawCeiling = worldFrontZ1 > 0 || sector->ceilingTextureName == *(engine.getSkyId()); // decide if drawing ceiling is needed
    bool drawFloor = worldFrontZ2 < 0; // decide if drawing floor is needed

    ResourcesData::Image* wallTexture = level->getTexture(wallTextureName);
    ResourcesData::Image* ceilTexture = level->getTexture(ceilTextureName);
    ResourcesData::Image* floorTexture = level->getTexture(floorTextureName);

    double rwNormalAngle = (double) segment.segment.angle + 90.0; 
    double offsetAngle = rwNormalAngle - (double) segment.rwAngle;
    double hypotenuse = calculateDistance( (double) player->getPosX(), (double) player->getPosY(), (double) startVertex->x, (double) startVertex->y);
    double rwDistance = hypotenuse * cos(radians(offsetAngle));
    double rwScale1 = scaleFromGlobalAngle(x1, rwNormalAngle, rwDistance);
    double rwScaleStep;

    if (std::abs(std::fmod(offsetAngle, 360.0) - 90.0) <= 1.0e-6) {
        rwScale1 *= 0.01;
    }

    if(x1 < x2)
    {
        double scale2 = scaleFromGlobalAngle(x2, rwNormalAngle, rwDistance);
        rwScaleStep = (double) (scale2 - rwScale1) / (double) (x2 - x1);
    }else
    {
        rwScaleStep = 0.0;
    }

    // -------------------------------------------------------------------------- //
    // determine how the wall texture are vertically aligned
    float vTop;
    float middleTexAlt;
    if(line->flags.unpeggedLowerTexture == true)
    {
        vTop = sector->floorHeight + wallTexture->getHeight();
        middleTexAlt = vTop - player->getHeight();
    }else
    {
        middleTexAlt = worldFrontZ1;  
    }
    
    middleTexAlt += side->yOffset;


    // determine how the wall textures are horizontally aligned

    float rwOffset = hypotenuse * sin(radians(offsetAngle));
    rwOffset += segment.segment.offset + side->xOffset;
    float rwCenterAngle = rwNormalAngle - player-> getAngle();

    // -------------------------------------------------------------------------- //

    // determine where on the screen the wall is drawn
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
            //drawVerticalLine(i, cy1, cy2, ceilColor);
            drawFlat(ceilTexture, lightLevel, i, cy1, cy2, worldFrontZ1);
        }

        if(drawWall)
        {
            int wy1 = (int) max((double)(drawWallY1), (double) (upperClip[i] + 1.0));
            int wy2 = (int) min((double)(drawWallY2), (double) (lowerClip[i] - 1.0)); 

            if(wy1 < wy2)
            {
                float angle = rwCenterAngle - xToAngleTable[i];
                float textureColumn = rwDistance * tan(radians(angle)) - rwOffset;
                float invScale = 1.0 / rwScale1;
                //drawVerticalLine(i, wy1, wy2, wallColor);
                drawWallColumn(wallTexture, textureColumn, i, wy1, wy2, middleTexAlt, invScale, lightLevel);
                
            }
                  
        }

        if(drawFloor)
        {
            int fy1 = (int) max((double)(drawWallY2 + 1.0), (double) (upperClip[i] + 1.0));
            int fy2 = (int) std::round(lowerClip[i] - 1.0);
            //drawVerticalLine(i, fy1, fy2, florColor);
            drawFlat(floorTexture, lightLevel, i, fy1, fy2, worldFrontZ2);
        }

        rwScale1 += rwScaleStep;
        wallY1 += wallY1Step;
        wallY2 += wallY2Step;

    } 

}

void LevelRenderer::drawFlat(ResourcesData::Image* texture, int lightLevel, int x, int y1, int y2, int worldZ)
{
    if(y1 < y2)
    {
        if(texture->getName() == *(engine.getSkyId()) )
        {
            string textureName = *(engine.getSkyTextureName());
            float skyInvScale = 160.0 / *(engine.getWindowHeight());
            float skyTextureAlt = 100.0;
            ResourcesData::Image* skyTexture = level->getTexture(textureName);
            float textureColumn = 2.2 * ( player->getAngle() + xToAngleTable[x]);
            drawWallColumn(skyTexture, textureColumn, x, y1, y2, skyTextureAlt, skyInvScale, 255);
        }else
        {
           drawFlatColumn(texture, x, y1, y2, lightLevel, worldZ, player->getAngle(), player->getPosX(), player->getPosY());
        }
    }
}                         

void LevelRenderer::drawFlatColumn(ResourcesData::Image* texture, int x, int y1, int y2, int lightLevel, int worldZ, int playerAngle, float playerX, float playerY)
{
    float H_WIDTH = *(engine.getHalfWindowWidth());
    float H_HEIGHT = *(engine.getHalfWindowHeight());
    float WIDTH = *(engine.getWindowWidth());

    double playerDirX = cos(radians(playerAngle));
    double playerDirY = sin(radians(playerAngle));

    for(int iy = y1; iy < y2 + 1; iy++)
    {
        int z = H_WIDTH * worldZ / (H_HEIGHT - iy);

        double px = playerDirX * z + playerX;
        double py = playerDirY * z + playerY;

        double leftX = -playerDirY * z + px;
        double leftY = playerDirX * z + py;
        double rightX = playerDirY * z + px;
        double rightY = -playerDirX * z + py;

        double dx = (rightX - leftX) / WIDTH;
        double dy = (rightY - leftY) / WIDTH;

        double tx = int(leftX + dx * x) & 63;
        double ty = int(leftY + dy * x) & 63;

        PlayPalData::colorRGB_t color = texture->getPixel(ty, tx, lightLevel);
        
            int red = (int) color.red ;//* (lightLevel / 255.0);
            int green = (int) color.green;// * (lightLevel / 255.0);
            int blue = (int) color.blue;// * (lightLevel / 255.0);
            sf::Color sfColor;
            sfColor.r = red;
            sfColor.g = green;
            sfColor.b = blue;
            sfColor.a = 255;
            drawPixel(x, iy, sfColor);
        
    }     
}

void LevelRenderer::drawWallColumn(ResourcesData::Image* wallTexture, float textureColumn, int x, int y1, int y2, float middleTexAlt, float invScale, int lightLevel)
{
    //cout << wallTexture->getName() << endl;
    if(y1 < y2)
    {
        
        int textureWidth = wallTexture->getWidth();
        int textureHeight = wallTexture->getHeight();
        
        textureColumn = modulo( (int) textureColumn, textureWidth);
        float textureY = middleTexAlt +  (float) ( (float) y1 - (float) *(engine.getHalfWindowHeight())) * invScale;

        for(int iy = y1; iy < y2 + 1; iy++)
        {
            int xCoord =  (int) textureColumn;
            int yCoord =  modulo( (int) textureY, textureHeight);


            PlayPalData::colorRGB_t color = wallTexture->getPixel( yCoord, xCoord, lightLevel );
            
                int red = (int) color.red; //* (lightLevel / 255.0);
                int green = (int) color.green;// * (lightLevel / 255.0);
                int blue = (int) color.blue;// * (lightLevel / 255.0);
                sf::Color sfColor;
                sfColor.r = red;
                sfColor.g = green;
                sfColor.b = blue;
                sfColor.a = 255;
                drawPixel(x, iy, sfColor);
            
            textureY += invScale; 
        }
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

    std::uniform_int_distribution<int> distribution(40, 150);

    sf::Color color;
    color.r = distribution(mt) ;//+ lightLevel / 2;
    color.g = distribution(mt) ;//+ lightLevel / 2;
    color.b = distribution(mt) ;//+ lightLevel / 2;
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
    int place = (y * width + x) * 4;
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
    pixels = new uint8_t[width * height * 4];
    texture.create(width, height);
    //clearDrawingBoard();

    texture.update(pixels);
    sprite.setTexture(texture);
}