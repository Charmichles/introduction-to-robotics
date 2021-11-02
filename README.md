# Introduction to Robotics 2021 - 2022
This repo will showcase the projects I worked on for the Introduction to Robotics course during the 3rd year of the Computer Science program at the University of Bucharest. Each project description contains a list of requirements, implementation details, code and image/video material.
## Project 1
The first project is a controllable RGB LED using a separate potentiometer for each of the colors.

Requirements:
* 1 RGB LED
* 3 potentiometers
* wires and resistors for each component

Additionally, I used another single-color LED for a minigame which consists of matching a target color written in the source code using the controllable RGB LED.

Here's a picture of the setup and the [link](https://youtu.be/NDZlMUl-0iA) to my video showcasing functionality.

<img src = "https://github.com/Charmichles/introduction-to-robotics/blob/12506e7ef0ef9ed2d788a26fb91f4a2b1cbc86e4/project-1/setup.jpeg">

Possible improvements:
* adding another RGB LED which displays the target color the user is trying to match
* implementing a game loop where the user wins after matching the given color and the game restarts, randomizing the target color
* using a mathematical function to determine how close the RGB LED is to match the target color (like a Hot and Cold Game)

This was a cool first project for getting used to Arduino code and the different types of electrical signals and how to process them.

Soon after I turned this project in, I found out that each of the three LEDs have different forward voltages, so using a signle resistor for the common anode/cathode causes the colors to have inconsisent intensities due to a single LED dominating the current flow. The solution is using the correct value resistor for each LED, calculated using Ohm's Law. [This](https://www.youtube.com/watch?v=3kntdICTuUQ) video explains it quite well.

## Project 2

TBD
