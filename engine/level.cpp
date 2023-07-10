#include <iostream>
#include <fstream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>

#include "wad.h"

using namespace std;


class Level {      
  public: 
    string filePath = "";
    char levelName[9];

    struct Thing
    {
        int16_t x;
        int16_t y;
        int16_t angle;
        int16_t type;
        int16_t flags;
    };

    struct Linedef
    {
        int16_t startVertex;
        int16_t endVertex;
        int16_t flags;
        int16_t specialType;
        int16_t sectorTag;
        int16_t frontSidedef;
        int16_t backSidedef;
    };

    struct Sidedef
    {
        int16_t xOffset;
        int16_t yOffset;
        char upperTextureName[8];
        char lowerTextureName[8];
        char middleTextureName[8];
        int16_t sectorNumber;
    };

    struct Vertex
    {
        int16_t x;
        int16_t y;
    };

    struct Seg
    {
        int16_t startVertex;
        int16_t endVertex;
        int16_t angle;
        int16_t lindefNumber;
        int16_t direction;
        int16_t offset;
    };

    struct SSector
    {
        int16_t segCount;
        int16_t firistSegNumber;
    };

    struct Node
    {
        int16_t xPartition;
        int16_t yPartition;
        int16_t xPartitionDiff;
        int16_t yPartitionDiff;
        int16_t rightBoxTop;
        int16_t rightBoxBottom;
        int16_t rightBoxLeft;
        int16_t rightBoxRight;
        int16_t leftBoxTop;
        int16_t leftBoxBottom;
        int16_t leftBoxLeft;
        int16_t leftBoxRight;
        int16_t rightChild;
        int16_t leftChild;
    };

    struct Sector
    {
        int16_t floorHeight;
        int16_t ceilingHeight;
        char floorTextureName[8];
        char ceilingTextureName[8];
        int16_t lightLevel;
        int16_t specialType;
        int16_t tagNumber;
    };

    vector<Thing> things;
    vector<Linedef> linedefs;
    vector<Sidedef> sidedefs;
    vector<Vertex> vertexs;
    vector<Seg> segs;
    vector<SSector> subSectors;
    vector<Node> nodes;
    vector<Sector> sectors;

    

    Level(string path, WAD::levelInfo_t levelData)
    {
        filePath = path;
        std::ifstream file(filePath, std::ios::binary);

        strcpy(levelName, levelData.name);

        
        loadThings(&file, levelData.THINGS);
        loadLinedefs(&file, levelData.LINEDEFS);
        loadSidedefs(&file, levelData.SIDEDEFS);
        loadVertexes(&file, levelData.VERTEXES);
        loadSegs(&file, levelData.SEGS);
        loadSSectors(&file, levelData.SSECTORS);
        loadNodes(&file, levelData.NODES);
        loadSectors(&file, levelData.SECTORS);
        loadReject(&file, levelData.REJECT);
        loadBlockmap(&file, levelData.BLOCKMAP);
        

        file.close();
    }

    void loadThings(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(Thing);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            Thing thing;
            
            file->read(reinterpret_cast<char*>(&thing), sizeof(Thing));
            
            things.push_back(thing);
        }
    }

    void loadLinedefs(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(Linedef);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            Linedef linedef;
            
            file->read(reinterpret_cast<char*>(&linedef), sizeof(Linedef));
            
            linedefs.push_back(linedef);
        }
    }

    void loadSidedefs(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(Sidedef);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            Sidedef sidedef;
            
            file->read(reinterpret_cast<char*>(&sidedef), sizeof(Sidedef));
            
            sidedefs.push_back(sidedef);
        }
    }

    void loadVertexes(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(Vertex);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            Vertex vertex;
            
            file->read(reinterpret_cast<char*>(&vertex), sizeof(Vertex));
            
            vertexs.push_back(vertex);
        }
    }

    void loadSegs(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(Seg);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            Seg seg;
            
            file->read(reinterpret_cast<char*>(&seg), sizeof(Seg));
            
            segs.push_back(seg);
        }
    }

    void loadSSectors(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(SSector);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            SSector ssector;
            
            file->read(reinterpret_cast<char*>(&ssector), sizeof(SSector));
            
            subSectors.push_back(ssector);
        }
    }

    void loadNodes(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(Node);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            Node node;
            
            file->read(reinterpret_cast<char*>(&node), sizeof(Node));
            
            nodes.push_back(node);
        }
    }

    void loadSectors(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        std::size_t num = lump.size / sizeof(Sector);
        
        for (std::size_t i = 0; i < num; ++i)
        {
            Sector sector;
            
            file->read(reinterpret_cast<char*>(&sector), sizeof(Sector));
            
            sectors.push_back(sector);
        }
    }

    void loadReject(ifstream *file, WAD::lumpInfo_t lump)
    {
        file->seekg(lump.filepos);
        int num = lump.size;
        cout << num << endl;

        char buffer[num];
        file->read(buffer, num);

        cout << "read" << endl;
        for(int i =0; i<num;i++)
        {
            unsigned int hexValue = static_cast<unsigned char>(buffer[i]);
            bitset<8> bits(hexValue);
            for (size_t i = 0; i < bits.size() / 2; ++i) {
                bool temp = bits[i];
                bits[i] = bits[bits.size() - i - 1];
                bits[bits.size() - i - 1] = temp;
            }
            cout << hexValue << " -> " << bits << std::endl;            
        }
        cout << endl;

    }

    void loadBlockmap(ifstream *file, WAD::lumpInfo_t lump)
    {

    }

    void printLevelInfo()
    {

        cout << "name: " << levelName << endl;
        cout << "things amount: " << things.size() << endl; 
        cout << "linedefs amount: " << linedefs.size() << endl; 
        cout << "sidedefs amount: " << sidedefs.size() << endl;
        cout << "vertex amount: " << vertexs.size() << endl; 
        cout << "segs amount: " << segs.size() << endl; 
        cout << "subsectors amount: " << subSectors.size() << endl;
        cout << "nodes amount: " << nodes.size() << endl;
        cout << "sectors amount: " << sectors.size() << endl;

        cout << endl << "|THINGS|" << endl;
        cout << setw(10) << "x" << setw(10) << "y" << setw(10) << "angle" << setw(10) << "type" << setw(10) << "flags" << endl;
        for(int x =0; x<things.size(); x++)
        {
            cout << setw(10) << things[x].x;
            cout << setw(10) << things[x].y;
            cout << setw(10) << things[x].angle;
            cout << setw(10) << things[x].type;
            cout << setw(10) << things[x].flags << endl;
        }

        cout << endl << "|LINEDEFS|" << endl;
        cout << setw(10) << "vertex1" << setw(10) << "vertex2" << setw(10) << "flags" << setw(10) << "special" << setw(10) << "sector tag" << setw(10) << "front side" << setw(10) << "back side" << endl;
        for(int x =0; x< linedefs.size(); x++)
        {
            cout << setw(10) << linedefs[x].startVertex;
            cout << setw(10) << linedefs[x].endVertex;
            cout << setw(10) << linedefs[x].flags;
            cout << setw(10) << linedefs[x].specialType;
            cout << setw(10) << linedefs[x].sectorTag;
            cout << setw(10) << linedefs[x].frontSidedef;
            cout << setw(10) << linedefs[x].backSidedef << endl;
        }


        cout << endl << "|SIDEDEFS|" << endl;
        cout << setw(10) << "x" << setw(10) << "y" << setw(10) << "upper" << setw(10) << "lower" << setw(10) << "middle" << setw(10) << "sector" << endl;
        for(int x =0; x< sidedefs.size(); x++)
        {
            cout << setw(10) << sidedefs[x].xOffset;
            cout << setw(10) << sidedefs[x].yOffset;
            cout << setw(10) << sidedefs[x].upperTextureName;
            cout << setw(10) << sidedefs[x].lowerTextureName;
            cout << setw(10) << sidedefs[x].middleTextureName;
            cout << setw(10) << sidedefs[x].sectorNumber << endl;
        }

        cout << endl << "|VERTEXS|" << endl;
        cout << setw(10) << "x" << setw(10) << "y" << endl;
        for(int x =0; x< vertexs.size(); x++)
        {
            cout << setw(10) << vertexs[x].x;
            cout << setw(10) << vertexs[x].y << endl;
        }


        cout << endl << "|SEGS|" << endl;
        cout << setw(10) << "vector1" << setw(10) << "vector2" << setw(10) << "angle" << setw(10) << "line" << setw(10) << "side" << setw(10) << "offset" << endl;
        for(int x =0; x< segs.size(); x++)
        {
            cout << setw(10) << segs[x].startVertex;
            cout << setw(10) << segs[x].endVertex;
            cout << setw(10) << segs[x].angle;
            cout << setw(10) << segs[x].lindefNumber;
            cout << setw(10) << segs[x].direction;
            cout << setw(10) << segs[x].offset << endl;
        }

        cout << endl << "|SSECTORS|" << endl;
        cout << setw(10) << "count" << setw(10) << "first" << endl;
        for(int x =0; x< subSectors.size(); x++)
        {
            cout << setw(10) << subSectors[x].segCount;
            cout << setw(10) << subSectors[x].firistSegNumber << endl;
        }

        cout << endl << "|NODES|" << endl;
        cout << setw(8) << "x" << setw(8) << "y" << setw(8) << "xDiff" << setw(8) << "yDiff" << setw(8) << "rTop" << setw(8) << "rBottom" << setw(8) << "rLeft" << setw(8) << "rRight" << setw(8) << "lTop" << setw(8) << "lBottom" << setw(8) << "lLeft" << setw(8) << "lRight" << setw(8) << "rChild" << setw(8) << "lChild" << endl;
        for(int x =0; x< nodes.size(); x++)
        {
            cout << setw(8) << nodes[x].xPartition;
            cout << setw(8) << nodes[x].yPartition;
            cout << setw(8) << nodes[x].xPartitionDiff;
            cout << setw(8) << nodes[x].yPartitionDiff;
            cout << setw(8) << nodes[x].rightBoxTop;
            cout << setw(8) << nodes[x].rightBoxBottom;
            cout << setw(8) << nodes[x].rightBoxLeft;
            cout << setw(8) << nodes[x].rightBoxRight;
            cout << setw(8) << nodes[x].leftBoxTop;
            cout << setw(8) << nodes[x].leftBoxBottom;
            cout << setw(8) << nodes[x].leftBoxLeft;
            cout << setw(8) << nodes[x].leftBoxRight;
            cout << setw(8) << nodes[x].rightChild;
            cout << setw(8) << nodes[x].leftChild << endl;
        }

        cout << endl << "|SECTORS|" << endl;
        cout << setw(10) << "fHeight" << setw(10) << "cHeight" << setw(20) << "fTexture" << setw(20) << "cTexture" << setw(10) << "light" << setw(10) << "special" << setw(10) << "tag" << setw(10) << endl;
        for(int x =0; x< sectors.size(); x++)
        {
            cout << setw(10) << sectors[x].floorHeight;
            cout << setw(10) << sectors[x].ceilingHeight;
            cout << setw(20) << sectors[x].floorTextureName;
            cout << setw(20) << sectors[x].ceilingTextureName;
            cout << setw(10) << sectors[x].lightLevel;
            cout << setw(10) << sectors[x].specialType;
            cout << setw(10) << sectors[x].tagNumber << endl;
        }

    }

};


int main()
{
    WAD wad = WAD("DOOM2.WAD");
    std::cout << "Path: " << wad.filePath << std::endl;
    cout << endl << "HEADER:" << endl;
    std::cout << "Identification: " << wad.header.identification << std::endl;
    std::cout << "Numlumps: " << wad.header.numlumps << std::endl;
    std::cout << "Infotableofs: " << wad.header.infotableofs << std::endl;
    cout << endl << "INFO:" << endl;
    cout << "levels amount: " << wad.levelsAmount << endl;

    cout << endl << "LEVEL:" << endl;
    Level level = Level(wad.filePath, wad.levelsList[0]);
    
    
    level.printLevelInfo();
    

    /*
    cout << endl << "ENTRIES:" << endl;
    cout << setw(5) << "[ENTRY]" << setw(10) << "[NAME]" << setw(10) << "[SIZE]" << setw(10) << "[FILEPOS]" << endl;
    for (int i = 0; i < wad.header.numlumps; ++i) {
        cout << setw(5) << i + 1;
        cout << setw(10) << wad.directory[i].name;
        cout << setw(10) << wad.directory[i].size;
        cout << setw(10) << wad.directory[i].filepos;
        cout << endl;
    }
    */


    /*
    for (size_t i = 0; i < wad.levelsAmount; ++i) {
        cout << wad.levelsList[i].name << endl;
    }
    */
   
    /*
    int num = wad.directory.filepos;

    unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&num);

    // Output each byte in hexadecimal format
    for (int i = 0; i < sizeof(num); ++i) {
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(bytePtr[i]) << " ";
    }

    std::cout << std::endl;
    */

}