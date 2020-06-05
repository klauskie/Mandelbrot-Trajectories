//
//  main.cpp
//  MandelbrotJuliaTrajectory
//
//  Created by Klaus Kientzle on 6/1/20.
//  Copyright © 2020 Klaus Kientzle. All rights reserved.
//

// NOTES
// https://stackoverflow.com/questions/20288861/mouse-drag-object-in-opengl-glut

#include <iostream>
#include <GLUT/glut.h>
#include <complex>
#include <vector>
#include <cmath>

int ww = 500, wh = 500;
int xOrigin = -1;

std::complex<double> complex(0.0, 0.0);
std::complex<double> zn(-0.7, 0.45);

std::vector<std::complex<double>> points;

void drawPoint(double x, double y, int flag)
{
    if (flag == 0)
    {
        glColor3f(1, 0, 0);
        glBegin(GL_QUADS);
        glVertex2d(x - 0.02, y - 0.02);
        glVertex2d(x - 0.02, y + 0.02);
        glVertex2d(x + 0.02, y + 0.02);
        glVertex2d(x + 0.02, y - 0.02);
        glEnd();
    }
    else if (flag == -1)
    {
        glColor3f(0, 1, 0.5);
        glBegin(GL_QUADS);
        glVertex2d(x - 0.02, y - 0.02);
        glVertex2d(x - 0.02, y + 0.02);
        glVertex2d(x + 0.02, y + 0.02);
        glVertex2d(x + 0.02, y - 0.02);
        glEnd();
    }
    else
    {
        glColor3f(0, 0, 1);
        glBegin(GL_QUADS);
        glVertex2d(x - 0.01, y - 0.01);
        glVertex2d(x - 0.01, y + 0.01);
        glVertex2d(x + 0.01, y + 0.01);
        glVertex2d(x + 0.01, y - 0.01);
        glEnd();
    }
}

void drawLine(double x1, double y1, double x2, double y2)
{
    //std::cout << x1 << " | " << y1 << " | " << x2 << " | " << y2 << std::endl;

    glLineWidth(2.5);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
    glFlush();
}

void display()
{
    glColor3f(1, 1, 1);
    glFlush();
}

void renderBitmapString(float x, float y, void *font, char *string)
{
    char *c;
    glRasterPos2f(x, y);
    glColor3i(1, 1, 1);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void drawGrid()
{
    drawLine(-1, 0, 1, 0);
    drawLine(0, 1, 0, -1);
    glFlush();
}

void trajectoryCalculation()
{
    std::cout << "ZN OG: " << zn.real() << " | " << zn.imag() << std::endl;
    std::complex<double> zSqr(0.0, 0.0);
    points.clear();
    
    //zn.real(0.0);
    //zn.imag(0.0);
    
    std::cout << "LIST SIZE: " << points.size() << std::endl;

    points.push_back(zn);
    std::cout << "LIST [0]: " << points[0].real() << " | " << points[0].imag() << std::endl;

    std::complex<double> tempZn = zn;
    int i = 8;
    while (i != 0)
    {
        --i;
        zSqr = pow(tempZn, 2);
        tempZn = zSqr + complex;
        std::cout << "Zn = " << zSqr.real() << " | " << zSqr.imag() << "        Complex =  " << complex.real() << " | " << complex.imag() << std::endl;
        points.push_back(tempZn);
    }
}

void trajectoryDisplay()
{
    trajectoryCalculation();

    if (points.size() == 0)
    {
        std::cout << "IS EMPTY" << std::endl;
        return;
    }

    drawPoint(complex.real(), complex.imag(), -1);

    for (int i = 0; i < points.size() - 1; i++)
    {
        double pointAx = points[i].real();
        double pointAy = points[i].imag();

        double pointBx = points[i + 1].real();
        double pointBy = points[i + 1].imag();

        drawLine(pointAx, pointAy, pointBx, pointBy);
        drawPoint(pointAx, pointAy, i);
    }
}

void mouse(int btn, int state, int x, int y)
{
    // Trigger A movement
    if (btn == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_UP)
        {
            xOrigin = -1;
        }
        else
        {
            xOrigin = 1;
        }
    }

    // Trigger C movement
    if (btn == GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        xOrigin = -1;
        double nx = (x - 250) * 2 / 500.0;
        double ny = (250 - y) * 2 / 500.0;
        
        //std::cout << points[0].real() << " : " << zn.real() << std::endl;
        
        zn.real(nx);
        zn.imag(ny);
        //trajectoryDisplay();
        // Clear screen
        
        drawPoint(complex.real(), complex.imag(), true);
    }
}

void mouseMove(int x, int y)
{

    if (xOrigin == 1)
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double nx = (x - 250) * 2 / 500.0;
        double ny = (250 - y) * 2 / 500.0;

        complex.real(nx);
        complex.imag(ny);
        trajectoryDisplay();
    }
    
    drawGrid();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ww, wh);
    glutCreateWindow("Fractal Trajectories");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);

    glutMainLoop();
    return 0;
}
