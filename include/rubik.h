#include <iostream>
#include <tools.h>
#include <cassert>
#include <array>

/*    Clockwise to rotate face    */
typedef enum { 
    CLOCKWISE = 0, COUNTERCLOCKWISE 
} Clockwise;

/*    Face of the cube. Total: 6    */
typedef enum
    { 
     FRONT = 0, // ORDER IS PART OF THE INTERFACE. DON'T CHANGE IT
     BACK, 
     LEFT, 
     RIGHT, 
     TOP, 
     BOTTOM,
     INVALID
    } FaceElement;

/**
  * @brief Class that represents a rubik cube
  *
  * Representation: six faces. Each face has CUBE_SIZE rows and CUBE_SIZE columnts.
  * row and columnt starts from top left corner of the face. Seen as would be seen in a rotation of a face. 
  * For example, LEFT face should be rotated 90 degrees around Y axis (vertial axis).
  */
template<typename T, size_t CUBE_SIZE = 3>
class RubikCube {
private:
    // Associated face element with an object
    struct AssocFaceElement {
        FaceElement element;
        const T *object;
        
        AssocFaceElement()
            : element(INVALID), object(nullptr) {}
    };
    
    // Declares a CUBE_SIZE x CUBE_SIZE matrix of FaceElement
    typedef std::array<std::array<AssocFaceElement, CUBE_SIZE>, CUBE_SIZE> Face;
    
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
      * @brief Returns number of elements in a layer
      */
    size_t GetNumElementsLayer() const;
    
    /**
      * @brief Calculates the coordinates of a element in a layer
      * @pre depth param is less than CUBE_SIZE
      * @param axis Axis of the layer
      * @param element_pos Position of the element in the layer. Clockwise order
      * @param depth Depth of the layer
      * @return Returned value is stored in 'face', 'row' and 'col'
      */
    void GetLayerCoords(Axis axis, int element_pos, size_t depth, FaceElement &face, size_t &row, size_t &col) const;
    
    /**
      * @brief Calculates the coordinates of a element in a face
      * @param face Identifier of the face
      * @param element_pos Position of the element in the face
      * @param padding Selects a perimeter of the matrix
      * @return Returned value is stored in 'row' and 'col'
      */
    void GetFaceElementCoords(FaceElement face, int element_pos, size_t padding, size_t &row, size_t &col) const;
    
    /**
      * @brief Get layer element, which is a FaceElement
      * @param axis Axis of the layer (see GetAxis)
      * @param element_pos Position of the element inside the layer
      * @param depth Depth of the selected layer
      * @return Element of the layer in selected depth and selected axis
      */
    FaceElement GetLayerElement(Axis axis, int element_pos, size_t depth) const;
    
    /**
      * @brief Get object associated with a face element in a specified layer
      * @param axis Axis of the layer (see GetAxis)
      * @param element_pos Position of the element inside the layer
      * @param depth Depth of the selected layer
      * @return Element of the layer in selected depth and selected axis
      */
    const T& GetLayerObject(Axis axis, int element_pos, size_t depth) const;
    
    /**
      * @brief Set layer element
      * @param axis Axis of the layer (see GetAxis)
      * @param element_pos Position of the element inside the layer
      * @param depth Depth of the selected layer
      * @param value Value to set in the specified element of the specified layer
      */
    void SetLayerElement(Axis axis, int element_pos, size_t depth, FaceElement value);
    
    /**
      * @brief Set layer element
      * @param axis Axis of the layer (see GetAxis)
      * @param element_pos Position of the element inside the layer
      * @param depth Depth of the selected layer
      * @param value Value to set in the specified element of the specified layer
      */
    void SetLayerObject(Axis axis, int element_pos, size_t depth, const T &value);
    
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
      * @param depth For cubes with cube_size >= 3, rotates external layer for depth 0, 'depth' inner layer for depth > 0
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
      * @brief Gets the object associated with a face element
      * @pre There is an object associated with the face element
      * @param face Specifies the face
      * @param row Specifies the row of the face
      * @param col Specifies the column of the face
      * @return Returns object associated with a face element
      */
    const T& GetFaceObject(FaceElement face, size_t row, size_t col) const;
    
    /**
      * @brief Sets the object associated with a face element
      * @param face Specifies the face
      * @param row Specifies the row of the face
      * @param col Specifies the column of the face
      * @param object Valid object to be associated with the face element
      */
    void SetFaceObject(FaceElement face, size_t row, size_t col, const T &object);
    
    /**
      * @brief Gets current coordinates of an object
      * @pre Object is associated to any face element of the cube
      * @param object Object to retreive the face element coordinates
      * @return face Face of the object
      * @return row Row of the object
      * @return col Column of the object
      */
    void GetObjectCoordinates(const T &object, FaceElement &face, size_t &row, size_t &col) const;
    
    /**
      * @brief Returns all face element objets of one layer, including lateral ones
      * @pre face shall not be INVALID
      * @pre depth shall be in range [0, CUBE_SIZE)
      * @param face Selected layer from face
      * @return Vector of objets, which are from a specified layer
      */
    std::vector<T> GetFaceObjects(FaceElement face, size_t depth) const;
    
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

/**
  * @brief Return the opposite face
  * @param face to be negated
  * @return Opposite face. If 'face' is INVALID, this operator will return INVALID
  */
FaceElement operator!(FaceElement face);


template<typename T, size_t CUBE_SIZE>
std::ostream& operator<<(std::ostream &os, const RubikCube<T, CUBE_SIZE> &cube);

std::ostream& operator<<(std::ostream &os, Clockwise clockwise);














/********** IMPLEMENTATION **********/
template<typename T, size_t CUBE_SIZE>
typename RubikCube<T, CUBE_SIZE>::Axis RubikCube<T, CUBE_SIZE>::GetAxis(FaceElement face) {
    Axis axis;
    if(face == LEFT || face == RIGHT)
        axis = AXIS_X;
    else if(face == TOP || face == BOTTOM)
        axis = AXIS_Y;
    else
        axis = AXIS_Z;
    return axis;
}

template<typename T, size_t CUBE_SIZE>
size_t RubikCube<T, CUBE_SIZE>::GetNumElementsLayer() const {
    return CUBE_SIZE * 4;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::GetLayerCoords(Axis axis, int element_pos, size_t depth, 
                                             FaceElement &face, size_t &row, size_t &col) const
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
        { TOP,   BACK,   BOTTOM, FRONT }, // X axis
        { FRONT, LEFT,   BACK,   RIGHT }, // Y axis
        { RIGHT, BOTTOM, LEFT,   TOP   }  // Z axis
    };
    size_t row_selector[3][4] = {
        { neg_face_element_pos, face_element_pos, neg_face_element_pos, neg_face_element_pos }, // X axis
        { depth,                depth,            depth,                depth                }, // Y axis
        { face_element_pos,     depth,            neg_face_element_pos, neg_depth            }  // Z axis
    };
    size_t col_selector[3][4] = {
        { neg_depth,            depth,                neg_depth,            neg_depth            }, // X axis
        { neg_face_element_pos, neg_face_element_pos, neg_face_element_pos, neg_face_element_pos }, // Y axis
        { depth,                neg_face_element_pos, neg_depth,            face_element_pos     }  // Z axis
    };
    // Store values
    face = face_selector[(int)axis][element_pos/CUBE_SIZE];
    row = row_selector[(int)axis][element_pos/CUBE_SIZE];
    col = col_selector[(int)axis][element_pos/CUBE_SIZE];
    //std::cout << TMP << ' ' << num_layer_elements << ' ' << element_pos << std::endl;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::GetFaceElementCoords(FaceElement face, int element_pos, size_t padding, size_t &row, size_t &col) const {
    size_t min = padding;
    size_t max = CUBE_SIZE-1 - padding;
    size_t edge_size = max-min;
    element_pos = positive_mod(element_pos, 4*edge_size);
    if((size_t)element_pos < edge_size) {
        row = min;
        col = min+element_pos;
    } else if((size_t)element_pos < 2*edge_size) {
        row = min + (element_pos - edge_size);
        col = max;
    } else if((size_t)element_pos < 3*edge_size) {
        row = max;
        col = max - (element_pos - 2*edge_size);
    } else {
        row = max - (element_pos - 3*edge_size);
        col = min;
    }
}

template<typename T, size_t CUBE_SIZE>
FaceElement RubikCube<T, CUBE_SIZE>::GetLayerElement(Axis axis, int element_pos, size_t depth) const {
    FaceElement face;
    size_t row, col;
    GetLayerCoords(axis, element_pos, depth, face, row, col);
    return faces[(int)face][row][col].element;
}

template<typename T, size_t CUBE_SIZE>
const T& RubikCube<T, CUBE_SIZE>::GetLayerObject(Axis axis, int element_pos, size_t depth) const {
    FaceElement face;
    size_t row, col;
    GetLayerCoords(axis, element_pos, depth, face, row, col);
    return *faces[(int)face][row][col].object;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::SetLayerElement(Axis axis, int element_pos, size_t depth, FaceElement value) {
    FaceElement face;
    size_t row, col;
    GetLayerCoords(axis, element_pos, depth, face, row, col);
    faces[(int)face][row][col].element = value;
    //std::cout << "Element pos: " << element_pos << ", face: " << FaceToString(face) 
    //          << ", row: " << row << ", col: " << col << std::endl;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::SetLayerObject(Axis axis, int element_pos, size_t depth, const T &value) {
    FaceElement face;
    size_t row, col;
    GetLayerCoords(axis, element_pos, depth, face, row, col);
    faces[(int)face][row][col].object = &value;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::RotateLayer(FaceElement face, Clockwise clockwise, size_t depth) {
    assert(depth < CUBE_SIZE);
    
    Axis axis = GetAxis(face);
    int offset;
    if(face == FRONT || face == RIGHT || face == TOP) {
        offset = CUBE_SIZE;
    } else {
        depth = CUBE_SIZE-1 - depth;
        offset = -(int)CUBE_SIZE;
    }
    if(clockwise == CLOCKWISE)
        offset *= -1;
    auto getter = [this, axis, depth](int pos) -> AssocFaceElement {
                      FaceElement face;
                      size_t row, col;
                      this->GetLayerCoords(axis, pos, depth, face, row, col);
                      return faces[(int)face][row][col];
                  };
    auto setter = [this, axis, depth](int pos, AssocFaceElement face_elem) {
                      FaceElement face;
                      size_t row, col;
                      this->GetLayerCoords(axis, pos, depth, face, row, col);
                      faces[(int)face][row][col] = face_elem;
                  };
    RotateElements<AssocFaceElement>(offset, 4*CUBE_SIZE, getter, setter);
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::RotateFaceElements(FaceElement face, Clockwise clockwise) {
    // template<typename T, typename Getter, typename Setter>
    // void RotateElements(int offset, size_t container_size, Getter get, Setter set)
    Face &face_matrix = faces[(int)face];
    for(size_t i=0; i<CUBE_SIZE/2; ++i) {
        size_t border_length = CUBE_SIZE-1 - 2*i;
        size_t perimeter = 4*border_length;
        int offset = border_length;
        if(clockwise == CLOCKWISE)
            offset *= -1;
        // void RubikCube<T, CUBE_SIZE>::GetFaceElementCoords(FaceElement face, int element_pos, size_t padding, size_t &row, size_t &col)
        auto getter = [this, face_matrix, face, i](int pos) -> AssocFaceElement {
                          size_t row, col;
                          this->GetFaceElementCoords(face, pos, i, row, col);
                          return face_matrix[row][col];
                      };
        auto setter = [this, &face_matrix, face, i](int pos, AssocFaceElement face_elem) {
                          size_t row, col;
                          this->GetFaceElementCoords(face, pos, i, row, col);
                          face_matrix[row][col] = face_elem;
                          //std::cout << pos << ": " << row << ' ' << col << std::endl;;
                      };
        RotateElements<AssocFaceElement>(offset, perimeter, getter, setter);
    }
}

template<typename T, size_t CUBE_SIZE>
RubikCube<T, CUBE_SIZE>::RubikCube() {
    size_t face_id;
    size_t i, j;
    for(face_id=0; face_id<NUM_FACES; ++face_id)
        for(i=0; i<CUBE_SIZE; ++i)
            for(j=0; j<CUBE_SIZE; ++j)
                faces[face_id][i][j].element = (FaceElement)face_id;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::RotateFace(FaceElement face, Clockwise clockwise, size_t depth) {
    std::cout << FaceToString(face) << ", " << clockwise << ", " << depth << std::endl;
    if(depth == 0)
        RotateFaceElements(face, clockwise);
    else if(depth+1 == CUBE_SIZE)
        RotateFaceElements(face, !clockwise);
    RotateLayer(face, clockwise, depth);
}

template<typename T, size_t CUBE_SIZE>
FaceElement RubikCube<T, CUBE_SIZE>::GetFaceElement(FaceElement face, size_t row, size_t col) const {
    assert(face >= 0 && face < NUM_FACES);
    assert(row < CUBE_SIZE);
    assert(col < CUBE_SIZE);
    return faces[(size_t)face][row][col].element;
}

template<typename T, size_t CUBE_SIZE>
const T& RubikCube<T, CUBE_SIZE>::GetFaceObject(FaceElement face, size_t row, size_t col) const {
    assert(face >= 0 && face < NUM_FACES);
    assert(row < CUBE_SIZE);
    assert(col < CUBE_SIZE);
    const T *object = faces[(size_t)face][row][col].object;
    assert(object != nullptr);
    return *object;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::SetFaceObject(FaceElement face, size_t row, size_t col, const T &object) {
    assert(face >= 0);
    assert(row < CUBE_SIZE);
    assert(col < CUBE_SIZE);
    faces[(size_t)face][row][col].object = &object;
}

template<typename T, size_t CUBE_SIZE>
void RubikCube<T, CUBE_SIZE>::GetObjectCoordinates(const T &object, FaceElement &face, size_t &row, size_t &col) const {
    size_t i, j, k;
    bool found = false;
    for(i=0; i<NUM_FACES && !found; ++i) {
        for(j=0; j<CUBE_SIZE && !found; ++j) {
            for(k=0; k<CUBE_SIZE && !found; ++k) {
                found = *faces[i][j][k].object == object;
                if(found) {
                    face = (FaceElement)i;
                    row = j;
                    col = k;
                }
            } // End column for
        } // End row for
    } // End face for
    assert(found);
}

template<typename T, size_t CUBE_SIZE>
std::vector<T> RubikCube<T, CUBE_SIZE>::GetFaceObjects(FaceElement face, size_t depth) const {
    assert(face >= 0 && face < INVALID);
    assert(depth >= 0 && depth < CUBE_SIZE);
    std::vector<T> face_objects;
    
    if(face == LEFT || face == BACK || face == BOTTOM) {
        depth = CUBE_SIZE-1 - depth;
    }
    
    // Add front face elements
    if(depth == 0 || depth+1 == CUBE_SIZE) {
        for(size_t i=0; i<CUBE_SIZE; ++i)
            for(size_t j=0; j<CUBE_SIZE; j++)
                face_objects.push_back(*(faces[face][i][j].object));
    }
    
    // Add lateral face elements
    size_t num_elements = GetNumElementsLayer();
    Axis axis = GetAxis(face);
    for(size_t i=0; i<num_elements; ++i)
        face_objects.push_back(GetLayerObject(axis, i, depth));
    
    return face_objects;
}

template<typename T, size_t CUBE_SIZE>
size_t RubikCube<T, CUBE_SIZE>::GetNumFaces() const {
    return NUM_FACES;
}



std::string FaceToString(FaceElement face) {
    std::string str;
    switch(face) {
    case FRONT:  str = "front";   break;
    case BACK:   str = "back";    break;
    case LEFT:   str = "left";    break;
    case RIGHT:  str = "right";   break;
    case TOP:    str = "top";     break;
    case BOTTOM: str = "bottom";  break;
    default:     str = "invalid"; break;
    }
    return str;
}

std::string FaceElementToString(FaceElement face_element) {
    std::string str;
    switch(face_element) {
    case FRONT:  str = "F";  break;
    case BACK:   str = "B";  break;
    case LEFT:   str = "L";  break;
    case RIGHT:  str = "R";  break;
    case TOP:    str = "U";  break;
    case BOTTOM: str = "D";  break;
    default:     str = "I";  break;
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

FaceElement operator!(FaceElement face) {
    switch(face) {
    case FRONT:  face = BACK;   break;
    case BACK:   face = FRONT;  break;
    case LEFT:   face = RIGHT;  break;
    case RIGHT:  face = LEFT;   break;
    case TOP:    face = BOTTOM; break;
    case BOTTOM: face = TOP;    break;
    }
    return face;
}

template<typename T, size_t CUBE_SIZE>
std::ostream& operator<<(std::ostream &os, const RubikCube<T, CUBE_SIZE> &cube) {
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

std::ostream& operator<<(std::ostream &os, Clockwise clockwise) {
    switch(clockwise) {
    case CLOCKWISE:
        os << "CW"; break;
    case COUNTERCLOCKWISE:
        os << "CCW"; break;
    default:
        os << "INVALID_CW";
    }
    return os;
}


