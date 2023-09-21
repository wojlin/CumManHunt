#include "../../include/Renderer/BSP.h"


BSP::BSP(Engine& lEngine, Player *lPlayer): engine(lEngine)
{
    player = lPlayer;
    level = engine.getLevelData();
    fov = *(engine.getPlayerFOV());
    hFov = (int) ((float) fov / 2.0);
    nodes = level->getNodes();
    subsectors = level->getSubSectors();
    segs = level->getSegs();
    vertexs = level->getVertexs();
    sectors = level->getSectors();
    lines = level->getLinedefs();
    sides = level->getSidedefs();
    traverse = true;

    segmentHandler = new SegmentHandler(engine, player);
}

void BSP::renderBsp()
{
    nodesTree.clear();
    segsTree.clear();
    traverse = true;


    segmentHandler->initScreenRange();

    renderBspNode(nodes.size()-1);
}     

vector<int>* BSP::getNodesTree()
{
    return &nodesTree;
}

vector<int>* BSP::getSegsTree()
{
    return &segsTree;
}

void BSP::setTraverse(bool state)
{
    traverse = state;
}

vector<segmentDrawData>* BSP::getDrawData()
{
    return &segmentHandler->drawData;
}

// private:

bspFov BSP::addSegmentToFOV(pair<int, int> vertex1, pair<int, int> vertex2)
{
    bspFov result;
    result.visible = false;
    result.x1 = 0;
    result.x2 = 0;
    result.rw_angle1 = 0;

    float angle1 = pointToAngle(vertex1);
    float angle2 = pointToAngle(vertex2);
    float span = norm(angle1 - angle2);
    if(span >= 180)
    {
        return result;
    }else
    {
        float rw_angle1 = angle1;

        angle1 -= (float) player->getAngle();
        angle2 -= (float) player->getAngle();

        float span1 = norm(angle1 + hFov);
        if(span1 > fov)
        {
            if(span1 >= span + fov)
            {   
                return result;
            }
            angle1 = hFov;
        }

        float span2 = norm(hFov - angle2);
        if(span2 > fov)
        {
            if(span2 >= span + fov)
            {
                return result;
            }
            angle2 = -hFov;
        }
        
        int x1 = angleToX(angle1);
        int x2 = angleToX(angle2);

        result.visible = true;
        result.x1 = x1;
        result.x2 = x2;
        result.rw_angle1 = rw_angle1;
        return result;
    }
}

int BSP::angleToX(float angle)
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


int BSP::getSubSectorHeight()
{
    int sub_sector_id = nodes.size()-1;

    while(sub_sector_id > 0)
    {
        LevelData::Node* node = &nodes[sub_sector_id];

        bool is_on_back = isOnBackSide(node);
        if(is_on_back)
        {
            sub_sector_id = nodes[sub_sector_id].leftChild;
        }
        else
        {
            sub_sector_id = nodes[sub_sector_id].rightChild;
        }      
    }

    LevelData::SSector sub_sector = subsectors[sub_sector_id + SUB_SECTOR_IDENTIFIER];
    LevelData::Seg seg = segs[sub_sector.firistSegNumber];
    LevelData::Linedef line = lines[seg.lindefNumber];
    LevelData::Sidedef side = sides[line.frontSidedef];
    LevelData::Sector sector = sectors[side.sectorNumber];
    return sector.floorHeight;
}

void BSP::renderSubSector(int subSectorId)
{
    LevelData::SSector subSector = subsectors[subSectorId];
    
    for(int i = 0; i < subSector.segCount; i++)
    {   
        LevelData::Seg seg = segs[subSector.firistSegNumber + i];
        int v1 = static_cast<int>(seg.startVertex);
        int v2 = static_cast<int>(seg.endVertex);
        LevelData::Vertex vertex1 = vertexs[v1];
        LevelData::Vertex vertex2 = vertexs[v2];

        bspFov result = addSegmentToFOV(pair<int,int>(vertex1.x, vertex1.y), pair<int,int>(vertex2.x, vertex2.y));

        if(result.visible == true)
        {
            segmentHandler->classifySegment(seg, result.x1, result.x2, result.rw_angle1);
            segsTree.push_back(subSector.firistSegNumber + i);
        }
        
    }
    
}

bool BSP::checkBbox(int boxBottom, int boxTop, int boxLeft, int boxRight)
{
    std::pair<int, int> a = std::pair(boxLeft, boxBottom);
    std::pair<int, int> b = std::pair(boxLeft, boxTop);
    std::pair<int, int> c = std::pair(boxRight, boxTop);
    std::pair<int, int> d = std::pair(boxRight, boxBottom);
    
    vector<vector<std::pair<int, int>>> bboxSides;

    int px = player->getPosX();
    int py = player->getPosY();

    if(px < boxLeft)
    {
        if(py > boxTop)
        {
            vector<std::pair<int, int>> v1;
            v1.push_back(b);
            v1.push_back(a);
            bboxSides.push_back(v1);
            vector<std::pair<int, int>> v2;
            v2.push_back(c);
            v2.push_back(b);
            bboxSides.push_back(v2);
            // (b, a), (c, b)
        }
        else if(py < boxBottom)
        {
            vector<std::pair<int, int>> v1;
            v1.push_back(b);
            v1.push_back(a);
            bboxSides.push_back(v1);
            vector<std::pair<int, int>> v2;
            v2.push_back(a);
            v2.push_back(d);
            bboxSides.push_back(v2);
            // (b, a), (a, b)
        }else
        {   
            vector<std::pair<int, int>> v1;
            v1.push_back(b);
            v1.push_back(a);
            bboxSides.push_back(v1);
            // (b, a)
        }
    }
    else if(px > boxRight)
    {
        if(py > boxTop)
        {
            vector<std::pair<int, int>> v1;
            v1.push_back(c);
            v1.push_back(b);
            bboxSides.push_back(v1);
            vector<std::pair<int, int>> v2;
            v2.push_back(d);
            v2.push_back(c);
            bboxSides.push_back(v2);
            // (c, b), (d, c)
        }
        else if(py < boxBottom)
        {
            vector<std::pair<int, int>> v1;
            v1.push_back(a);
            v1.push_back(d);
            bboxSides.push_back(v1);
            vector<std::pair<int, int>> v2;
            v2.push_back(d);
            v2.push_back(c);
            bboxSides.push_back(v2);
            // (a, d), (d, c)
        }else
        {
            vector<std::pair<int, int>> v1;
            v1.push_back(d);
            v1.push_back(c);
            bboxSides.push_back(v1);
            // (d, c)
        }
    }
    else
    {
        if(py > boxTop)
        {
            vector<std::pair<int, int>> v1;
            v1.push_back(c);
            v1.push_back(b);
            bboxSides.push_back(v1);
            // (c, b)
        }
        else if(py < boxBottom)
        {
            vector<std::pair<int, int>> v1;
            v1.push_back(a);
            v1.push_back(d);
            bboxSides.push_back(v1);
            // (a, d)
        }else
        {
            return true;
        }
    }

    for(int i = 0; i < bboxSides.size(); i++)
    {
        std::pair<int, int> v1 = bboxSides[i][0];
        std::pair<int, int> v2 = bboxSides[i][1];
        
        float angle1 = pointToAngle(v1);
        float angle2 = pointToAngle(v2);
        float span = norm(angle1 - angle2);
        angle1 -= player->getAngle() + 270;
        float span1 = norm(angle1 + hFov);
        if(span1 > fov)
        {
            if(span1 >= span + (float) fov * 2.0)
            {
                continue;
            }
            return true;
        }
    }     
    //cout << "bounding box:" << endl;
    //cout << "p=(" << a.first << ","<<a.second << ")" << endl;
    //cout << "p=(" << b.first << ","<<b.second << ")" << endl;
    //cout << "p=(" << c.first << ","<<c.second << ")" << endl;
    //cout << "p=(" << d.first << ","<<d.second << ")" << endl;
    //cout << "is outside of player angle of view:"<< endl;
    //cout << "player=(" << player->getPosX() << "," << player->getPosY() << ")" << endl;
    //cout << "angle=(" << player->getAngle() << ")" << endl;
    return false;
}

float BSP::pointToAngle(std::pair<int, int> vertex)
{
    //cout << "######" << endl;
    float deltaX = (float) vertex.first - (float) player->getPosX();
    float deltaY = (float) vertex.second - (float) player->getPosY();
    return degrees(atan2(deltaY, deltaX));
}

float BSP::norm(float angle)
{   
    angle = fmod(angle, 360.0);

    if(angle < 0)
    {
        return angle + 360;
    }else
    {
        return angle;
    }
}

void BSP::renderBspNode(int nodeId)
{ 
    if(traverse)
    {
        //cout << "traversing node: " << nodeId  << endl;
        if(nodeId < 0)
        {
            //cout << "found subsector!" << endl;
            int subSectorId = nodeId + SUB_SECTOR_IDENTIFIER;
            //cout << nodeId << " " <<subSectorId << endl;
            renderSubSector(subSectorId);
            return;
        }

        nodesTree.push_back(nodeId);
        
        LevelData::Node* node = &nodes[nodeId];

        // front = right, back = left

        if(isOnBackSide(node) == true)
        {
            //cout << "player is on back side!" << endl;
            //cout << nodeId << endl;
            renderBspNode(node->leftChild);
            if(checkBbox(node->rightBoxBottom, node-> rightBoxTop, node->rightBoxLeft, node->rightBoxRight))
            {
                renderBspNode(node->rightChild);
            }
            
            
        }
        else
        {
            //cout << "player is on front side!" << endl;
            //cout << nodeId << endl;
            renderBspNode(node->rightChild);
            
            if(checkBbox(node->leftBoxBottom, node-> leftBoxTop, node->leftBoxLeft, node->leftBoxRight))
            {
                renderBspNode(node->leftChild);
            }
            
        }
    }       
    
}

bool BSP::isOnBackSide(LevelData::Node *node)
{   
    int dx = player->getPosX() - node->xPartition;
    int dy = player->getPosY() - node->yPartition;
    return dx * node->yPartitionDiff - dy * node->xPartitionDiff <= 0;
}
