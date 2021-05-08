# BattleBot

In this project, I developed a program that will simulate two bots moving around the arena and battling each other. I used OpenGL with C++ in order to create surfaces of revolution to create the bot parts and render them on the screen.

- The two bots were texture-mapped
- A simulation of an FPV (First Person View) was implemented in order to make the game more realistic.

The project also implements an opponent which is not controlled by the user. It uses some artificial intelligence to move on the screen.

- Collision Detection and Health tracking for both the user and the opponent were also implemented

- In order to run the project you need to:

- Clone or download the repository from github
- cd into the BattleBot folder
- run \
   ```g++ BattleBot.cpp QuadMesh.cpp -o battleBot -framework OpenGL -framework GLUT```\
and \
    ```./battleBot```


