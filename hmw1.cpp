
/*
   
CTIS164 Homework

STUDENT :Fettah
 HOMEWORK: #1

*/

#define GL_SILENCE_DEPRECATION
#ifdef _MSC_VER
#endif
#include <GLUT/GLUT.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT  800

#define TIMER_PERIOD    20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

// Modes
#define STOP             0
#define MOVE             1


bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int mx, my;               // Current position of mouse pointer
int xShape, yShape;       // Assigned shape locations
int mode = STOP;          // Move: Start Moving, STOP: stop
bool Timer = false;       // timer
int sCnt = 0;             // Shape count
int r = 0;

typedef struct {
    int count,
        sx, sy;           // Shape Coordinates
}shape_t;

shape_t Shape[10];


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

void background() {
        
        //Sky
        glColor3ub(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(-500, 550);
        glVertex2f(500, 550);
        glColor3ub(117, 0, 115);
        glVertex2f(500, -550);
        glVertex2f(-500, -550);
        glEnd();

        //Earth
        glColor3ub(255, 195, 0);
        glRectf(-500, -250, 500, -400);
        glColor3f(180, 140, 0);
        glRectf(-500, -300, 500, -350);
    
        //Sign Board
        glColor3f(0, 0, 0);
        glRectf(290, -160, 430, -90);
        glColor3f(255, 0, 0);
        glRectf(300, -150, 420, -100);
        glColor3f(0, 0, 0);
        glRectf(350, -150, 370, -250);
        glColor3f(0, 0, 0);
        vprint(310, -130, GLUT_BITMAP_HELVETICA_18, "!AREA 51!");
    
        //objects
        glColor3f(255, 255, 255);
    vprint(-391, +180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-291, 180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-191, 180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-91, 180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(9, 180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(109, 180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(209, 180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(309, 180, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(195, 240, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(296, 240, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(395, 240, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-270, 240, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-170, 240, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-70, 240, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(100, 240, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-400, 337, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-338, 362, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-267, 307, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(-172, 392, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(0, 192, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(46, 343, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(138,364 , GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(224, 315, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(59, 341, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    vprint(283, 360, GLUT_BITMAP_TIMES_ROMAN_24, "<>");
    
    
        //Name
        glColor3ub(75, 245, 248);
        glRectf(-120, 293, 95, 400);
        glColor3ub(75, 248, 81);
        glRectf(-110, 303, 85, 390);
        glColor3ub(100, 20, 20);
        vprint(-95,370, GLUT_BITMAP_HELVETICA_18, "Fettah Elcik");
        vprint(-95,350, GLUT_BITMAP_HELVETICA_18, "21903182");


}

void ufo() {
    
    for (int q = 1; q <= 10; q++)
    {
        xShape = (Shape + q-1)->sx;
        yShape = (Shape + q-1)->sy;
        
        
        //body
        glBegin(GL_POLYGON);
        glColor3ub(109, 109, 109);
        glVertex2f(xShape -150, yShape -15);
        glVertex2f(xShape - 100, yShape +15);
        glVertex2f(xShape +85, yShape +15);
        glVertex2f(xShape +135, yShape -15);
        glVertex2f(xShape -25, yShape +50);
        glVertex2f(xShape +20, yShape +50);
        glVertex2f(xShape +85, yShape +15);
        glEnd();
        
        //Body shape line
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glColor3ub(255,255,0);
        glVertex2f(xShape -150, yShape -15);
        glVertex2f(xShape - 100, yShape +15);
        glVertex2f(xShape +85, yShape +15);
        glVertex2f(xShape +135, yShape -15);
        glEnd();
        
        //Atenna
        glBegin(GL_POLYGON);
        glColor3ub(0, 0, 0);
        glVertex2f(xShape +30, yShape + 45);
        glVertex2f(xShape +30, yShape +55);
        glVertex2f(xShape -30, yShape +45);
        glVertex2f(xShape -30, yShape +55);
        glEnd();
        
        
        //Window
        glLineWidth(1);
        glBegin(GL_QUADS);
        glColor3ub(140,250,252);
        glVertex2f(xShape -30, yShape + 35);
        glVertex2f(xShape +30, yShape +35);
        glVertex2f(xShape +30, yShape +20);
        glVertex2f(xShape -30, yShape +20);
        glEnd();
        
        //Lights of ufo
        glColor3ub(240, 246, 115);
        glRectf(xShape -50, yShape -148, xShape +50, yShape -15);
        
        
        // Lines of ufo
        glColor3ub(255, 255, 255);
        glRectf(xShape -90, yShape +8, xShape +75, yShape +3);
        glColor3ub(255, 255, 255);
        glRectf(xShape -110, yShape -5, xShape +95, yShape +0);
        
    }
}
    
void howTO() {
    glColor3ub(252, 3, 3);
    vprint(250, -320, GLUT_BITMAP_TIMES_ROMAN_24, "F1: Move-Stop");
    vprint(200,-340, GLUT_BITMAP_HELVETICA_18,  "Click on The Screen to Pop Up Ufo");

}


void display() {
    
    
    // clear window to black
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    background();
    howTO();
    ufo();
   
    glColor3ub(0, 0, 102);
    vprint(380, -380, GLUT_BITMAP_9_BY_15, "(%-4d : %-4d)", mx, my);

    glutSwapBuffers();
}

void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC pressed
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void onSpecialKeyDown(int key, int x, int y)
{
    
    if (key == GLUT_KEY_F1) {
        if (mode == STOP) {
            mode = MOVE;
            Timer = true;
         
        }
        else {
            mode = STOP;
            Timer = false;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onSpecialKeyUp(int key, int x, int y)
{

    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void onClick(int button, int stat, int x, int y)
{
    xShape = x - winWidth / 2;
    yShape = winHeight / 2 - y;

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && sCnt < 10 && Timer == false) {
        (*(Shape + r)).count = sCnt;
        (*(Shape + r)).sx = xShape;
        (*(Shape + r)).sy = yShape;
        
        sCnt++;
        r++;

        if (Shape[sCnt].sx > 590) {
            
            for (int y = 0; y <= 10; y++) {
                Shape[y] = shape_t(); //reset the struct
                sCnt = 0;
                r = 0;
                
            }
           
        }
        
    }

    glutPostRedisplay();
}

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

    glutPostRedisplay();
}

void onMove(int x, int y) {

    mx = x - winWidth / 2;
    my = winHeight / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

  
    if (Timer)
        for (int q = 0; q <= 10; q++) {
            Shape[q].sy += 15;
           
        }
    if (Shape[sCnt].sy > 1000) {
        Timer = false;
    }
   
    glutPostRedisplay(); // display()

}
#endif


void Init() {

    // Smoothing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("AREA 51");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    // keyboard registration
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    // mouse registration
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
