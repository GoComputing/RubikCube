#include <rubik.h>
#include <iostream>

int main() {
    RubikCube<3> cube;
    
    cube.RotateFace(LEFT, CLOCKWISE, 0);
    std::cout << cube << std::endl << std::endl;
    cube.RotateFace(TOP, COUNTERCLOCKWISE, 1);
    std::cout << cube << std::endl;
    
    return 0;
}
