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

void RotateNode(irr::scene::ISceneNode *node, const irr::core::vector3df &rotation_axis, const irr::core::vector3df &center, float angle) {
    
    // Rotates object around it's origin
    irr::core::matrix4 rotation_transform;
    irr::core::matrix4 rotation_matrix;
    irr::core::matrix4 transform;
    rotation_transform.setRotationDegrees(node->getRotation());
    rotation_matrix.setRotationAxisRadians(angle, rotation_axis);
    transform = rotation_matrix * rotation_transform;
    node->setRotation(transform.getRotationDegrees());
    
    // Rotates object position around center
    irr::core::vector3df position = node->getPosition() - center;
    rotation_matrix.rotateVect(position);
    position = position + center;
    node->setPosition(position);
}

void RotateNodes(const std::vector<irr::scene::ISceneNode*> &nodes, const irr::core::vector3df &rotation_axis, const irr::core::vector3df &center, float angle) {
    for(size_t i=0; i<nodes.size(); ++i)
        RotateNode(nodes[i], rotation_axis, center, angle);
}

std::ostream& operator<<(std::ostream &os, const irr::video::SColor &color) {
    os << '{' << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ", " << color.getAlpha() << '}';
    return os;
}
