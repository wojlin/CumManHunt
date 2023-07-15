#include <map>

struct color_t
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};


class PlayPalData
{
    public:
        PlayPalData(WADStructure *wad)
        {
            filePath = wad->filePath;
            playPalLump = wad->findLump("PLAYPAL");
            readPlayPal();
        }

        color_t getColor(int palleteNumber, uint8_t index)
        {
            return pallete[palleteNumber][index];
        }

        void printColor(int palleteNumber, uint8_t index)
        {
            color_t color = pallete[palleteNumber][index];
            cout << "r:" << unsigned(color.red) << ",   g:"<< unsigned(color.green) << ",   b:" << unsigned(color.blue) << endl;
        }

        void printInfo()
        {
            for(int i = 0; i < PALLETES_AMOUNT; i++)
            {
                cout << "|PALLETE " << i << "|" << endl;
                for(int x = 0; x < PALLETE_COLORS; x++)
                {   
                    cout << x << "   ";
                    printColor(i, x);
                }
            }
        }

    private:
        string filePath;
        WADStructure::lumpInfo_t playPalLump;

        int PALLETE_COLORS = 255;
        int PALLETES_AMOUNT = 14;


        

        vector<vector<color_t>> pallete;

        void readPlayPal()
        {
            cout << playPalLump.name << endl;
            cout << playPalLump.size << endl;
            cout << playPalLump.filepos << endl;

            std::ifstream file(filePath, std::ios::binary);
            file.seekg(playPalLump.filepos);

            for(int i = 0; i < PALLETES_AMOUNT; i++)
            {
                pallete.push_back(vector<color_t>());
                for(int x = 0; x < PALLETE_COLORS; x++)
                {
                    color_t color;
                    file.read(reinterpret_cast<char*>(&color), sizeof(color));  
                    pallete[i].push_back(color);
                }
            }
            file.close();
        }
};