#include <event_handler.h>

bool RubikCubeEventHandler::OnEvent(const irr::SEvent &e) {
    if(e.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        switch(e.MouseInput.Event) {
        case irr::EMIE_LMOUSE_PRESSED_DOWN:
            _left_mouse_pressed = true;
            break;
                
        case irr::EMIE_LMOUSE_LEFT_UP:
            _left_mouse_pressed = false;
            break;
                
        case irr::EMIE_RMOUSE_PRESSED_DOWN:
            _right_mouse_pressed = true;
            break;
                
        case irr::EMIE_RMOUSE_LEFT_UP:
            _right_mouse_pressed = false;
            break;
                
        case irr::EMIE_MOUSE_MOVED:
            _cursor_position.X = e.MouseInput.X;
            _cursor_position.Y = e.MouseInput.Y;
            break;
                
        default:
            break;
        }
    }
        
    return false;
}
