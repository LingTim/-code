#include<GL/freeglut.h>
#include<math.h>
#include<iostream>
#include<string>
using namespace std;

#define WHITE 1
#define RED 2
#define GREEN 3
#define BLUE 4

#define POINT 1
#define LINE 2
#define POLYGON 3
#define CIRCLE  4
#define CURVE   5

#define    MY_QUIT -1
#define    MY_CLEAR -2
#define    MY_SAVE  -3
#define    MY_BLEND -4
#define    MY_LOAD  -5

#define ON 1
#define OFF 2

#define    SIZEX   800
#define    SIZEY   800

int width = 512;
int height = 512;
int first = 0;
int pos_x = -1, pos_y = -1;
int obj_type = -1;
int side = 0;
int vertex[128][2];
int input_row = 0;
float pnt_size = 5;
float color[3] = { 0.0, 0.0, 0.0 };
unsigned char  image[SIZEX * SIZEY][4];
bool IdleActive = false;
bool IdleDisplay = false;
bool InputString = false;
string inputText[10] = {"", "", "",  "",  "",  "",  "",  "",  "",  ""};

typedef int menu_t;
menu_t top_m, color_m, type_m, size_menu, file_m, idle_m, idle_display_m, input_m;

void display()
{
	if (InputString)
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0;i <= input_row;i++)
		{
			glRasterPos2f(0, 500 - (i * 30));

			for (char c : inputText[i]) {
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
			}
		}
	}
	
	if (IdleDisplay)
	{
		glRasterPos2i(0, 0);
		glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
		cout << "Idle load complete" << endl;
	}

	glFlush();
}

void reshape(int new_w, int new_h)
{
	height = new_h;
	width = new_w;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)width, 0.0, (double)height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
	cout << "reshape" << endl;
}

void init_window(void)
{
	/*Do nothing else but clear window to black*/

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)width, 0.0, (double)height);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	cout << "init_window" << endl;
}

void draw_polygon()
{
	int  i;
	glBegin(GL_POLYGON);
	for (i = 0;i < side;i++)
		glVertex2f(vertex[i][0], height - vertex[i][1]);
	glEnd();
	glFinish();
	side = 0;    /* set side=0 for next polygon */
	cout << "draw_polygon" << endl;
}

void draw_circle()
{
	static GLUquadricObj* mycircle = NULL;

	if (mycircle == NULL) {
		mycircle = gluNewQuadric();
		gluQuadricDrawStyle(mycircle, GLU_FILL);
	}
	glPushMatrix();
	glTranslatef(pos_x, height - pos_y, 0.0);
	gluDisk(mycircle,
		0.0,           
		50.0,          
		16,            
		10);
	glPopMatrix();
	cout << "draw_circle" << endl;
}

void keyboard(unsigned char key, int x, int y)
{
	if(InputString)
	{
		if (key == 13)//enter
		{
			if(input_row < 9)
				input_row++;
			cout << "enter" << endl;
		}
		else if (key == 8)//backspace
		{
			if (!inputText[input_row].empty())// array is not empty
			{
				inputText[input_row].pop_back();
				cout << "backspace" << endl;
			}
			else
			{
				if (input_row > 0)
				{
					input_row--;
					cout << "return previous row" << endl;
				}
			}
		}
		else
		{
			inputText[input_row] += key;
			cout << key << endl;
		}

		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	switch (obj_type) {
	case POINT:
		glPointSize(pnt_size);     /*  Define point size */
		glBegin(GL_POINTS);     /*  Draw a point */
		glVertex2f(x, height - y);
		glEnd();
		break;
	case LINE:
		if (first == 0) {
			first = 1;
			pos_x = x; pos_y = y;
			glPointSize(pnt_size);
			glBegin(GL_POINTS);   /*  Draw the 1st point */
			glVertex3f(x, height - y, 0);
			glEnd();
		}
		else {
			first = 0;
			glLineWidth(pnt_size);     /* Define line width */
			glBegin(GL_LINES);    /* Draw the line */
			glVertex2f(pos_x, height - pos_y);
			glVertex2f(x, height - y);
			glEnd();
		}
		break;
	case POLYGON:  /* Define vertices of poly */
		if (side == 0) {
			vertex[side][0] = x; vertex[side][1] = y;
			side++;
		}
		else {
			if (fabs(vertex[side - 1][0] - x) + fabs(vertex[side - 1][1] - y) < 2)
				draw_polygon();
			else {
				glBegin(GL_LINES);
				glVertex2f(vertex[side - 1][0], height - vertex[side - 1][1]);
				glVertex2f(x, height - y);
				glEnd();
				vertex[side][0] = x;
				vertex[side][1] = y;
				side++;
			}
		}
		break;
	case CIRCLE:
		pos_x = x; pos_y = y;
		draw_circle();
		break;
	default:
		break;
	}
	glFinish();
}

void motion(int  x, int y)
{
	if (obj_type != CURVE) return;
	if (first == 0) {
		first = 1;
		pos_x = x; pos_y = y;
	}
	else {
		glBegin(GL_LINES);
		glVertex3f(pos_x, height - pos_y, 0.0);
		glVertex3f(x, height - y, 0.0);
		glEnd();
		pos_x = x; pos_y = y;
	}
	glFinish();
}

void top_menu_func(int value)
{
	return;
}

void  color_func(int value)
{
	switch (value) {
	case WHITE:
		color[0] = color[1] = color[2] = 1.0;
		break;

	case RED:
		color[0] = 1.0;
		color[1] = color[2] = 0.0;
		break;

	case GREEN:
		color[0] = color[2] = 0.0;
		color[1] = 1.0;
		break;

	case BLUE:
		color[0] = color[1] = 0.0;
		color[2] = 1.0;
		break;

	default:
		break;
	}
	glColor3f(color[0], color[1], color[2]);
}

void draw_type(int value)
{
	obj_type = value;
	if (value == LINE || value == CURVE)
		first = 0;
	else if (value == POLYGON) side = 0;
}

void size_func(int value)
{
	if (value == 1) {
		if (pnt_size < 10.0) pnt_size += 1.0;
	}
	else {
		if (pnt_size > 1.0) pnt_size = pnt_size - 1.0;
	}
}

void file_func(int value)
{
	int i, j;

	if (value == MY_QUIT) exit(0);
	else if (value == MY_CLEAR)
	{
		init_window();
		cout << "clear" << endl;
	}
	else if (value == MY_SAVE) { /* Save current window */
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
		for (i = 0;i < width;i++)   /* Assign 0 opacity to black pixels */
			for (j = 0;j < height;j++)
				if (image[i * width + j][0] == 0 && image[i * width + j][1] == 0 && image[i * width + j][2] == 0)
					image[i * width + j][3] = 0;
				else image[i * width + j][3] = 127;
		cout << "save complete" << endl;
	}
	else if (value == MY_LOAD) { /* Restore the saved image */
		glRasterPos2i(0, 0);
		glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
		cout << "load complete" << endl;
	}
	else if (value == MY_BLEND) { /* Blending current image with the saved image */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glRasterPos2i(0, 0);
		glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glDisable(GL_BLEND);
	}

	glFlush();
}

void Idle()
{
	if (IdleActive)
	{
		int i, j;
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
			image);
		for (i = 0;i < width;i++)   /* Assign 0 opacity to black pixels */
			for (j = 0;j < height;j++)
				if (image[i * width + j][0] == 0 && image[i * width + j][1] == 0 && image[i * width + j][2] == 0)
					image[i * width + j][3] = 0;
				else image[i * width + j][3] = 127;
		cout << "Idle save!" << endl;
		IdleActive = false;
		cout << "idle active off" << endl;
	}
}

void change_idle_active(int value)
{
	IdleActive = true;
	cout << "idle active on" << endl;
}

void idle_display_active(int value)
{
	if (value == 1)
	{
		IdleDisplay = true;
		cout << "idle display on" << endl;
	}
	else
	{
		IdleDisplay = false;
		cout << "idle display off" << endl;
	}


}

void input_string_active(int value)
{
	if (value == 1)
	{
		InputString = true;
		cout << "Input String on" << endl;
	}
	else
	{
		InputString = false;
		cout << "Input String off" << endl;
	}
}

void init_func()
{
	glReadBuffer(GL_FRONT);
	glDrawBuffer(GL_FRONT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	cout << "init_func" << endl;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	init_func();

	glutCreateWindow("Menu");
	glClear(GL_COLOR_BUFFER_BIT);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(Idle);

	color_m = glutCreateMenu(color_func); /* Create color-menu */
	glutAddMenuEntry("white", WHITE);
	glutAddMenuEntry("red", RED);
	glutAddMenuEntry("green", GREEN);
	glutAddMenuEntry("blue", BLUE);

	type_m = glutCreateMenu(draw_type);   /* Create draw-type menu */
	glutAddMenuEntry("Point", POINT);
	glutAddMenuEntry("Line", LINE);
	glutAddMenuEntry("Poly", POLYGON);
	glutAddMenuEntry("Curve", CURVE);
	glutAddMenuEntry("Circle", CIRCLE);

	size_menu = glutCreateMenu(size_func);
	glutAddMenuEntry("Bigger", 1);
	glutAddMenuEntry("Smaller", 2);

	file_m = glutCreateMenu(file_func);
	glutAddMenuEntry("save", MY_SAVE);
	glutAddMenuEntry("load", MY_LOAD);
	glutAddMenuEntry("blend", MY_BLEND);
	glutAddMenuEntry("clear", MY_CLEAR);
	glutAddMenuEntry("quit", MY_QUIT);

	idle_m = glutCreateMenu(change_idle_active);
	glutAddMenuEntry("on", ON);

	idle_display_m = glutCreateMenu(idle_display_active);
	glutAddMenuEntry("on", ON);
	glutAddMenuEntry("off", OFF);

	input_m = glutCreateMenu(input_string_active);
	glutAddMenuEntry("on", ON);
	glutAddMenuEntry("off", OFF);

	top_m = glutCreateMenu(top_menu_func);
	glutAddSubMenu("colors", color_m);
	glutAddSubMenu("type", type_m);
	glutAddSubMenu("Size", size_menu);
	glutAddSubMenu("file", file_m);
	glutAddSubMenu("IdleAutoSave", idle_m);
	glutAddSubMenu("IdleSaveDisplay", idle_display_m);
	glutAddSubMenu("InputString", input_m);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	if (glutLayerGet(GLUT_OVERLAY_POSSIBLE)) {
		fprintf(stderr, "Overlay is available\n");
	}
	else {
		fprintf(stderr, "Overlay is NOT available, May encounter problems for menu\n");
	}

	glutMainLoop();
}
