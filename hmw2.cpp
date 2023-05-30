/*********
   CTIS164 - Template Source Program
----------
STUDENT : FETTAH
SECTION : 2
HOMEWORK: #2
----------
----------
ADDITIONAL FEATURES:
  - We can aim the gun.
  PRESS F1 : Reset the game
 
*********/

#ifdef _MSC_VER
#endif  
#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#define GL_SILENCE_DEPRECATION

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT  800

#define TIMER_PERIOD    6
#define TIMER_ON         1

#define D2R 0.0174532

#define FPS 60
#define MAX_FIRE 1000
#define SINGLE   1
#define DOUBLE   2


bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
    double x, y;
}point_t;
typedef struct {
    point_t pos;
    double angle;
}player_t;
typedef struct {
    point_t pos;
    double radius;
    bool hit;
    double x, y;
}alien1_t;
typedef struct {
    double x, y;
    bool hit;
}alien2_t;
typedef struct {
    double x, y;
    bool hit;
}alien3_t;
typedef struct {
    double x, y;
    bool hit;
}alien4_t;
typedef struct {
    double x, y;
    bool hit;
}alien5_t;
typedef struct {
    point_t pos;
    float angle;
    bool active;
}fire_t;

player_t p = { {0,-250}, 0 };
alien1_t alien1 = { -40, false };
alien2_t alien2 = { -250, 350 };
alien3_t alien3 = { 250, 220 };
alien4_t alien4 = { -350, 0 };
alien5_t alien5 = { -540, false };
fire_t fire[MAX_FIRE];
point_t c;
bool spacebar = false;
int fire_rate = 0,
mode = SINGLE,
FIRERATE = 15,

tankDif = 7,
fSpeed = 8;


void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void vertex(point_t P, point_t Tr, double angle)
{
    float xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    float yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2f(xp, yp);
}

void drawBackground() {


    glColor3ub(255, 195, 0 );
    glBegin(GL_POLYGON);
    glVertex2f(-winWidth / 2, winHeight / 2);
    glVertex2f(winWidth / 2, winHeight / 2);
    glVertex2f(winWidth / 2, -winHeight / 2);
    glVertex2f(-winWidth / 2, -winHeight / 2);
    glEnd();

    
    glColor3ub(91, 73, 0   );
    glRectf(-1000, -280, 1000, -800);
    glEnd();
    
    //rocks
    glColor3ub(216, 167, 0);
    circle(100,100, 30);
    circle(600, 330, 21);
    circle(267, 300, 5);
    circle(-230, 300, 12);
    circle(-400, 100, 4);
    circle(-600, 300, 6);
    circle(540, -80, 13);
    circle(0, 140, 8);
    circle(400, 100, 6);
    circle(-200, -100, 14);
    circle(-440, -100, 9);



}
void drawLabel()
{
    glColor3ub(255, 92, 0);
    vprint(-480, 350, GLUT_BITMAP_HELVETICA_18, "Fettah ELCIK");
}
void Settings() {

    glColor3f(255, 0, 0);
    vprint(-200, 350, GLUT_BITMAP_HELVETICA_18, "MARS");
    glColor3f(68, 255, 0);
    vprint(-35, -380, GLUT_BITMAP_9_BY_15, "SPACEBAR: FIRE");
    vprint(150, -380, GLUT_BITMAP_9_BY_15, "UP/DOWN: AIM GUN");
    vprint(320, -380, GLUT_BITMAP_9_BY_15, "LEFT/RIGHT: MOVE VEHICLE");

}

void tank() {
    float angle = p.angle * D2R;

    
    glBegin(GL_POLYGON);
    glColor3ub(0, 113, 19);
    vertex({ -60  ,  0 }, p.pos, angle);
    vertex({ 60, 0 }, p.pos, angle);
    vertex({ 30, -30 }, p.pos, angle);
    vertex({ -30  , -30 }, p.pos, angle);
    glEnd();
    
    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
    vertex({ -30, 0 }, p.pos, angle);
    vertex({ -20, 20 }, p.pos, angle);
    vertex({ 20  ,  20 }, p.pos, angle);
    vertex({ 30, 0 }, p.pos, angle);

    glEnd();


    glBegin(GL_POLYGON);
    vertex({ -5, 35 }, p.pos, angle);
    vertex({ 5, 35 }, p.pos, angle);
    vertex({ 5  ,  20}, p.pos, angle);
    vertex({ -5, 20}, p.pos, angle);
    glEnd();
    
    

}

void ALIEN1() {

    //Head of alien
    glColor3ub(26, 189, 0);
    circle(alien1.x, alien1.y, 31);
    glColor3ub(255, 0, 189  );
    circle(alien1.x, alien1.y, 14);
    
    //body
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien1.x +7, alien1.y -30);
    glVertex2f(alien1.x + 7,alien1.y -100);
    glVertex2f(alien1.x -7, alien1.y -100);
    glVertex2f(alien1.x - 7,alien1.y -30);
    glEnd();
    
    //Arms
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien1.x +30, alien1.y -50);
    glVertex2f(alien1.x +30,alien1.y -60);
    glVertex2f(alien1.x -30, alien1.y -60);
    glVertex2f(alien1.x -30,alien1.y -50);
    glEnd();
    
    //legfirst
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien1.x +55, alien1.y -100);
    glVertex2f(alien1.x +55,alien1.y -110);
    glVertex2f(alien1.x -55, alien1.y -110);
    glVertex2f(alien1.x -55,alien1.y -100);
    glEnd();
    
    //leg right
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien1.x +55, alien1.y -100);
    glVertex2f(alien1.x +55,alien1.y -160);
    glVertex2f(alien1.x +45, alien1.y -160);
    glVertex2f(alien1.x +45,alien1.y -100);
    glEnd();
    //leg left
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien1.x -45, alien1.y -100);
    glVertex2f(alien1.x -45,alien1.y -160);
    glVertex2f(alien1.x -55, alien1.y -160);
    glVertex2f(alien1.x -55,alien1.y -100);
    glEnd();
    
    
}
void ALIEN2() {

    //Head of alien
    glColor3ub(26, 189, 0);
    circle(alien2.x, alien2.y, 31);
    glColor3ub(255, 0, 189  );
    circle(alien2.x, alien2.y, 14);
    
    //body
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien2.x +7, alien2.y -30);
    glVertex2f(alien2.x + 7,alien2.y -100);
    glVertex2f(alien2.x -7, alien2.y -100);
    glVertex2f(alien2.x - 7,alien2.y -30);
    glEnd();
    
    //Arms
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien2.x +30, alien2.y -50);
    glVertex2f(alien2.x +30,alien2.y -60);
    glVertex2f(alien2.x -30, alien2.y -60);
    glVertex2f(alien2.x -30,alien2.y -50);
    glEnd();
    
    //legfirst
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien2.x +55, alien2.y -100);
    glVertex2f(alien2.x +55,alien2.y -110);
    glVertex2f(alien2.x -55, alien2.y -110);
    glVertex2f(alien2.x -55,alien2.y -100);
    glEnd();
    
    //leg right
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien2.x +55, alien2.y -100);
    glVertex2f(alien2.x +55,alien2.y -160);
    glVertex2f(alien2.x +45, alien2.y -160);
    glVertex2f(alien2.x +45,alien2.y -100);
    glEnd();
    //leg left
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien2.x -45, alien2.y -100);
    glVertex2f(alien2.x -45,alien2.y -160);
    glVertex2f(alien2.x -55, alien2.y -160);
    glVertex2f(alien2.x -55,alien2.y -100);
    glEnd();


}

void ALIEN3() {

    //Head of alien
    glColor3ub(26, 189, 0);
    circle(alien3.x, alien3.y, 31);
    glColor3ub(255, 0, 189  );
    circle(alien3.x, alien3.y, 14);
    
    //body
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien3.x +7, alien3.y -30);
    glVertex2f(alien3.x + 7,alien3.y -100);
    glVertex2f(alien3.x -7, alien3.y -100);
    glVertex2f(alien3.x - 7,alien3.y -30);
    glEnd();
    
    //Arms
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien3.x +30, alien3.y -50);
    glVertex2f(alien3.x +30,alien3.y -60);
    glVertex2f(alien3.x -30, alien3.y -60);
    glVertex2f(alien3.x -30,alien3.y -50);
    glEnd();
    
    //legfirst
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien3.x +55, alien3.y -100);
    glVertex2f(alien3.x +55,alien3.y -110);
    glVertex2f(alien3.x -55, alien3.y -110);
    glVertex2f(alien3.x -55,alien3.y -100);
    glEnd();
    
    //leg right
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien3.x +55, alien3.y -100);
    glVertex2f(alien3.x +55,alien3.y -160);
    glVertex2f(alien3.x +45, alien3.y -160);
    glVertex2f(alien3.x +45,alien3.y -100);
    glEnd();
    //leg left
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien3.x -45, alien3.y -100);
    glVertex2f(alien3.x -45,alien3.y -160);
    glVertex2f(alien3.x -55, alien3.y -160);
    glVertex2f(alien3.x -55,alien3.y -100);
    glEnd();


}
void ALIEN4() {

    //Head of alien
    glColor3ub(26, 189, 0);
    circle(alien4.x, alien4.y, 31);
    glColor3ub(255, 0, 189  );
    circle(alien4.x, alien4.y, 14);
    
    //body
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien4.x +7, alien4.y -30);
    glVertex2f(alien4.x + 7,alien4.y -100);
    glVertex2f(alien4.x -7, alien4.y -100);
    glVertex2f(alien4.x - 7,alien4.y -30);
    glEnd();
    
    //Arms
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien4.x +30, alien4.y -50);
    glVertex2f(alien4.x +30,alien4.y -60);
    glVertex2f(alien4.x -30, alien4.y -60);
    glVertex2f(alien4.x -30,alien4.y -50);
    glEnd();
    
    //legfirst
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien4.x +55, alien4.y -100);
    glVertex2f(alien4.x +55,alien4.y -110);
    glVertex2f(alien4.x -55, alien4.y -110);
    glVertex2f(alien4.x -55,alien4.y -100);
    glEnd();
    
    //leg right
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien4.x +55, alien4.y -100);
    glVertex2f(alien4.x +55,alien4.y -160);
    glVertex2f(alien4.x +45, alien4.y -160);
    glVertex2f(alien4.x +45,alien4.y -100);
    glEnd();
    //leg left
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien4.x -45, alien4.y -100);
    glVertex2f(alien4.x -45,alien4.y -160);
    glVertex2f(alien4.x -55, alien4.y -160);
    glVertex2f(alien4.x -55,alien4.y -100);
    glEnd();


}
void ALIEN5() {

    //Head of alien
    glColor3ub(26, 189, 0);
    circle(alien5.x, alien5.y, 31);
    glColor3ub(255, 0, 189  );
    circle(alien5.x, alien5.y, 14);
    
    //body
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien5.x +7, alien5.y -30);
    glVertex2f(alien5.x + 7,alien5.y -100);
    glVertex2f(alien5.x -7, alien5.y -100);
    glVertex2f(alien5.x - 7,alien5.y -30);
    glEnd();
    
    //Arms
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien5.x +30, alien5.y -50);
    glVertex2f(alien5.x +30,alien5.y -60);
    glVertex2f(alien5.x -30, alien5.y -60);
    glVertex2f(alien5.x -30,alien5.y -50);
    glEnd();
    
    //legfirst
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien5.x +55, alien5.y -100);
    glVertex2f(alien5.x +55,alien5.y -110);
    glVertex2f(alien5.x -55, alien5.y -110);
    glVertex2f(alien5.x -55,alien5.y -100);
    glEnd();
    
    //leg right
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien5.x +55, alien5.y -100);
    glVertex2f(alien5.x +55,alien5.y -160);
    glVertex2f(alien5.x +45, alien5.y -160);
    glVertex2f(alien5.x +45,alien5.y -100);
    glEnd();
    //leg left
    glColor3ub(26, 189, 0);
    glBegin(GL_POLYGON);
    glVertex2f(alien5.x -45, alien5.y -100);
    glVertex2f(alien5.x -45,alien5.y -160);
    glVertex2f(alien5.x -55, alien5.y -160);
    glVertex2f(alien5.x -55,alien5.y -100);
    glEnd();


}
void Fires() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active && mode == SINGLE) {
            glColor3ub(0, 0, 0);
            circle(fire[i].pos.x, fire[i].pos.y, 5);
        }

    }
}

int AvailableFire() {
    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active == false) return i;
    }
    return -1;
}

bool alien1_hit(fire_t fire, alien1_t alien1) {
    if (fire.pos.x <= alien1.x + 90 && fire.pos.x >= alien1.x &&
        fire.pos.y >= -5 && fire.pos.y <= 17)
        return true;
    else
        return false;
}

bool alien2_hit(fire_t fire, alien2_t alien2) {
    if (fire.pos.x <= alien2.x + 90 && fire.pos.x >= alien2.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}

bool alien3_hit(fire_t fire, alien3_t alien3) {
    if (fire.pos.x <= alien3.x + 90 && fire.pos.x >= alien3.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}
bool alien4_hit(fire_t fire, alien4_t alien4) {
    if (fire.pos.x <= alien4.x + 90 && fire.pos.x >= alien4.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}

bool alien5_hit(fire_t fire, alien5_t alien5) {
    if (fire.pos.x <= alien5.x + 90 && fire.pos.x >= alien5.x &&
        fire.pos.y >= 307.5 && fire.pos.y <= 330)
        return true;
    else
        return false;
}
//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    drawLabel();
    Fires();
    tank();
    Settings();
    if (!alien1.hit) ALIEN1();
    if (!alien2.hit) ALIEN2();
    if (!alien3.hit) ALIEN3();
    if (!alien4.hit) ALIEN4();
    if (!alien5.hit) ALIEN5();
    if (alien1.hit == 1 && alien2.hit == 1 && alien3.hit == 1 && alien4.hit == 1 && alien5.hit == 1)
    {
        glColor3ub(255, 0, 254);
        vprint2(-300, -50, 0.2, "        PRESS F1 TO RESTART");

    }
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    switch (key) {
    case ' ': spacebar = true; break;
        mode = SINGLE;

    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
        spacebar = false;
    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y) {
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F1:
        if (alien2.hit == true)
            alien2.x = -240;
            alien1.hit = false;
            alien2.hit = false;
        break;
    }

    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }


    // to refresh the window it calls display() function
    //glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   mx = x - winWidth / 2;
//   my = winHeight / 2 - y;
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    // Moving tank and Aiming
    if (right) p.pos.x += 18;
    if (left) p.pos.x -= 18;
    if (down) p.angle += 3;
    if (up) p.angle -= 3;

    if (p.pos.x + 40 >= winWidth / 2) p.pos.x -= 5;
    if (p.pos.x - 40 <= -winWidth / 2) p.pos.x += 5;
    if (p.angle > 45)p.angle -= 3;
    if (p.angle < -45)p.angle += 3;

    alien5.x += tankDif;
    if (alien5.x >= winWidth / 2)
        alien5.x = -winWidth / 2 - 120;
    alien4.x += tankDif;
    if (alien4.x >= winWidth / 2)
        alien4.x = -winWidth / 2 - 120;
    alien3.x += tankDif;
    if (alien3.x >= winWidth / 2)
        alien3.x = -winWidth / 2 - 120;
    
    alien2.x += tankDif;
    if (alien2.x >= winWidth / 2)
        alien2.x = -winWidth / 2 - 120;

    alien1.x += tankDif;
    if (alien1.x >= winWidth / 2)
        alien1.x = -winWidth / 2 - 120;

    if (spacebar && fire_rate == 0) {
        int availablefire = AvailableFire();
        if (availablefire != -1) {
            fire[availablefire].pos = p.pos;
            fire[availablefire].angle = p.angle + 90;
            fire[availablefire].active = true;
            fire_rate = FIRERATE;
        }
    }

    if (fire_rate > 0)
        fire_rate--;

    for (int i = 0; i < MAX_FIRE; i++) {
        if (fire[i].active) {
            fire[i].pos.x += fSpeed * cos(fire[i].angle * D2R);
            fire[i].pos.y += fSpeed * sin(fire[i].angle * D2R);

            if (alien1_hit(fire[i], alien1))
                alien1.hit = true;

            if (alien2_hit(fire[i], alien2))
                alien2.hit = true;
            if (alien3_hit(fire[i], alien3))
                alien3.hit = true;
            if (alien4_hit(fire[i], alien4))
                alien4.hit = true;
            if (alien5_hit(fire[i], alien5))
                alien5.hit = true;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(250, 120);
    glutCreateWindow("PROTECT MARS");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
