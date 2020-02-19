#include <irrlicht.h>
#include <stdexcept>
#include <string>
#include <rubik.h>
#include <event_handler.h>
#include <irrlicht_tools.h>
#include <array>
#include <unordered_map>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

template<size_t CUBE_SIZE>
class GraphicalRubikCube : public RubikCube<ISceneNode*, CUBE_SIZE> {
private:
    typedef enum {
                  IDLE,
                  MOVE_CAMERA,
                  ROTATE_LAYER
    } State;
    
    typedef struct {
        irr::core::position2di cursor_pos;
    } StateVariables;
    
    static constexpr double CAMERA_DISTANCE = 2.0;
    
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
    
    static constexpr int NUM_FACES = 6;
    int initial_cube_id = 0;
    // Center of cube
    ISceneNode *cube;
    // Can be indexed by ID assigned to ISceneNode face element, if you substract ID by initial_cube_id (eg. faces_element_nodes[ID-initial_cube_id])
    std::array<ISceneNode*, NUM_FACES*CUBE_SIZE*CUBE_SIZE> faces_element_nodes;
    // Materials of each face, ordered by FaceElement enum
    std::array<irr::video::SMaterial*, NUM_FACES> faces_element_materials;
    
protected:
    /**
      * @brief Initializes 'faces_element_materials' attribute. Here you can find face colors, and reflexive properties
      */
    void InitializeFacesColors();
    
    /**
      * @brief Initializes cube faces with default colors
      * @param size Size of cube, in units of Irrlicht
      */
    void GenerateFaces(float cube_length, ISceneNode *parent, int initial_id=0);
    
    /**
      * @brief Setup scene, including all models, messages, etc
      * @pre Environment and scene manager are initialized
      */
    void PrepareScene(float cube_length);
    
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
      * @brief Returns face element which is in 'pos' position (screen coordinates), or invalid if any face is pointed
      * @param pos Position, in screen space
      * @return face_element: Selected face, or INVALID if any face is selected
      * @return row: Row of the face element. It is valid only if face_element isn't INVALID
      * @return col: Column of the face element. It is valid only if face_element isn't INVALID
      */
    void PointedFaceElement(const irr::core::position2di &pos, FaceElement &face_element, size_t &row, size_t &col);
    
    /**
      * @brief Returns face associated with a face element, which is in row and column
      * @pre face_element is a valid face element. Row and col is in range [0, CUBE_SIZE)
      * @param face_element Selected face element
      * @param row Row of the selected face element
      * @param col Column of the selected face element
      * @return Selected face in 'face' and 'depth'. For example, is face_element is FRONT, row is 0, col is 1 and CUBE_SIZE is 3, 
      *         selected face is RIGHT, depth is 1 and clockwise CLOCKWISE
      */
    void SelectedFace(FaceElement face_element, size_t row, size_t col, FaceElement &face, size_t &depth, Clockwise &clockwise);
    
    /**
      * @brief Returns face pointed by 'pos'
      * @param pos Position, in screen space
      * @return Selected face, or INVALID if any face is selected. Depth will be valid only if 'face' isn't INVALID. Preference values are determined by 'Selected Face'
      */
    void PointedFace(const irr::core::position2di &pos, FaceElement &face, size_t &depth, Clockwise &clockwise);
    
    /**
      * @brief Get face element coordinates from ID assigned to Irrlicht ISceneNode
      * @param id ID assigned to Irrlicht ISceneNode
      * @return Coordinates are returned to 'face', 'row', and 'col'
      */
    void GetCoordsFromID(int id, FaceElement &face, int &row, int &col) const;
    
public:
    /**
      * @brief Creates a graphical environment which will show a rubik cube 
      * @param window_title Title of the window
      * @param width Width of the window, in pixels
      * @param height Height of the window, in pixels
      */
    GraphicalRubikCube(const std::wstring &window_title, size_t width, size_t height, float cube_length);
    
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
void GraphicalRubikCube<CUBE_SIZE>::InitializeFacesColors() {
    // Colors. Format ARGB. Range [0, 255]
    std::array<irr::video::SColor, NUM_FACES> faces_colors =
        {
         irr::video::SColor {255,   0, 255,   0}, // Front
         irr::video::SColor {255,   0,   0, 255}, // Back
         irr::video::SColor {255, 255, 255, 255}, // Left
         irr::video::SColor {255, 255, 255,   0}, // Right
         irr::video::SColor {255, 255,   0,   0}, // Top
         irr::video::SColor {255, 255, 128,   0}  // Bottom
        };
    // Initialize materials
    for(int i=0; i<NUM_FACES; ++i) {
        irr::video::SMaterial *material = new irr::video::SMaterial();
        
        // Set material properties
        //material->setFlag(EMF_GOURAUD_SHADING, false);
        material->setFlag(EMF_LIGHTING, true);
        material->setFlag(EMF_FRONT_FACE_CULLING, false);
        material->setFlag(EMF_BACK_FACE_CULLING, false);
        material->setFlag(EMF_ANISOTROPIC_FILTER, false);
        material->setFlag(EMF_ANTI_ALIASING, false);
        material->setFlag(EMF_COLOR_MATERIAL, true);
        material->setFlag(EMF_USE_MIP_MAPS, false);
        
        // Set material colors
        material->AmbientColor = faces_colors[i];
        material->DiffuseColor = faces_colors[i];
        material->SpecularColor = faces_colors[i];
        material->EmissiveColor = faces_colors[i];
        material->Shininess = 20.0f;
        
        // Save value
        faces_element_materials[i] = material;
    }
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::GenerateFaces(float cube_length, ISceneNode *parent, int initial_id) {
    initial_cube_id = initial_id;
    
    InitializeFacesColors();
    // Initialize meshes
    const IGeometryCreator *creator = smgr->getGeometryCreator();
    float element_size = cube_length / CUBE_SIZE;
    core::dimension2d<float> element_dim = {element_size, element_size};
    core::dimension2d<unsigned int> tiles_per_element = {1, 1};
    
    float min_val = -cube_length * 0.5f;
    float max_val =  cube_length * 0.5f;
    
    // Yaw, pitch, roll
    std::array<irr::core::vector3df, NUM_FACES> rotation_table =
        {
         irr::core::vector3df { 90.0f, 0.0f,   0.0f}, // Front
         irr::core::vector3df {270.0f, 0.0f,   0.0f}, // Back
         irr::core::vector3df {  0.0f, 0.0f,  90.0f}, // Left
         irr::core::vector3df {  0.0f, 0.0f, 270.0f}, // Right
         irr::core::vector3df {  0.0f, 0.0f,   0.0f}, // Top
         irr::core::vector3df {  0.0f, 0.0f, 180.0f}  // Bottom
        };
    
    for(int i = 0; i < faces_element_nodes.size(); ++i) {
        FaceElement face;
        int face_id;
        int row;
        int col;
        float pos_row;
        float pos_col;
        float x, y, z;
        irr::core::vector3df position;
        irr::core::vector3df rotation;
        irr::core::vector3df scale = {0.9f, 0.9f, 0.9f};
        
        GetCoordsFromID(i, face, row, col);
        face_id = (int)face;
        
        pos_row = cube_length * (float)row / (float)CUBE_SIZE - cube_length*0.5f + element_size*0.5f;
        pos_col = cube_length * (float)col / (float)CUBE_SIZE - cube_length*0.5f + element_size*0.5f;
        
        switch(face_id) {
        case 0:  x = -pos_col; y = -pos_row; z =  max_val; break; // Front
        case 1:  x =  pos_col; y = -pos_row; z =  min_val; break; // Back
        case 2:  x =  max_val; y = -pos_col; z = -pos_row; break; // Left
        case 3:  x =  min_val; y = -pos_row; z = -pos_col; break; // Right
        case 4:  x = -pos_col; y =  max_val; z =  pos_row; break; // Top
        case 5:  x = -pos_col; y =  min_val; z = -pos_row; break; // Bottom
        default: assert(false);
        }
        
        position = irr::core::vector3df {x, y, z};
        rotation = rotation_table[face_id];
        irr::scene::IMesh* plane = creator->createPlaneMesh(element_dim, tiles_per_element, faces_element_materials[face_id]);
        faces_element_nodes[i] = smgr->addMeshSceneNode(plane, parent, initial_cube_id+i, position, rotation, scale);
        ITriangleSelector *triangle_selector = smgr->createTriangleSelector(plane, faces_element_nodes[i]);
        faces_element_nodes[i]->setTriangleSelector(triangle_selector);
        
        this->SetFaceObject(face, row, col, faces_element_nodes[i]);
    }
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::PrepareScene(float cube_length) {
    /* Message */
    guienv->addStaticText(L"Hello World! This is Irrlicht with the burnings software renderer!",
                          rect<s32>(10,10,260,22), true);
    
    
    /* Models */
    cube = smgr->addEmptySceneNode();
    GenerateFaces(cube_length, cube);
    
    
    /* Camera */
    camera = smgr->addCameraSceneNode(0, vector3df(0,0,-2*cube_length));
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
    size_t depth;
    Clockwise clockwise;
    
    PointedFace(curr_state_variables.cursor_pos, selected, depth, clockwise);
    
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
void GraphicalRubikCube<CUBE_SIZE>::PointedFaceElement(const irr::core::position2di &pos, FaceElement &face_element, size_t &row, size_t &col) {
    ISceneCollisionManager *picker = smgr->getSceneCollisionManager();
    irr::core::line3d<float> intersect_ray = picker->getRayFromScreenCoordinates(pos, camera);
    ISceneNode *selected;
    irr::core::vector3df collision_point;
    irr::core::triangle3df triangle;
    
    // Gets selected scene node from screen coordinates
    selected = picker->getSceneNodeAndCollisionPointFromRay(intersect_ray,
                                                            collision_point,
                                                            triangle,
                                                            0,
                                                            cube);
    
    if(selected != nullptr)
        this->GetObjectCoordinates(selected, face_element, row, col);
    else
        face_element = INVALID;
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::SelectedFace(FaceElement face_element, size_t row, size_t col, FaceElement &face, size_t &depth, Clockwise &clockwise) {
    assert(face_element >= 0 && face_element < INVALID);
    assert(row >= 0 && row < CUBE_SIZE);
    assert(col >= 0 && col < CUBE_SIZE);
    
    /* TODO */
    // Gets selected face from face element, row and column. For example,
    // if front face element is selected, row is 0 and column is 1 (3x3 rubik cube),
    // selected face is top
    
    bool l_row = row == 0;           // Low row
    bool h_row = row+1 == CUBE_SIZE; // High row
    bool l_col = col == 0;           // Low column
    bool h_col = col+1 == CUBE_SIZE; // High column
    
    std::unordered_map<FaceElement, FaceElement> left_faces =
        {
         {FRONT,  LEFT},
         {BACK,   RIGHT},
         {LEFT,   BACK},
         {RIGHT,  FRONT},
         {TOP,    LEFT},
         {BOTTOM, LEFT}
        };
    
    std::unordered_map<FaceElement, FaceElement> top_faces =
        {
         {FRONT,  TOP},
         {BACK,   TOP},
         {LEFT,   TOP},
         {RIGHT,  TOP},
         {TOP,    BACK},
         {BOTTOM, FRONT}
        };
    
    if(!l_row && !h_row && !l_col && !h_col) {
        // Face element is a middle element, so there are multiple options. We don't have enough data to calculate face, so face is INVALID
        face = INVALID;
    } else if((l_row && l_col) || (l_row && h_col) || (h_row && l_col) || (h_row && h_col)) {
        // Face element is in a corner
        
        // Clockwise
        if(row == col)
            clockwise = COUNTERCLOCKWISE;
        else
            clockwise = CLOCKWISE;
        
        // Face
        if(face_element == TOP || face_element == BOTTOM) {
            clockwise = !clockwise;
            
            if(l_row)
                face = top_faces[face_element];
            else
                face = !top_faces[face_element];
        } else {
            if(l_col)
                face = left_faces[face_element];
            else
                face = !left_faces[face_element]; // Opposite face ('right_faces')
        }
        
        // Depth
        depth = 0;
    } else {
        // Face element is in a external layer
        
        // Clockwise
        if(l_row || l_col)
            clockwise = CLOCKWISE;
        else
            clockwise = COUNTERCLOCKWISE;
        
        // Face and depth
        if(l_row || h_row) {
            face = !left_faces[face_element]; // 'right_faces'
            depth = CUBE_SIZE-1 - col; // From 'right' face perspective
        } else {
            face = top_faces[face_element];
            depth = row; // From 'top' perspective
        }
    }
    
    std::cout << FaceToString(face) << ", Depth: " << depth << ", " << clockwise << std::endl;
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::PointedFace(const irr::core::position2di &pos, FaceElement &face, size_t &depth, Clockwise &clockwise) {
    FaceElement face_element;
    size_t row, col;
    
    // Gets face element
    this->PointedFaceElement(pos, face_element, row, col);
    
    // SelectedFace should be called only if face_element isn't invalid
    if(face_element != INVALID)
        this->SelectedFace(face_element, row, col, face, depth, clockwise);
    else
        face = INVALID;
}

template<size_t CUBE_SIZE>
void GraphicalRubikCube<CUBE_SIZE>::GetCoordsFromID(int id, FaceElement &face, int &row, int &col) const {
    int face_id = (id-initial_cube_id) / (CUBE_SIZE*CUBE_SIZE);
    int element_id = (id-initial_cube_id) % (CUBE_SIZE*CUBE_SIZE);
    assert(face_id >= 0 && face_id < NUM_FACES*CUBE_SIZE*CUBE_SIZE);
    face = (FaceElement)face_id;
    row = element_id / CUBE_SIZE;
    col = element_id % CUBE_SIZE;
}

template<size_t CUBE_SIZE>
GraphicalRubikCube<CUBE_SIZE>::GraphicalRubikCube(const std::wstring &window_title, size_t width, size_t height, float cube_length) {
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
    PrepareScene(cube_length);
}

template<size_t CUBE_SIZE>
GraphicalRubikCube<CUBE_SIZE>::~GraphicalRubikCube() {
    device->drop();
    for(int i=0; i<faces_element_materials.size(); ++i)
        delete faces_element_materials[i];
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
