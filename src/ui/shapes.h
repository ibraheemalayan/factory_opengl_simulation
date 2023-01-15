#ifndef __UI_SHAPES_H_
#define __UI_SHAPES_H_

#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include "../include.h"
#include "../std.h"
#include "./structs.h"

void reshape(int, int);
void background();
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, int R, int G, int B);
void draw_rectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height, int R, int G, int B);

void draw_item(ItemObj *item);
void drawMetalDetector();
void draw_text();
void draw_truck(int x, int y);
void draw_printer_animation(GLfloat x, GLfloat y, float rotation);

/*
 * Function that handles the drawing of a circle using the triangle fan
 * method. This will create a filled circle.
 *
 * Params:
 *	x (GLFloat) - the x position of the center point of the circle
 *	y (GLFloat) - the y position of the center point of the circle
 *	radius (GLFloat) - the radius that the painted circle will have
 */
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, int R, int G, int B)
{
    int i;
    int triangleAmount = 30; // # of triangles used to draw circle

    // GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * M_PI;

    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(R, G, B);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}

void draw_truck(int x, int y)
{
    int R = 0, G = 100, B = 0;

    glBegin(GL_QUADS);
    glColor3ub(R, G, B);
    glVertex2f(x, y);
    glVertex2f(x + TRUCK_CONTAINER_WIDTH, y);
    glVertex2f(x + TRUCK_CONTAINER_WIDTH, y + TRUCK_CONTAINER_HEIGHT);
    glVertex2f(x, y + TRUCK_CONTAINER_HEIGHT);
    glEnd();

    R = 0;
    G = 0;
    B = 0;

    glBegin(GL_QUADS);
    glColor3ub(R, G, B);

    int cabin_x = x + TRUCK_CONTAINER_WIDTH;
    int cabin_y = y + TRUCK_CABIN_Y_OFFSET;

    glVertex2f(cabin_x, cabin_y);
    glVertex2f(cabin_x + TRUCK_CABIN_WIDTH, cabin_y);
    glVertex2f(cabin_x + TRUCK_CABIN_WIDTH, cabin_y + TRUCK_CABIN_HEIGHT);
    glVertex2f(cabin_x, cabin_y + TRUCK_CABIN_HEIGHT);
    glEnd();
}

void draw_item(ItemObj *item)
{

    GLubyte *color = NULL;
    switch (item->chocolate_type)
    {
    case TYPE_A:
        color = TYPE_A_COLOR_RGB_V;
        break;

    case TYPE_B:
        color = TYPE_B_COLOR_RGB_V;
        break;

    case TYPE_C:
        color = TYPE_C_COLOR_RGB_V;
        break;
    }

    switch (item->pkg_type)
    {
    case PRODUCT:
        drawFilledCircle(item->current_coords.x, item->current_coords.y, 15, color[0], color[1], color[2]);
        break;

    case PATCH:
        glBegin(GL_TRIANGLES);
        // glColor3ubv(color);
        glColor3ub(color[0], color[1], color[2]);
        glVertex2f(item->current_coords.x + 15, item->current_coords.y - 15);
        glVertex2f(item->current_coords.x - 15, item->current_coords.y - 15);
        glVertex2f(item->current_coords.x, item->current_coords.y + 15);
        glEnd();
        break;

    case CARTON_BOX:
        draw_rectangle(item->current_coords.x, item->current_coords.y, 30, 30, color[0], color[1], color[2]);
        // draw_rectangle(item->current_coords.x, item->current_coords.y, 30, 30, 0, 0, 0);

        break;
    }
}

void draw_rectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height, int R, int G, int B)
{
    glBegin(GL_QUADS);
    glColor3ub(R, G, B);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void draw_walls()
{
    int R = 100, G = 0, B = 100;

    float wall_thickness = ROLLING_CROSS_SIZE / 10;
    float wall_x = ROLLING_CROSSS_X - wall_thickness / 2;

    // printer h-walls
    draw_rectangle(PRINTER_ANIMATION_X_VALUE + 50, 60, -200, wall_thickness, R, G, B);
    draw_rectangle(PRINTER_ANIMATION_X_VALUE + 50, -65, -200, wall_thickness, R, G, B);

    // ****************************
    // Printer Vertical Wall
    // ****************************

    float wall_2_x = METAL_DETECTOR_X - wall_thickness / 2;

    float metal_detector_top = METAL_DETECTOR_SIZE / 2;
    float metal_detector_bottom = -METAL_DETECTOR_SIZE / 2;

    draw_rectangle(wall_2_x, metal_detector_top, wall_thickness, metal_detector_top, R, G, B);
    draw_rectangle(wall_2_x, metal_detector_bottom, wall_thickness, metal_detector_bottom, R, G, B);
}

void draw_printer_animation(GLfloat x, GLfloat y, float rotation)
{
    float height = ROLLING_CROSS_SIZE, width = ROLLING_CROSS_SIZE, thickness = ROLLING_CROSS_SIZE / 10;

    glPushMatrix();

    float x_translate = x;
    float y_translate = y;

    glTranslatef(x_translate, y_translate, 0); // move the Z axis to the center of the gate

    glRotatef(rotation, 0, 0, 1); // set the rotation to be aroung the Z axis

    glTranslatef(-x_translate, -y_translate, 0); // move the Z axis back to the original position

    glBegin(GL_QUADS);
    glColor3ub(0, 20, 0);
    glVertex2f(x - thickness / 2, y - height / 2);
    glVertex2f(x + thickness / 2, y - height / 2);
    glVertex2f(x + thickness / 2, y + height / 2);
    glVertex2f(x - thickness / 2, y + height / 2);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(0, 20, 0);
    glVertex2f(x - height / 2, y - thickness / 2);
    glVertex2f(x + height / 2, y - thickness / 2);
    glVertex2f(x + height / 2, y + thickness / 2);
    glVertex2f(x - height / 2, y + thickness / 2);
    glEnd();

    glPopMatrix();
};

void drawMetalDetector()
{
    int R = 0, G = 20, B = 240;
    float wall_2_x = METAL_DETECTOR_X - (METAL_DETECTOR_SIZE / 10);

    float metal_detector_top = METAL_DETECTOR_SIZE / 2;
    float metal_detector_bottom = -METAL_DETECTOR_SIZE / 2;

    draw_rectangle(wall_2_x, metal_detector_bottom, METAL_DETECTOR_SIZE / 5, metal_detector_top * 2, R, G, B);
}

void reshape(int width, int height)
{

    // regardless the width & the height, do not change the viewport

    glViewport(0, 0, 900, 450);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1250, 750, -500, 500); // left, right, bottom, top

    glMatrixMode(GL_MODELVIEW);
}

void background()
{
    glClearColor(0.95, 0.95, 0.95, 1.0);
}

void draw_text()
{
    char *string1 = "Manufacturing";

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(MANUFACTURING_TITLE_TEXT_X, TITLE_TEXT_Y); // define position on the screen
    for (size_t i = 0; i < 13; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string1[i]);
    }
    glEnd();

    char *string2 = "Patching";

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(PATCHING_TITLE_TEXT_X, TITLE_TEXT_Y); // define position on the screen
    for (size_t i = 0; i < 8; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string2[i]);
    }
    glEnd();

    char *string6 = "Printing";

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(PRINTING_TITLE_TEXT_X, TITLE_TEXT_Y); // define position on the screen
    for (size_t i = 0; i < 8; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string6[i]);
    }
    glEnd();

    char *string3 = "Containers";

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(CONTAINER_TITLE_TEXT_X, TITLE_TEXT_Y); // define position on the screen
    for (size_t i = 0; i < 10; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string3[i]);
    }
    glEnd();

    char *string4 = "Storage";

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(STORAGE_TITLE_TEXT_X, TITLE_TEXT_Y); // define position on the screen
    for (size_t i = 0; i < 7; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string4[i]);
    }
    glEnd();

    char *string5 = "Truck Parking";

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(TRUCK_TITLE_TEXT_X, TITLE_TEXT_Y); // define position on the screen
    for (size_t i = 0; i < 13; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string5[i]);
    }
    glEnd();

    // char *string2 = "Ix";

    // glColor3f(0.0, 0.0, 0.0);
    // glRasterPos2f(TELLER_TEXT_X, TELLER_Ix_Y_VALUE); // define position on the screen
    // for (size_t i = 0; i < 2; i++)
    // {
    //     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string2[i]);
    // }
    // glEnd();

    // char *string3 = "Tx";

    // glColor3f(0.0, 0.0, 0.0);
    // glRasterPos2f(TELLER_TEXT_X, TELLER_Tx_Y_VALUE); // define position on the screen
    // for (size_t i = 0; i < 2; i++)
    // {
    //     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string3[i]);
    // }
    // glEnd();

    // char *string4 = "Rx";

    // glColor3f(0.0, 0.0, 0.0);
    // glRasterPos2f(TELLER_TEXT_X, TELLER_Rx_Y_VALUE); // define position on the screen
    // for (size_t i = 0; i < 2; i++)
    // {
    //     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string4[i]);
    // }
    // glEnd();
}

#endif
