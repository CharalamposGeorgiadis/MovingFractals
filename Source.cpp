#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>

int points = 10000; // Points of the default fractal
int vert = 5; // Vertices of the default fractal
bool flag = false; // Controls color for every point
float cameraX, cameraY; // x and y coordinates of the camera
float lastMouseX, lastMouseY; // Last x and y coordinates of the mouse

// Randomzing the color of glColor3f()
void randomColor(void)
{
    // Appying random color every time we switch 8000 to 10000 points and vice versa
    glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
}

// GLUT callback for window resizing
void reshape(int width, int height)
{
    // Updating viewport
    glViewport(0, 0, width, height);

    // Setting projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 600.0, 0.0, 600.0);
    glMatrixMode(GL_MODELVIEW);

}

// GLUT callback for window drawing
void display(void)
{
    // Clear Window
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glLoadIdentity();
    glTranslatef(-cameraX, -cameraY, 0.9f);
    typedef GLfloat point2[2];
    // Coordinates of hexagon
    point2 verticesH[6] = {{600.0, 463.0},{500.0, 463.0},{450.0, 550.0},{500.0, 637.0},{600.0, 637.0},{650.0, 550.0}};
    // Coordinates of pentagon
    point2 verticesP[5] = {{609.0, 631.0},{645.0, 519.0},{550.0, 450.0},{455.0, 519.0},{491.0, 631.0}};
    point2 p = { 75.0, 50.0 };
    glClear(GL_COLOR_BUFFER_BIT);
    for (int k = 0; k < points; k++)
    {
        int j = rand() % vert;
        if (vert == 6) // Creates Hexagon
        {
            p[0] = (p[0] + verticesH[j][0]) / 3;
            p[1] = (p[1] + verticesH[j][1]) / 3;
        }
        if (vert == 5)  //Creates Pentagon
        {
            p[0] = (p[0] + verticesP[j][0]) / 3;
            p[1] = (p[1] + verticesP[j][1]) / 3;
        }
        glBegin(GL_POINTS);
            if (flag) // Creates unique color for every point(case 2)
                randomColor();
            glVertex2fv(p);
        glEnd();
    }
    glutSwapBuffers();
}

// Using the mouse's current and past coordinates to drag the fractal accordingly
void drag(int x, int y)   
{
    float dx = x - lastMouseX;
    float dy = y - lastMouseY;
    const float speed = 0.8;
    cameraX -= dx * speed;
    cameraY += dy * speed;
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

// GLUT callback for mouse movement
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glutMotionFunc(drag);
        lastMouseX = x;
        lastMouseY = y;
    }
    else
        glutMotionFunc(NULL);
}

// GLUT callback for menu options
void MenuOptions(int selection)
{
    switch (selection)
    {
    case 1: // 8000 points
        glutPostRedisplay();
        points = 8000;
        flag = 0;
        randomColor();
        glutDisplayFunc(display);
        break;
    case 2: // 10000 points, random color
        glutPostRedisplay();
        points = 10000;
        flag = 1;
        break;
    case 3: // Pentagon
        glutPostRedisplay();
        vert = 5;
        flag = 0;
        glutDisplayFunc(display);
        break;
    case 4: // Hexagon
        glutPostRedisplay();
        vert = 6;
        flag = 0;
        glutDisplayFunc(display);
        break;
    case 5: // Shut Down
        exit(0);
    }
}

// Creating a menu
void menu()
{
    int menu;
    menu = glutCreateMenu(MenuOptions);
    glutAddMenuEntry("8000 points", 1);
    glutAddMenuEntry("10000 points, random color", 2);
    glutAddMenuEntry("Pentagon", 3);
    glutAddMenuEntry("Hexagon", 4);
    glutAddMenuEntry("Shut Down", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Main
int main(int argc, char** argv)
{
    // Setting a different random nu
    srand(GetTickCount64());
    // Creating the main window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 800) / 2,
        (glutGet(GLUT_SCREEN_HEIGHT) - 800) / 2);
    glutCreateWindow("Moving Fractals");

    // Setting callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);

    // Randomizing the color of the first fractal
    randomColor();

    // Creating the menu
    menu();
    
    // Starting the event processing loop
    glutMainLoop();
}
