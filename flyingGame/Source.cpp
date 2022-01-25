#include<windows.h>
#include<stdlib.h>
#include<gl/glut.h>
#include<time.h>
#include<stdio.h>
#include<math.h>

#define BLOCKSPEED 0.009
#define BOOSTER_MAX 50

int SCREENH = 600, SCREENW = 800;
//---------------- Obstacles declaration----------
typedef struct building
{
	float block_x, block_y;
	bool state;
	int no_floors;
}building;

typedef struct Cloud
{
	float block_x, block_y;
	bool state;
}Cloud;
//-------------------declarations---------------
float bspd = BLOCKSPEED;  // block speed
bool pause = false, lflag = true, wflag = true, gameEndStatus = false, instflag = false, abtflag = false, start = false;  //flags
float plane_mvmt = 0.0;//jet movement up or down
float score = 1;
char score_Str[20], slevel[20];   //score string and levelstring
int level = 1, buildColor;     // initial level=1
building b;  // building struct
Cloud s;     // cloud struct
float booster = BOOSTER_MAX, boost = 0;
//plane bounds
///-------------function prototypes------------------
void keyPressed(unsigned char, int, int);
void mouse(int button, int state, int x, int y);
void printString(float x, float y, float z, void* font, char* string);//what does this do??
void buildingBlock();
void CloudBlock();
void init();
void drawJet();
void gameEnd();
void drawBg();
void welcome();
void drawBuilding();
void drawCloud();
bool cloudHit();
bool buildingHit();
void printScore();
void display();
void moveJetU();
void moveJetD();

void buildingBlock()
{
	b.block_x = 50.0;
	srand(time(0));
	b.no_floors = rand() % 3 + 4;
	buildColor = rand() % 3;
	b.block_y = b.no_floors * 10 + 15;   // generate block y cordinate depending on no of floors
	b.state = true;
	s.state = false;
}
void CloudBlock()
{
	s.block_x = 50.0;
	srand(time(0));
	s.block_y = (rand() % 30) + 50;   //randomly generate block y cordinate
	s.state = true;
	b.state = false;
}

void semiCircle(float p1, float q1, float radius)
{
	float p, q;
	float angle;
	glBegin(GL_POINTS);

	for (angle = 1.0f; angle < 360.0f; angle++)
	{
		p = p1 + sin(angle) * radius;
		q = q1 + cos(angle) * radius;
		if (q >= 100)
			glVertex2f(p, q);
	}
	glEnd();

}


void Circle(float x1, float y1, float radius)
{
	float x2, y2;
	float angle;
	glBegin(GL_POINTS);

	for (angle = 1.0f; angle < 360.0f; angle++)
	{
		x2 = x1 + sin(angle) * radius;
		y2 = y1 + cos(angle) * radius;
		glVertex2f(x2, y2);
	}
	glEnd();

}

void drawJet()
{
	//left tail wing

	glColor3f(1.0, 1.0, 0.6);
	glBegin(GL_POLYGON);
	glVertex2f(5.5, 47.0);
	glVertex2f(8.5, 47.0);
	glVertex2f(5.5, 48.0);
	glVertex2f(4.5, 48.0);
	glEnd();


	//left front wing

	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_POLYGON);
	glVertex2f(13.0, 47.0);
	glVertex2f(20.0, 47.0);
	glVertex2f(13.0, 50.0);
	glVertex2f(11.0, 50.0);
	glEnd();

	//tail
	glColor3f(1.0, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(4.7, 45.0);
	glVertex2f(5.5, 51.0);
	glVertex2f(7.0, 51.0);
	glVertex2f(9.0, 45.0);
	glEnd();


	//body
	glColor3f(0.5, 1.0, 0.5);
	glBegin(GL_POLYGON);
	glVertex2f(5.0, 48.0);
	glVertex2f(11.0, 48.0);
	glVertex2f(22.0, 46.5);
	glVertex2f(22.0, 45.0);
	glVertex2f(5.0, 45.0);
	glEnd();


	//right front wing
	glColor3f(1.0, 0.6, 0.6);
	glBegin(GL_POLYGON);
	glVertex2f(13.0, 46.0);
	glVertex2f(18.0, 46.0);
	glVertex2f(13.0, 41.0);
	glVertex2f(11.0, 41.0);
	glEnd();


	//dome
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(13.0, 47.0);
	glVertex2f(15.0, 48.5);
	glVertex2f(17.0, 49.0);
	glVertex2f(19.0, 48.0);
	glVertex2f(21.0, 46.0);
	glVertex2f(17.0, 46.0);
	glVertex2f(15.0, 47.5);
	glVertex2f(13.0, 47.0);
	glEnd();


	//right tail wing
	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_POLYGON);
	glVertex2f(5.5, 47.0);
	glVertex2f(8.5, 47.0);
	glVertex2f(5.5, 43.0);
	glVertex2f(4.5, 43.0);
	glEnd();


	// front tip
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_POLYGON);
	glVertex2f(22.0, 45.0);
	glVertex2f(22.3, 45.375);
	glVertex2f(22.6, 45.75);
	glVertex2f(22.3, 46.125);
	glVertex2f(22.0, 46.5);
	glEnd();
}


void drawString(float x, float y, float z, void* font, char* string)
{
	char* c;
	glRasterPos3f(x, y, z);

	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void gameEnd()
{
	gameEndStatus = true;
	glColor3f(0.3, 0.56, 0.84);   //game end background screen
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.137, 0.137, 0.556);
	glVertex3f(100.0, 0.0, 0.0);
	glColor3f(0.196, 0.196, 0.8);
	glVertex3f(100.0, 100.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	glEnd();
	glPushMatrix();
	glScalef(0.8, 0.8, 0);
	drawJet();
	glPopMatrix();

	glColor3f(0.196, 0.196, 0.8);  // disp box
	glRectf(20.0, 20.0, 80.0, 80.0);
	glColor3f(0.8, 0.8, 0.8);
	glRectf(21.0, 21.0, 79.0, 79.0);

	glColor3f(0.196, 0.196, 0.8);   //restart button
	glRectf(40, 5, 60, 10);
	glColor3f(0.8, 0.8, 0.8);
	glRectf(40.5, 5.5, 59.5, 9.5);
	glColor3f(0.137, 0.137, 0.556);

	drawString(43, 6, 0, GLUT_BITMAP_TIMES_ROMAN_24, "RESTART");
	drawString(41, 71, 0, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER!!!");
	drawString(23, 61, 0, GLUT_BITMAP_HELVETICA_18, "DISTANCE :");
	drawString(40, 61, 0, GLUT_BITMAP_TIMES_ROMAN_24, score_Str);
	printf("m\n");
	printf("\n");
	drawString(23, 56, 0, GLUT_BITMAP_HELVETICA_18, "LEVEL         :");
	drawString(40, 56, 0, GLUT_BITMAP_TIMES_ROMAN_24, slevel);



	drawString(33, 30, 0, GLUT_BITMAP_HELVETICA_18, " ENJOY PLAYING THE GAME");

	glutPostRedisplay();

}

void drawBg()
{
	glPushMatrix();

	glColor3f(0.0, 0.48, 0.047);		// green floor

	glBegin(GL_POLYGON);
	glVertex3f(0.0, 9.0, 0.0);
	glVertex3f(100.0, 9.0, 0.0);
	glColor3f(0.0, 0.3, 0.03);
	glVertex3f(100.0, 10.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);
	glVertex3f(0.0, 9.0, 0.0);
	glEnd();

	glColor3f(0.474, 0.298, 0.074);  // brown ground
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glColor3f(0.3, 0.1, 0.03);
	glVertex3f(100.0, 9.0, 0.0);
	glVertex3f(0.0, 9.0, 0.0);
	glEnd();

	glColor3f(0.5, 0.6, 0.79);
	glBegin(GL_POLYGON);				//ceiling
	glVertex3f(0.0, 100.0, 0.0);
	glVertex3f(100.0, 100.0, 0.0);
	glColor3f(0.6, 0.7, 0.89);
	glVertex3f(100.0, 80.0, 0.0);
	glVertex3f(0.0, 80.0, 0.0);
	glEnd();

	glColor3f(0.5, 0.6, 0.79);// sky blue
	glBegin(GL_POLYGON);   //background screen
	glVertex3f(0.0, 90.0, 5.0);
	glVertex3f(100.0, 90.0, 5.0);
	glColor3f(0.7, 0.8, 0.99);//sky
	glVertex3f(100.0, 10.0, 5.0);
	glVertex3f(0.0, 10.0, 5.0);
	glEnd();

	glPopMatrix();
}

void welcome()
{
	glColor3f(0.3, 0.56, 0.84);   //welcome background
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glColor3f(0.137, 0.137, 0.556);
	glVertex3f(100.0, 0.0, 0.0);
	glColor3f(0.196, 0.196, 0.8);
	glVertex3f(100.0, 100.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	glEnd();
	drawJet();

	// button 1 .. PLAY
	glColor3f(0.196, 0.196, 0.8);
	glRectf(39.5, 39.5, 60.5, 45.5);

	glColor3f(0.8, 0.8, 0.8);
	glRectf(40, 40, 60, 45);
	glColor3f(0.137, 0.137, 0.556);
	drawString(47, 42, 0, GLUT_BITMAP_HELVETICA_18, "PLAY");

	// button 2 .. instructions
	glColor3f(0.196, 0.196, 0.8);
	glRectf(39.5, 29.5, 60.5, 35.5);

	glColor3f(0.8, 0.8, 0.8);
	glRectf(40, 30, 60, 35);
	glColor3f(0.137, 0.137, 0.556);
	drawString(41, 31, 0, GLUT_BITMAP_HELVETICA_18, "INSTRUCTIONS");

	// button 3 .. ABOUT
	glColor3f(0.196, 0.196, 0.8);
	glRectf(39.5, 19.5, 60.5, 25.5);

	glColor3f(0.8, 0.8, 0.8);
	glRectf(40, 20, 60, 25);
	glColor3f(0.137, 0.137, 0.556);
	drawString(46, 21, 0, GLUT_BITMAP_HELVETICA_18, "ABOUT");

	// button 4 .. exit
	glColor3f(0.196, 0.196, 0.8);
	glRectf(39.5, 9.5, 60.5, 15.5);

	glColor3f(0.8, 0.8, 0.8);
	glRectf(40, 10, 60, 15);
	glColor3f(0.137, 0.137, 0.556);
	drawString(47, 11, 0, GLUT_BITMAP_HELVETICA_18, "EXIT");


	glPushMatrix();

	glColor3f(0.8, 0.8, 0.8);
	drawString(25.5, 92, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Simulation For Real-world Events of Airplane");
	drawString(35.5, 80, 0, GLUT_BITMAP_TIMES_ROMAN_24, "AIRPLANE GAME");
	glPopMatrix();
	glColor3f(0.137, 0.137, 0.556);

}


int main(int argc, char** argv)
{
	printf("\nHow To Play");
	printf("Click and hold mouse left key to gain altitude of the plane\n");
	printf("Release the mouse left key to reduce the altitude\n");
	printf("Use the Right mouse key to speed up the plane(NOS)\n");
	printf("\n");
	printf("-->The main aim of the game is to avoid the obstacles: buildings and clouds\n");
	printf("-->Also the meter at the bottom shows the distance travelled,NITROS left,Atitude and LEVEL.\n");
	printf("\n");
	printf("-->As you reach distance multples of 50 tour level increases as well as the speed of the plane.\n");
	printf("==>ENJOY PLAYING THE GAME\n");
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREENW, SCREENH);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Flying game");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyPressed);
	glutMainLoop();
	return 0;
}
