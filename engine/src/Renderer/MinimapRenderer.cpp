#include "../../include/Renderer/MinimapRenderer.h"
#include "../../include/Controller/Player.h"

MinimapRenderer::MinimapRenderer(Engine& lEngine, Player *lPlayer) : engine(lEngine)
{
    int shortestLine = *(engine.getWindowWidth());
    if(*(engine.getWindowHeight()) < shortestLine)
    {
        shortestLine = *(engine.getWindowHeight());
    }

    player = lPlayer;
    int size = static_cast<int>(((float) shortestLine * ( (float) *(engine.getMinimapSize()) / 100.0)));
    width = size;
    height = size;
    minimapSize = size;
    level = engine.getLevelData();
    fov = *(engine.getPlayerFOV());
    fovDistance = static_cast<int>(((float) shortestLine * ( (float) *(engine.getMinimapFovDistancePercent()) / 100.0))) * 2;
    hFov = static_cast<int>((float) fov / 2.0);
    offset = static_cast<int>(((float) shortestLine * ( (float) *(engine.getMinimapContentPercentageOffset()) / 100.0)));
    border = static_cast<int>(((float) shortestLine * ( (float) *(engine.getMinimapBorderPercentage()) / 100.0)));

    vertexsBounds = engine.getVertexsBounds();
    vertexs = level->getVertexs();
    lines =  level->getLinedefs();
    nodes =  level->getNodes();
    segs =  level->getSegs();


    setupDrawingBoard();  
    
}


void MinimapRenderer::drawMinimap()
{
    
    clearDrawingBoard();
    drawVertexs();  
    drawLines(); 
    drawOuter();
    drawPlayer();
    drawPlayerFov();
    texture.update(pixels);
    sprite.setTexture(texture);
}

void MinimapRenderer::update()
{
    texture.update(pixels);
    sprite.setTexture(texture);
}

void MinimapRenderer::drawNodeById(int id)
{
    drawNode(nodes[id]);

}

void MinimapRenderer::drawSegById(int id)
{
    //cout << id << endl;
    //cout << segs[id].startVertex << " " << segs[id].endVertex << endl;
    drawSeg(segs[id]);
}



sf::Sprite* MinimapRenderer::getMinimap()
{
    return &(sprite);
}

int MinimapRenderer::getMinimapSize()
{
    return minimapSize;
}


// private

void MinimapRenderer::drawSeg(LevelData::Seg seg)
{   
    int v1 = seg.startVertex;
    int v2 = seg.endVertex;
    LevelData::Vertex vertex1 = vertexs[v1];
    LevelData::Vertex vertex2 = vertexs[v2];
    int x0 = minimapRemap(vertex1.x, false);
    int y0 = minimapRemap(vertex1.y, true);
    int x1 = minimapRemap(vertex2.x, false);
    int y1 = minimapRemap(vertex2.y, true);
    drawSimpleLine(x0, y0, x1, y1, 2, sf::Color::Green);
}

void MinimapRenderer::clearDrawingBoard()
{
    for(int i = 0; i < width*height*4; i += 4) {
        pixels[i] = backgroundColor.r;
        pixels[i+1] = backgroundColor.g;
        pixels[i+2] = backgroundColor.b;
        pixels[i+3] = backgroundColor.a;
    }
}

void MinimapRenderer::setupDrawingBoard()
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

void MinimapRenderer::drawNode(LevelData::Node node)
{

    int x0;
    int x1;
    int y0;
    int y1;

    sf::Color frontColor = sf::Color::Green;
    sf::Color backColor = sf::Color::Red;
    sf::Color partitionColor = sf::Color::Blue;

    int thickness = 3;

    x0 = minimapRemap(node.leftBoxLeft, false);
    y0 = minimapRemap(node.leftBoxBottom, true);
    x1 = minimapRemap(node.leftBoxRight, false);
    y1 = minimapRemap(node.leftBoxTop, true);         

    drawSimpleSquare(x0, y0, x1, y1, thickness, backColor);

    x0 = minimapRemap(node.rightBoxLeft, false);
    y0 = minimapRemap(node.rightBoxBottom, true);
    x1 = minimapRemap(node.rightBoxRight, false);
    y1 = minimapRemap(node.rightBoxTop, true);

    drawSimpleSquare(x0, y0, x1, y1, thickness, frontColor);


    x0 = minimapRemap(node.xPartition, false);
    y0 = minimapRemap(node.yPartition, true);
    x1 = minimapRemap(node.xPartition + node.xPartitionDiff, false);
    y1 = minimapRemap(node.yPartition + node.yPartitionDiff, true);

    drawSimpleLine(x0, y0, x1, y1, thickness, partitionColor);

}

void MinimapRenderer::drawPlayer()
{
    int x = remap(player->getPosX(), vertexsBounds.xPosMin, vertexsBounds.xPosMax, 0, width);
    
    int y = remap(player->getPosY(), vertexsBounds.yPosMin, vertexsBounds.yPosMax, 0, height);
    drawSimpleCircle(x, y, 3, playerColor);
}

void MinimapRenderer::drawPlayerFov()
{
    int thickness = 2;
    sf::Color color = sf::Color::White;

    int x = remap(player->getPosX(), vertexsBounds.xPosMin, vertexsBounds.xPosMax, 0, width);
    int y = remap(player->getPosY(), vertexsBounds.yPosMin, vertexsBounds.yPosMax, 0, height);
    int angle = -player->getAngle() + 90;

    int plusAngle = (float) angle + (float) hFov;
    int minusAngle = (float) angle - (float) hFov;
    
    float minusRads = radians(minusAngle);
    float plusRads = radians(plusAngle);

    float sin_a1 = std::sin(minusRads);
    float cos_a1 = std::cos(minusRads);
    float sin_a2 = std::sin(plusRads);
    float cos_a2 = std::cos(plusRads);

    int x1 = minimapRemap(static_cast<int>((float) player->getPosX() + (float) fovDistance * (float) sin_a1), false);
    int y1 = minimapRemap(static_cast<int>((float) player->getPosY() + (float) fovDistance * (float) cos_a1), true);
    int x2 = minimapRemap(static_cast<int>((float) player->getPosX() + (float) fovDistance * (float) sin_a2), false);
    int y2 = minimapRemap(static_cast<int>((float) player->getPosY() + (float) fovDistance * (float) cos_a2), true);

    drawSimpleLine(x, y, x1, y1, thickness, color);
    drawSimpleLine(x, y, x2, y2, thickness, color);

}


void MinimapRenderer::drawSimpleSquare(int x0, int y0, int x1, int y1, int thickness, sf::Color color)
{
    drawSimpleLine(x0, y0, x1, y0, thickness, color);
    drawSimpleLine(x0, y1, x1, y1, thickness, color);
    drawSimpleLine(x1, y0, x1, y1, thickness, color);
    drawSimpleLine(x0, y0, x0, y1, thickness, color);
}

void MinimapRenderer::drawSimpleLine(int x0, int y0, int x1, int y1, int thickness, sf::Color color)
{

        int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int steps = std::max(dx, dy); // Use the longer dimension for steps

    float xIncrement = static_cast<float>(x1 - x0) / steps;
    float yIncrement = static_cast<float>(y1 - y0) / steps;

    for (int t = 0; t < thickness; ++t)
    {
        float x = static_cast<float>(x0);
        float y = static_cast<float>(y0);

        for (int i = 0; i <= steps; ++i)
        {
            drawPixel(static_cast<int>(x + 0.5), height - static_cast<int>(y + 0.5), color); // Round and draw pixel

            x += xIncrement;
            y += yIncrement;
        }

        // Offset the starting and ending points for the next line
        x0 -= static_cast<int>(xIncrement);
        y0 -= static_cast<int>(yIncrement);
        x1 -= static_cast<int>(xIncrement);
        y1 -= static_cast<int>(yIncrement);
    }

}

void MinimapRenderer::drawSimpleCircle(int centerX, int centerY, int radius, sf::Color color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                drawPixel(centerX + x, height - (centerY + y), color);
            }
        }
    }
}

void MinimapRenderer::drawOuter()
{
    for(int i = 0; i < border; i++)
    {
        for(int x = 0; x < width; x++)
        {
            drawPixel(x, 0 + i, borderColor);
            drawPixel(x, height - i, borderColor);
        }
        for(int y = 0; y < height; y++)
        {
            drawPixel(0 + i, y, borderColor);
            drawPixel(width - i, y, borderColor);
        }
    }
}

void MinimapRenderer::drawVertexs()
{            
    for(LevelData::Vertex vertex: vertexs)
    {
        drawVertex(&(vertex));
    }
}

void MinimapRenderer::drawLines()
{   
    for(LevelData::Linedef line: lines)
    {
        drawLine(&(line));
    }
}

void MinimapRenderer::drawLine(LevelData::Linedef* line)
{
    int start = line->startVertex;
    int end = line->endVertex;
    LevelData::Vertex vertexStart = vertexs[start];
    LevelData::Vertex vertexEnd = vertexs[end];

    int thickness = 1;

    int x0 = remap(vertexStart.x, vertexsBounds.xPosMin, vertexsBounds.xPosMax, 0, width);
    int x1 = remap(vertexEnd.x, vertexsBounds.xPosMin, vertexsBounds.xPosMax, 0, width);
    int y0 = remap(vertexStart.y, vertexsBounds.yPosMin, vertexsBounds.yPosMax, 0, width);
    int y1 = remap(vertexEnd.y, vertexsBounds.yPosMin, vertexsBounds.yPosMax, 0, width);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    for (int t = 0; t < thickness; ++t) {
        int x = x0;
        int y = y0;

        while (true) {
            cout << "x:" << x << "   y:" << y << endl;
            drawPixel( x, height - y, lineColor);

            if (x == x1 && y == y1)
                break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }

        // Offset points for the next thickness level
        x0 -= sx;
        y0 -= sy;
        x1 -= sx;
        y1 -= sy;
    }

}



void MinimapRenderer::drawVertex(LevelData::Vertex* vertex)
{
    int x = vertex->x;
    int y = vertex->y;
    
    int x_remapped = remap(x, vertexsBounds.xPosMin, vertexsBounds.xPosMax, 0, width);
    int y_remapped = remap(y, vertexsBounds.yPosMin, vertexsBounds.yPosMax, 0, height);

    drawCross(x_remapped, y_remapped, 3);
}

void MinimapRenderer::drawCross(int x, int y, int size)
{   
        for (int i = -size; i <= size; ++i) {
        drawPixel(x + i,  height - y, vertexColor);
        drawPixel(x, height - (y + i), vertexColor);
    }      
}

void MinimapRenderer::drawPixel(int x, int y, sf::Color color)
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

int MinimapRenderer::minimapRemap(int value, bool isVertical)
{
    if(isVertical == true)
    {
        return remap(value, vertexsBounds.yPosMin, vertexsBounds.yPosMax, 0, height);
    }
    else
    {
        return remap(value, vertexsBounds.xPosMin, vertexsBounds.xPosMax, 0, width);
    }
}

int MinimapRenderer::remap(int x, int x_min, int x_max, int y_min, int y_max) {
    x_min += offset;
    x_max -= offset;
    y_min += offset;
    y_max -= offset;
    double scaled = static_cast<double>(x - x_min) / (x_max - x_min) * (y_max - y_min) + y_min;
    return static_cast<int>(scaled + 0.5);
}