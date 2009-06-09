/**
TODO: - Onder chips (en batterijen?) moet een platform komen.

Level inladen: op X as van 0 -> breedte
			   op Y van lengte -> 0

*/
#include "stdafx.h"
#include <windows.h>
#include <math.h>

//OpenGL klassen
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

#include "glm.h"
//#include <gl\freeglut.h>

#include "RoboRally.h"

using namespace std;

//Objecten met obj, om naamconflict met typedefs te voorkomen
enum objecten{OBJBATTERY,OBJCHIP,OBJEXIT,OBJLIFT,OBJPLATFORM,OBJROBOT,OBJWALL,OBJWATER};

vector<GLMmodel*> _models;

void display();
void reshape(int width, int height);

RoboRally rrGame;

bool _freeView = false;					//Freeview (vrij rondvliegen met camera
bool _thirdPerson = true;				//Thirdperson
float eyeX,eyeY,eyeZ;					//Camerstandpunten
float lookX, lookZ;						//Waar wordt er naar gekeken vanuit oog/camera-punt
float rotation = 0;

GLfloat scalefact;

//Initialisatie
void init(){
    glClearColor(0.7, 0.7, 0.7, 1.0);	//Schermkleur (achtergrond)
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

//Inladen van de 3D objecten
void loadModels(){
	//Controle toevoegen of alles goed ingeladen word
	_models.resize(8);
	_models[OBJBATTERY]		= glmReadOBJ("Battery.obj");
	_models[OBJCHIP]		= glmReadOBJ("Chip.obj");
	_models[OBJEXIT]		= glmReadOBJ("Exit.obj");
	_models[OBJLIFT]		= glmReadOBJ("Lift.obj");
	_models[OBJPLATFORM]	= glmReadOBJ("Platform.obj");
	_models[OBJROBOT]		= glmReadOBJ("Robot.obj");
	_models[OBJWALL]		= glmReadOBJ("Wall.obj");
 	_models[OBJWATER]		= glmReadOBJ("Water.obj");

	//scalefact = glmUnitize(_models[OBJWALL]);

	for(unsigned int i=0;i<_models.size();i++){
		if(_models[i]!=NULL){
			// Schalen van de objecten
			glmUnitize(_models[i]);
//			glScalef(scalefact, scalefact, scalefact);
			glmFacetNormals(_models[i]);        
			glmVertexNormals(_models[i], 90.0);
		}
	}

	//Objecten scalen
	glmScale(_models[OBJPLATFORM], 0.5);
	glmScale(_models[OBJCHIP], 0.25);
	glmScale(_models[OBJBATTERY], 0.25);
	glmScale(_models[OBJWATER], 0.5);
	glmScale(_models[OBJLIFT], 0.5);
	glmScale(_models[OBJEXIT], 0.5);
	glmScale(_models[OBJROBOT], 0.5);
}

void specialKeys(int key, int x, int y){
	switch(key) {
		case GLUT_KEY_F1:		//Move camera to players position
			if (_freeView)
				_freeView = false;
			else
				_freeView = true;
			break;
		case GLUT_KEY_UP:
			rrGame.moveRobot(FORWARD);
			break;
		case GLUT_KEY_DOWN:
			rrGame.moveRobot(BACKWARD);
			break;
		case GLUT_KEY_LEFT:
			rrGame.turnRobotLeft();
			break;
		case GLUT_KEY_RIGHT:
			rrGame.turnRobotRight();
			break;
		}
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
//	 case 'c':
//		 if (i == 7)
//			 i = 0;
//		 else
//			 i++;
//		 break;
	 case 'q':
		eyeZ += 1.0;
		break;
	 case 'e':
		eyeZ -= 1.0;
		break;
	 case 's':
		 eyeY -= 1.0;
		 break;
	 case 'w':
		 eyeY += 1.0;
		 break;
	 case 'a':
		 eyeX -= 1.0;
		 break;
	 case 'd':
		 eyeX += 1.0;
		 break;
   }
   glutPostRedisplay();
}

void animate()
{
	rotation+=10.0;
	if (rotation>360) rotation=0;
	glutPostRedisplay();
}

int main(int argc, char **argv){
	glutInit(&argc, argv);										//Initialisser OpenGL
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);	//Kleurmode | Diepte | Buffer
	glutInitWindowSize(512,512);								//Geef aan hoe groot het venster is
	glutInitWindowPosition(100,100);							//Geef aan, vanaf welk punt het venster getekend moet worden
	glutCreateWindow("RoboRally 3D");							//Creeër het venster
	
	//Initialistie opengl window
	init();
	
	//Initialiseer spel
	loadModels();												//Modellen/objecten inladen en scalen
	glEnable(GL_CULL_FACE);										//Backface culling aanzetten (textures alleen buitenkant object
	
	glutDisplayFunc(display);									//De functie die voor een "redraw" wordt aangeroepen
	glutReshapeFunc(reshape);									//De functie bij een resize
	glutKeyboardFunc(keyboard);									//De functie voor het keyboard
	glutSpecialFunc(specialKeys);								//De functie voor de speciale toetsen

	glutIdleFunc(animate);										//Idle functie zorgt o.a. voor de animatie
	glutMainLoop();												//Default GLUT loop

	return 0;	//Eind programma
}

//Tekenen
void display(){
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	unsigned int ori = rrGame.getCurOrientation();		//Bepaald waar de robot heen kijkt

	//Als _freeView aanstaat wordt er geen stap gezet
	if (!_freeView){	
		switch(ori){
			case NORTH:
				lookX = 0.0;
				lookZ = -1.0;
				break;
			case EAST:
				lookX = 90.0;
				lookZ = 0.0;
				break;
			case SOUTH:
				lookX = 0.0;
				lookZ = +1.0;
				break;
			case WEST:
				lookX = -90.0;
				lookZ = 0.0;
				break;
		}

		eyeX = rrGame.getCurPositionX();// + 0.5;
		eyeY = rrGame.getCurPositionPlatform()*2.0;// + 0.5;
		eyeZ = rrGame.getCurPositionY();// + 0.5;
	}
		
	if (_thirdPerson){
		gluLookAt(eyeX, eyeY + 1.0, eyeZ, 
			      eyeX + lookX, eyeY - 0.5, eyeZ + lookZ,
				  0.0f,1.0f,0.0f);
	} else {
		gluLookAt(eyeX, eyeY, eyeZ, 
			      eyeX + lookX, eyeY, eyeZ + lookZ,
				  0.0f,1.0f,0.0f);
	}
	//Level tekenen (momenteel 1 laag)
	//int p = 0;
	for (int p = 0; p < rrGame.getCurLevel().getPlatforms(); p++){
		for (unsigned int x = 0; x < rrGame.getCurLevel().getWidth(); x++){
			for (unsigned int y = 0; y < rrGame.getCurLevel().getHeight(); y++){
				switch (rrGame.getCurLevel().getElement(x,y,p)){
					case EXIT:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glRotatef(rotation,0,1,0);
						glmDraw(_models[OBJEXIT], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case LIFT:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJLIFT], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case WATER:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJWATER], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case BATTERY:
						glPushMatrix();
						glTranslatef(x,(p*2.0)+0.30,y);		//De Z as representeerd de Y uit de 2D wereld
						glRotatef(rotation,0,1,0);
						glmDraw(_models[OBJBATTERY], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case CHIP:
						glPushMatrix();
						glTranslatef(x,(p*2.0)+0.125,y);		//De Z as representeerd de Y uit de 2D wereld
						glRotatef(rotation,0,1,0);					
						//glScalef(0.25,0.25,0.25);//test
						glmDraw(_models[OBJCHIP], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case WALL:
						glPushMatrix();
						glTranslatef(x,(p*2.0)+1.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJWALL], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case HOLE:
						//Niks
						break;
					default:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
				}
			}
		}
	}

	//Robot tekenen
	glPushMatrix();
	glTranslatef(rrGame.getCurPositionX(),rrGame.getCurPositionPlatform()*2.0,rrGame.getCurPositionY());
	//Rotatie (welke kant kijkt de robot op?)
	switch(ori){
		case NORTH:
			glRotatef(90.0, 0.0, 1.0, 0.0);
			break;
		case EAST:
			glRotatef(0.0, 0.0, 1.0, 0.0);			
			break;
		case SOUTH:
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			break;
		case WEST:
			glRotatef(180.0, 0.0, 1.0, 0.0);
			break;
	}
	

	glmDraw(_models[OBJROBOT], GLM_SMOOTH | GLM_TEXTURE);
	glPopMatrix();



	//Sleep(1);
	glutSwapBuffers();  
	
}

void reshape (int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 1000.0);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
}

