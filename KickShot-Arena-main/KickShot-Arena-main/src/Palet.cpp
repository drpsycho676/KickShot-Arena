#include "Palet.hpp"

// Constructor
Palet::Palet(float x, float y, float r, Texture2D t) : GameObject(x, y, r, t) {
    initialPosition.x = x;
    initialPosition.y = y;
}


void Palet::Reset() {
    position.x = initialPosition.x;
    position.y = initialPosition.y;
    speed.x = 0;
    speed.y =0;
}

void Palet::Update(){
    GameObject::Update();
    if (position.x > (1210 - radius)) {
        position.x = (1210 - radius);
        speed.x = -speed.x;
    }

    // Left boundary
    if (position.x < (radius + 70) ) {
        position.x = (radius + 70);
        speed.x = -speed.x;
    }
}
