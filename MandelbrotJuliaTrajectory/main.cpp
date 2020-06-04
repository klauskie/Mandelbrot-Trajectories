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

int ww=500,wh=500;
int xi,yi,xf,yf;

std::complex<double> complex(-0.3, 0.45);
std::complex<double> zn(-0.7, 0.45);

std::vector<std::complex<double>> points;


void drawLine(double x1,double y1,double x2,double y2)
{
    //std::cout << x1 << " | " << y1 << " | " << x2 << " | " << y2 << std::endl;
    glLineWidth(2.0);
    glColor3f(1,1,1);
    glBegin(GL_LINES);
        glVertex2d(x1,y1);
        glVertex2d(x2,y2);
    glEnd();
    glFlush();
}

void display()
{
    glColor3f(1, 1, 1);
    glFlush();
}

void renderBitmapString(float x,float y,void *font,char *string) {
    char *c;
    glRasterPos2f(x, y);
    glColor3i(1,1,1);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void trajectoryCalculation()
{
    std::complex<double> zSqr(0.0, 0.0);
    points.clear();
    
    points.push_back(zn);
    
    int i = 50;
    while (i != 0) {
        --i;
        zSqr = pow(zn, 2);
        zn = zSqr + complex;
        points.push_back(zn);
    }
    
}

void trajectoryDisplay()
{
    trajectoryCalculation();
    
    if (points.size() == 0) {
        return;
    }
    
    for (int i = 0; i < points.size() - 1; i++) {
        double pointAx = points[i].real();
        double pointAy = points[i].imag();
        
        double pointBx = points[i+1].real();
        double pointBy = points[i+1].imag();
        
        drawLine(pointAx, pointAy, pointBx, pointBy);
    }
}

void mouse(int btn,int state,int x,int y)
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw line from origin to point
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        double nx = (x - 250) * 2 / 500.0;
        double ny = (250 - y) * 2 / 500.0;
        
        std::cout << x << " | " << y << " -> " << nx << " | " << ny << std::endl;
        
        std::complex<double> newZn(nx, ny);
        zn = newZn;
        trajectoryDisplay();
    }
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ww,wh);
    glutCreateWindow("Fractal Trajectories");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
