#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<math.h>
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"Winmm.lib")

static int year = 0;
static int speed = 1;
static float newx,newy;
static float zoomz = 5,zoomx,zoomy;
const float DEG2RAD = 3.14159/180;
void init(){
	//Sets the background to black and sets up light
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);

	GLfloat light_specular[] = {1.0,1.0,0.0};
	GLfloat light_position[] = {0.0,0.0,0.0,1.0};

	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void drawRing(float radius)
{
//Draws a circle for rings of saturn
   int i;

   glBegin(GL_POLYGON);
   for (i=0; i < 360; i++)
   {
      float degInRad = i*DEG2RAD;
      glVertex3f(cos(degInRad)*radius,0,sin(degInRad)*radius);
   }
   glEnd();
}
void planet(float radii,float rotation,float translation,float red,float green,float blue){
	GLfloat mats[] = {red,green,blue,0.0};
	glPushMatrix();
	glRotatef((GLfloat)year/rotation,0.0,1.0,0.0);
	glTranslatef(translation,0.0,0.0);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mats);
	glutSolidSphere(radii,200,200); 
	glPopMatrix();
}
void display(){
	GLfloat mats[] = {0.0,0.0,0.0,0.0};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*All sizes done in miles
	  Distances scaled by roughly .000000001
	  Radii scaled by roughly .000003
	  Yearly rotations done on a scale based on Earth years*/

	//Sun
	glPushMatrix();
	mats[0] = 1.0;mats[1] = 0.8;mats[2] = 0.0;mats[3] = 0.0;
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,mats);
	glutSolidSphere(1.42,200,200); 
	glPopMatrix();
	
	//Mercury
	planet(0.005,0.24,1.48,0.8,0.6,0.6);

	//Venus
	planet(0.0124,0.61,1.521,0.0,0.4,0.0);

	//Earth
	planet(0.01305,1,1.574,0.0,0.0,0.8);

	//Mars
	planet(0.00696,1.88,1.655,1.0,0.4,0.0);

	//Asteroids... kinda
	glPushMatrix();
	mats[0] = 1.0;mats[1] = 1.0;mats[2] = 1.0;mats[3] = 0.0;
	glRotatef((GLfloat)year,0.0,1.0,0.0);
	glRotatef(90,1.0,0.0,0.0);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mats);
	glutWireTorus(0.1,1.8,50,200);
	glPopMatrix();

	//Jupiter
	planet(0.1463,12,2.225,0.8,0.6,0.4);

	//Saturn, doesn't use planet function so it can have rings
	mats[0] = 0.8;mats[1] = 0.7;mats[2] = 0.5;mats[3] = 0.0;
	glPushMatrix();
	glRotatef((GLfloat)year/29.5,0.0,1.0,0.0);
	glTranslatef(2.897,0.0,0.0);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mats);
	glutSolidSphere(0.123,200,200);
	drawRing(0.18);
	glPopMatrix();

	//Uranus
	planet(0.05207,84,4.391,0.0,0.5,0.0);

	//Neptune
	planet(0.04981,165,4.912,0.0,0.5,0.8);
	
	glutSwapBuffers();
}
//Changes the position of the camera
void changeView(){
	glLoadIdentity();
	gluLookAt(zoomx,zoomy,zoomz,
			  newx,newy,0.0,
			  0.0,1.0,0.0);
}
void reshape(int w,int h){
	//Sets the viewport and the starting location to look at
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0,(GLfloat)w/(GLfloat)h,0.1,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0,
			  0.0,0.0,0.0,
			  0.0,1.0,0.0);
}

void keyboard(unsigned char key,int x, int y){
//Speeds up or slows down the animation
	switch(key){
		//Increases the speed of animation
		case 'r':
			speed++;
			break;
		//Decreases the speed of animation
		case 'f':
			speed--;
			break;
		//moves the camera towards the center
		case 'e':
			zoomz += 0.1;
			changeView();
			break;
		//moves the camera away from the center
		case 'q':
			zoomz -= 0.1;
			changeView();
			break;
		//moves the camera down relative to the center
		case 's':
			zoomy -= 0.1;
			changeView();
			break;
		//moves the camera up relative to the center
		case 'w':
			zoomy += 0.1;
			changeView();
			break;
		//moves the camera left relative to the center
		case 'a':
			zoomx -= 0.1;
			changeView();
			break;
		//moves the camera right relative to the center
		case 'd':
			zoomx += 0.1;
			changeView();
			break;
	}
}
//Moves the center of the viewing port roughly with the mouse
void mouse(int x,int y){
	newx = (x - 644)/126.0;
	newy = (y - 392)/-68.0;
	changeView();
}

void idle(){
//Increases the rotation factor and redisplays the solar system
	year = (year + speed) % 59400;
	glutPostRedisplay();
}

void menu(GLint menuItemNumber){
	//resets the camera moving variables to defaults
	newx = 0;
	newy = 0;
	zoomz = 5;
	zoomx = 0;
	zoomy = 0;
	//Resets the view to default eye-level view
	switch(menuItemNumber){
		case 1:
			changeView();
			break;
		case 2:
			exit(1);
			break;
	}
}

int main(int argc, char** argv){
	PlaySound("Forbidden Fruit.wav",NULL,SND_ASYNC | SND_FILENAME);
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(2000,2000);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Solar System");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutCreateMenu(menu);
		glutAddMenuEntry("Reset View",1);
		glutAddMenuEntry("Exit",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
