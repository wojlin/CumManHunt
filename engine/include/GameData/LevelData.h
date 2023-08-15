#ifndef LEVELDATA_H
#define LEVELDATA_H

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
#include "Interfaces.h"


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
    string upperTextureName;
    string lowerTextureName;
    string middleTextureName;
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
    string floorTextureName;
    string ceilingTextureName;
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
    

    LevelData();

    /**
     * @brief Construct a new Level Data object
     * 
     * @param path 
     * @param levelData 
     */
    LevelData(WADStructure *wadObj, WADStructure::levelInfo_t *levelData);

    /**
     * @brief Get the Level Name in string format
     * 
     * @return string 
     */
    string getLevelName();

    /**
     * @brief prints info about level
     * 
     */
    void printInfo();

    /**
     * @brief prints detailed info about level
     * 
     */
    void printDetailedInfo();

    /**
     * @brief Get the Things object
     * 
     * @note you can read more about Things here: https://doomwiki.org/wiki/Thing
     * 
     * @return vector<Thing> 
     */
    vector<Thing> getThings();

    /**
     * @brief Get the Linedefs object
     * 
     * @note you can read more about Linedefs here: https://doomwiki.org/wiki/Linedef
     * 
     * @return vector<Linedef>  
     */
    vector<Linedef> getLinedefs();

    /**
     * @brief Get the Sidedefs object
     * 
     * @note you can read more about Sidedefs here: https://doomwiki.org/wiki/Sidedef
     * 
     * @return vector<Sidedef>  
     */
    vector<Sidedef> getSidedefs();

    /**
     * @brief Get the Vertexs object
     * 
     * @note you can read more about Vertexs here: https://doomwiki.org/wiki/Vertex
     * 
     * @return vector<Vertex>  
     */
    vector<Vertex> getVertexs();

    /**
     * @brief Get the Segs object
     * 
     * @note you can read more about Segs here: https://doomwiki.org/wiki/Seg
     * 
     * @return vector<Seg>  
     */
    vector<Seg> getSegs();

    /**
     * @brief Get the SubSectors object
     * 
     * @note you can read more about SubSectors here: https://doomwiki.org/wiki/Subsector
     * 
     * @return vector<SSector>  
     */
    vector<SSector> getSubSectors();

    /**
     * @brief Get the Nodes object
     * 
     * @note you can read more about Nodes here: https://doomwiki.org/wiki/Node
     * 
     * @return vector<Node>  
     */
    vector<Node> getNodes();

    /**
     * @brief Get the Sectors object
     * 
     * @note you can read more about Sectors here: https://doomwiki.org/wiki/Sector
     * 
     * @return vector<Sector>  
     */
    vector<Sector> getSectors();

    /**
     * @brief Get the Reject Table object
     * 
     * @note you can read more about Reject Table here: https://doomwiki.org/wiki/Reject
     * 
     * @return vector<vector<bool>> 
     */
    vector<vector<bool>> getRejectTable();

    /**
     * @brief Get the Blockmap object
     * 
     * @note you can read more about Blockmap here: https://doomwiki.org/wiki/Blockmap
     * 
     * @return Blockmap
     */
    Blockmap getBlockmap();

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

        void nullTerminate(char* array, size_t maxSize);

        string charsToString(char* chars, int size);

        vector<Thing> loadThings(WADStructure::lumpInfo_t& lump);

        vector<Linedef> loadLinedefs(WADStructure::lumpInfo_t lump);

        vector<Sidedef> loadSidedefs(WADStructure::lumpInfo_t lump);

        vector<Vertex> loadVertexes(WADStructure::lumpInfo_t lump);

        vector<Seg> loadSegs(WADStructure::lumpInfo_t lump);

        vector<SSector> loadSSectors(WADStructure::lumpInfo_t lump);

        vector<Node> loadNodes(WADStructure::lumpInfo_t lump);

        vector<Sector> loadSectors(WADStructure::lumpInfo_t lump);

        vector<vector<bool>> loadReject(WADStructure::lumpInfo_t lump);

        Blockmap loadBlockmap(WADStructure::lumpInfo_t lump);

};

#endif