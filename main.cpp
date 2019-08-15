#include <iostream>

template<size_t CUBE_SIZE = 3>
class RubikCube {
public:
    /*    Clockwise to rotate face    */
    typedef enum { 
        CLOCKWISE, COUNTERCLOCKWISE 
    } Clockwise;
    
    /*    Face of the cube. Total: 6    */
    typedef enum { 
        FRONT, 
        BACK, 
        LEFT, 
        RIGHT, 
        TOP, 
        BOTTOM 
    } Face;
    
private:
    /* TO-DO */
    
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
};

int main() {
    
    
    return 0;
}