
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#ifndef DUCTTAPE_ENGINE_TESTS_TEST_GUI
#define DUCTTAPE_ENGINE_TESTS_TEST_GUI

#include <Core/Root.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/GuiWidgetComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

namespace test_gui {

class Main: public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void Click(MyGUI::Widget* _sender) {
        dt::Logger::Get().Debug("Clicked! " + QString::fromStdString(_sender->getName()));
        if(_sender->getName() == "b1") {
            static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
        } else if(_sender->getName() == "b2") {
            static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
        } else if(_sender->getName() == "b3") {
            static_cast<MyGUI::Button*>(_sender)->setCaption("Not implemented!");
        } else if(_sender->getName() == "b4") {
            dt::StateManager::Get()->Pop(1);
        }
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            if(mRuntime > 3.0) {
                dt::StateManager::Get()->Pop(1);
            }
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));

        dt::ResourceManager::Get()->AddResourceLocation("sinbad.zip","Zip", true);
        dt::ResourceManager::Get()->AddResourceLocation("gui","FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        dt::Node* camnode = scene->AddChildNode(new dt::Node("camnode"));
        dt::CameraComponent* cam = camnode->AddComponent(new dt::CameraComponent("cam"));
        camnode->SetPosition(Ogre::Vector3(0, 5, 10));
        cam->LookAt(Ogre::Vector3(0, 0, 0));

        dt::Node* meshnode = scene->AddChildNode(new dt::Node("meshnode"));
        dt::MeshComponent* mesh = new dt::MeshComponent("Sinbad.mesh");
        meshnode->AddComponent(mesh);
        mesh->SetAnimation("Dance");
        mesh->SetLoopAnimation(true);
        mesh->PlayAnimation();

        // GUI
        dt::Node* w1 = scene->AddChildNode(new dt::Node("w1"));
        dt::GuiWidgetComponent* b1 = w1->AddComponent(new dt::GuiWidgetComponent("Button", "Button", "b1"));
        b1->GetWidget<MyGUI::Button>()->setCaption("Campaign");
        b1->GetWidget<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);
        w1->SetPosition(Ogre::Vector3(0.4, 0.15, 0.0));
        w1->SetScale(Ogre::Vector3(0.2, 0.1, 0.0));

        dt::Node* w2 = scene->AddChildNode(new dt::Node("w2"));
        dt::GuiWidgetComponent* b2 = w2->AddComponent(new dt::GuiWidgetComponent("Button", "Button", "b2"));
        b2->GetWidget<MyGUI::Button>()->setCaption("Tutorial");
        b2->GetWidget<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);
        w2->SetPosition(Ogre::Vector3(0.4, 0.35, 0.0));
        w2->SetScale(Ogre::Vector3(0.2, 0.1, 0.0));

        dt::Node* w3 = scene->AddChildNode(new dt::Node("w3"));
        dt::GuiWidgetComponent* b3 = w3->AddComponent(new dt::GuiWidgetComponent("Button", "Button", "b3"));
        b3->GetWidget<MyGUI::Button>()->setCaption("Options");
        b3->GetWidget<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);
        w3->SetPosition(Ogre::Vector3(0.4, 0.55, 0.0));
        w3->SetScale(Ogre::Vector3(0.2, 0.1, 0.0));

        dt::Node* w4 = scene->AddChildNode(new dt::Node("w4"));
        dt::GuiWidgetComponent* b4 = w4->AddComponent(new dt::GuiWidgetComponent("Button", "Button", "b4"));
        b4->GetWidget<MyGUI::Button>()->setCaption("Exit");
        b4->GetWidget<MyGUI::Button>()->eventMouseButtonClick += MyGUI::newDelegate(this, &Main::Click);
        w4->SetPosition(Ogre::Vector3(0.4, 0.75, 0.0));
        w4->SetScale(Ogre::Vector3(0.2, 0.1, 0.0));

    }

private:
    double mRuntime;

};

int Run(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}

} // namespace test_gui

#endif
