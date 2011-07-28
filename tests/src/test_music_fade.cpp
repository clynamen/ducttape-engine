
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Root.hpp"
#include "game/Game.hpp"

#include "component/SoundComponent.hpp"
#include "component/MusicComponent.hpp"

#include "event/MusicPauseEvent.hpp"
#include "event/MusicStartEvent.hpp"
#include "event/MusicStopEvent.hpp"

#include "scene/Node.hpp"
#include "scene/Scene.hpp"

class Main : public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 5.0) {
                dt::EventManager::Get()->InjectEvent(std::make_shared<dt::MusicStopEvent>());
                dt::StateManager::Get()->Pop(1);
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        std::string music1 = "test_music_intro.ogg";
        std::string music2 = "test_music_loop.ogg";

        dt::MusicComponent* music_component1 = new dt::MusicComponent(music1);
        dt::MusicComponent* music_component2 = new dt::MusicComponent(music2);

        auto node = scene->AddChildNode(new dt::Node("music_node"));
        node->AddComponent(music_component1);
        node->AddComponent(music_component2);

        auto resmgr = dt::ResourceManager::Get();
        float origin_vol = 100.0f;

        resmgr->GetMusicFile(music1)->SetVolume(origin_vol);
        music_component1->Fade(5.0, 0.0f);

        resmgr->GetMusicFile(music2)->SetVolume(0.0f);
        music_component2->Fade(5.0, origin_vol);
    }

private:
    double mRuntime;

};

int main(int argc, char* argv[]) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}
