//Standaard klassen
#include "stdafx.h"
#include <windows.h>
#include <math.h>

//OpenGL klassen
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include "glm.h"

//Spel klasse
#include "RoboRally.h"	

using namespace std;

//Objecten met obj, om naamconflict met typedefs te voorkomen
enum objecten{OBJBATTERY,OBJCHIP,OBJEXIT,OBJLIFT,OBJPLATFORM,OBJROBOT,OBJWALL,OBJWATER};

void display();
void reshape(int width, int height);

RoboRally rrGame;						//Spelinstantie
bool _freeView = false;					//Freeview (vrij rondvliegen met camera
bool _thirdPerson = true;				//Thirdperson
float eyeX,eyeY,eyeZ;					//Camerstandpunten
float lookX, lookY, lookZ;				//Waar wordt er naar gekeken vanuit oog/camera-punt
float rotation = 0;						//Rotatiefactor van objecten
vector<GLMmodel*> _models;				//Bevat alle objecten/modellen/elementen
int width = 512, height = 512;			//Hoogte en breedte van het scherm
int frame = 0, time, timebase=0;		//Benodigd om de framerate te bepalen
float fps;								//Frames per second
bool _drawFPS = false;					//Teken het aantal frames per second

//Print tekst op het scherm.
void print(int x, int y, string text, float colorR, float colorG, float colorB){
       glPushMatrix ();
       glLoadIdentity ();
       glMatrixMode(GL_PROJECTION);
       glPushMatrix ();
       glLoadIdentity();

       GLint viewport [4];
       glGetIntegerv (GL_VIEWPORT, viewport);
       gluOrtho2D (0,viewport[2], viewport[3], 0);

       glDepthFunc (GL_ALWAYS);
       glColor3f (colorR, colorG, colorB);
       glRasterPos2f(x, y);
       for (unsigned int i = 0; i<text.length(); ++i)
               glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
       glDepthFunc (GL_LESS);
       glPopMatrix ();
       glMatrixMode(GL_MODELVIEW);
       glPopMatrix ();

	   glColor3f (255,255,255);
}


//Initialisatie
void init(){
    glClearColor(0.3, 0.3, 0.3, 1.0);	//Schermkleur (achtergrond)
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

	//Objecten unitizen in het midden zetten van een raster en resizen raster x = -1 tot 1 en y ook
	for(unsigned int i=0;i<_models.size();i++){
		if(_models[i]!=NULL){
			glmUnitize(_models[i]);
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

//Afvangen van speciale keyboard input
void specialKeys(int key, int x, int y){
	switch(key) {
		case GLUT_KEY_F1:		//Move camera to players position
			if (_freeView)
				_freeView = false;
			else
				_freeView = true;
			break;
		case GLUT_KEY_F2:		//Switchen tussen first en third person view
			if (_thirdPerson)
				_thirdPerson = false;
			else
				_thirdPerson = true;
			break;
		case GLUT_KEY_F3:		//Teken het aantal frames per seconde
			if (_drawFPS)
				_drawFPS = false;
			else
				_drawFPS = true;
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

//Afvangen van keyboard input
void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case 27:
			exit(0);
			break;
		//Spel starten
		case 'b':
			rrGame.startGame();
			break;
		case 'B':
			rrGame.startGame();
			break;
		//Vrije camera bewegen (voornml. voor debugging)
		case 'q':
			eyeY -= 1.0;
			break;
		case 'e':
			eyeY += 1.0;
			break;
		case 's':
			eyeZ += 1.0;
			break;
		case 'w':
			eyeZ -= 1.0;
			break;
		case 'a':
			eyeX -= 1.0;
			break;
		case 'd':
			eyeX += 1.0;
			break;
		case 'r':
			lookY += 0.5;
			break;
		case 'f':
			lookY -= 0.5;
			break;
	}
	glutPostRedisplay();
}

//Zorgen dat objecten gaan draaien
void animate(){
	rotation+=10.0;	//Aantal stappen in een keer
	if (rotation>360) rotation=0;
	glutPostRedisplay();
}

//Main
int main(int argc, char **argv){
	glutInit(&argc, argv);										//Initialisser OpenGL
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);	//Kleurmode | Diepte | Buffer
	glutInitWindowSize(512,512);								//Geef aan hoe groot het venster is
	glutInitWindowPosition(100,100);							//Geef aan, vanaf welk punt het venster getekend moet worden
	glutCreateWindow("RoboRally 3D");							//Cree�r het venster
	
	//Initialistie opengl window
	init();
	
	//Initialiseer spel
	loadModels();												//Modellen/objecten inladen en scalen
	//glEnable(GL_CULL_FACE);									//Backface culling aanzetten (textures alleen buitenkant object

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
		eyeX = rrGame.getCurPositionX();
		eyeY = rrGame.getCurPositionPlatform()*2.0;
		eyeZ = rrGame.getCurPositionY();
		
		lookX = eyeX;
		lookZ = eyeZ;		
		//Afhankelijk van de view renderen
		if (_thirdPerson){			//Third person
			switch(ori){
				case NORTH:					
					lookZ -= 1.0;
					eyeZ  += 1.0;
					break;
				case EAST:
					lookX += 1.0;
					eyeX  -= 1.0;
					break;
				case SOUTH:
					lookZ += 1.0;
					eyeZ  -= 1.0;
					break;
				case WEST:
					lookX -= 1.0;
					eyeX  += 1.0;
					break;
			}
			gluLookAt(eyeX, eyeY + 1.1, eyeZ, 
					  lookX, eyeY, lookZ,
					  0.0f,1.0f,0.0f);
		} else {					//First person
			switch(ori){
				case NORTH:
					lookZ -= 1.0;
					eyeZ  += 0.5;
					break;
				case EAST:
					lookX += 1.0;
					eyeX  -= 0.5;
					break;
				case SOUTH:
					lookZ += 1.0;
					eyeZ  -= 0.5;
					break;
				case WEST:
					lookX -= 1.0;
					eyeX  += 0.5;
					break;
			}
			gluLookAt(eyeX, eyeY + 0.5, eyeZ, 
					  lookX, eyeY + 0.5, lookZ,
					  0.0f,1.0f,0.0f);
		}
	} else {		//Freeview
		gluLookAt(eyeX, eyeY + 0.5, eyeZ, 
				  eyeX, eyeY + 0.05 + lookY, eyeZ - 1,
				  0.0f,1.0f,0.0f);
	}

	//Level tekenen (De Z as representeerd de Y uit de 2D wereld)
	for (unsigned int p = 0; p < rrGame.getCurLevel().getPlatforms(); p++){
		for (unsigned int x = 0; x < rrGame.getCurLevel().getWidth(); x++){
			for (unsigned int y = 0; y < rrGame.getCurLevel().getHeight(); y++){
				switch (rrGame.getCurLevel().getElement(x,y,p)){
					case EXIT:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);		
						glRotatef(rotation,0,1,0);
						glmDraw(_models[OBJEXIT], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case LIFT:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);
						glmDraw(_models[OBJLIFT], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case WATER:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);
						glmDraw(_models[OBJWATER], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case BATTERY:
						glPushMatrix();
						glTranslatef(x,(p*2.0)+0.30,y);
						glRotatef(rotation,0,1,0);
						glmDraw(_models[OBJBATTERY], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case CHIP:
						glPushMatrix();
						glTranslatef(x,(p*2.0)+0.125,y);
						glRotatef(rotation,0,1,0);					
						glmDraw(_models[OBJCHIP], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						glPushMatrix();
						glTranslatef(x,p*2.0,y);
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case WALL:
						glPushMatrix();
						glTranslatef(x,(p*2.0)+1.0,y);
						glmDraw(_models[OBJWALL], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
					case HOLE:
						//(nog) Niks
						break;
					default:
						glPushMatrix();
						glTranslatef(x,p*2.0,y);
						glmDraw(_models[OBJPLATFORM], GLM_SMOOTH | GLM_TEXTURE);
						glPopMatrix();
						break;
				}
			}
		}
	}

	//Robot tekenen als je in third person staat of freeview
	if (_thirdPerson || _freeView){
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
	}	

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	
	if (time - timebase > 1000) {
		fps = (frame*1000.0/(time-timebase));
	 	timebase = time;		
		frame = 0;
	}


	//Tekst op het scherm
	if (_drawFPS)
		print(20,20,"FPS: "+toString(fps),100.0,0.0,0.0);
	if (!rrGame.isStarted())
		print(width/2 - 100, height/2, "Druk op B om te beginnen", 0.0,0.0, 100.0);
	print(10, height - 20, "SCORE: " + toString(rrGame.getCurScore()), 100.0, 0.0, 0.0);
	print(width/2.0 - 50, height - 20, "ENERGY: " + toString(rrGame.getCurEnergy()), 100.0, 0.0, 0.0);
	print(width - 85, height - 20, "LIVES: " + toString(rrGame.getCurLives()), 100.0, 0.0, 0.0);

	glutSwapBuffers();  	
}

//Zorgt dat verhoudingen behouden blijven
void reshape (int w, int h){
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 1.0, 50.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
	width = w;
	height = h;
}