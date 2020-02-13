#ifndef __RUBIK_IRRLICHT_TOOLS_H__
#define __RUBIK_IRRLICHT_TOOLS_H__

#include <irrlicht.h>
#include <iostream>

/**
  * @brief Sets the camera angles, which looks at some target
  * @pre Camera has bound target and rotations (see ICameraSceneNode::bindTargetAndRotation)
  * @pre Distance is positive (> 0)
  * @param camera Camera to set angles
  * @param pitch Pitch of Euler angles
  * @param yaw Yaw of Euler angles
  * @param target Target of camera
  * @param distance Distance from target to camera (always positive)
  */
void SetCameraAngles(irr::scene::ICameraSceneNode *camera, float pitch, float yaw,
                     const irr::core::vector3df &target, float distance);

/**
  * @brief Prints color value, in {R, G, B, A} format
  * @param os Output stream to print
  * @param color Color to print
  * @return Same stream as input. Maybe some flags (like badbit) are set after operation
  */
std::ostream& operator<<(std::ostream &os, const irr::video::SColor &color);

#endif
