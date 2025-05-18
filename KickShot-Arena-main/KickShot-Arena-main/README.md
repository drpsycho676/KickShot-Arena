# Kickshot Arena

**Kickshot Arena** is an exciting 2D soccer game developed in C++ using the Raylib library, where players score by strategically launching pucks to push a soccer ball into the opponent's goal. The game combines elements of carrom and billiards, offering a dynamic and competitive soccer experience with realistic physics, intuitive controls, and engaging gameplay.

## Features

- **Object-Oriented Design**: Utilizes OOP principles such as abstraction, inheritance, polymorphism, and encapsulation to enhance code readability, maintainability, and reusability.
- **Real-time Collision Detection**: Dynamic and accurate interactions between the ball and pallets, ensuring a smooth gameplay experience.
- **High-Performance Graphics**: Leveraging Raylib's capabilities to provide stunning visuals and smooth performance.
- **Intuitive Controls**: Simple and responsive controls allow players of all skill levels to easily pick up and enjoy the game.
- **Engaging Sound Effects and Music**: High-quality sound effects and music to enhance the overall gaming experience.

## Game Architecture
The architecture includes three main classes, each implemented in separate header (hpp) and implementation (cpp) files:
  1. **GameObject Class (Base, Abstract)**: Provides a common interface and shared attributes for all game objects, with virtual functions for extension by derived classes.
  2. **Ball Class (Derived)**: Inherits from GameObject and implements specific attributes and behaviours for the ball, such as movement and collision detection.
  3. **Palet Class (Derived)**: Inherits from GameObject and encapsulates specific attributes and behaviours for the pallet, overriding virtual functions to provide unique implementations.

## How to Play

1. Use the left and right arrow keys to adjust the power level of your pallet.
2. Click and hold the left mouse button to select and aim the pallet, then release to launch it.

## Screenshots

*(To be uploaded)*

## Source Code

The source code includes the following key components:

- **Collision Detection**: Manages collisions between balls and pallets.
- **Palet Launching**: Handles the logic for launching the pallets based on player input.
- **Goal Post Collision**: Detects collisions with goalposts and updates scores accordingly.

## Contributions

- **Muhammad Abdullah Khan**: Designed and implemented collision logic and graphics, utilizing OOP concepts.
- **Furqanullah**: Developed the power bar, directional arrow, and title window, and handled texture loading.
- **Muhammad Rafay**: Created the selection window, graphical interface, and integrated sound effects.

## Future Expansions

- **Ball/Palet Rotation**: Rotate the ball and pallets to simulate realistic gameplay

## Installation

1. Clone the repository.
   ```bash
   git clone https://github.com/abdul-542004/KickShot-Arena.git
2. Compile the project using your preferred C++ compiler.
3. Run the executable to start playing Kickshot Arena.

## Requirements
<ul>
<li>C++ Compiler</li>
<li>Raylib Library</li>
</ul>



