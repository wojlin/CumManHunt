#include <map>

class PlayPalData
{
    public:
        PlayPalData(WADStructure *wad)
        {
            filePath = wad->filePath;
            playPalLump = wad->findLump("PLAYPAL");
            readPlayPal();
        }



    private:
        string filePath;
        WADStructure::lumpInfo_t playPalLump;

        void readPlayPal()
        {
            cout << playPalLump.name << endl;
            cout << playPalLump.size << endl;
            cout << playPalLump.filepos << endl;

            std::ifstream file(filePath, std::ios::binary);
            file.seekg(playPalLump.filepos);
            file.close();
        }
};