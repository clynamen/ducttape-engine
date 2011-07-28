#include "GuiManager.hpp"

#include "DisplayManager.hpp"
#include "input/InputManager.hpp"

namespace dt {

GuiManager::GuiManager() {
    mPlatform = nullptr;
    mGuiSystem = nullptr;
    mMouseCursorVisible = false;
}

void GuiManager::Initialize() {
    if(mGuiSystem == nullptr) {
        CameraComponent* c = DisplayManager::Get()->GetMainCamera();
        if(c == nullptr || c->GetCamera() == nullptr) {
            // cannot initialize, there is no camera, so there is no viewport
            Logger::Get().Error("Cannot initialize GUI System - no main camera set.");
            return;
        }
        Ogre::SceneManager* scene_mgr = c->GetCamera()->getSceneManager();

        mPlatform = new MyGUI::OgrePlatform();
        mPlatform->initialise(DisplayManager::Get()->GetRenderWindow(), scene_mgr, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        mGuiSystem = new MyGUI::Gui();
        mGuiSystem->initialise();

        EventManager::Get()->AddListener(this);

        // show/hide mouse cursor
        SetMouseCursorVisible(mMouseCursorVisible);
    }
}

void GuiManager::Deinitialize() {
    // make sure to remove the listener anyway!
    EventManager::Get()->RemoveListener(this);

    if(mGuiSystem != nullptr) {
        mGuiSystem->shutdown();
        delete mGuiSystem;
        mGuiSystem = nullptr;

        mPlatform->shutdown();
        delete mPlatform;
        mPlatform = nullptr;
    }
}

void GuiManager::HandleEvent(std::shared_ptr<Event> e) {
    if(mGuiSystem == nullptr)
        return;

    // inject the events into the gui system
    auto mygui_inputmgr = MyGUI::InputManager::getInstancePtr();
    if(mygui_inputmgr == nullptr) {
        return;
    }

    if(e->GetType() == "DT_MOUSEEVENT") {
        std::shared_ptr<MouseEvent> m = std::dynamic_pointer_cast<MouseEvent>(e);
        if(m->GetAction() ==  MouseEvent::MOVED) {
            mygui_inputmgr->injectMouseMove(m->GetMouseState().X.abs,
                                           m->GetMouseState().Y.abs,
                                           m->GetMouseState().Z.abs);
        } else if(m->GetAction() ==  MouseEvent::PRESSED) {
            mygui_inputmgr->injectMousePress(m->GetMouseState().X.abs,
                                             m->GetMouseState().Y.abs,
                                        MyGUI::MouseButton::Enum(m->GetButton()));
        } else if(m->GetAction() ==  MouseEvent::RELEASED) {
            mygui_inputmgr->injectMouseRelease(m->GetMouseState().X.abs,
                                               m->GetMouseState().Y.abs,
                                        MyGUI::MouseButton::Enum(m->GetButton()));
        }
    } else if(e->GetType() == "DT_KEYBOARDEVENT") {
        std::shared_ptr<KeyboardEvent> k = std::dynamic_pointer_cast<KeyboardEvent>(e);
        if(k->GetAction() == KeyboardEvent::PRESSED) {
            mygui_inputmgr->injectKeyPress(MyGUI::KeyCode::Enum(k->GetCode()),
                                           k->GetText());
        } else if(k->GetAction() == KeyboardEvent::RELEASED) {
            mygui_inputmgr->injectKeyRelease(MyGUI::KeyCode::Enum(k->GetCode()));
        }
    }
}

void GuiManager::SetSceneManager(Ogre::SceneManager* scene_manager) {
    if(mPlatform != nullptr) {
        mPlatform->getRenderManagerPtr()->setSceneManager(scene_manager);
    }
}

MyGUI::Gui* GuiManager::GetGuiSystem() {
    // initialize if not already happened
    Initialize();
    return mGuiSystem;
}

void GuiManager::SetMouseCursorVisible(bool visible) {
    mMouseCursorVisible = visible;
    if(mGuiSystem == nullptr && visible) {
        // we have no gui system, but we want to show the cursor
        // so we need to create the GUI system first
        Initialize();
    }

    if(mGuiSystem != nullptr) {
        mGuiSystem->setVisiblePointer(visible);
    }
}

GuiManager* GuiManager::Get() {
    return DisplayManager::Get()->GetGuiManager();
}

}