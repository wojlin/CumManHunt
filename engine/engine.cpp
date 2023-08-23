#include "engine.h"




//public

Engine::Engine()
{

}

float* Engine::getDeltaSeconds()
{
    return &deltaSeconds;
}

int* Engine::getPlayerSpeed()
{
    return &PLAYER_SPEED;
}

int* Engine::getPlayerRotationSpeed()
{
    return &PLAYER_ROTATION_SPEED;
}

sf::RenderWindow* Engine::getWindow()
{
    return &window;
}

void Engine::run()
{
    GameData gameData = GameData();
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.compile();

    WADStructure::WADStructure* wad = gameData.getResourceFromWAD<WADStructure::WADStructure>();
    EndoomData::EndoomData* endoom = gameData.getResourceFromWAD<EndoomData::EndoomData>();
    PlayPalData::PlayPalData* playpal = gameData.getResourceFromWAD<PlayPalData::PlayPalData>();
    ColorMapData::ColorMapData* colormap = gameData.getResourceFromWAD<ColorMapData::ColorMapData>();
    ResourcesData::ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData::ResourcesData>();
    AudioInfoData::AudioInfoData* audio = gameData.getResourceFromWAD<AudioInfoData::AudioInfoData>();
    SoundData::SoundData* sound = gameData.getResourceFromWAD<SoundData::SoundData>();
    DemoData::DemoData* demo = gameData.getResourceFromWAD<DemoData::DemoData>();

    gameData.printInfo();

    unique_ptr<LevelData::LevelData> level = gameData.getLevelData(0);
    level->printInfo();

    LevelBuild::LevelBuild levelBuild = LevelBuild::LevelBuild(&level, wad);
    

    vector<Player> players;
    for(int i = 1; i < 5; i++)
    {
        players.push_back(Player(levelBuild.getPlayerInfo(i)));
    }
    
    BSP bsp = BSP(&level, &players[0], FOV);
    bsp.renderBsp();


    MinimapRenderer::MinimapRenderer minimapRenderer = MinimapRenderer::MinimapRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, MINIMAP_SIZE, MINIMAP_CONTENT_PERCENTAGE_OFFSET, MINIMAP_BORFER_PERCENTAGE, FOV, MINIMAP_FOV_DISTANCE_PERCENT, &level, &levelBuild, &players);
    

    setupWindow();
    setupFPS();

    Input input = Input(*this);
            

    while (window.isOpen())
    {
        calculateFPS();                          

        window.clear(sf::Color::Black);

        input.manageInputs(&players[0]);

        bsp.renderBsp();
        minimapRenderer.drawMinimap();

        vector<int> nodes = bsp.getNodesTree();
        vector<int> segs = bsp.getSegsTree();
        for(int i =0; i < segs.size(); i++)
        {
            minimapRenderer.drawSegById(segs[i]);
        }
        //for(int i =0; i < nodes.size(); i++)
        //{
        //    minimapRenderer.drawNodeById(nodes[i]);
        //}
    
        minimapRenderer.update();

        window.draw(*minimapRenderer.getMinimap());
        window.draw(fpsText);
        window.display();       
    }
}



//private

void Engine::setupWindow()
{
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE);
    window.setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setVerticalSyncEnabled(VSYNC);
    window.setFramerateLimit(FRAMERATE_LIMIT);
    window.setMouseCursorVisible(false);
}

void Engine::setupFPS()
{      
    
    if (!fpsFont.loadFromFile("font.ttf")) {
        // Handle font loading error
        return;
    }

    fpsText.setFont(fpsFont);
    fpsText.setCharacterSize(34);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(100.0f, 100.0f);
}

void Engine::calculateFPS()
{
    deltaTime = clock.restart();
    deltaSeconds = deltaTime.asSeconds();
    fpsMeasure = 1.0f / deltaSeconds;

    std::ostringstream ss;
    ss << "FPS: " << static_cast<int>(fpsMeasure);
    fpsText.setString(ss.str());
}



// main code

int main()
{
    Engine engine = Engine();
    engine.run();

    return 0;
}





