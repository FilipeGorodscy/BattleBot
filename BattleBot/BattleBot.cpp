#define GL_SILENCE_DEPRECATION
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glut/glut.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "cube.h"
#include "QuadMesh.h"

// Window size
const int vWidth = 650;
const int vHeight = 500;

// robot center point
float robotCenterX = 0.0;
float robotCenterY = 0.0;
float robotCenterZ = 0.0;

// Direction point structure
typedef struct DirectionP
{
    float x;
    float y;
    float z;
} DirectionP;

// Length of direction point
float length;

// Direction point (in front of the robot on the original position)
DirectionP dirPoint = {-5.0, 0.0, 0.0};

// Robot body dimensions and center point (base)
float bodyWidth = 3.0;
float bodyLength = 3.0;
float bodyDepth = 7.0;
float bodyCenterX = robotCenterX;
float bodyCenterY = robotCenterY;
float bodyCenterZ = robotCenterZ;

// Weapon dimensions and center point
float weaponWidth = 4.0;
float weaponLength = 1.0;
float weaponHeight = 0.1;
float weaponCenterX = bodyCenterX + weaponWidth;
float weaponCenterY = bodyCenterY;
float weaponCenterZ = bodyCenterZ;

// Wheels dimenstions and center points
float wheelWidth = 0.7;
float wheelLength = 0.7;
float wheelDepth = 0.5;
float wheelsX = 0.5 * (bodyCenterX + bodyWidth) + 0.5 * wheelWidth;
float wheelsY = bodyCenterY;
float wheelsZ = 0.5 * (bodyCenterZ + bodyDepth) + 0.5 * wheelDepth;

// Angles to rotate the robot, the wheels and the weapon
float robotAngle = 0.0;
float wheelOneAngle = 0.0;
float wheelTwoAngle = 0.0;
float weaponAngle = 0.0;

// Weapon supports dimensions
float supportWidth = 0.5;
float supportLength = 2.0;
float supportDepth = 3.7;

// How much the robot will move forward
float mov = 0.0;

// Direction vector
float directionX = 0.0;
float directionY = 0.0;
float directionZ = 0.0;

// Material and light
GLfloat robotBody_mat_ambient[] = {0.05f, 0.5f, 0.1f, 2.0f};
GLfloat robotBody_mat_diffuse[] = {0.2f, 0.2f, 0.8f, 1.0f};
GLfloat robotBody_mat_specular[] = {0.7f, 0.6f, 0.6f, 1.0f};
GLfloat robotBody_mat_shininess[] = {32.0F};

GLfloat support_mat_ambient[] = {0.05f, 0.5f, 0.1f, 2.0f};
GLfloat support_mat_diffuse[] = {0.6f, 0.35f, 0.0f, 1.0f};
GLfloat support_mat_specular[] = {0.7f, 0.6f, 0.6f, 1.0f};
GLfloat support_mat_shininess[] = {32.0F};

GLfloat wheels_ambient[] = {0.35f, 0.35f, 0.35f, 1.0f};
GLfloat wheels_diffuse[] = {0.01f, 0.0f, 0.01f, 0.01f};
GLfloat wheels_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat wheels_shininess[] = {75.0F};

GLfloat weapon_mat_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
GLfloat weapon_mat_diffuse[] = {0.3f, 0.3f, 0.3f, 1.0f};
GLfloat weapon_mat_specular[] = {0.774597f, 0.774597f, 0.774597f, 1.0f};
GLfloat weapon_mat_shininess[] = {100.0F};

GLfloat light_position0[] = {-4.0F, 8.0F, 8.0F, 1.0F};
GLfloat light_position1[] = {4.0F, 8.0F, 8.0F, 1.0F};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[] = {0.2F, 0.2F, 0.2F, 1.0F};

CubeMesh *cubeMesh = createCubeMesh();
QuadMesh *groundMesh = NULL;

int meshSize = 40;

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void drawBody();
void drawRobot();
void drawWheelOne();
void drawWheelTwo();
void drawWeapon();
void drawSupportWeaponOne();
void drawSupportWeaponTwo();
void drawGoal();
void drawPyramid();
void drawTopSupport();
void drawBotSupport();
void drawVertexBufferObject();

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Batte Bot Assignment 1");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);

    // Start event loop, never returns
    glutMainLoop();

    return 0;
}

void initOpenGL(int w, int h)
{
    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1); // This second light is currently off

    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);              // Remove hidded surfaces
    glShadeModel(GL_SMOOTH);              // Use smooth shading, makes boundaries between polygons harder to see
    glClearColor(0.4F, 0.4F, 0.4F, 0.0F); // Color and depth for glClear
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);                            // Renormalize normal vectors
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nicer perspective

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Other initializatuion
    // Set up ground quad mesh
    VECTOR3D origin = VECTOR3D(-32.0f, 0.0f, 32.0f);
    VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
    VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
    groundMesh = new QuadMesh(meshSize, 50.0);
    groundMesh->InitMesh(meshSize, origin, 75.0, 75.0, dir1v, dir2v);

    VECTOR3D ambient = VECTOR3D(0.0f, 0.0f, 0.0f);
    VECTOR3D diffuse = VECTOR3D(0.3f, 0.3f, 0.6f);
    VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
    float shininess = 0.2;
    groundMesh->SetMaterial(ambient, diffuse, specular, shininess);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 16.0, 35.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Make the direction vector always point in the forward direction in relation to the robot
    directionX = dirPoint.x - robotCenterX;
    directionY = dirPoint.y - robotCenterY;
    directionZ = dirPoint.z - robotCenterZ;

    length = sqrt(directionX * directionX + directionY * directionY + directionZ * directionZ);

    // Normalization of the vector
    directionX /= length;
    directionY /= length;
    directionZ /= length;

    glPushMatrix();
    glRotatef(robotAngle, 0.0, 1.0, 0.0);
    glTranslatef(directionX * mov, directionY * mov, directionZ * mov);

    drawRobot();

    glPopMatrix();

    glPushMatrix();
    drawVertexBufferObject();
    //drawGoal();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -5.0, -5.0);
    groundMesh->DrawMesh(meshSize);
    glPopMatrix();

    glutSwapBuffers();
}

void drawVertexBufferObject()
{

    vboIds = new GLuint[3];
    glGenBuffers(3, vboIds);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Set axes data
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]); // coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(ave), ave, GL_STATIC_DRAW);
    glVertexPointer(nCoordsComponents, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]); // color
    glBufferData(GL_ARRAY_BUFFER, sizeof(ace), ace, GL_STATIC_DRAW);
    glColorPointer(nColorComponents, GL_FLOAT, 0, 0);

    // Draw axes
    glDrawArrays(GL_LINES, 0, nLines * nVerticesPerLine);

    // Set pyramid data
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]); // coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(pve), pve, GL_STATIC_DRAW);
    glVertexPointer(nCoordsComponents, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[3]); // color
    glBufferData(GL_ARRAY_BUFFER, sizeof(pce), pce, GL_STATIC_DRAW);
    glColorPointer(nColorComponents, GL_FLOAT, 0, 0);

    // Draw pyramid
    glDrawArrays(GL_TRIANGLES, 0, nFaces * nVerticesPerFace);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    // Disable the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Pyramid mesh (used to make the two supports for the weapon)
void drawPyramid()
{
    glBegin(GL_TRIANGLES);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

void drawGoal()
{
    glPushMatrix();
    glTranslatef(8.0, 0, 3.0);
    glScalef(0.5, 0.5, 0.5);
    drawCubeMesh(cubeMesh);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(9.0, 1.0, 3.0);
    glScalef(2.0, 0.5, 0.5);
    drawCubeMesh(cubeMesh);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10.0, 0, 3.0);
    glScalef(0.5, 0.5, 0.5);
    drawCubeMesh(cubeMesh);
    glPopMatrix();
}

void drawRobot()
{

    glPushMatrix();
    drawBody();

    glPushMatrix();
    glTranslatef(-weaponCenterX, -weaponCenterY, -weaponCenterZ);
    glRotatef(weaponAngle, 0.0, 1.0, 0.0);
    glTranslatef(weaponCenterX, weaponCenterY, weaponCenterZ);
    drawWeapon();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelsX, wheelsY, -wheelsZ);
    glRotatef(wheelOneAngle, 0.0, 0.0, 1.0);
    glTranslatef(-wheelsX, -wheelsY, wheelsZ);
    drawWheelOne();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wheelsX, wheelsY, wheelsZ);
    glRotatef(wheelTwoAngle, 0.0, 0.0, 1.0);
    glTranslatef(-wheelsX, -wheelsY, -wheelsZ);
    drawWheelTwo();
    glPopMatrix();

    glPushMatrix();
    drawBotSupport();
    drawTopSupport();
    glPopMatrix();

    glPopMatrix();
}

void drawTopSupport()
{

    glMaterialfv(GL_FRONT, GL_AMBIENT, support_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, support_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, support_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, support_mat_shininess);

    glPushMatrix();
    glTranslatef(-(bodyWidth + 0.5), bodyLength * 0.5, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(supportWidth, supportLength, supportDepth);
    drawPyramid();
    glPopMatrix();
}

void drawBotSupport()
{

    glMaterialfv(GL_FRONT, GL_AMBIENT, support_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, support_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, support_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, support_mat_shininess);

    glPushMatrix();
    glTranslatef(-(bodyWidth + 0.5), -bodyLength * 0.5, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glScalef(supportWidth, supportLength, supportDepth);
    drawPyramid();
    glPopMatrix();
}

void drawBody()
{

    glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

    glPushMatrix();
    glTranslatef(bodyCenterX, bodyCenterY, bodyCenterZ);
    glScalef(bodyWidth, bodyLength, bodyDepth);
    glTranslatef(-bodyCenterX, -bodyCenterY, -bodyCenterZ);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawWeapon()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, weapon_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, weapon_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, weapon_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, weapon_mat_shininess);

    glPushMatrix();
    glTranslatef(-weaponCenterX, -weaponCenterY, -weaponCenterZ);

    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(weaponWidth - 3.1, weaponLength, weaponHeight);
    glutSolidTorus(0.7, 2, 52, 52);
    glPopMatrix();

    drawSupportWeaponOne();
    drawSupportWeaponTwo();

    glPushMatrix();
    glTranslatef(0, -1.0, 0);
    glScalef(1.0, 2.5, 1.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(gluNewQuadric(), 0.5, 0.5, 1.0, 20, 10);
    glPopMatrix();

    glPopMatrix();
}

void drawSupportWeaponOne()
{
    glPushMatrix();
    glTranslatef(0.0, 0.5, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(0.9, 0.7, 0.25);
    glutSolidTorus(0.7, 1.5, 52, 52);
    glPopMatrix();
}

void drawSupportWeaponTwo()
{
    glPushMatrix();
    glTranslatef(0, -0.5, 0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glScalef(0.9, 0.7, 0.25);
    glutSolidTorus(0.7, 1.5, 52, 52);
    glPopMatrix();
}

void drawWheelOne()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, wheels_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wheels_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wheels_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, wheels_shininess);

    glPushMatrix();
    glTranslatef(wheelsX, wheelsY, -wheelsZ);

    glPushMatrix();
    glScalef(wheelWidth, wheelLength, wheelDepth);
    glutSolidTorus(0.8, 1.2, 52, 52);
    glPopMatrix();

    // Cube to show that the wheel is rotating!
    /* glPushMatrix();
    glTranslatef(0.5 * wheelDepth + 1.0, 0.0, 0.5 * wheelDepth + 1.0);
    glutSolidCube(1.0);
    glPopMatrix(); */

    glPopMatrix();
}

void drawWheelTwo()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, wheels_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wheels_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wheels_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, wheels_shininess);

    glPushMatrix();
    glTranslatef(wheelsX, wheelsY, wheelsZ);

    glPushMatrix();
    glScalef(wheelWidth, wheelLength, wheelDepth);
    glutSolidTorus(0.8, 1.2, 52, 52);
    glPopMatrix();

    // Cube to show that the wheel is rotating!
    /* glPushMatrix();
    glTranslatef(0.5 * wheelDepth + 1.0, 0.0, 0.5 * wheelDepth + 1.0);
    glutSolidCube(1.0);
    glPopMatrix(); */

    glPopMatrix();
}

void reshape(int w, int h)
{

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)w / h, 0.2, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 16.0, 35.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

bool stop = false;

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'f':
        mov += 0.5;
        wheelOneAngle += 2.0;
        wheelTwoAngle += 2.0;
        break;
    case 'b':
        mov -= 0.5;
        wheelOneAngle -= 2.0;
        wheelTwoAngle -= 2.0;
        break;
    case 's':
        glutTimerFunc(10, animationHandler, 0);
        break;
    case 'S':
        stop = true;
        break;
    }

    glutPostRedisplay();
}

void animationHandler(int param)
{
    if (!stop)
    {
        weaponAngle += 5.0;
        glutPostRedisplay();
        glutTimerFunc(10, animationHandler, 0);
    }
}

void functionKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_F1)
    {
        printf("Commands: HOW TO CONTROL THE BATTLE BOT!\n");
        printf("============================================\n");
        printf("Left Arrow '<-' to turn bot to the left.\n");
        printf("Right Arrow '->' to turn bot to the Right.\n");
        printf("Press 'f' to move forward.\n");
        printf("Press 'b' to move backward.\n");
        printf("Press 's' to animate the weapon.\n");
        printf("Press 'S' to stop animation of the weapon.\n");
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        robotAngle -= 2.0;
        wheelTwoAngle += 2.0;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        robotAngle += 2.0;
        wheelOneAngle += 2.0;
    }

    glutPostRedisplay();
}
