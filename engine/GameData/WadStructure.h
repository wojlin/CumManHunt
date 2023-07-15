#include <iostream>
#include <fstream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;


class WADStructure {      
  public: 
    bool parsed = false;
    string filePath = "";

    struct {             
        string identification;  //The ASCII characters "IWAD" or "PWAD". 
        int numlumps;           //An integer specifying the number of lumps in the WAD. 
        int infotableofs;       //An integer holding a pointer to the location of the directory. 
        }header;     

    struct lumpInfo_t {
        int filepos;
        int size;
        char name[9];
    };

    struct levelInfo_t {
        char name[9];
        lumpInfo_t THINGS;
        lumpInfo_t LINEDEFS;
        lumpInfo_t SIDEDEFS;
        lumpInfo_t VERTEXES;
        lumpInfo_t SEGS;
        lumpInfo_t SSECTORS;
        lumpInfo_t NODES;
        lumpInfo_t SECTORS;
        lumpInfo_t REJECT;
        lumpInfo_t BLOCKMAP;
    };

    lumpInfo_t* directory;
    vector<levelInfo_t> levelsList; 
    int levelsAmount;
    int directoryCount = 0;


    WADStructure(string path){
        filePath = path;
        std::ifstream file(path, std::ios::binary);

        if (!file) {
            cout << "Error opening file." << std::endl;
            exit(0);
            return;
        }

        readHeader(&file);
        readDirectory(&file);
        readLevelsList();

        parsed = true;

        file.close();
    }

    ~WADStructure() {
        free(directory);
    }

    lumpInfo_t findLump(string lumpName)
    {
        bool markerFound = false;
        for(int i = 0; i < directoryCount;i++)
        {
            if(lumpName == directory[i].name)
            {
                markerFound = true;
                return directory[i];
            }
        }

        cout << "COLORMAP not found in WAD file!" << endl;
        exit(0);
    }

    private:


        void readHeader(ifstream *file) {  
        char identification[4];
            int numlumps;
            int infotableofs;

            file->read(reinterpret_cast<char*>(&identification), sizeof(identification));
            file->read(reinterpret_cast<char*>(&numlumps), sizeof(numlumps));
            file->read(reinterpret_cast<char*>(&infotableofs), sizeof(infotableofs));

            header.identification = identification;
            header.numlumps = numlumps;
            header.infotableofs = infotableofs;
        }      

        void readDirectory(ifstream *file) {  

            const int entrySize = 16;  // Length of each entry
            int numEntries = header.numlumps;  // Number of entries in the directory
            int startPos = header.infotableofs;


            directory = (lumpInfo_t*)malloc(numEntries * sizeof(lumpInfo_t));
            if (!directory) {
                cerr << "Failed to allocate memory." << endl;
                file->close();
            }

            file->seekg(startPos);

            for (int i = 0; i < numEntries; ++i) {
                char entryData[entrySize];
                file->read(entryData, entrySize);
                directoryCount++;
                memcpy(&directory[i].filepos, entryData, sizeof(int));
                memcpy(&directory[i].size, entryData + sizeof(int), sizeof(int));
                memcpy(directory[i].name, entryData + 2 * sizeof(int), 8);
                directory[i].name[8] = '\0';  // Ensure null termination
            }

        }  

        void readLevelsList()
        {
            

            for (int i = 0; i < header.numlumps; i++) 
            {   
                
                if(directory[i].size == 0)
                {
                    levelInfo_t level;
                    strcpy(level.name, directory[i].name);

                    for (int y = 1; y <= 10; y++) 
                    {   

                        if(strcmp("THINGS", directory[i+y].name) == 0)
                        {
                            level.THINGS = directory[i+y];
                        }
                        else if(strcmp("LINEDEFS", directory[i+y].name) == 0)
                        {
                            level.LINEDEFS = directory[i+y];
                        }
                        else if(strcmp("SIDEDEFS", directory[i+y].name) == 0)
                        {
                            level.SIDEDEFS = directory[i+y];
                        }
                        else if(strcmp("VERTEXES", directory[i+y].name) == 0)
                        {   
                            level.VERTEXES = directory[i+y];
                        }
                        else if(strcmp("SEGS", directory[i+y].name) == 0)
                        {
                            level.SEGS = directory[i+y];
                        }
                        else if(strcmp("SSECTORS", directory[i+y].name) == 0)
                        {
                            level.SSECTORS = directory[i+y];
                        }
                        else if(strcmp("NODES", directory[i+y].name) == 0)
                        {
                            level.NODES = directory[i+y];
                        }
                        else if(strcmp("SECTORS", directory[i+y].name) == 0)
                        {
                            level.SECTORS = directory[i+y];
                        }
                        else if(strcmp("REJECT", directory[i+y].name) == 0)
                        {
                            level.REJECT = directory[i+y];
                        }
                    else if(strcmp("BLOCKMAP", directory[i+y].name) == 0)
                        {
                            level.BLOCKMAP = directory[i+y];
                        }else
                        {
                            continue;
                        }
                    }

                    levelsList.push_back(level);
                    i += 10;

                    if(directory[i+1].size != 0)
                    {
                        break;
                    }               
                }
            }

            levelsAmount = levelsList.size();

        }
        


};