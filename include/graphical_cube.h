#include <irrlicht.h>
#include <stdexcept>
#include <rubik.h>
#include <string>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

template<size_t CUBE_SIZE>
class GraphicalRubikCube : public RubikCube<CUBE_SIZE> {
private:
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;
    
protected:
    /**
      * @brief Setup scene, including all models, messages, etc
      * @pre Environment and scene manager are initialized
      */
    void PrepareScene();
    
public:
    /**
      * @brief Creates a graphical environment which will show a rubik cube 
      * @param window_title Title of the window
      * @param width Width of the window, in pixels
      * @param height Height of the window, in pixels
      */
    GraphicalRubikCube(const std::wstring &window_title, size_t width, size_t height);
    
    /**
      * @brief It frees resources */
    ~GraphicalRubikCube();
    
    /**
      * @brief Normal case. Window can continue
      * @return False if window should close and program should end. True in any other case
      */
    bool ShouldContinue() const;
    
    /**
      * @brief Draws a new frame
      */
    void UpdateFrame() const;
};





/******* IMPLEMENTATION *******/
template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::PrepareScene() {
    /* Message */
    guienv->addStaticText(L"Hello World! This is Irrlicht with the burnings software renderer!",
                          rect<s32>(10,10,260,22), true);
    
    /* Camera */
    smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
}

template<size_t CUBE_SIZE>
GraphicalRubikCube<CUBE_SIZE>::GraphicalRubikCube(const std::wstring &window_title, size_t width, size_t height) {
    /* Create a device */
    device = createDevice(video::EDT_BURNINGSVIDEO, dimension2d<u32>(width, height), 16, false, false, false, 0);
    if(!device)
        throw std::runtime_error("Error creating a device");
    
    /* Set title */
    device->setWindowCaption(window_title.c_str());
    
    /* Managers */
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    
    /* Prepare scene */
    PrepareScene();
}

template<size_t CUBE_SIZE>
GraphicalRubikCube<CUBE_SIZE>::~GraphicalRubikCube() {
    device->drop();
}

template<size_t CUBE_SIZE>
bool GraphicalRubikCube<CUBE_SIZE>::ShouldContinue() const {
    return device->run();
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::UpdateFrame() const {
    driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(255,100,101,140));
    
    smgr->drawAll();
    guienv->drawAll();
    
    driver->endScene();
}
