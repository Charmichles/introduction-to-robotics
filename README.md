# Introduction to Robotics 2021 - 2022

This repo will showcase the projects I worked on for the Introduction to Robotics course during the 3rd year of the Computer Science program at the University of Bucharest. Each project description contains a list of requirements, implementation details, code and image/video material.

## adjustable-rgb-led

The first project is a controllable RGB LED using a separate potentiometer for each of the colors.

Here's a picture of the setup and the [link](https://youtu.be/NDZlMUl-0iA) to my video showcasing functionality.

<img src = "https://github.com/Charmichles/introduction-to-robotics/blob/12506e7ef0ef9ed2d788a26fb91f4a2b1cbc86e4/project-1/setup.jpeg">

<details>
<summary>See details.</summary><br/>

### Requirements:

-   1 RGB LED
-   3 potentiometers
-   wires and resistors for each component

**Additionally**, I used another single-color LED for a minigame which consists of matching a target color written in the source code using the controllable RGB LED.

### Possible improvements:

-   adding another RGB LED which displays the target color the user is trying to match
-   implementing a game loop where the user wins after matching the given color and the game restarts, randomizing the target color
-   using a mathematical function to determine how close the RGB LED is to match the target color (like a Hot and Cold Game)

This was a cool first project for getting used to Arduino code and the different types of electrical signals and how to process them.

Soon after I turned this project in, I found out that each of the three LEDs have different forward voltages, so using a single resistor for the common anode/cathode causes the colors to have inconsisent intensities due to a single LED dominating the current flow. The solution is using the correct value resistor for each LED, calculated using Ohm's Law. [This](https://www.youtube.com/watch?v=3kntdICTuUQ) video explains it quite well.

</details>
  
## traffic-lights-sim

The second project is a traffic lights simulator with a button that pedestrians need to use for crossing. It also uses a buzzer to signal when it is safe to cross.

Below is a picture of the setup and here's the [link](https://youtu.be/6ApnEIVFysw) to my video showcasing functionality.

<img src = "https://github.com/Charmichles/introduction-to-robotics/blob/cba0ee2761f05f1c13d5b41ed5ccf06a291ab90f/project-2/setup.jpeg">

<details>
<summary>See details.</summary><br/>

### Requirements:

-   2 red LEDs, 2 green LEDs, 1 yellow LED
-   a button
-   an active/passive buzzer
-   wires and resistors for each component

**Additionally**, I used two more blue LEDs for a Reaction Time minigame. The aim is to press the button when a LED lights up at random intervals. A correct press is signaled by the other LED turning on/off.

### Possible improvements:

-   using only one resistor for each group of LEDs, which works because only one LED per group is turned on during execution
-   using interrupts for events instead of the millis() function; I believe this would improve code readability and modularity
-   adding a screen which displays the user's score in the Reaction Time minigame

This was a fun project which taught me how to think of ways to emulate doing multiple tasks at the same time using the Arduino.

</details>
  
## EMF-detector

Project three is an EMF Detector. We had artistic liberty concerning the parts we could use, so I chose a seven-segment display for EMF measurement output and an active buzzer for even more user feedback.

Below's the picture of my setup and here's the [link](https://youtu.be/ltkqo6wFsuc) to the video.

<img src = "https://github.com/andrei-brihac/introduction-to-robotics/blob/75c0bfe7fa2c60558654c86664edc4960c29946d/project-3/setup.jpg">

<details>
<summary>See details.</summary><br/>

### Requirements:

-   seven-segment display or array of LEDs arranged however you like
-   an active/passive buzzer
-   ideally, a 3.3M resistor for the antenna, I only had 1M
-   wires and resistors for the other components
-   some type of antenna (preferably not a paperclip)

### Possible improvements:

-   replacing the 1M antenna resistor with a 3.3M for better sensitivity
-   replacing the paperclip antenna with a proper antenna
-   playing around with values in the code for better sensitivity
-   making it portable with a 9V battery and a better layout

Due to the limitations of my human form, I couldn't use this device to find and communicate with ghosts. Would not recommend.

</details>
  
## controllable-4digit-7segment

Project number four is a controllable 4 digit 7 segment display using a SIPO shift register and a joystick. The digits of a number can be individually chosen in an intuitive way utilizing the joystick.

Here's the picture of the layout and the [link](https://youtu.be/HgGJLYIaZXo) to the video. The setup is quite cluttered, but I hope it's visible enough. No vlogging this time because my roommates were sleeping. :(

<img src = "https://github.com/andrei-brihac/introduction-to-robotics/blob/5e62027150b7ec5fef0bc2e932772f7bcfa27cef/controllable-4digit-7segment/setup.jpeg">

<details>
<summary>See details.</summary><br/>

### Requirements:

-   a 4 digit 7 segment display
-   a SN74HC595 shift register
-   a joystick
-   wires and resistors when needed

### Possbile improvements:

-   a cleaner setup
-   implementing a blink functionality for the chosen digit
-   tweaking values in the code for better joystick input

A good start for working with integrated circuits.

</details>
