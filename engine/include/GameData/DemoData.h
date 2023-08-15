#include <map>
#include <math.h>
#include <string>

#ifndef INTERFACES_H
#define INTERFACES_H
#include "Interfaces.h"
#endif

struct demoHeader_t
{
    string version;
    int skillLevel;
    int episode;
    int map;
    bool multiplayerRule;
    bool respawn;
    bool fast;
    bool nomonsters;
    bool recordingPlayer;
    bool greenPlayer;
    bool indigoPlayer;
    bool brownPlayer;
    bool redPlayer;
};

struct tick_t
{
    int movement;
    int strafe;
    int turn;
    bool fireWeapon;
    bool use;
    bool changeWeapon;
    int weaponInUse;
    bool pause;
    bool save;
    int saveSlot;
};


struct demo_t
{
    string name;
    demoHeader_t header;
    vector<tick_t> ticks;
};

class DemoData: public baseResourceWAD
{
    public:

        DemoData()
        {
        }

        DemoData(WADStructure *wad)
        {
            filePath = wad->filePath;
            vector<WADStructure::lumpInfo_t> demoLumps = wad->findLumps("DEMO");
            
            for(int i = 0; i < demoLumps.size(); i++)
            {
                readDEMO(demoLumps[i]);
            }
        }

        vector<demo_t> getDemos()
        {
            return demos;
        }

        demo_t getDemoByName(string name)
        {
            for(int i = 0; i < demos.size(); i++)
            {
                if(demos[i].name.compare(name) == 0)
                {
                    return demos[i];
                }
            }
        }

        demoHeader_t getDemoHeaderByName(string name)
        {
            for(int i = 0; i < demos.size(); i++)
            {
                if(demos[i].name.compare(name) == 0)
                {
                    return demos[i].header;
                }
            }
        }

        vector<tick_t> getDemoTicksByName(string name)
        {
            for(int i = 0; i < demos.size(); i++)
            {
                if(demos[i].name.compare(name) == 0)
                {
                    return demos[i].ticks;
                }
            }
        }

        void printInfo()
        {
            for(int i = 0; i < demos.size(); i++)
            {
                cout << "|" << demos[i].name << "|" << endl;
                cout << "   version: " << demos[i].header.version << endl;
                cout << "   skill level: " << demos[i].header.skillLevel << endl;
                cout << "   episode: " << demos[i].header.episode << endl;
                cout << "   map: " <<demos[i].header.map << endl;   
                cout << "   multiplayerRule: ";
                printBool(demos[i].header.multiplayerRule);
                cout << endl;  
                cout << "   respawn: ";
                printBool(demos[i].header.respawn);
                cout << endl;  
                cout << "   fast: ";
                printBool(demos[i].header.fast);
                cout << endl;  
                cout << "   nomonsters: ";
                printBool(demos[i].header.nomonsters);
                cout << endl;     
                cout << "   recordingPlayer: ";
                printBool(demos[i].header.recordingPlayer);
                cout << endl;     
                cout << "   greenPlayer: ";
                printBool(demos[i].header.greenPlayer);
                cout << endl;    
                cout << "   indigoPlayer: ";
                printBool(demos[i].header.indigoPlayer);
                cout << endl;     
                cout << "   brownPlayer: ";
                printBool(demos[i].header.brownPlayer);
                cout << endl;  
                cout << "   redPlayer: ";
                printBool(demos[i].header.redPlayer);
                cout << endl;  
                cout << "   ticksAmount:" << demos[i].ticks.size() << endl;
                cout << "   recordingTime:" << demos[i].ticks.size() / (float)TICKS_PER_SECOND << "s" << endl;
                
                /*for(int x = 0; x < demos[i].ticks.size(); x++)
                {
                    cout << "movement: " << demos[i].ticks[x].movement << endl;
                    cout << "strafe: " << demos[i].ticks[x].strafe << endl;
                    cout << "turn: " << demos[i].ticks[x].turn << endl;
                    cout << "fireWeapon: " << printBool(demos[i].ticks[x].fireWeapon) << endl;
                    cout << "use: " << printBool(demos[i].ticks[x].use) << endl;
                    cout << "changeWeapon: " << printBool(demos[i].ticks[x].changeWeapon) << endl;
                    cout << "weaponInUse: " << demos[i].ticks[x].weaponInUse << endl;
                    cout << "pause: " << printBool(demos[i].ticks[x].pause) << endl;
                    cout << "save: " << printBool(demos[i].ticks[x].save) << endl;
                    cout << "saveSlot: " << demos[i].ticks[x].saveSlot << endl;
                    cout << endl;  
                }*/
                    
            }
        }


    private:
        string filePath;
        vector<demo_t> demos;
        int TICKS_PER_SECOND = 35;

        void printBool(bool value)
        {
            if(value)
            {
                cout << "true";
            }else
            {
                cout << "false";
            }
        }

        void readDEMO(WADStructure::lumpInfo_t lump)
        {
            cout << "reading \"" << lump.name << "\" lump..." << endl;

            demo_t demo;
            demo.name = lump.name;

            std::ifstream file(filePath, std::ios::binary);
            file.seekg(lump.filepos);


            struct demoHeader_raw_t
            {
                uint8_t version;
                uint8_t skillLevel;
                uint8_t episode;
                uint8_t map;
                uint8_t multiplayerRule;
                uint8_t respawn;
                uint8_t fast;
                uint8_t nomonsters;
                uint8_t recordingPlayer;
                uint8_t greenPlayer;
                uint8_t indigoPlayer;
                uint8_t brownPlayer;
                uint8_t redPlayer;
            };

            struct tick_raw_t
            {
                uint8_t movement;
                uint8_t strafe;
                uint8_t turn;
                uint8_t action;
            };

            demoHeader_raw_t header_raw;


            file.read(reinterpret_cast<char*>(&header_raw), sizeof(header_raw));
   
            std::string numberStr = std::to_string(header_raw.version);
            for (char &c : numberStr) {
                if (c == '0') {
                    c = '.';
                }
            }

            demo.header.version = numberStr;
            demo.header.skillLevel = header_raw.skillLevel;
            demo.header.episode = header_raw.episode;
            demo.header.map = header_raw.map;
            demo.header.multiplayerRule = static_cast<bool>(header_raw.multiplayerRule);
            demo.header.respawn = static_cast<bool>(header_raw.respawn);
            demo.header.fast = static_cast<bool>(header_raw.fast);
            demo.header.nomonsters = static_cast<bool>(header_raw.nomonsters);
            demo.header.recordingPlayer = static_cast<bool>(header_raw.recordingPlayer);
            demo.header.greenPlayer = static_cast<bool>(header_raw.greenPlayer);
            demo.header.indigoPlayer = static_cast<bool>(header_raw.indigoPlayer);
            demo.header.brownPlayer = static_cast<bool>(header_raw.brownPlayer);
            demo.header.redPlayer = static_cast<bool>(header_raw.redPlayer);

            int ticksAmount = (lump.size - sizeof(demoHeader_raw_t) - 1) / sizeof(tick_raw_t);
            

            for(int i =0; i < ticksAmount; i++)
            {

                tick_raw_t tick_raw;
                tick_t tick;

                tick_raw.movement = 0;
                tick_raw.strafe = 0;
                tick_raw.turn = 0;
                tick_raw.action = 0;

                tick.movement = 0;
                tick.strafe = 0;
                tick.turn = 0;
                tick.fireWeapon = false;
                tick.use = false;
                tick.changeWeapon = false;
                tick.weaponInUse = 0;
                tick.pause = false;
                tick.save = false;
                tick.saveSlot = 0;

                file.read(reinterpret_cast<char*>(&tick_raw), sizeof(tick_raw));

                //cout << "raw------ " << i << endl;
                //cout << unsigned(tick_raw.movement) << endl;
                //cout << unsigned(tick_raw.strafe) << endl;
                //cout << unsigned(tick_raw.turn) << endl;
                //cout << "------ " << i << endl;

                

                int bit0 = ((tick_raw.action >> 7) & 1);
                int bit1 = ((tick_raw.action >> 6) & 1);
                int bit2 = ((tick_raw.action >> 5) & 1);
                int bit3 = ((tick_raw.action >> 4) & 1);
                int bit4 = ((tick_raw.action >> 3) & 1);
                int bit5 = ((tick_raw.action >> 2) & 1);
                int bit6 = ((tick_raw.action >> 1) & 1);
                int bit7 = ((tick_raw.action >> 0) & 1);


                bool changedBehaviour = false;

                if(bit7 == 1)
                {
                    changedBehaviour = true;
                }else
                {
                    changedBehaviour = false;
                } 

                if(changedBehaviour == true)
                {
                    tick.fireWeapon = false;
                    tick.use = false;

                    if(bit0 == 1)
                    {
                        tick.pause = true;
                    }else
                    {
                        tick.pause = false;
                    }

                    if(bit1 == 1)
                    {
                        tick.save = true;
                    }else
                    {
                        tick.save = false;
                    }

            
                    tick.changeWeapon = false;
                    tick.weaponInUse = 0;

                    int w_bit1 = bit2;
                    int w_bit2 = bit3;
                    int w_bit3 = bit4;

                    int combinedValue = (bit3 << 2) | (bit2 << 1) | bit1;

                    tick.saveSlot = combinedValue;

                }else
                {
                    tick.pause = false;
                    tick.save = false;

                    if(bit0 == 1)
                    {
                        tick.fireWeapon = true;
                    }else
                    {
                        tick.fireWeapon = false;
                    }

                    if(bit1 == 1)
                    {
                        tick.use = true;
                    }else
                    {
                        tick.use = false;
                    } 

                    if(bit2 == 1)
                    {
                        tick.changeWeapon = true;
                    }else
                    {
                        tick.changeWeapon = false;
                    } 

                    int w_bit1 = bit3;
                    int w_bit2 = bit4;
                    int w_bit3 = bit5;

                    int combinedValue = (bit3 << 2) | (bit2 << 1) | bit1;

                    tick.weaponInUse = combinedValue;

                    tick.movement = tick_raw.movement;
                    tick.strafe = tick_raw.strafe;
                    tick.turn = tick_raw.turn;

                }
                
                //cout << "tick------ " << i << endl;
                //cout << tick.movement << endl;
                //cout << tick.strafe << endl;
                //cout << tick.turn << endl;
                //cout << "------ " << i << endl;

                demo.ticks.push_back(tick);
            }

            uint8_t value;

            file.read(reinterpret_cast<char*>(&value), sizeof(value));

            if(value != 128)
            {
                cout << lump.name << " lump is broked and cannot be read!" << endl;
                demos.push_back(demo);
            }else
            {
                demos.push_back(demo);
            }

        }
};