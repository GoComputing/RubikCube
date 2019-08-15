#include <iostream>
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
    void RotateFace(Face face, Clockwise clockwise, size_t depth = 0);
    
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

template<size_t CUBE_SIZE>
std::ostream& operator<<(std::ostream &os, const RubikCube<CUBE_SIZE> &cube);

int main() {
    RubikCube<3> cube;
    
    std::cout << cube << std::endl;
    
    return 0;
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
void RubikCube<CUBE_SIZE>::RotateFace(Face face, Clockwise clockwise, size_t depth) {
    // TODO
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