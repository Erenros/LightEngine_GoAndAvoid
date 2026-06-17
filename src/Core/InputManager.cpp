#include "InputManager.h"

void InputManager::Update() {
    std::unordered_map<short, bool>::iterator it;

    for (it = m_keysDownReset.begin(); it != m_keysDownReset.end(); it++) {
        if (it->second == false) {
            if ((GetAsyncKeyState(it->first) & 0x8000) == 0) {
                it->second = true;
            }
        }
    }
}

bool InputManager::IsDown(const short key){
    if ((GetAsyncKeyState(key) & 0x8000) != 0) {
        if (m_keysDownReset.find(key) == m_keysDownReset.end()) {
            m_keysDownReset[key] = false;
            return true;
        }
        else {
            if (m_keysDownReset[key] == true) {
                m_keysDownReset[key] = false;
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

bool InputManager::IsHeld(const char key){
    return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

bool InputManager::IsUp(const char key){
    return ((GetAsyncKeyState(key) & 0x8000) == 0);
}

Vector2<long> InputManager::GetMouseRelativePosition(){
    
    POINT pos ;

    GetCursorPos(&pos);
    return { pos.x, pos.y };
}

Vector2<long> InputManager::GetMouseWorldPosition(){
    //TODO a faire
    return Vector2<long>();
}
