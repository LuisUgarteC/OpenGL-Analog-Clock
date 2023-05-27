#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>

// Enum to represent colors
typedef enum { BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE, GREY, DARK_GREY, LIGHT_GREY } COLOR;

// Color palette array
float COLOR_PALETTE[11][3] = {
    0, 0, 0,         // BLACK
    0, 0, 1,         // BLUE
    0, 1, 0,         // GREEN
    0, 1, 1,         // CYAN
    1, 0, 0,         // RED
    1, 0, 1,         // MAGENTA
    1, 1, 0,         // YELLOW
    1, 1, 1,         // WHITE
    0.5, 0.5, 0.5,   // GREY
    0.25, 0.25, 0.25,// DARK_GREY
    0.8, 0.8, 0.8    // LIGHT_GREY
};

// Structure to represent a point
typedef struct {
    float x;
    float y;
} PUNTO;

// Structure to represent a line
typedef struct {
    PUNTO ini;
    PUNTO fin;
    COLOR color;
} LINEA;

// Structure to represent a circle
typedef struct {
    PUNTO c;
    float r;
} CIRCULO;

// Function to draw text on screen
void DibujarTexto(char* Texto, float x, float y);

// Function to assign a color based on the enum value
void AsignaColor(COLOR c);

// Display function called by OpenGL for drawing
void display(void);

// Function to draw a line
void Linea(LINEA L);

// Function to draw a circle
void Circulo(CIRCULO ci);

// Function to rotate a line around a point
void RotacionLinea(LINEA* L, float xr, float yr, float Theta);

// Function to scale a line
void EscalamientoLinea(LINEA* L, float xr, float yr, float sx, float sy);

// Function to convert window coordinates to OpenGL coordinates
PUNTO CVent_a_Cplano(int x, int y, int AnchoV, int AltoV, float xini, float xfin, float yini, float yfin);

const float pi = 3.14159;

// Animation function for clock movement
void Animar(void);

// Global variables for the clock's second and minute hands
LINEA segundero;
LINEA minutero;

int main(int argc, char** argv) {
    // Initializing the clock's second and minute hands
    segundero.ini.x = 0;
    segundero.ini.x = 0;
    segundero.ini.y = 0;
    segundero.fin.x = 0;
    segundero.fin.y = 10;
    segundero.color = RED;

    minutero.ini.x = 0;
    minutero.ini.x = 0;
    minutero.ini.y = 0;
    minutero.fin.x = 0;
    minutero.fin.y = 10;
    minutero.color = YELLOW;

    // Initializing GLUT and creating the window
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Simple Analog Clock");

    // Setting up the coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -10.0, 10.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);

    // Registering the display and animation functions
    glutDisplayFunc(display);
    glutIdleFunc(Animar);

    // Starting the main event loop
    glutMainLoop();

    return 0;
}

// Display function called by OpenGL for drawing
void display(void) {
    // Variables for clock elements
    CIRCULO c1;
    LINEA l1;
    int k;
    float ang = 30, x, y, theta;
    char Hora[5];

    // Clearing the background
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing the clock face
    AsignaColor(GREEN);
    c1.c.x = 0;
    c1.c.y = 0;
    c1.r = 10;
    Circulo(c1);

    l1.color = GREEN;
    l1.ini.x = 8;
    l1.ini.y = 0;
    l1.fin.x = 10;
    l1.fin.y = 0;

    // Drawing the hour segments of the clock face
    for (k = 0; k < 12; k++) {
        RotacionLinea(&l1, 0, 0, ang * (pi / 180));
        Linea(l1);
    }

    // Drawing the hour numbers on the clock face
    k = 1;
    for (theta = 1.047197; theta > -5.22; theta -= 0.523598) {
        x = 0 + 7.5 * cos(theta);
        y = 0 + 7.5 * sin(theta);
        sprintf(Hora, "%i", k);
        DibujarTexto(Hora, x, y);
        k++;
    }

    // Drawing the clock hands
    Linea(segundero);
    Linea(minutero);

    glFlush();
}

int elapsed_time = 0;

// Animation function for clock movement
void Animar(void) {
    Sleep(1000); // Wait for 1 second

    // Rotate the second hand
    RotacionLinea(&segundero, 0, 0, -0.10471);

    elapsed_time++;

    // Rotate the minute hand every 60 seconds
    if (elapsed_time % 60 == 0) {
        RotacionLinea(&minutero, 0, 0, -0.10471);
    }

    glutPostRedisplay(); // Request a redraw of the display
}

// Function to scale a line
void EscalamientoLinea(LINEA* L, float xr, float yr, float sx, float sy) {
    L->ini.x = L->ini.x * sx + xr * (1 - sx);
    L->ini.y = L->ini.y * sy + yr * (1 - sy);
    L->fin.x = L->fin.x * sx + xr * (1 - sx);
    L->fin.y = L->fin.y * sy + yr * (1 - sy);
}

// Function to rotate a line around a point
void RotacionLinea(LINEA* L, float xr, float yr, float Theta) {
    float xp, yp;
    xp = xr + (L->ini.x - xr) * cos(Theta) - (L->ini.y - yr) * sin(Theta);
    yp = yr + (L->ini.x - xr) * sin(Theta) + (L->ini.y - yr) * cos(Theta);
    L->ini.x = xp;
    L->ini.y = yp;
    xp = xr + (L->fin.x - xr) * cos(Theta) - (L->fin.y - yr) * sin(Theta);
    yp = yr + (L->fin.x - xr) * sin(Theta) + (L->fin.y - yr) * cos(Theta);
    L->fin.x = xp;
    L->fin.y = yp;
}

// Function to translate a line
void TraslacionLinea(LINEA* L, float tx, float ty) {
    L->ini.x += tx;
    L->ini.y += ty;
    L->fin.x += tx;
    L->fin.y += ty;
}

// Function to draw text on the screen
void DibujarTexto(char* Texto, float x, float y) {
    int k = 0;
    glRasterPos2f(x, y);
    while (Texto[k] != '\0') {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Texto[k]);
        k++;
    }
}

// Function to convert screen coordinates to normalized coordinates
PUNTO CVent_a_Cplano(int x, int y, int AnchoV, int AltoV, float xini, float xfin, float yini, float yfin) {
    PUNTO pos;
    float RangoX = xfin - xini;
    float RangoY = yfin - yini;
    float porcentaje;
    porcentaje = ((float)x / AnchoV);
    pos.x = (porcentaje * (RangoX)) - (RangoX) / 2;
    porcentaje = ((float)y / AltoV);
    pos.y = ((1 - porcentaje) * (RangoY)) - (RangoX) / 2;
    return pos;
}

// Function to set the current color
void AsignaColor(COLOR c) {
    glColor3f(COLOR_PALETTE[c][0], COLOR_PALETTE[c][1], COLOR_PALETTE[c][2]);
}

// Function to draw a circle
void Circulo(CIRCULO ci) {
    float x, y, theta;
    glBegin(GL_LINE_LOOP);
    for (theta = 0; theta < 6.28; theta += 0.1) {
        x = ci.c.x + ci.r * cos(theta);
        y = ci.c.y + ci.r * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

// Function to draw a line
void Linea(LINEA L) {
    AsignaColor(L.color);
    glBegin(GL_LINES);
    glVertex2f(L.ini.x, L.ini.y);
    glVertex2f(L.fin.x, L.fin.y);
    glEnd();
}
