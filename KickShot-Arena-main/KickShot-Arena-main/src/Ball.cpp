#include "Ball.hpp"


// Constructor
Ball::Ball(float x, float y, float r, Texture2D t) : GameObject(x, y, r, t) {}

// Resets the ball's position and speed
void Ball::Reset()
{
    position.x = 640;
    position.y = 400;
    speed.x = 0;
    speed.y = 0;
}

void Ball::Update()
{
    GameObject::Update();
    // Right boundary
    if (position.x > (1210 - radius) && ((position.y + radius) < 250 || (position.y + radius) > 550))
    {
        position.x = (1210 - radius);
        speed.x = -speed.x;
    }

    // Left boundary
    if (position.x < (radius + 70) && ((position.y + radius) < 250 || (position.y + radius) > 550))
    {
        position.x = (radius + 70);
        speed.x = -speed.x;
    }
}