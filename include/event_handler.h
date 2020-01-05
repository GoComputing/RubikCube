#ifndef __RUBIK_EVENT_HANDLER_H__
#define __RUBIK_EVENT_HANDLER_H__

#include <irrlicht.h>

class RubikCubeEventHandler : public irr::IEventReceiver {
private:
    irr::core::position2di _cursor_position;
    bool _left_mouse_pressed;
    bool _right_mouse_pressed;
    
protected:
    /**
      * @brief A callback method called when an event is produced. More info
      *        in Irrlich docs.
      */
    virtual bool OnEvent(const irr::SEvent &e);
    
public:
    /**
      * @brief Initializes event store variables. 
      */
    RubikCubeEventHandler()
        : _left_mouse_pressed(false),
          _right_mouse_pressed(false)
    {
        
    }
    
    /**
      * @brief Returns if left mouse button is pressed.
      * @return Returns true if left mouse button is pressed. False in other case.
      */
    bool GetLeftMousePressed() const {
        return _left_mouse_pressed;
    }
    
    /**
      * @brief Returns if right mouse button is pressed.
      * @return Returns true if right mouse button is pressed. False in other case.
      */
    bool GetRightMousePressed() const {
        return _right_mouse_pressed;
    }
    
    /**
      * @brief Returns cursor position.
      * @return Returns cursor position from top left corner of window. It
      *         has X and Y as integer attributes.
      */
    const irr::core::position2di& GetCursorPosition() const {
        return _cursor_position;
    }
};

#endif
