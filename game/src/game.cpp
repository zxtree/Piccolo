#include "game.h"
#include "assert.h"

namespace Piccolo
{
    PiccoloGame::PiccoloGame() 
    {

    }

    PiccoloGame::~PiccoloGame()
    {

    }

    void PiccoloGame::initialize(PiccoloEngine* engine_runtime) 
    {
        assert(engine_runtime);

        g_is_editor_mode = false;
        m_engine_runtime = engine_runtime;

        net_node.init();
    }

    void PiccoloGame::clear() {}

    void PiccoloGame::run()
    {
        assert(m_engine_runtime);

        float delta_time;

        while (true)
        {
            delta_time = m_engine_runtime->calculateDeltaTime();



            if(!m_engine_runtime->tickOneFrame(delta_time))
                return;
        }
        

    }
}

int main(int argc, char** argv)
{
    std::filesystem::path executable_path(argv[0]);
    std::filesystem::path config_file_path = executable_path.parent_path() / "PiccoloEditor.ini";

    Piccolo::PiccoloEngine* engine = new Piccolo::PiccoloEngine();

    engine->startEngine(config_file_path.generic_string());
    engine->initialize();

    Piccolo::PiccoloGame* game = new Piccolo::PiccoloGame();
    game->initialize(engine);

    game->run();

    game->clear();

    engine->clear();
    engine->shutdownEngine();

    return 0;
}