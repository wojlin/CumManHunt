class ColorMapData
{
    public:
        ColorMapData(WADStructure *wad)
        {
            filePath = wad->filePath;
            colormapLump = wad->findLump("COLORMAP");
            readColormap();
        }



    private:
        string filePath;
        WADStructure::lumpInfo_t colormapLump;

        void readColormap()
        {
            cout << colormapLump.name << endl;
            cout << colormapLump.size << endl;
            cout << colormapLump.filepos << endl;
        }
};