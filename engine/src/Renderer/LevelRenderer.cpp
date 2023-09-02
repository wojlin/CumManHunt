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
        x = *(engine.getScreenDist()) - tan(radians(angle)) * *(engine.getWindowWidth());
    }
    else
    {
        x = -tan(radians(angle)) * *(engine.getWindowWidth()) + *(engine.getScreenDist());
    }

    return *(engine.getWindowWidth()) - (int) x;
}

int LevelRenderer::remapValue(int value, int oldMin, int oldMax, int newMin, int newMax) {
    // Ensure the value is within the original range
    if (value < oldMin)
        value = oldMin;
    else if (value > oldMax)
        value = oldMax;

    // Calculate the range of the original values
    int oldRange = oldMax - oldMin;

    // Calculate the range of the new values
    int newRange = newMax - newMin;

    // Map the value to the new range
    int remappedValue = ((value - oldMin) * newRange) / oldRange + newMin;

    return remappedValue;
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
    float num = *(engine.getScreenDist()) * cos(radians(rwNormalAngle - xAngle - player->getAngle()));
    float den = rwDistance * cos(radians(xAngle));

    float scale = num / den;
    scale = min(MAX_SEG_SCALE, max(MIN_SEG_SCALE, scale));

    return scale;
}

void LevelRenderer::drawPortalWall(segmentDrawData segment)
{

}

void LevelRenderer::drawSolidWall(segmentDrawData segment)
{

    int x1 = segment.x1;
    int x2 = segment.x2;
    /*
    if(x1 > x2)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    */

    if( x1 < 0 || x2 < 0 || x1 > *(engine.getWindowWidth()) || x2 > *(engine.getWindowWidth()))
    {
        return;
    }



    cout << "drawing line: " << segment.segment.lindefNumber << endl;

    cout << "x1: " << x1 << "   x2:" << x2 << endl;

    int lineId = segment.segment.lindefNumber;

    LevelData::Linedef line = lines[lineId];
    LevelData::Sidedef side = sides[line.frontSidedef];
    LevelData::Sector sector = sectors[side.sectorNumber];
    LevelData::Vertex startVertex = vertexs[segment.segment.startVertex];
    LevelData::Vertex endVertex = vertexs[segment.segment.endVertex];

    string wallTexture = side.middleTextureName;
    string ceilTexture = side.upperTextureName;
    string floorTexture = side.lowerTextureName;
    int lightLevel = sector.lightLevel;


    int worldFrontZ1 = static_cast<int>(sector.ceilingHeight) - static_cast<int>(player->getHeight());
    int worldFrontZ2 = static_cast<int>(sector.floorHeight) - static_cast<int>(player->getHeight());

    bool drawWall = side.middleTextureName != "-";
    bool drawCeiling = worldFrontZ1 > 0;
    bool drawFloor = worldFrontZ2 < 0;

    float rwNormalAngle = remapValue(segment.segment.angle, -32768, 32767, 0, 360) + 90;
    cout << "pre angle:" << segment.segment.angle << "    rwNormalAngle:" << rwNormalAngle << endl;
    float offsetAngle = rwNormalAngle - segment.rwAngle;

    float hypotenuse = calculateDistance(player->getPosX(), player->getPosY(), startVertex.x, startVertex.y);
    float rwDistance = hypotenuse * cos(radians(offsetAngle));
    cout << "hypotenuse:" << hypotenuse << "   rwDistance: " << rwDistance << endl;

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

    cout << "sclae1:" << rwScale1 << "   step:" << rwScaleStep << endl;

    float wallY1 = (*(engine.getWindowHeight()) / 2) - worldFrontZ1 * rwScale1;
    float wallY1Step = -rwScaleStep * worldFrontZ1;

    float wallY2 = (*(engine.getWindowHeight()) / 2) - worldFrontZ2 * rwScale1;
    float wallY2Step = -rwScaleStep * worldFrontZ2;

    

    

    sf::Color color = getSegmentColor(segment.segment.lindefNumber);

    cout << "lets go to drawing!" << endl;

    for(int i = x1; i < x2 + 1; i ++)
    {
        float drawWallY1 = wallY1 - 1;
        float drawWallY2 = wallY2;

        if(drawCeiling)
        {

        }

        if(drawWall)
        {
            int wy1 = (int) drawWallY1;
            int wy2 = (int) drawWallY2;

            if(wy1 < 0)
            {wy1=0;}

            if(wy2 < 0)
            {wy2=0;}

            cout << i << " " << wy1 << " " << wy2 << " color:" << unsigned(color.r) << "," << unsigned(color.g) << "," << unsigned(color.b) << endl;
            drawVerticalLine(i, wy1, wy2, color);
                
                        
        }

        if(drawFloor)
        {
            
        }

        wallY1 += wallY1Step;
        wallY2 += wallY2Step;

    } 

    if(x1 > 0)
    {
        if(x1 < *(engine.getWindowWidth()))
        {
            drawVerticalLine(x1, 0, 0, sf::Color::Green);
        }
    }
    if(x2 > 0)
    {
        if(x2 < *(engine.getWindowWidth()))
        {
            drawVerticalLine(x2, 0, 0, sf::Color::Red);
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

sf::Color LevelRenderer::getSegmentColor(int segmentId)
{
    std::seed_seq seed_sequence{segmentId};
    std::mt19937 rng(seed_sequence);
    std::uniform_int_distribution<int> distribution(40, 255);

    sf::Color color;
    color.r = distribution(rng);
    color.g = distribution(rng);
    color.b = distribution(rng);
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

        float rwAngle = angle1;
   
        angle1 -= player->getAngle();
        angle2 -= player->getAngle();

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
        if(x1 > 0)
        {
            if(x1 < *(engine.getWindowWidth()))
            {
                drawVerticalLine(x1, 0, 0, sf::Color::Green);
            }
        }
        if(x2 > 0)
        {
            if(x2 < *(engine.getWindowWidth()))
            {
                drawVerticalLine(x2, 0, 0, sf::Color::Red);
            }
        }
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