#include <irrlicht_tools.h>
#include <cassert>
#include <cmath>

void SetCameraAngles(irr::scene::ICameraSceneNode *camera, float pitch, float yaw,
                     const irr::core::vector3df &target, float distance)
{
    assert(camera->getTargetAndRotationBinding());
    assert(distance > 0);
    
    // Rotation matrix
    irr::core::matrix4 rotation_matrix;
    rotation_matrix.setRotationRadians({yaw, pitch, 0.0f});
    
    // Calculate new position
    irr::core::vector3df pos = {0.0f, 0.0f, distance};
    rotation_matrix.rotateVect(pos);
    
    // Calculate new up
    irr::core::vector3df up = {0.0f, 1.0f, 0.0f};
    rotation_matrix.rotateVect(up);
    camera->setUpVector(up);
    
    // Sets new position
    camera->setPosition(pos);
    camera->updateAbsolutePosition();
    camera->setTarget(target);
}

std::ostream& operator<<(std::ostream &os, const irr::video::SColor &color) {
    os << '{' << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ", " << color.getAlpha() << '}';
    return os;
}
