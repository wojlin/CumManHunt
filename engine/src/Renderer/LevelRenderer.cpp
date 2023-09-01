#include "../../include/Renderer/LevelRenderer.h"

LevelRenderer::LevelRenderer(Engine& lEngine, Player *lPlayer): engine(lEngine)
{
    player = lPlayer;
    level = engine.getLevelData();
    vertexs = level->getVertexs();
    lines = level->getLinedefs();
    nodes = level->getNodes();
    segs = level->getSegs();
    width = (*engine.getWindowWidth());
    height = (*engine.getWindowHeight());

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

void LevelRenderer::drawData(vector<segmentDrawData>* drawData)
{
    for(int i = 0; i < drawData->size(); i++)
    {
        segmentDrawData segment = (*drawData)[i];
        int x1 = segment.x1;
        int x2 = segment.x2;

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
    for(int i = bottomOffset; i < height - topOffset; i++)
    {   
        //cout << i << endl;
        drawPixel(posX, i, color);
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