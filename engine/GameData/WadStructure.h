#include <iostream>
#include <fstream>
#include <bitset>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;


class WADStructure {      
  public: 
    
    struct header_t
    {             
        string identification;  //The ASCII characters "IWAD" or "PWAD". 
        int numlumps;           //An integer specifying the number of lumps in the WAD. 
        int infotableofs;       //An integer holding a pointer to the location of the directory. 
    };

    struct lumpInfo_t 
    {
        string name;
        int filepos;
        int size; 
    };
 

    struct levelInfo_t {
        string name;
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

    struct pwad_t
    {
        string path;
        header_t header;
        vector<lumpInfo_t> directory;
        vector<levelInfo_t> levelsList; 
    };

    string filePath = "";
    header_t header;
    vector<lumpInfo_t> directory;
    vector<levelInfo_t> levelsList; 

    vector<pwad_t> pwads;
    
    bool iWadFound = false;
    bool compiled = false;

    WADStructure() {
    }

    WADStructure(string path){
        readFile(path);
    }

    ~WADStructure() {

    }

   void compile()
    {   
        cout << "compiling wad file..." << endl;

        if(iWadFound == false)
        {
            cout << "IWAD file is not loaded!" << std::endl;
            exit(0);
            return;
        }

        // !!!
        // TODO: this code will fail at replacing levels lumps because the internal lumps of each map is called the same
        // !!!

        vector<string> blockedLumps = {"THINGS", "LINEDEFS", "SIDEDEFS", "VERTEXES", "SEGS", "SSECTORS", "NODES", "SECTORS", "REJECT", "BLOCKMAP"};
        
        for(int x = 0; x < pwads.size(); x++)
        {   
            for(int y = 0; y < pwads[x].directory.size(); y++)
            {

                bool foundBlocked = false; 
                for(int a = 0; a < blockedLumps.size(); a++)
                {         
                    if(pwads[x].directory[y].name == blockedLumps[a])
                    {
                        foundBlocked = true;
                    }
                }
                if(foundBlocked == true)
                {
                    continue;
                }

                if(pwads[x].directory[y].size == 0)
                {
                    continue;
                }

                bool foundReplacement = false;
                for(int z = 0; z < directory.size(); z++)
                {

                    if(pwads[x].directory[y].name != directory[z].name)
                    {
                        continue;
                    }   

                    cout << "replacing \"" << pwads[x].directory[y].name << "\" lump..." << endl;
                    directory[z] = pwads[x].directory[y];
                    foundReplacement = true;
                            
                    
                }
                if(foundReplacement == false)
                {
                    cout << "adding \"" << pwads[x].directory[y].name << "\" lump..." << endl;
                    directory.push_back(pwads[x].directory[y]);
                }
            }
        }

        for(int x = 0; x < pwads.size(); x++)
        {
            for(int y = 0; y < pwads[x].levelsList.size(); y++)
            {
                bool foundReplacement = false;

                for(int z = 0; z < levelsList.size(); z++)
                {
                    if(pwads[x].levelsList[y].name == levelsList[z].name)
                    {
                        cout << "replacing \"" << pwads[x].levelsList[y].name << "\" map..." << endl;
                        foundReplacement = true;
                        levelsList[z] = pwads[x].levelsList[y];
                    }
                }

                if(foundReplacement == false)
                {
                    cout << "adding \"" << pwads[x].levelsList[y].name << "\" map..." << endl;
                    levelsList.push_back(pwads[x].levelsList[y]);
                }
            } 
        }

        cout << "wad compilation successful!" << endl << endl;
        compiled = true;
    }

    int getlevelsAmount()
    {
        return levelsList.size();
    }

    int getDirectoryCount()
    {
        return directory.size();
    }

    void loadPWAD(string path)
    {
        readFile(path);
    }


    lumpInfo_t findLump(string lumpName)
    {
        bool markerFound = false;
        for(int i = 0; i < directory.size();i++)
        {   
            if(lumpName == directory[i].name)
            {
                markerFound = true;
                return directory[i];
            }
        }

        cout << lumpName << " not found in WAD file!" << endl;
        exit(0);
    }


    vector<lumpInfo_t> findLumps(string lumpName)
    {
        vector<lumpInfo_t> lumps;
        for(int i = 0; i < directory.size();i++)
        {
            if(string(directory[i].name).find(lumpName) != std::string::npos)
            {
                lumps.push_back(directory[i]);
            }
        }

        if(lumps.size() > 0)
        {
            return lumps;
        }

        cout << lumpName << " not found in WAD file!" << endl;
        exit(0);
    }

    void printInfo()
    {
        cout << endl << "ENTRIES:" << endl;
        cout << setw(5) << "[ENTRY]" << setw(10) << "[NAME]" << setw(10) << "[SIZE]" << setw(10) << "[FILEPOS]" << endl;
        for (int i = 0; i < header.numlumps; ++i) 
        {
            cout << setw(5) << i + 1;
            cout << setw(10) << directory[i].name;
            cout << setw(10) << directory[i].size;
            cout << setw(10) << directory[i].filepos;
            cout << endl;
        }
    }

    header_t getHeader()
    {
        return header;
    }

    private:

        


        string charsToString(char* chars, int size)
        {   
            string str(chars, size);
            return str.c_str();
        }

        void readFile(string path)
        {
            cout << "reading \"" << path << "\" file..." << endl;
            std::ifstream file(path, std::ios::binary);

            if (!file) {
                cout << "Error opening file." << std::endl;
                exit(0);
                return;
            }

            header_t lHeader = readHeader(&file);

            if(lHeader.identification == "IWAD")
            {
                cout << "detected internal WAD file!" << endl;

            }else if (lHeader.identification == "PWAD")
            {
                cout << "detected patch WAD file!" << endl;
            }else
            {
                cout << "Error while reading \"" << path << "\".";
                cout << " File header identification should be \"IWAD\" or \"PWAD\".";
                cout << " received: \"" << lHeader.identification << "\"" << std::endl;
                exit(0);
                return;
            }

            if(lHeader.identification == "IWAD" && iWadFound == true)
            {
                cout << "Error while reading \"" << path << "\".";
                cout << " IWAD file is already loaded! ";
                exit(0);
                return;
            }

            vector<lumpInfo_t> lDirectory = readDirectory(&file, lHeader);
            vector<levelInfo_t> lLevels = readLevelsList(lHeader, lDirectory);

            if(lHeader.identification == "IWAD")
            {
                header = lHeader;
                filePath = path;
                directory = lDirectory;
                levelsList = lLevels;
                iWadFound = true;

            }else if (lHeader.identification == "PWAD")
            {
                pwad_t pwad;
                pwad.path = path;
                pwad.header = lHeader;
                pwad.directory = lDirectory;
                pwad.levelsList = lLevels;
                pwads.push_back(pwad);
            }

            file.close();

            cout << "file readout successful!" << endl << endl; 
        }

        header_t readHeader(ifstream *file) 
        {  
            struct raw_header_t 
            {
                char identification[4];
                uint32_t numlumps;
                uint32_t infotableofs;           
            };

            header_t lHeader;
            raw_header_t rawHeader;
  
            file->read(reinterpret_cast<char*>(&rawHeader), sizeof(rawHeader)); 

            lHeader.identification = charsToString(rawHeader.identification, 4);
            lHeader.numlumps = rawHeader.numlumps;
            lHeader.infotableofs = rawHeader.infotableofs;

            return lHeader;
        }      

        vector<lumpInfo_t> readDirectory(ifstream *file, header_t lHeader) 
        {  
            vector<lumpInfo_t> lDirectory;
            file->seekg(lHeader.infotableofs);

            struct raw_lumpInfo_t 
            {
                int filepos;
                int size;
                char name[8];
            };

            lumpInfo_t lump;
            raw_lumpInfo_t raw_lump;

            for (int i = 0; i < lHeader.numlumps; i++) 
            {
                file->read(reinterpret_cast<char*>(&raw_lump), sizeof(raw_lumpInfo_t));
                lump.name =  charsToString(raw_lump.name, 8);
                lump.filepos = raw_lump.filepos;
                lump.size = raw_lump.size;
                lDirectory.push_back(lump);           
            }
            return lDirectory;
        }  

        vector<levelInfo_t> readLevelsList(header_t lHeader, vector<lumpInfo_t> lDirectory)
        {
            vector<levelInfo_t> lLevels;
            for (int i = 0; i < lHeader.numlumps; i++) 
            {   

                if(lDirectory[i].size == 0)
                {

                    if(i >= lDirectory.size() - 1)
                    {
                        return lLevels;
                    }

                    int lumpsToScan = lDirectory.size() - i - 1;
                    if(lumpsToScan > 10)
                    {
                        lumpsToScan = 10;
                    }

                    levelInfo_t level;
                    bool isMap = false;
                    int lumpsAmount = 0;

                    level.name = lDirectory[i].name;

                    for (int y = 1; y <= lumpsToScan; y++) 
                    {   

                        if(lDirectory[i+y].name == "THINGS")
                        {
                            level.THINGS = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "LINEDEFS")
                        {
                            level.LINEDEFS = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "SIDEDEFS")
                        {
                            level.SIDEDEFS = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "VERTEXES")
                        {   
                            level.VERTEXES = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "SEGS")
                        {
                            level.SEGS = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "SSECTORS")
                        {
                            level.SSECTORS = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "NODES")
                        {
                            level.NODES = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "SECTORS")
                        {
                            level.SECTORS = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "REJECT")
                        {
                            level.REJECT = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }
                        else if(lDirectory[i+y].name == "BLOCKMAP")
                        {
                            level.BLOCKMAP = lDirectory[i+y];
                            isMap = true;
                            lumpsAmount += 1;
                        }else
                        {
                            continue;
                        }
                    }

                    if(isMap == true)
                    {
                        lLevels.push_back(level);
                        i += lumpsAmount;
                    }             
                }
            }

            return lLevels;

        }
        


};