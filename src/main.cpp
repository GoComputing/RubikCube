#include <graphical_cube.h>
#include <iostream>

int main() {
    auto cube = GraphicalRubikCube<2>(L"Rubik Cube", 512, 512, 1.0);
    
    //cube.RotateFace(LEFT, CLOCKWISE, 0);
    //std::cout << cube << std::endl << std::endl;
    //cube.RotateFace(TOP, COUNTERCLOCKWISE, 1);
    //std::cout << cube << std::endl;
    
    while(cube.ShouldContinue()) {
        cube.UpdateFrame();
    }
    
    return 0;
}
