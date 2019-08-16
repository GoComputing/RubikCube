#include <iostream>
#include <cstdlib>
#include <vector>
#include <array>

/*    Clockwise to rotate face    */
typedef enum { 
    CLOCKWISE = 0, COUNTERCLOCKWISE 
} Clockwise;

/*    Face of the cube. Total: 6    */
typedef enum { 
    FRONT = 0, 
    BACK, 
    LEFT, 
    RIGHT, 
    TOP, 
    BOTTOM 
} FaceElement;

template<size_t CUBE_SIZE = 3>
class RubikCube {
private:
    // Declares a CUBE_SIZE x CUBE_SIZE matrix of FaceElement
    typedef std::array<std::array<FaceElement, CUBE_SIZE>, CUBE_SIZE> Face;
    
    static const int NUM_FACES = 6;
    std::array<Face, NUM_FACES> faces;
    
protected:
    typedef enum { AXIS_X=0, AXIS_Y, AXIS_Z } Axis;
    
    /**
      * @brief Get axis from face
      * @param face Face from we will get the axis
      * @return Axis of the face. For example, RIGHT face have axis X
      */ 
    static Axis GetAxis(FaceElement face);
    
    /**
      * @brief Calculates the coordinates of a element in a layer
      * @pre depth param is less than CUBE_SIZE
      * @param axis Axis of the layer
      * @param element_pos Position of the element in the layer. Clockwise order
      * @param depth Depth of the layer
      * @return Returned value is stored in 'face', 'row' and 'col'
      */
    void GetLayerElementCoords(Axis axis, int element_pos, size_t depth, FaceElement &face, size_t &row, size_t &col);
    
    /**
      * @brief Calculates the coordinates of a element in a face
      * @param face Identifier of the face
      * @param element_pos Position of the element in the face
      * @param padding Selects a perimeter of the matrix
      * @return Returned value is stored in 'row' and 'col'
      */
    void GetFaceElementCoords(FaceElement face, int element_pos, size_t padding, size_t &row, size_t &col);
    
    /**
      * @brief Get layer element, which is a FaceElement
      * @param axis Axis used to determine rotation
      * @param element_pos Position of the element inside the layer
      * @param depth Depth of the selected layer
      * @return Element of the layer in selected depth and selected axis
      */
    FaceElement GetLayerElement(Axis axis, int element_pos, size_t depth);
    
    /**
      * @brief Set layer element
      * @param axis Axis used to determine rotation
      * @param element_pos Position of the element inside the layer
      * @param depth Depth of the selected layer
      * @param value Value to set in the specified element of the specified layer
      */
    void SetLayerElement(Axis axis, int element_pos, size_t depth, FaceElement value);
    
    /**
      * @brief Rotates a layer of the cube. 
      * @param axis Axis of rotation
      * @param clockwise Clockwise of rotation
      * @param depth Depth of the layer
      * 
      * This method rotates only the layer, ie, it doesn't rotate a face
      * For example, if axis is AXIS_X, clockwise is CLOCKWISE and depth is 0, 
      * it rotates the left layer in COUNTERCLOCKWISE. 
      */
    void RotateLayer(FaceElement face, Clockwise clockwise, size_t depth);
    
    /**
      * @brief Rotates elements of a face in clockwise order
      * @param face Face to rotate
      * @param clockwise Clockwise of the rotation. It follows cube notations
      */
    void RotateFaceElements(FaceElement face, Clockwise clockwise);
    
public:
    /**
      * @brief Initializes a NxX rubik cube
      */
    RubikCube();
    
    /**
      * @brief Rotates a face at a specified depth
      * @param face Specifies which face to rotate
      * @param clockwise Specifies the clockwise of the rotation. General cube notation is used
      * @param depth For cubes with cube_size > 3, rotates external layer for depth 0, 'depth' inner layer for depth > 0
      */
    void RotateFace(FaceElement face, Clockwise clockwise, size_t depth = 0);
    
    /**
      * @brief Gets the 'color' of the face element selected
      * @param face Specifies the face
      * @param row Specifies the row of the face
      * @param col Specifies the column of the face
      */
    FaceElement GetFaceElement(FaceElement face, size_t row, size_t col) const;
    
    /**
      * @brief Get number of faces
      * @return Number of faces
      */
    size_t GetNumFaces() const;
};


/**
  * @brief Converts a face element to a human readable string. It is guaranteed all strings have the same length
  * @param face Face id to convert
  */
std::string FaceElementToString(FaceElement face_element);

/**
  * @brief Converts a face to a human readable string
  * @param face Face id to convert
  */
std::string FaceToString(FaceElement face);

/**
  * @brief Return the reversed clockwise
  * @param clockwise Clockwise to reverse
  * @return Reserved clockwise
  */
Clockwise operator!(Clockwise clockwise);

template<size_t CUBE_SIZE>
std::ostream& operator<<(std::ostream &os, const RubikCube<CUBE_SIZE> &cube);

/**
  * @brief Rotate elements in a "container"
  * @pre getter and setter are callable objects whichs receives any integer index
  * @param offset Offset of the shift. Positive value rotates to left, and negative value rotates to right
  * @param container_size Number of elements in the container
  * @param get Callable object which returns the element in the n-th position. Parameters: (pos)
  * @param set Callable object which sets the element in the n-th position.    Parameters: (pos, val)
  */
template<typename T, typename Getter, typename Setter>
void RotateElements(int offset, size_t container_size, Getter get, Setter set);

int main() {
    RubikCube<3> cube;
    
    cube.RotateFace(LEFT, COUNTERCLOCKWISE, 0);
    std::cout << cube << std::endl;
    
    return 0;
}




size_t positive_mod(int numerator, size_t denominator) {
    int module = numerator % (int)denominator;
    if(module < 0)
        module = denominator + module;
    return module;
}

template<size_t CUBE_SIZE>
typename RubikCube<CUBE_SIZE>::Axis RubikCube<CUBE_SIZE>::GetAxis(FaceElement face) {
    Axis axis;
    if(face == LEFT || face == RIGHT)
        axis = AXIS_X;
    else if(face == TOP || face == BOTTOM)
        axis = AXIS_Y;
    else
        axis = AXIS_Z;
    return axis;
}

template<size_t CUBE_SIZE>
void RubikCube<CUBE_SIZE>::GetLayerElementCoords(Axis axis, int element_pos, size_t depth, 
                                                 FaceElement &face, size_t &row, size_t &col) 
{
    //int TMP = element_pos;
    assert(depth < CUBE_SIZE);
    size_t num_layer_elements = 4*CUBE_SIZE;
    size_t face_element_pos;
    size_t neg_face_element_pos; // Negated element position
    size_t neg_depth; // Negated depth
    element_pos = (int)positive_mod(element_pos, num_layer_elements);
    face_element_pos = element_pos % CUBE_SIZE;
    neg_face_element_pos = CUBE_SIZE-1 - face_element_pos;
    neg_depth = CUBE_SIZE-1 - depth;
    FaceElement face_selector[3][4] = {
        { BOTTOM,  FRONT,  TOP,    BACK   }, // X axis
        { BACK,    RIGHT,  FRONT,  LEFT   }, // Y axis
        { LEFT,    TOP,    RIGHT,  BOTTOM }  // Z axis
    };
    size_t row_selector[3][4] = {
        { neg_face_element_pos, neg_face_element_pos, face_element_pos,     face_element_pos     }, // X axis
        { neg_depth,            neg_depth,            neg_depth,            neg_depth            }, // Y axis
        { neg_face_element_pos, neg_depth,            face_element_pos,     neg_depth            }  // Z axis
    };
    size_t col_selector[3][4] = {
        { depth,                depth,                depth,                depth                }, // X axis
        { face_element_pos,     face_element_pos,     neg_face_element_pos, neg_face_element_pos }, // Y axis
        { depth,                face_element_pos,     depth,                neg_face_element_pos }  // Z axis
    };
    // Store values
    face = face_selector[(int)axis][element_pos/CUBE_SIZE];
    row = row_selector[(int)axis][element_pos/CUBE_SIZE];
    col = col_selector[(int)axis][element_pos/CUBE_SIZE];
    //std::cout << TMP << ' ' << num_layer_elements << ' ' << element_pos << std::endl;
}

template<size_t CUBE_SIZE>
void RubikCube<CUBE_SIZE>::GetFaceElementCoords(FaceElement face, int element_pos, size_t padding, size_t &row, size_t &col) {
    // TODO
}

template<size_t CUBE_SIZE>
FaceElement RubikCube<CUBE_SIZE>::GetLayerElement(Axis axis, int element_pos, size_t depth) {
    FaceElement face;
    size_t row, col;
    GetLayerElementCoords(axis, element_pos, depth, face, row, col);
    return faces[(int)face][row][col];
}

template<size_t CUBE_SIZE>
void RubikCube<CUBE_SIZE>::SetLayerElement(Axis axis, int element_pos, size_t depth, FaceElement value) {
    FaceElement face;
    size_t row, col;
    GetLayerElementCoords(axis, element_pos, depth, face, row, col);
    faces[(int)face][row][col] = value;
    //std::cout << "Element pos: " << element_pos << ", face: " << FaceToString(face) 
    //          << ", row: " << row << ", col: " << col << std::endl;
}

template<size_t CUBE_SIZE>
void RubikCube<CUBE_SIZE>::RotateLayer(FaceElement face, Clockwise clockwise, size_t depth) {
    assert(depth < CUBE_SIZE);
    
    Axis axis = GetAxis(face);
    int offset;
    if(face == FRONT || face == RIGHT || face == TOP) {
        depth = CUBE_SIZE-1 - depth;
        offset = CUBE_SIZE;
    } else {
        offset = -CUBE_SIZE;
    }
    auto getter = [this, axis, depth](int pos) -> FaceElement {
        return this->GetLayerElement(axis, pos, depth);
    };
    auto setter = [this, axis, depth](int pos, FaceElement face_elem) {
        this->SetLayerElement(axis, pos, depth, face_elem);
    };
    RotateElements<FaceElement>(offset, 4*CUBE_SIZE, getter, setter);
}

template<size_t CUBE_SIZE>
void RubikCube<CUBE_SIZE>::RotateFaceElements(FaceElement face, Clockwise clockwise) {
    // template<typename T, typename Getter, typename Setter>
    // void RotateElements(int offset, size_t container_size, Getter get, Setter set)
    Face &face_matrix = faces[(int)face];
    for(size_t i=0; i<CUBE_SIZE/2; ++i) {
        size_t border_length = CUBE_SIZE - 2*i;
        size_t perimeter = 4*(border_length-1);
        int offset = border_length;
        if(clockwise == CLOCKWISE)
            offset *= -1;
        auto getter = [](int pos) -> FaceElement {
            // TODO
        };
        auto setter = [](int pos, FaceElement face_elem) {
            // TODO
        }
        RotateElements<FaceElement>(offset, perimeter, getter, setter);
    }
}

template<size_t CUBE_SIZE>
RubikCube<CUBE_SIZE>::RubikCube() {
    size_t face_id;
    size_t i, j;
    for(face_id=0; face_id<NUM_FACES; ++face_id)
        for(i=0; i<CUBE_SIZE; ++i)
            for(j=0; j<CUBE_SIZE; ++j)
                faces[face_id][i][j] = (FaceElement)face_id;
}

template<size_t CUBE_SIZE>
void RubikCube<CUBE_SIZE>::RotateFace(FaceElement face, Clockwise clockwise, size_t depth) {
    RotateLayer(face, clockwise, depth);
    if(depth == 0)
        RotateFaceElements(face, clockwise);
    else if(depth+1 == CUBE_SIZE)
        RotateFaceElements(face, !clockwise);
}

template<size_t CUBE_SIZE>
FaceElement RubikCube<CUBE_SIZE>::GetFaceElement(FaceElement face, size_t row, size_t col) const {
    assert(face >= 0 && face < NUM_FACES);
    assert(row < CUBE_SIZE);
    assert(col < CUBE_SIZE);
    return faces[(size_t)face][row][col];
}

template<size_t CUBE_SIZE>
size_t RubikCube<CUBE_SIZE>::GetNumFaces() const {
    return NUM_FACES;
}



std::string FaceToString(FaceElement face) {
    std::string str;
    switch(face) {
    case FRONT:  str = "front";  break;
    case BACK:   str = "back";   break;
    case LEFT:   str = "left";   break;
    case RIGHT:  str = "right";  break;
    case TOP:    str = "top";    break;
    case BOTTOM: str = "bottom"; break;
    }
    return str;
}

std::string FaceElementToString(FaceElement face_element) {
    std::string str;
    switch(face_element) {
    case FRONT:  str = "F";  break;
    case BACK:   str = "B";   break;
    case LEFT:   str = "L";   break;
    case RIGHT:  str = "R";  break;
    case TOP:    str = "U";    break;
    case BOTTOM: str = "D"; break;
    }
    return str;
}

Clockwise operator!(Clockwise clockwise) {
    if(clockwise == CLOCKWISE)
        clockwise = COUNTERCLOCKWISE;
    else
        clockwise = CLOCKWISE;
    return clockwise;
}

template<size_t CUBE_SIZE>
std::ostream& operator<<(std::ostream &os, const RubikCube<CUBE_SIZE> &cube) {
    size_t face_id;
    size_t row;
    size_t col;
    for(face_id=0; face_id<cube.GetNumFaces(); ++face_id) {
        os << '\n' << FaceToString((FaceElement)face_id) << '\n';
        for(row=0; row<CUBE_SIZE; ++row) {
            for(col=0; col<CUBE_SIZE; ++col)
                os << FaceElementToString(cube.GetFaceElement((FaceElement)face_id, row, col)) << ' ';
            os << '\n';
        }
    }
    return os;
}

template<typename T,typename Getter, typename Setter>
void RotateElements(int offset, size_t container_size, Getter get, Setter set) {
    std::vector<T> tmp = std::vector<T>(std::abs(offset));
    int i_offset = 0;
    int increment = (offset < 0 ? -1 : 1);
    if(offset < 0)
        i_offset = 1;
    offset = std::abs(offset);
    for(int i=0; i<offset; ++i)
        tmp[i] = get((i+i_offset)*increment);
    for(int i=0; i<(int)container_size-offset; ++i)
        set((i+i_offset)*increment, get((i+i_offset+offset)*increment));
    for(int i=0; i<offset; ++i)
        set(((int)container_size-offset+i+i_offset)*increment, tmp[i]);
}