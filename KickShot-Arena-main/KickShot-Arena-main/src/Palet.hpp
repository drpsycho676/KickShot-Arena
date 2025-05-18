#pragma once
#include "GameObject.hpp"

class Palet : public GameObject {
private:
    Vector2 initialPosition;
public:
    // Constructor
    Palet(float x, float y, float r, Texture2D t);


    void Update();

    // Resets the palet's position and speed
    void Reset() override;

};