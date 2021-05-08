Please compile the program using
g++ BattleBot.cpp QuadMesh.cpp -o program -framework OpenGL -framework GLUT
on macOS.

ONE wrong/Incomplete part:
- I had a problem with rotating and translating the robot in the direction that it's facing. However,
I came up with a slightly bugged outcome; My robot move in the right direction, but when it turns; it turns
with respect to the origin. I tried to translate it to the origin; rotate and then translate it back, but it
didn't work. What happened was that the rotation worked fine and the translation was only in the X-axis, in that case.
I figured it had something to do with the order of the glRotatef() and glTranslatef() calls in the display(),
but I couldn't fix it.

All of the other requirements should be satisfied.