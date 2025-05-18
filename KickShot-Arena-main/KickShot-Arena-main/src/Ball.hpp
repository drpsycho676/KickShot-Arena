#pragma once
#include "GameObject.hpp"
#include "Palet.hpp"


// Ball is a child of GameObject class
class Ball : public GameObject {
public:
    // Constructor
    Ball(float x, float y, float r, Texture2D t);

    // Resets the ball's position and speed
    void Reset() override;

    // Update the position of the ball
    void Update();

};