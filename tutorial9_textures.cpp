//Windows includes - DO NOT EDIT AS LOTS OF INTERDEPENDENCE
//Working collision and dull project 
//No space bar movement --> left target cannot be shot

#include "shared/GLee.h"
#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>
#include <vector>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif


/*TEXTURES*/
#define IMAGE1   0
#define FOOTBALL_IMAGE   1
#define FLOWER_WALL 2
#define GRASS 3
#define TARGET_DULL 4
#define TARGET_BLUE 5
#define TARGET_RED 6
#define TARGET_GREEN 7
#define SKYBOX 8
#define ui_right_texture 9
#define ui_left_texture 10

#define TEXTURE_COUNT 11

GLuint textures[TEXTURE_COUNT];

GLint iWidth, iHeight, iComponents;
GLenum eFormat;
GLbyte* pyBytes0;
const char* textureFiles[TEXTURE_COUNT] = {
    "mybricktexture_longplant.tga",
    "FootballCompleteMap.tga",
    "flowerplant_Wall.tga",
    "grass_diff.tga",
    "targetDull.tga",
    "targetBlue.tga",
    "targetRed.tga",
    "targetGreen.tga",
    "stormydays_large.tga",
    "fillBarVerticalR.tga",
    "fillBarHorizontal.tga" };


GLfloat cubeVertexData[108] =
{
    //object vertex coordinates for cube made from triangles
    // Data layout for each line below is:
    // positionX, positionY, positionZ
    //wind counter-clockwise

    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,

    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    //4
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    //5
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    //6
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f

};


//the texture coordinates - work will skybox texture of this shape +--

GLfloat textureCoordsSkyBox[72] = {
    //face 1

    0.75,0.33,      //    0,1,
    0.75,0.67,     //    1,1,
    0.5,0.33,     //    0,0,
    0.5,0.33,     //    0,0,
    0.75,0.67,    //    1,0,
    0.5,0.67,   //    1,1,

    //face 2
    0.5,1.0, //    1,1,
    0.25,1, //    0,1,
    0.5,0.67, //    1,0,
    0.5,0.67, //    1,0,
    0.25,1.0, //    0,1,
    0.25,0.67, //    1,1,
    //face 3
    0,0.67,//    1,1,
    0,0.33,//    0,1,
    0.25,0.67,//    1,0,
    0.25,0.67,//    1,0,
    0,0.33,//    0,1,
    0.25,0.33,//    0,0,
    //face 4
    0.25,0.0,//    0,1,
    0.5,0.0,//    1,1,
    0.25,0.33,//    0,0,
    0.25,0.33,//    0,0,
    0.5,0.0,//    1,1,
    0.5,0.33,//    0,0,
    //face 5
    0.5,0.67,//    1,0,
    0.25,0.67,//    0,0,
    0.5,0.33,//    1,1,
    0.5,0.33,//    1,1,
    0.25,0.67,//    0,0,
    0.25,0.33,//    0,1,
    //face 6
    0.75,0.33,//    1,1,
    1.0,0.33,//    0,1,
    0.75,0.67,//    1,0,
    0.75,0.67,//    1,0,
    1.0,0.33,//    0,1,
    1.0,0.67//    0,0

};

//the lighting normals - all facing out from each face
GLfloat gCubeVertexdataNormals[108] =
{
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f

};

GLUquadricObj* quadricFootball;


/* Window properties */
GLuint windowWidth = 1280;
GLuint windowHeight = 720;


/* Camera properties */
GLfloat cameraX = 0;
GLfloat cameraY = 50;
GLfloat cameraZ = 200;

bool moveCamera = false;
bool movePlayer = false;


GLfloat LightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

/* TOP LIGHT OVERALL SCENE */
GLfloat LightPos[] = { 0.0f, 100.0f, 20.f, 0.0f };
GLfloat LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat LightDif[] = { 1.0, 1.0f, 1.0f, 1.0f };

/* BACKIGHT LIGHT */
GLfloat Light1Pos[] = { 0, 20, 180, 0.0f };
GLfloat Light1Ambient[] = { 0.6, 0.6, 0.4, 1.0f };
GLfloat Light1Dif[] = { 1.0, 1.0f, 1.0f, 1.0f };

/* LIGHT SETTINGS */
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat Materialshine = 80;

GLfloat whiteLight[] = { 1.0, 1.0, 1.0, 1.0 };


/* Vector3 struct to set colour on objects */
struct vector3
{
    float x, y, z;
    vector3() : x(0), y(0), z(0) {} //set all variables to 0
    /* Constructor parameter variables set to struct variables */
    vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    /* Does not update the variables, returns single calculation at given time*/
    vector3 operator+(const vector3& vector) const {
        return vector3(x + vector.x, y + vector.y, z + vector.z);
    }
    /* Updates the variables by adding new value to the current value of x,y */
    vector3 operator+=(const vector3& vector) {
        return vector3(x += vector.x, y += vector.y, z += vector.z);
    }
    /* Does not update the variables, returns single calculation at given time*/
    vector3 operator-(const vector3& vector) const {
        return vector3(x - vector.x, y - vector.y, z - vector.z);
    }
    vector3 operator-=(const vector3& vector) {
        return vector3(x -= vector.x, y -= vector.y, z -= vector.z);
    }
};

struct vector4
{
    float x, y, z, a;
    vector4() : x(0), y(0), z(0), a(0) {} //set all variables to 0
   /* Constructor parameter variables set to struct variables */
    vector4(float x_, float y_, float z_, float a_) : x(x_), y(y_), z(z_), a(a_) {}
};

/* GameObject class to render 2D Game primitives e.g. Player/Obstacles */
class GameObject {
public:
    GameObject();
    //GameObject contructor to set x,y,width, height & colour for glRect()
    GameObject(const GLfloat rotation, const vector3& position, const vector3& size, const vector3& rotationAxis);

    void Football(GLfloat r);
    void TexQuad(int image, GLenum blend, GLenum alpha, GLenum one_minus) const;
    void Camera_withPlayer();

    // getter/setters to get position/set position of object
    vector3 get_position() const;
    void set_position(const vector3& position);
    //getter/setter to get the width & height of the glRect takes vector3 for width and height
    vector3 get_size() const;
    void set_Size(const vector3& size);
    //getter/setter for rotation of the GameObject, taking angle to set glRotatef() angle
    GLfloat get_rotation() const;
    void set_rotation(GLfloat angle);
    //set colour of a GameObject, if it needs to be updated from original from constructor
    void set_Colour(const vector3& color);

    // Check collision between objects. Since all objects being rendered are of type GameObject
    // All have m_position.x,y & m_size.x,y and so we can loop through and check their values against the value of the object
    // we wish to check if collsion has occured e.g. the player, so we pass player position & size
    //bool isCollision(const vector3& position, const vector3& size) const
    //{
    //    /* if the follwing returns true, there is collision */
    //    return position.x + size.x > m_position.x &&  // check if x pos + width of player > object x pos
    //        position.x < m_position.x + m_size.x &&  //  check if x pos of player < object x pos + width
    //        position.y + size.y > m_position.y &&   //   check if y pos + height of player > object y pos
    //        position.y < m_position.y + m_size.y;   //   check if player y pos < object y pos + height
    //}

private:
    /* Class variabels to store position, size of object, rotation & colour */
    vector3 m_position;
    vector3 m_rotationAxis;
    vector3 m_size;
    GLfloat m_rotation;
};

GameObject::GameObject() {}
GameObject::GameObject(const GLfloat rotation, const vector3& position, const vector3& size, const vector3& rotationAxis)
{
    /* set class variables to contain the values passed in when contrucutor is initialized */
    m_position = position;
    m_rotation = rotation;
    m_rotationAxis = rotationAxis;
    m_size = size;
}

void GameObject::Football(GLfloat r)
{
    glPushMatrix();

    glFrontFace(GL_CCW);
    glTranslatef(m_position.x, m_position.y, m_position.z);
    // Create and texture the ball
    glRotatef(m_rotation, 1, 0, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[FOOTBALL_IMAGE]);
    // glDisable(GL_LIGHTING);
    glColor3f(0.5, 0.5, 0.5);
    quadricFootball = gluNewQuadric();
    gluQuadricDrawStyle(quadricFootball, GLU_FILL);
    gluQuadricNormals(quadricFootball, GLU_SMOOTH);
    gluQuadricOrientation(quadricFootball, GLU_OUTSIDE);
    gluQuadricTexture(quadricFootball, GL_TRUE);
    glMaterialf(GL_FRONT, GL_SHININESS, Materialshine);
    gluSphere(quadricFootball, r, 50, 35);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void GameObject::TexQuad(int image, GLenum blend, GLenum alpha, GLenum one_minus) const
{
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, m_position.z);
    glRotatef(m_rotation, m_rotationAxis.x, m_rotationAxis.y, m_rotationAxis.z);
    glEnable(blend);
    glBlendFunc(alpha, one_minus);
    glEnable(GL_TEXTURE_2D);
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[image]); //bind texture via ID
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-m_size.x, 0.0, 100.0f); //bottom left
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(m_size.x, 0.0, 100.0f);  //bottom right
    glTexCoord2f(1.0, 1.0);
    glVertex3f(m_size.x, m_size.y, m_size.z); //top right
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-m_size.x, m_size.y, m_size.z);//top left
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

vector3 GameObject::get_position() const //returns the position of the object
{
    return m_position;
}

void GameObject::set_position(const vector3& position) //sets the position of the object
{
    m_position = position;
}

vector3 GameObject::get_size() const
{
    return m_size;
}

void GameObject::set_Size(const vector3& size)
{
    m_size = size;
}


GLfloat GameObject::get_rotation() const //gets the rotation of the object
{
    return m_rotation;
}

void GameObject::set_rotation(GLfloat angle) //set the rotation of the object
{
    m_rotation = angle;
}

void GameObject::Camera_withPlayer()
{
    cameraZ -= 50;
}

/* GameInterface class to create objects relating to the games UI or similar */
class GameInterface
{
public:

    //Display text on screen
    void writeBitMapString(const vector3& position, // Writing text onto the screen
        const vector3& color, void* font, char* string);

    //Rendering 2D UI Textures 
    void imageUI(int image);

    //Rendering skybox
    void SkyBox();
    //Rotating skybox
    GLfloat getSkyBoxRotation();
    //Setting skybox rotation
    void setSkyBoxRotation(GLfloat skyboxrotation);

    //2D UI Orthographic projection
    void setOrthographicProjection();
    //Resetting 2D UI Orthographic projection
    void resetPerspectiveProjection();

    GLfloat get_Points(); //get the current amount of points
    void set_Points(GLfloat points); //set the current points

    //Get current value of attempts variable
    GLfloat get_Attemps();
    //Set the value of attempts variable
    void set_Attemps(GLfloat updateAttemps);

    void GameInstructions(); // print instructions into game terminal

private:
    GLfloat m_points; //stores the points in the game
    GLint m_Grid = 0.0;//determine if grid is on or off 1.0 == on 0.0 == off
    GLfloat m_skyBoxRotation = 0.0f;
    GLfloat m_attempts = 10.0f;
};


/* Writng text to screen & positioning function */
void GameInterface::writeBitMapString(const vector3& position,
    const vector3& color, void* font, char* string)
{
    glColor3f(color.x, color.y, color.z); //set colour of text
    glRasterPos2f(position.x, position.y); //position the text on the screen

    char* c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c); //create the text on the screen
}

GLfloat GameInterface::getSkyBoxRotation()
{
    return m_skyBoxRotation;
}

void GameInterface::setSkyBoxRotation(GLfloat skyboxrotation)
{
    m_skyBoxRotation = skyboxrotation;
}



void GameInterface::imageUI(int image)
{
    //add some lighting normals for each vertex
     //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
    // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);

    glVertex3f(-10.0, 0.0, 50.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 50.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(10.0, 50.0, 50.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-10.0, 50.0, 50.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void GameInterface::SkyBox()
{
    //bind your texture here


    glFrontFace(GL_CCW); //texture the inside

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[SKYBOX]);
    glTranslatef(0, 50.0f, 0);
    glScalef(450, 450, -500);
    glRotatef(m_skyBoxRotation, 0, 1, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, gCubeVertexdataNormals);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cubeVertexData);
    //   glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsSkyBox);
    // draw a cube - type - start number - number of vertices to draw (so 3 for single triangle)
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

}

GLfloat GameInterface::get_Points() //returns the amount of points currently in the game
{
    return m_points;
}

void GameInterface::set_Points(GLfloat points)//sets the points in the game
{
    m_points = points;
}

GLfloat GameInterface::get_Attemps()
{
    return m_attempts;
}

void GameInterface::set_Attemps(GLfloat updateAttemps)
{
    m_attempts = updateAttemps;
}

/* Game instructions printing to the terminal function */
void GameInterface::GameInstructions()
{
    std::cout << "Welcome to the Football game! " << std::endl;
    std::cout << "****************************** " << std::endl;
    std::cout << "Left & Right Arrow keys aim left or right " << std::endl;
    std::cout << "Up & Down Arrow keys aim Up or Down" << std::endl;
    std::cout << "Press Z to shoot the ball " << std::endl;
    std::cout << "Press F to change camera view to Football " << std::endl;
    std::cout << "Press R to reset the game " << std::endl;
    std::cout << "Press Q to quit the game " << std::endl;
}

void GameInterface::setOrthographicProjection() //set 2d projection for UI
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1280, 0, 720);
    glScalef(1, -1, 1);
    glTranslatef(0, -720, 0);
    glMatrixMode(GL_MODELVIEW);
}

void GameInterface::resetPerspectiveProjection() //reset projection
{

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

vector3 followPlayer; //vector to position centre of camera to follow ball
vector3 FootballShootAngle; //vector for shooting the ball

GameObject player(0, vector3(0.0f, 15, 100), vector3(0, 0, 0), vector3(0, 0, 0)); //Player creation/properties set

/* SCENE OBJECTS -- WALLS -- FLOOR -- DULL_TARGET */
GameObject Wall1(0, vector3(0, 0, -200), vector3(200, 100.0f, 100.0f), vector3(0, 1, 0));
GameObject Wall2(90, vector3(-300, 0, 35.0f), vector3(135.0f, 100.0f, 100.0f), vector3(0, 1, 0));
GameObject Wall3(-90, vector3(300, 0, 35.0f), vector3(135.0f, 100.0f, 100.0f), vector3(0, 1, 0));
GameObject Floor(-90, vector3(0, -100, 170), vector3(200.0f, 280.0f, 110), vector3(1, 0, 0));
GameObject targetDull(-90, vector3(0, -97, 110), vector3(12, 20.0f, 100.0f), vector3(1, 0, 0)); 


enum colour {
    blue,
    red,
    green,
    dull
};

struct GameTarget : public GameObject {

    GameTarget() {}

    colour targetColour;

    GameTarget(colour targetColour_, const GLfloat rotation, const vector3& position, const vector3& size, const vector3& rotationAxis)
        : GameObject(rotation, position, size, rotationAxis), targetColour(targetColour_) {}

};

GameTarget targets[8]; //targets array to hold GameTarget objects

GameInterface GameUI; //Make an instance of GameInterface called GameUI so we can change UI components throughout the code/add/remove


/* Animation properties */
GLfloat ballSpin = 0.0f;

/* Reshape function to set viewport/ set viewport scaling */
void reshape(GLsizei width, GLsizei height)
{
    GLfloat fAspect;

    if (height == 0) //make sure the window height is not 0 -> This can lead game to break if height == 0
        height = 1;

    glViewport(0, 0, width, height);

    fAspect = (GLfloat)width / (GLfloat)height;

    glMatrixMode(GL_PROJECTION); //Using GL_PROJECTION so we can adjust camera to view the game
    glLoadIdentity();//Load identity matrix

    gluPerspective(90.0f, fAspect, 1.0, 1000.0);

    /* Model view allows for object transformation */
    glMatrixMode(GL_MODELVIEW);
}

/* Arrowkey handling function for Player movement input */
void specialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP && FootballShootAngle.y < 3)
    {
        FootballShootAngle.y += 0.1;
    }
    else if (key == GLUT_KEY_DOWN && FootballShootAngle.y > 0)
    {
        FootballShootAngle.y -= 0.1f;
    }

    else if (key == GLUT_KEY_LEFT && FootballShootAngle.x <= 7.1)
    {
        FootballShootAngle.x += 0.5f;
    }

    else if (key == GLUT_KEY_RIGHT && FootballShootAngle.x >= -7.2)
    {
        FootballShootAngle.x -= 0.5f;
    }
    glutPostRedisplay();
}
/* Check player keyboard input for reset/kickball/change camera/quit game */
void keyboard(unsigned char key, int x, int y)
{
    //switch case to see which key was pressed, switch the case according to the key
    switch (key)
    {
    case 'z': //Allow player to move ball : update attemps : check attemps : check reset for attemps
        movePlayer = !movePlayer;
        GameUI.set_Attemps(GameUI.get_Attemps() - 1.0f);
        if (GameUI.get_Attemps() <= 0)
        {
            movePlayer = false;
            if (key != 'r') {
                GameUI.set_Attemps(0);
            }
        }
        break;
    case 'f':
        moveCamera = !moveCamera;
        break;
    case 'r': //reset the game -> reset points/attemps : re-draw targets : if camera moving? reset to original camera
        GameUI.set_Attemps(0);
        GameUI.set_Attemps(GameUI.get_Attemps() + 10.0f);
        GameUI.set_Points(0.0f);
        player.set_position(vector3(0.0f, 15, 100));
        targets[0] = GameTarget(colour::blue, 0, vector3(-40, 10, -199), vector3(20, 40, 100), vector3(0, 0, 0));
        targets[1] = GameTarget(colour::blue, 0, vector3(40, 45, -199), vector3(20, 40, 100), vector3(0, 0, 0));

        targets[2] = GameTarget(colour::red, 0, vector3(-83, 65, -199), vector3(12, 24, 100), vector3(0, 0, 0));
        targets[3] = GameTarget(colour::red, 0, vector3(-180, 10, -199), vector3(12, 24, 100), vector3(0, 0, 0));
        targets[4] = GameTarget(colour::red, 0, vector3(110, 65, -199), vector3(12, 24, 100), vector3(0, 0, 0));
        targets[5] = GameTarget(colour::red, 0, vector3(170, 40, -199), vector3(12, 24, 100), vector3(0, 0, 0));

        targets[6] = GameTarget(colour::green, 0, vector3(-140, 40, -199), vector3(25, 48, 100), vector3(0, 0, 0));
        targets[7] = GameTarget(colour::green, 0, vector3(110, 10, -199), vector3(25, 48, 100), vector3(0, 0, 0));
        for (const auto& i : targets) {
            i.TexQuad((i.targetColour == colour::blue ? TARGET_BLUE : i.targetColour == colour::red ? TARGET_RED : i.targetColour == colour::green ? TARGET_GREEN : TARGET_DULL), GL_BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        if (moveCamera == true)
        {
            moveCamera = false;
        }
        break;
    case 'q': //quit game if q pressed
        std::cout << "You quit the game" << std::endl; //print message to console letting user know, the game has been exited
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

/* Dislay function for rendering all OpenGL components */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();//identity matrix needed for object transformation
    glMatrixMode(GL_MODELVIEW); //set model view to allow us to transform objects

    gluLookAt(cameraX, cameraY, cameraZ,//eye
        followPlayer.x, followPlayer.y, followPlayer.z,//centre
        0.00, 1.00, 0.00);//up  

    //Set position of lights
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
    glLightfv(GL_LIGHT1, GL_POSITION, Light1Pos);

    /* LIGHT0 POS POINT */
    glPushMatrix();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(16.0);
    glBegin(GL_POINTS);
    glColor3ub(255, 255, 255);
    glVertex3f(LightPos[0], LightPos[1], LightPos[2]);
    glEnd();
    glPopMatrix();

    ///* LIGHT1 POS POINT */
    glPushMatrix();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(16.0);
    glBegin(GL_POINTS);
    glColor3ub(0, 0, 255);
    glVertex3f(Light1Pos[0], Light1Pos[1], Light1Pos[2]);
    glEnd();
    glPopMatrix();

    /*  Rendering the walls & Floor*/
    Wall1.TexQuad(FLOWER_WALL, NULL, NULL, NULL);
    Wall2.TexQuad(IMAGE1, NULL, NULL, NULL);
    Wall3.TexQuad(IMAGE1, NULL, NULL, NULL);
    Floor.TexQuad(GRASS, NULL, NULL, NULL);

    /* Draw the football */
    player.Football(12);

    // Loop throughtargets array and check the colour value of each object. Based on colour, set appropriate target texture
    for (const auto& i : targets) {
        i.TexQuad((i.targetColour == colour::blue ? TARGET_BLUE : i.targetColour == colour::red ? TARGET_RED : i.targetColour == colour::green ? TARGET_GREEN : TARGET_DULL), GL_BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    /* DRAW DULL TARGET */
    targetDull.TexQuad(TARGET_DULL, GL_BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Skybox render
    GameUI.SkyBox();


    //2D UI 
    glPushMatrix();
    glLoadIdentity();
    GameUI.setOrthographicProjection();

    glDisable(GL_DEPTH_TEST);

    //POINTS SCORE 
    char score[32];
    sprintf_s(score, "Score: %.0f", GameUI.get_Points());
    GameUI.writeBitMapString(vector3(200, 45, 0), vector3(1, 1, 1), GLUT_BITMAP_HELVETICA_18, score);

    //ATTEMPTS REMAINING 
    char attemps[32];
    sprintf_s(attemps, "Attemps: %.0f", GameUI.get_Attemps());
    GameUI.writeBitMapString(vector3(350, 45, 0), vector3(1, 1, 1), GLUT_BITMAP_HELVETICA_18, attemps);

    //HUD TEXTURE - TOP LEFT
    glBindTexture(GL_TEXTURE_2D, textures[ui_left_texture]);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i(170, 50);
    glTexCoord2i(0, 1);
    glVertex2i(170, 20);
    glTexCoord2i(1, 1);
    glVertex2i(20, 20);
    glTexCoord2i(1, 0);
    glVertex2i(20, 50);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //HUD TEXTURE - BOTTOM RIGHT
    glBindTexture(GL_TEXTURE_2D, textures[ui_right_texture]);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i(1200, 475);
    glTexCoord2i(0, 1);
    glVertex2i(1200, 625);
    glTexCoord2i(1, 1);
    glVertex2i(1230, 625);
    glTexCoord2i(1, 0);
    glVertex2i(1230, 475);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Movement controller - Top left
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0);
    glVertex2f(95 - FootballShootAngle.x * 10, 60);
    glVertex2f(95 - FootballShootAngle.x * 10, 10);
    glEnd();

    //Movement controller - Bottom right
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(1190, 625 - FootballShootAngle.y * 48);
    glVertex2f(1240, 625 - FootballShootAngle.y * 48);
    glEnd();

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    GameUI.resetPerspectiveProjection();

    glutSwapBuffers(); //swap buffer, swaps between buffers to keep window up to date with drawing/events 
}
/* init function to init loading textures, lighting & load GameTargets into targets array  */
void init(void)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(TEXTURE_COUNT, textures);

    for (int i = 0; i < TEXTURE_COUNT; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        pyBytes0 = gltLoadTGA(textureFiles[i], &iWidth, &iHeight, &iComponents,
            &eFormat);

        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pyBytes0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        free(pyBytes0);
    }
    glEnable(GL_TEXTURE_2D);

    /* --------------------------------- LIGHTING -------------------------------------*/

    glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);

    /* OVERALL SCENE LIGHT */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);

    /* Light 0 settings */
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
    glEnable(GL_LIGHT0);

    /* LIGHT 1 SETTINGS */
    glLightfv(GL_LIGHT1, GL_AMBIENT, Light1Ambient);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
    glEnable(GL_LIGHT1);


    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    /* SPECULAR SETTINGS */
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);


    glClearColor(0.0, 0.0, 0.0, 0.0); 

    // Add each target to targets array : Loop to draw each object
    targets[0] = GameTarget(colour::blue, 0, vector3(-40, 10, -199), vector3(20, 40, 100), vector3(0, 0, 0));
    targets[1] = GameTarget(colour::blue, 0, vector3(40, 45, -199), vector3(20, 40, 100), vector3(0, 0, 0));

    targets[2] = GameTarget(colour::red, 0, vector3(-83, 65, -199), vector3(12, 24, 100), vector3(0, 0, 0));
    targets[3] = GameTarget(colour::red, 0, vector3(-180, 10, -199), vector3(12, 24, 100), vector3(0, 0, 0));
    targets[4] = GameTarget(colour::red, 0, vector3(110, 65, -199), vector3(12, 24, 100), vector3(0, 0, 0));
    targets[5] = GameTarget(colour::red, 0, vector3(170, 40, -199), vector3(12, 24, 100), vector3(0, 0, 0));

    targets[6] = GameTarget(colour::green, 0, vector3(-140, 40, -199), vector3(25, 48, 100), vector3(0, 0, 0));
    targets[7] = GameTarget(colour::green, 0, vector3(110, 10, -199), vector3(25, 48, 100), vector3(0, 0, 0));
}

constexpr auto PI = 3.141592;

constexpr auto radians(float value) {
    return value * PI / 180.0;
}

constexpr auto degrees(float value) {
    return value * 180.0 / PI;
}

//Collision function -> between player ball and target objects 
bool colliding(const vector3& p1, const vector3& s1, const vector3& p2, const vector3& s2) {
    return p1.x + s1.x / 2 > p2.x - s2.x &&
        p1.x - s1.x / 2 < p2.x + s2.x &&
        p1.y + s1.y / 2 > p2.y - s2.y &&
        p1.y - s1.y / 2 < p2.y + s2.y &&
        p1.z - s2.z / 2 < p2.z + s2.z &&
        p1.z + s2.z / 2 > p2.z - s2.z;
}

void TimerFunc(int value)
{

    GLfloat currentRotation = player.get_rotation();

    GLfloat skyBoxRoate = GameUI.getSkyBoxRotation();
    GameUI.setSkyBoxRotation(skyBoxRoate += 0.06f);

    if (movePlayer)
    {
        player.set_position(player.get_position() + vector3(-FootballShootAngle.x, FootballShootAngle.y,  -7));
        //player.set_position(player.get_position() + vector3(degrees(cos(radians(90 + FootballShootAngle.x))), sin(FootballShootAngle.y * PI / 180) * 180.0 / PI, -7));
        player.set_rotation(currentRotation += 7.0f);

        if (player.get_position().z <= -60)
        {
            player.set_position(vector3(0.0f, 15, 100));
            movePlayer = false;
        }
        if (currentRotation > 360.0f)
        {
            player.set_rotation(0.0f);
        }
    }

    if (moveCamera)
    {
        followPlayer.x = player.get_position().x;
        followPlayer.y = player.get_position().y;
        followPlayer.z = player.get_position().z;

        cameraZ = player.get_position().z + 30.0f;
        cameraY = player.get_position().y + 30.0f;
    }
    else
    {
        followPlayer.z = 0;
        followPlayer.y = 0;
        followPlayer.z = 0;
        cameraZ = 195.0f;
    }

    if (skyBoxRoate > 360.0f)
    {
        GameUI.setSkyBoxRotation(0.0f);
    }

    // check for collision between targets and player
    for (auto& i : targets) {
        if (colliding(player.get_position(), player.get_size(), i.get_position(), i.get_size())) {
            if (i.targetColour == colour::blue) {
                i.TexQuad(TARGET_DULL, GL_BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                GameUI.set_Points(GameUI.get_Points() + 20);
                i.targetColour = colour::dull;
            }
            else if (i.targetColour == colour::green) {
                i.TexQuad(TARGET_DULL, GL_BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                GameUI.set_Points(GameUI.get_Points() + 10);
                i.targetColour = colour::dull;
            }
            else if (i.targetColour == colour::red) {
                i.TexQuad(TARGET_DULL, GL_BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                GameUI.set_Points(GameUI.get_Points() + 30);
                i.targetColour = colour::dull;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(25, TimerFunc, 1);
}

int main(int argc, char** argv)
{
    GameUI.GameInstructions();//Call GameInstructions function to print game instructions into the console
    GameUI.set_Points(0.0f); //set the points of the game

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);//set window size
    glutInitWindowPosition(100, 100);
    glutCreateWindow("FootballGame");//set window title
    init();
    glutDisplayFunc(display);//call our display function here for rendering
    glutReshapeFunc(reshape);//call reshape function to scale window correctly
    glutSpecialFunc(specialKeys);//register specialkeys so window understand arrow key input
    glutKeyboardFunc(keyboard);//register keyboard input so that window understands keyboard input
    glutTimerFunc(25, TimerFunc, 1);//register glutTimerFunc to use in game 
    glutMainLoop();
    return 0;
}