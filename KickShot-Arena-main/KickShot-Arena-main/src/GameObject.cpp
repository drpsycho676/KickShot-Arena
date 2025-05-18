#include "GameObject.hpp"
#include <cmath>

// Constructor

GameObject::GameObject(float x, float y, float r, Texture2D t) {
    position.x = x;
    position.y = y;
    radius = r;
    texture = t;
    speed.x = 0;
    speed.y = 0;
}
// Initialize static members
int GameObject::player1_score = 0;
int GameObject::player2_score = 0;

// Updates the position of the object
void GameObject::Update() {
    position.x += speed.x;
    position.y += speed.y;

    // Apply friction to the object's speed
    speed.x *= friction;
    speed.y *= friction;

    // Check if the object collides with the limits of the screen
    // Right boundary


    // GameObject(both ball and palets) should bounce on collision with bottom boundary  
    if (position.y >= 800 - radius) { // since our field height is 800
        position.y = 800 -radius;
        speed.y = -speed.y;
    }
    //GameObject(both ball and palets) should bounce on collision with top boundary  
    if (position.y < radius) {
        position.y = radius;
        speed.y = -speed.y;
    }

    // Stop the object if its speed is very low to prevent infinite deceleration
    if (fabs(speed.x) < 0.02f) speed.x = 0;
    if (fabs(speed.y) < 0.02f) speed.y = 0;
}

// Function to draw the ball/palet on the screen
void GameObject::Draw() {
    DrawTexture(texture, static_cast<float>(position.x - radius), static_cast<float>(position.y - radius), WHITE);
}

