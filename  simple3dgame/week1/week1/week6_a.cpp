//OpenGL klassen
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

#include <math.h>

#include "glm.h"
//#include <gl\freeglut.h>

#include "RoboRally.h"

using namespace std;

static float ypoz = 0, zpoz = 0;
//GLMmodel* pmodel1 = NULL;

vector<GLMmodel*> models;

void display();
void reshape(int width, int height);

int i = 6;
RoboRally rrGame;

//Test met freeview
float camX = 0.0;
float camY = 0.0;
float camZ = 15.0;

void loadModels(){
	//Misschien mooier om dit in een map te doen?
	//Controle toevoegen of alles goed ingeladen word
	models.resize(8);
	models[0] = glmReadOBJ(".\\objects\\Battery.obj");
	models[1] = glmReadOBJ(".\\objects\\Chip.obj");
	models[2] = glmReadOBJ(".\\objects\\Exit.obj");
	models[3] = glmReadOBJ(".\\objects\\Lift.obj");
	models[4] = glmReadOBJ(".\\objects\\Platform.obj");
	models[5] = glmReadOBJ(".\\objects\\Robot.obj");
	models[6] = glmReadOBJ(".\\objects\\Wall.obj");
 	models[7] = glmReadOBJ(".\\objects\\Water.obj");
}

//Teken het model
void drawModel(int modelNr){
    GLfloat factor;
	factor = glmUnitize(models[modelNr]);						//Schaal het model
    glmFacetNormals(models[modelNr]);					
	glmVertexNormals(models[modelNr], 90.0);
    glmDraw(models[i], GLM_SMOOTH | GLM_TEXTURE);		//Teken het model
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
	//Initialiseer spel
	loadModels();

	glutInit(&argc, argv);										//Initialisser OpenGL
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);	//Kleurmode | Diepte | Buffer
	glutInitWindowSize(512,512);								//Geef aan hoe groot het venster is
	glutInitWindowPosition(100,100);							//Geef aan, vanaf welk punt het venster getekend moet worden
	glutCreateWindow("RoboRally 3D");							//Creeër het venster
		
	//Init
    glClearColor(0.0, 0.0, 0.0, 0.0);   
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
	
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
		for (int x = 0; x < rrGame.getCurLevel().getWidth(); x++){
			for (int y = 0; y < rrGame.getCurLevel().getHeight(); y++){
				if (rrGame.getCurLevel().getElement(x,y,0) == WALL){
					glPushMatrix();
					glTranslatef(x,0,y);		//De Z as representeerd de Y uit de 2D wereld
					drawModel(i);
					glPopMatrix();
				}
			}
		}
	//}

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