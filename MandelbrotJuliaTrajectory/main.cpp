//
//  main.cpp
//  MandelbrotJuliaTrajectory
//
//  Copyright © 2020 Klaus Kientzle. All rights reserved.
//  Copyright © 2020 Luis Sanchez. All rights reserved.
//

// NOTES
// https://www.youtube.com/watch?v=FFftmWSzgmk

#include <iostream>
#include <GLUT/glut.h>
#include <complex>
#include <vector>
#include <cmath>

int ww = 500, wh = 500;
int xOrigin = -1;

std::complex<double> complex(0.0, 0.0);
std::complex<double> zn(0.0, 0.0);

std::vector<std::complex<double>> points;

void renderBitmapString(float x, float y, void *font, std::string str);

void drawPoint(double x, double y, int flag)
{
    if (flag == 0)
    {
        renderBitmapString(x + 0.03, y + 0.03, GLUT_BITMAP_HELVETICA_18, "Zo");
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
        renderBitmapString(x + 0.03, y + 0.03, GLUT_BITMAP_HELVETICA_18, "C");
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

void renderBitmapString(float x, float y, void *font, std::string str)
{
    glRasterPos2f(x, y);
    glColor3i(1, 1, 1);
    for (int i = 0; i < str.size(); i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

void drawGrid()
{
    drawLine(-1, 0, 1, 0);
    drawLine(0, 1, 0, -1);

    drawLine(0.5, 0.02, 0.5, -0.02);
    drawLine(-0.5, 0.02, -0.5, -0.02);

    drawLine(-0.02, 0.5, 0.02, 0.5);
    drawLine(-0.02, -0.5, 0.02, -0.5);

    renderBitmapString(0.25, -0.8, GLUT_BITMAP_HELVETICA_12, "(R) Zo = [" + std::to_string(zn.real()) + " : " + std::to_string(zn.imag()) + "]");
    renderBitmapString(0.25, -0.9, GLUT_BITMAP_HELVETICA_12, "(L) C   = [" + std::to_string(complex.real()) + " : " + std::to_string(complex.imag()) + "]");

    glFlush();
}

void trajectoryCalculation()
{
    std::complex<double> zSqr(0.0, 0.0);
    points.clear();

    points.push_back(zn);

    std::complex<double> tempZn = zn;
    int i = 50;
    while (i != 0)
    {
        --i;
        zSqr = pow(tempZn, 2);
        tempZn = zSqr + complex;
        points.push_back(tempZn);
    }
}

void trajectoryDisplay()
{
    trajectoryCalculation();

    if (points.size() == 0)
    {
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
    if (btn == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_UP)
        {
            xOrigin = -1;
        }
        else
        {
            xOrigin = 2;
        }
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

    if (xOrigin == 2)
    {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double nx = (x - 250) * 2 / 500.0;
        double ny = (250 - y) * 2 / 500.0;

        zn.real(nx);
        zn.imag(ny);
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
    glutCreateWindow("Fractal Trajectories - Kientzle & Sanchez");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);

    glutMainLoop();
    return 0;
}
