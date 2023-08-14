#include <iostream>
#include <fstream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <functional>
#include <variant>

#include "WadStructure.h"

#ifndef ENDOOMDATA_H
#define ENDOOMDATA_H
#include "Interfaces.h"
#endif


#ifndef LEVELDATA_H
#define LEVELDATA_H
#include "../Utils/Info.h"
#endif


using namespace std;


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

struct BLOCKMAPHeader 
{
    int16_t originX;
    int16_t originY;
    int16_t numColumns;
    int16_t numRows;
};

struct Blockmap
{
    BLOCKMAPHeader blockmapHeader;
    vector<int16_t> blockmapOffsets;
    vector<vector<uint16_t>> blockmapBlockLists;
};


class LevelData: public baseResourceWAD {      
  public: 
    

    LevelData(){}

    /**
     * @brief Construct a new Level Data object
     * 
     * @param path 
     * @param levelData 
     */
    LevelData(WADStructure *wadObj, WADStructure::levelInfo_t *levelData)
        {
            wad = wadObj;
            level = levelData;

            filePath = wad->filePath;
            levelName = level->name;
            
            
            std::ifstream file(filePath, std::ios::binary);

            things = loadThings(levelData->THINGS);
            linedefs = loadLinedefs(levelData->LINEDEFS);
            sidedefs = loadSidedefs(levelData->SIDEDEFS);
            vertexs = loadVertexes(levelData->VERTEXES);
            segs =  loadSegs(levelData->SEGS);
            subSectors = loadSSectors(levelData->SSECTORS);
            nodes = loadNodes(levelData->NODES);
            sectors = loadSectors(levelData->SECTORS);
            rejectTable = loadReject(levelData->REJECT);
            blockmap = loadBlockmap(levelData->BLOCKMAP);
            

            file.close();
    }

    /**
     * @brief Get the Level Name in string format
     * 
     * @return string 
     */
    string getLevelName()
    {
        return levelName;
    }

    void printInfo()
    {
        printInfoHeader("H1", "LEVEL");
        cout << printInfoTab("H1") << "name: " << levelName << endl;
        cout << printInfoTab("H1") << "things amount: " << things.size() << endl; 
        cout << printInfoTab("H1") << "linedefs amount: " << linedefs.size() << endl; 
        cout << printInfoTab("H1") << "sidedefs amount: " << sidedefs.size() << endl;
        cout << printInfoTab("H1") << "vertex amount: " << vertexs.size() << endl; 
        cout << printInfoTab("H1") << "segs amount: " << segs.size() << endl; 
        cout << printInfoTab("H1") << "subsectors amount: " << subSectors.size() << endl;
        cout << printInfoTab("H1") << "nodes amount: " << nodes.size() << endl;
        cout << printInfoTab("H1") << "sectors amount: " << sectors.size() << endl;
        printInfoHeader("H1");
    }

    void printDetailedInfo()
    {
        printInfoHeader("H1", "LEVEL");

        cout << printInfoTab("H1") << "name: " << levelName << endl;
        cout << printInfoTab("H1") << "things amount: " << things.size() << endl; 
        cout << printInfoTab("H1") << "linedefs amount: " << linedefs.size() << endl; 
        cout << printInfoTab("H1") << "sidedefs amount: " << sidedefs.size() << endl;
        cout << printInfoTab("H1") << "vertex amount: " << vertexs.size() << endl; 
        cout << printInfoTab("H1") << "segs amount: " << segs.size() << endl; 
        cout << printInfoTab("H1") << "subsectors amount: " << subSectors.size() << endl;
        cout << printInfoTab("H1") << "nodes amount: " << nodes.size() << endl;
        cout << printInfoTab("H1") << "sectors amount: " << sectors.size() << endl;

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

        cout << endl << "|REJECT TABLE|" << endl;

        for(int y = 0; y<rejectTable.size(); y++)
        {
            for(int x = 0; x<rejectTable.size(); x++)
            {
                cout << rejectTable[y][x];
            }
            cout << endl;
        }

        cout << endl << "|BLOCKMAP|" << endl;

        cout << "Origin X: " << blockmapHeader.originX << std::endl;
        cout << "Origin Y: " << blockmapHeader.originY << std::endl;
        cout << "Number of Columns: " << blockmapHeader.numColumns << std::endl;
        cout << "Number of Rows: " << blockmapHeader.numRows << std::endl;
        cout << "Offsets:" << endl;
        for(int y = 0; y<blockmapOffsets.size(); y++)
        {
            cout << blockmapOffsets[y] << endl;
        }
        cout << "Block Lists:" << endl;
        for(int y = 0; y<blockmapBlockLists.size(); y++)
        {
            cout << y << ":" << endl;
            for(int x = 0; x<blockmapBlockLists[y].size(); x++)
            {
                cout << "   " << blockmapBlockLists[y][x] << endl;
            }
        }

        printInfoHeader("H1");
    }

    /**
     * @brief Get the Things object
     * 
     * @note you can read more about Things here: https://doomwiki.org/wiki/Thing
     * 
     * @return vector<Thing> 
     */
    vector<Thing> getThings()
    {
        return things;
    }

    /**
     * @brief Get the Linedefs object
     * 
     * @note you can read more about Linedefs here: https://doomwiki.org/wiki/Linedef
     * 
     * @return vector<Linedef>  
     */
    vector<Linedef> getLinedefs()
    {
        return linedefs;
    }

    /**
     * @brief Get the Sidedefs object
     * 
     * @note you can read more about Sidedefs here: https://doomwiki.org/wiki/Sidedef
     * 
     * @return vector<Sidedef>  
     */
    vector<Sidedef> getSidedefs()
    {
        return sidedefs;
    }

    /**
     * @brief Get the Vertexs object
     * 
     * @note you can read more about Vertexs here: https://doomwiki.org/wiki/Vertex
     * 
     * @return vector<Vertex>  
     */
    vector<Vertex> getVertexs()
    {
        return vertexs;
    }

    /**
     * @brief Get the Segs object
     * 
     * @note you can read more about Segs here: https://doomwiki.org/wiki/Seg
     * 
     * @return vector<Seg>  
     */
    vector<Seg> getSegs()
    {
        return segs;
    }

    /**
     * @brief Get the SubSectors object
     * 
     * @note you can read more about SubSectors here: https://doomwiki.org/wiki/Subsector
     * 
     * @return vector<SSector>  
     */
    vector<SSector> getSubSectors()
    {
        return subSectors;
    }

    /**
     * @brief Get the Nodes object
     * 
     * @note you can read more about Nodes here: https://doomwiki.org/wiki/Node
     * 
     * @return vector<Node>  
     */
    vector<Node> getNodes()
    {
        return nodes;
    }

    /**
     * @brief Get the Sectors object
     * 
     * @note you can read more about Sectors here: https://doomwiki.org/wiki/Sector
     * 
     * @return vector<Sector>  
     */
    vector<Sector> getSectors()
    {
        return sectors;
    }

    /**
     * @brief Get the Reject Table object
     * 
     * @note you can read more about Reject Table here: https://doomwiki.org/wiki/Reject
     * 
     * @return vector<vector<bool>> 
     */
    vector<vector<bool>> getRejectTable()
    {
        return rejectTable;
    }

    /**
     * @brief Get the Blockmap object
     * 
     * @note you can read more about Blockmap here: https://doomwiki.org/wiki/Blockmap
     * 
     * @return Blockmap
     */
    Blockmap getBlockmap()
    {
        return blockmap;
    }

    private:
        WADStructure* wad;
        WADStructure::levelInfo_t* level;

        string levelName;
        string filePath = "";

        vector<Thing> things;
        vector<Linedef> linedefs;
        vector<Sidedef> sidedefs;
        vector<Vertex> vertexs;
        vector<Seg> segs;
        vector<SSector> subSectors;
        vector<Node> nodes;
        vector<Sector> sectors;
        vector<vector<bool>> rejectTable;
        Blockmap blockmap;

        BLOCKMAPHeader blockmapHeader;
        vector<int16_t> blockmapOffsets;
        vector<vector<uint16_t>> blockmapBlockLists;

        string loadError = "ERROR: failed to load resource: ";

        void nullTerminate(char* array, size_t maxSize) 
        {
            size_t length = std::strlen(array);
            
            if (length < maxSize) {
                array[length] = '\0';
            }
        }

        vector<Thing> loadThings(WADStructure::lumpInfo_t& lump)
        {
            vector<Thing> lThings;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);

                std::size_t num = lump.size / sizeof(Thing);
                
                for (std::size_t i = 0; i < num; ++i)
                {
                    Thing thing;
                    
                    file.read(reinterpret_cast<char*>(&thing), sizeof(Thing));
                    
                    lThings.push_back(thing);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "THINGS" << endl << "reason: " << e.what() << endl;
                return vector<Thing>();
            }
            return lThings;
        }

        vector<Linedef> loadLinedefs(WADStructure::lumpInfo_t lump)
        {

            vector<Linedef> lLinedefs;
               
            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                std::size_t num = lump.size / sizeof(Linedef);
                for (std::size_t i = 0; i < num; ++i)
                {
                    Linedef linedef;
                    
                    file.read(reinterpret_cast<char*>(&linedef), sizeof(Linedef));
                    
                    lLinedefs.push_back(linedef);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "LINEDEFS" << endl << "reason: " << e.what() << endl;
                return vector<Linedef>();
            }
            return lLinedefs;
        }

        vector<Sidedef> loadSidedefs(WADStructure::lumpInfo_t lump)
        {
            vector<Sidedef> lSidedefs;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                std::size_t num = lump.size / sizeof(Sidedef);
                
                for (std::size_t i = 0; i < num; ++i)
                {
                    Sidedef sidedef;
                    
                    file.read(reinterpret_cast<char*>(&sidedef), sizeof(Sidedef));
                    nullTerminate(sidedef.middleTextureName, 8);
                    nullTerminate(sidedef.upperTextureName, 8);
                    nullTerminate(sidedef.lowerTextureName, 8);
                    lSidedefs.push_back(sidedef);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "SIDEDEFS" << endl << "reason: " << e.what() << endl;
                return vector<Sidedef>();
            }
            return lSidedefs;
        }

        vector<Vertex> loadVertexes(WADStructure::lumpInfo_t lump)
        {
            vector<Vertex> lVertexs;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                std::size_t num = lump.size / sizeof(Vertex);
                
                for (std::size_t i = 0; i < num; ++i)
                {
                    Vertex vertex;
                    
                    file.read(reinterpret_cast<char*>(&vertex), sizeof(Vertex));
                    
                    lVertexs.push_back(vertex);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "VERTEXS" << endl << "reason: " << e.what() << endl;
                return vector<Vertex>();
            }
            return lVertexs;
        }

        vector<Seg> loadSegs(WADStructure::lumpInfo_t lump)
        {
            vector<Seg> lSegs;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                std::size_t num = lump.size / sizeof(Seg);
                
                for (std::size_t i = 0; i < num; ++i)
                {
                    Seg seg;       
                    file.read(reinterpret_cast<char*>(&seg), sizeof(Seg));
                    lSegs.push_back(seg);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "SEGS" << endl << "reason: " << e.what() << endl;
                return vector<Seg>();
            }
            return lSegs;
        }

        vector<SSector> loadSSectors(WADStructure::lumpInfo_t lump)
        {
            vector<SSector> lSubSectors;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                std::size_t num = lump.size / sizeof(SSector);
                
                for (std::size_t i = 0; i < num; ++i)
                {
                    SSector ssector;
                    
                    file.read(reinterpret_cast<char*>(&ssector), sizeof(SSector));
                    
                    lSubSectors.push_back(ssector);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "SSECTORS" << endl << "reason: " << e.what() << endl;
                return vector<SSector>();
            }
            return lSubSectors;
        }

        vector<Node> loadNodes(WADStructure::lumpInfo_t lump)
        {
            vector<Node> lNodes;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                std::size_t num = lump.size / sizeof(Node);
                
                for (std::size_t i = 0; i < num; ++i)
                {
                    Node node;
                    
                    file.read(reinterpret_cast<char*>(&node), sizeof(Node));
                    
                    lNodes.push_back(node);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "NODES" << endl << "reason: " << e.what() << endl;
                return vector<Node>();
            }
            return lNodes;
        }

        vector<Sector> loadSectors(WADStructure::lumpInfo_t lump)
        {
            vector<Sector> lSectors;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                std::size_t num = lump.size / sizeof(Sector);
                
                for (std::size_t i = 0; i < num; ++i)
                {
                    Sector sector;
                    
                    file.read(reinterpret_cast<char*>(&sector), sizeof(Sector));
                    
                    lSectors.push_back(sector);
                }
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "SECTORS" << endl << "reason: " << e.what() << endl;
                return vector<Sector>();
            }
            return lSectors;
        }

        vector<vector<bool>> loadReject(WADStructure::lumpInfo_t lump)
        {
            vector<vector<bool>> lReject;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                int num = lump.size;
                char buffer[num];
                char bits_array[num*8];

                file.read(buffer, num);     
                
                for(int i =0; i<num;i++)
                {
                    unsigned int hexValue = static_cast<unsigned char>(buffer[i]);
                    bitset<8> bits(hexValue);
                    for (size_t i = 0; i < bits.size() / 2; ++i) {
                        bool temp = bits[i];
                        bits[i] = bits[bits.size() - i - 1];
                        bits[bits.size() - i - 1] = temp;
                    }

                    std::string binaryString = bits.to_string();
                    const char* binaryCharArray = binaryString.c_str();

                    for(int x = 0; x <8; x++)
                    {
                        bits_array[(i*8)+x] = binaryCharArray[x];
                    }           
                }

                int sectors_amount = sectors.size();
                rejectTable.resize(sectors_amount, std::vector<bool>(sectors_amount, false));

                for(int y = 0; y<sectors_amount; y++)
                {
                    for(int x = 0; x<sectors_amount; x++)
                    {
                        int index = (y*sectors_amount)+x;
                        bool state = false;
                        if(bits_array[index] == '1')
                        {
                            state = true;
                        }
                        rejectTable[y][x] = state;
                    }
                } 
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "REJECT" << endl << "reason: " << e.what() << endl;
                return vector<vector<bool>>();
            }
            return lReject;  
        }

        Blockmap loadBlockmap(WADStructure::lumpInfo_t lump)
        {
            Blockmap lBlockmap;

            try
            {
                std::ifstream file(lump.path, std::ios::binary);
                file.seekg(lump.filepos);
                file.read(reinterpret_cast<char*>(&blockmapHeader), sizeof(BLOCKMAPHeader));
                int N = blockmapHeader.numColumns * blockmapHeader.numRows;
                int start_offset = lump.filepos + 8;
                int end_offset = start_offset + 2 * (N - 1) + 2;
                int readAmount = end_offset - start_offset;
                file.seekg(start_offset, std::ios::beg);
                for(int i = 0; i < readAmount / 2; i ++)
                {
                    int16_t value;
                    file.read(reinterpret_cast<char*>(&value), 2);
                    blockmapOffsets.push_back(value);
                }

                file.seekg(end_offset, std::ios::beg);

                vector<uint16_t> list;

                int header_and_offests_size = 8 + readAmount;
                int blocklist_size = lump.size - header_and_offests_size;
                for(int i = 0; i < blocklist_size / 2; i ++)
                {
                    uint16_t value;
                    file.read(reinterpret_cast<char*>(&value), 2);
                    //cout << value << endl;
                    if(value == 0)
                    {

                    }else if(value == 65535)
                    {
                        blockmapBlockLists.push_back(list);
                        list.clear();
                    }else
                    {
                        list.push_back(value);
                    }
                }

                lBlockmap.blockmapHeader = blockmapHeader;
                lBlockmap.blockmapBlockLists = blockmapBlockLists;
                lBlockmap.blockmapOffsets = blockmapOffsets;
                file.close();
            }
            catch (const std::exception& e) 
            {
                std::cerr << loadError << "BLOCKMAP" << endl << "reason: " << e.what() << endl;
                return Blockmap();
            }
            return lBlockmap;
        }

};