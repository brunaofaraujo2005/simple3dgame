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

static float ypoz = 0, zpoz = 0;
//GLMmodel* pmodel1 = NULL;

vector<GLMmodel*> _models;

void display();
void reshape(int width, int height);

int i = 6;
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

void loadModels(){
	//Misschien mooier om dit in een map te doen?
	//Controle toevoegen of alles goed ingeladen word
	_models.resize(8);
	_models[0] = glmReadOBJ("Battery.obj");
	_models[1] = glmReadOBJ("Chip.obj");
	_models[2] = glmReadOBJ("Exit.obj");
	_models[3] = glmReadOBJ("Lift.obj");
	_models[4] = glmReadOBJ("Platform.obj");
	_models[5] = glmReadOBJ("Robot.obj");
	_models[6] = glmReadOBJ("Wall.obj");
 	_models[7] = glmReadOBJ("Water.obj");
	for(unsigned int i=0;i<_models.size();i++){
		if(_models[i]!=NULL){
			// Schalen van de objecten
			glmUnitize(_models[i]);
			// These 2 functions calculate triangle and vertex normals from the geometry data.
			// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
			// So if you can export these directly from you modeling tool do it and comment these line
			// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
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
	 case 'c':
		 if (i == 7)
			 i = 0;
		 else
			 i++;
		 break;
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
	ypoz+=0.5;
	if (ypoz>360) ypoz=0;
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
	//glutIdleFunc(animate);
	glutMainLoop();

	return 0;	//Eind programma
}

//Tekenen
void display(){
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	  
	gluLookAt(camX, camY, camZ, 
		      camX, camY, camZ-15,
			  0.0f,1.0f,0.0f);


	
	//Level tekenen
	//for (int z = 0; z < rrGame.getCurLevel().getPlatforms(); z++){
		for (unsigned int x = 0; x < rrGame.getCurLevel().getWidth(); x++){
			for (unsigned int y = 0; y < rrGame.getCurLevel().getHeight(); y++){
				if (rrGame.getCurLevel().getElement(x,y,0) == WALL){
					glPushMatrix();
					glTranslatef(x,0,y);		//De Z as representeerd de Y uit de 2D wereld
					glmDraw(_models[6], GLM_SMOOTH | GLM_TEXTURE);
					glPopMatrix();
				}
			}
		}
	//}
//				glPushMatrix();
//				glTranslatef(x,p,y);
//				glmDraw(_objects[Wall], GLM_SMOOTH| GLM_TEXTURE);
//				glPopMatrix();

	//glPushMatrix();
	//for (int y = rrGame.getCurLevel().getHeight(); y > 0 ; y--){
	//	if (rrGame.getCurLevel().getElement(0,y,0) == WALL){
	//		glTranslatef(0,0,-1);		
	//		drawModel(i);
	//	}
	//}
	//glPopMatrix();
	//

//	glTranslatef(0,0,0);	
//	drawModel(i);
//	glTranslatef(1,0,0);	
//	drawModel(i);
//	glTranslatef(1,0,0);	
//	drawModel(i);


	//glPushMatrix();	
	// I added these to be able to rotate the whole scene so you can see the box and textures
	//	glRotatef(ypoz,0,1,0);
	//	glRotatef(zpoz,0,0,1);		
	//	drawModel(i);
	//	drawModel(i);
	//glPopMatrix();

	//glTranslatef(0,0,-1);
	//glPushMatrix();	
	//	drawModel(i);
	//glPopMatrix();

	//Sleep(5);
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

