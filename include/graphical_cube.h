#include <irrlicht.h>
#include <stdexcept>
#include <string>
#include <rubik.h>
#include <event_handler.h>
#include <irrlicht_tools.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

template<size_t CUBE_SIZE>
class GraphicalRubikCube : public RubikCube<CUBE_SIZE> {
private:
    typedef enum {
                  IDLE,
                  MOVE_CAMERA,
                  ROTATE_LAYER
    } State;
    
    typedef struct {
        irr::core::position2di cursor_pos;
    } StateVariables;
    
    static constexpr double CAMERA_DISTANCE = 40.0;
    
    double frame_duration = 0.0f;
    double last_time = 0.0f;
    
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;
    RubikCubeEventHandler event_handler;

    State curr_state;
    StateVariables curr_state_variables;
    
    ICameraSceneNode *camera;
    float camera_pitch = 0.0f;
    float camera_yaw = 0.0f;
    float camera_pitch_sensitivity = 2.0f;
    float camera_yaw_sensitivity = 2.0f;
    ISceneNode *cube; /* TODO */
    
protected:
    /**
      * @brief Setup scene, including all models, messages, etc
      * @pre Environment and scene manager are initialized
      */
    void PrepareScene();
    
    /**
      * @brief Reads current state of the frame. It is used by state machine.
      * @return Current state. 
      */
    StateVariables ReadState() const;
    
    /**
      * @brief It reads player events and it acts on them
      */
    void UpdateEvents();
    
    /**
      * @brief Updates camera
      * @param next State variables of current frame
      */
    void UpdateCamera(const StateVariables &next);
    
    /**
      * @brief Updates rotation of layer
      * @param next State variables of current frame
      */
    void UpdateRotateLayer(const StateVariables &next);
    
    /**
      * @brief Returns selected face, or invalid if any face is selected
      * @param pos Position, in pixels
      * @return face: Selected face, or INVALID if any face is selected
      * @return coords: Coordinates of the element in the selected face. Only valid if 'face' isn't INVALID
      */
    void SelectedFace(const irr::core::position2di &pos, FaceElement &face, irr::core::position2di &coords);
    
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
    void UpdateFrame();
};





/******* IMPLEMENTATION *******/
template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::PrepareScene() {
    /* Message */
    guienv->addStaticText(L"Hello World! This is Irrlicht with the burnings software renderer!",
                          rect<s32>(10,10,260,22), true);
    
    
    /* Models */
    /* TODO */
    cube = smgr->addCubeSceneNode();
    
    
    /* Camera */
    camera = smgr->addCameraSceneNode(0, vector3df(0,0,-40));
    camera->bindTargetAndRotation(true);
    SetCameraAngles(camera, 0.0f, 0.0f, cube->getPosition(), CAMERA_DISTANCE);
}

template<size_t CUBE_SIZE>
typename GraphicalRubikCube<CUBE_SIZE>::StateVariables
GraphicalRubikCube<CUBE_SIZE>::ReadState() const {
    StateVariables state_variables;
    state_variables.cursor_pos = event_handler.GetCursorPosition();
    return state_variables;
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::UpdateEvents() {
    bool left_pressed = event_handler.GetLeftMousePressed();
    StateVariables next_state_variables = ReadState();
    State next_state;
    FaceElement selected;
    irr::core::position2di coords;
    
    SelectedFace(curr_state_variables.cursor_pos, selected, coords);
    
    switch(curr_state) {
    case IDLE:
        if(left_pressed)
            next_state = (selected == INVALID ? MOVE_CAMERA : ROTATE_LAYER);
        else
            next_state = IDLE;
        break;
        
    case MOVE_CAMERA:
        if(left_pressed)
            next_state = MOVE_CAMERA;
        else
            next_state = IDLE;
        break;
        
    case ROTATE_LAYER:
        if(left_pressed)
            next_state = ROTATE_LAYER;
        else
            next_state = IDLE;
        break;
    }
    
    if(next_state == MOVE_CAMERA) {
        UpdateCamera(next_state_variables);
    } else if(next_state == ROTATE_LAYER) {
        UpdateRotateLayer(next_state_variables);
    }
    
    curr_state = next_state;
    curr_state_variables = next_state_variables;
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::UpdateCamera(const StateVariables &next) {
    irr::core::position2di inc = next.cursor_pos - curr_state_variables.cursor_pos;
    float camera_limits = 0.01;
    if(inc.X != 0 || inc.Y != 0) {
        camera_pitch += inc.X * camera_pitch_sensitivity * frame_duration;
        camera_yaw -= inc.Y * camera_yaw_sensitivity * frame_duration;
        if(camera_yaw <= -0.5f*M_PI+camera_limits)
            camera_yaw = -0.5f*M_PI+camera_limits;
        else if(camera_yaw >= 0.5f*M_PI-camera_limits)
            camera_yaw = 0.5f*M_PI-camera_limits;
        SetCameraAngles(camera, camera_pitch, camera_yaw, cube->getPosition(), CAMERA_DISTANCE);
    }
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::UpdateRotateLayer(const StateVariables &next) {
    /* TODO */
}

template<size_t CUBE_SIZE>
void
GraphicalRubikCube<CUBE_SIZE>::SelectedFace(const irr::core::position2di &pos,
                                            FaceElement &face,
                                            irr::core::position2di &coords) {
    /* TODO */
    face = INVALID;
}

template<size_t CUBE_SIZE>
GraphicalRubikCube<CUBE_SIZE>::GraphicalRubikCube(const std::wstring &window_title, size_t width, size_t height) {
    curr_state = IDLE;
    last_time = get_current_time();
    
    /* Create a device */
    device = createDevice(video::EDT_BURNINGSVIDEO,
                          dimension2d<u32>(width, height),
                          16, false, false, false, &event_handler);
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
void GraphicalRubikCube<CUBE_SIZE>::UpdateFrame() {
    double current_time;
    current_time = get_current_time();
    frame_duration = current_time - last_time;
    last_time = current_time;
    
    driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(255,100,101,140));
    
    UpdateEvents();
    
    smgr->drawAll();
    guienv->drawAll();
    
    driver->endScene();
}
