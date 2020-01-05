#ifndef __RUBIK_IRRLICHT_TOOLS_H__
#define __RUBIK_IRRLICHT_TOOLS_H__

#include <irrlicht.h>

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

#endif
