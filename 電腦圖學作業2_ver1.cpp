#include <GL/freeglut.h>

float points[][3] = { {0.0, 0.0, 0.0}, {0.5, 0.0, 0.0}, {0.5, 0.0, 0.5},
	{0.0, 0.0, 0.5}, {0.0, 0.5, 0.0}, {0.5, 0.5, 0.0},{0.5, 0.5, 0.5}, {0.0, 0.5, 0.0} };
int face[][4] = { {0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1},
					{1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4} };
int    cube[6] = { 0, 1, 2, 3, 4, 5 };
float  color[][6] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.66, 0.66, 0.66}, {0.33, 0.33, 0.33} };

float a = 0.0;

void draw_cube(void)
{
	int    i;

	for (i = 0;i < 6;i++) {     /* draw the six faces one by one */
		glColor3fv(color[i % 6]);  /* Set the current color */
		glBegin(GL_POLYGON);  /* Draw the face */
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3fv(points[face[i][0]]);
		//glColor3f(0.0, 1.0, 0.0);
		glVertex3fv(points[face[i][1]]);
		//glColor3f(0.0, 0.0, 1.0);
		glVertex3fv(points[face[i][2]]);
		//glColor3f(1.0, 1.0, 1.0);
		glVertex3fv(points[face[i][3]]);
		glEnd();
	} //end-for
} //end-proc


void display()
{
	/*Clear previous frame and the depth buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //current mtx = modelview
	glLoadIdentity(); //initialize model view mtx.

  /*-------Draw the cube----*/
	glTranslatef(0.0, 0.0, 0.0);
	//glScalef(2, 2, 2);
	//glRotatef(a, 1.0, 0.0, 0.0);
	draw_cube(); //no geometrical transformation.
	glPushMatrix();
	glTranslatef(-1.0, -1.0, 0.0);
	glScalef(2, 2, 2);
	draw_cube();
	glPopMatrix();
	glScalef(2, 2, 2);
	draw_cube();
	glutSwapBuffers(); //swap buffer to display drawing results.
}

void click(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;
	a = a + 1;
	display();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutCreateWindow("test");
	glutDisplayFunc(display);
	//glutMouseFunc(click);
	glutMainLoop();
}
