#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>

//defining friction/drag
const float friction = 0.95f;



// abstract class
class GameObject {
public:
    Vector2 position;
    float radius;
    Vector2 speed;
    Texture2D texture;

    // static variables so that score remains same everywhere
    static int player1_score;
    static int player2_score;
    

    // Constructor
    GameObject(float x, float y, float r, Texture2D t);

    // function to draw the object 
    void Draw();

    // Updates the position of the object
    void Update();

    // function to reset the position of the object
    virtual void Reset() = 0;
};