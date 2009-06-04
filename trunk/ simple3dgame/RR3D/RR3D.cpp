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

static float ypoz = 0, zpoz = 0, rotation = 0;
//GLMmodel* pmodel1 = NULL;

vector<GLMmodel*> _models;

void display();
void reshape(int width, int height);

RoboRally rrGame;

//Test met freeview
float camX = 0.0;
float camY = 0.0;
float camZ = 15.0;

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

	for(unsigned int i=0;i<_models.size();i++){
		if(_models[i]!=NULL){
			// Schalen van de objecten
			glmUnitize(_models[i]);
			glmFacetNormals(_models[i]);        
			glmVertexNormals(_models[i], 90.0);
		}
	}
}

//Teken het model
void drawModel(int modelNr){
 //   GLfloat factor;
	//factor = glmUnitize(models[modelNr]);						//Schaal het model
    //glmFacetNormals(models[modelNr]);					
	//glmVertexNormals(models[modelNr], 90.0);
  //  glmDraw(models[i], GLM_SMOOTH | GLM_TEXTURE);		//Teken het model
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
	  case 'y':         
		  ypoz=ypoz+5;
		  if (ypoz>360) ypoz=0;
         glutPostRedisplay();
         break;
	 case 'z':         
		 zpoz = zpoz+5;
		 if (zpoz>360) zpoz=0;
         glutPostRedisplay();
         break;	
//	 case 'c':
//		 if (i == 7)
//			 i = 0;
//		 else
//			 i++;
//		 break;
	 case 'q':
		camZ += 1.0;
		break;
	 case 'e':
		camZ -= 1.0;
		break;
	 case 's':
		 camY -= 1.0;
		 break;
	 case 'w':
		 camY += 1.0;
		 break;
	 case 'a':
		 camX -= 1.0;
		 break;
	 case 'd':
		 camX += 1.0;
		 break;
   }
   glutPostRedisplay();
}

void animate()
{
	rotation+=1;
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
	loadModels();
	
	glutDisplayFunc(display);									//De functie die voor een "redraw" wordt aangeroepen
	glutReshapeFunc(reshape);									//De functie bij een resize
	glutKeyboardFunc(keyboard);									//De functie voor het keyboard

    //Do a little animation - rotate the object a bit so we can see it better DEBUG
	glutIdleFunc(animate);
	glutMainLoop();

	return 0;	//Eind programma
}

//Tekenen
void display(){
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	  
	gluLookAt(camX, camY+15.0, camZ, 
		      camX, camY, camZ-15,
			  0.0f,1.0f,0.0f);

	//Level tekenen (momenteel 1 laag)
	int p = 0;
	//for (int z = 0; z < rrGame.getCurLevel().getPlatforms(); z++){
		for (unsigned int x = 0; x < rrGame.getCurLevel().getWidth(); x++){
			for (unsigned int y = 0; y < rrGame.getCurLevel().getHeight(); y++){
				switch (rrGame.getCurLevel().getElement(x,y,0)){
					case EXIT:
						glPushMatrix();
						glTranslatef(x,p,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJEXIT], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case SPACE:
						glPushMatrix();
						glTranslatef(x,p,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case LIFT:
						glPushMatrix();
						glTranslatef(x,p,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJLIFT], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case WATER:
						glPushMatrix();
						glTranslatef(x,p,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJWATER], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case BATTERY:
						glPushMatrix();
						glTranslatef(x,p,y);		//De Z as representeerd de Y uit de 2D wereld
						glRotatef(rotation,0,1,0);
						glmDraw(_models[OBJBATTERY], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case CHIP:
						glPushMatrix();					
						glTranslatef(x,p,y);		//De Z as representeerd de Y uit de 2D wereld
						glRotatef(rotation,0,1,0);
						glmDraw(_models[OBJCHIP], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case WALL:
						glPushMatrix();
						glTranslatef(x,p,y);		//De Z as representeerd de Y uit de 2D wereld
						glmDraw(_models[OBJWALL], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
				}
			}
		}
	//}

	Sleep(5);
	glutSwapBuffers();  
	
}

void reshape (int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 1.0, 1000.0);
   glMatrixMode (GL_MODELVIEW);
}

