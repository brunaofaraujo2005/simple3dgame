#include <windows.h>		//Benodigd voor de WIN32 Calls
#include <GL/gl.h>			//Benodigd voor de OpenGL Calls (Header Module)
#include <GL/glu.h>			//Benodigd voor de OpenGL Calls (Utilities)
#include <stdio.h>

#include "RoboRally.h"

RoboRally rrGame;
unsigned int width, height;	//Bevat de hoogte en breedte van het level

//Bouwt de display op
void display(){
    ///* rotate a triangle around */
    glClear(GL_COLOR_BUFFER_BIT);
	glBegin( GL_QUADS );
	
	//Teken het level, selecteer het juiste object in de texturemap
  			int textX=2;
			int textY=1;

	/*  00000
		000..
	*/

	for (int y = 0; y < rrGame.getCurLevel().getHeight(); y++){
		for (int x = 0; x < rrGame.getCurLevel().getWidth(); x++){
			//Texture coords
//1  2   3   4
//5  6   7   8
//9 10 11 12
//
//1=floor
//2=wall
//3=water
//4=exit
//5=battery
//6=chip
//7=lift
//8=hole
//9=north
//10=south
//11=west
//12=east
//START, EXIT, SPACE, HOLE, LIFT, WATER, BATTERY, CHIP, WALL
			switch (rrGame.getCurLevel().getElement(x, y, rrGame.getCurPositionPlatform())){
				case EXIT:
					textX = 3;
					textY = 0;
					break;
				case SPACE:
					textX = 0;
					textY = 0;
					break;
				case HOLE:
					textX = 3;
					textY = 1;
					break;		
				case LIFT:
					textX = 2;
					textY = 1;
					break;
				case WATER:
					textX = 2;
					textY = 0;
					break;
				case BATTERY:
					textX = 0;
					textY = 1;
					break;
				case CHIP:
					textX = 1;
					textY = 1;
					break;
				case WALL:
					textX = 1;
					textY = 0;
					break;
				default:	//Standaard space
					textX = 0;
					textY = 0;
					break;
			}
			if ((rrGame.getCurPositionX() == x) && (rrGame.getCurPositionY() == y)){
					switch (rrGame.getCurOrientation()){
						//NORTH, EAST, SOUTH, WEST}
						case NORTH:
							textX = 0;
							textY = 2;
							break;
						case EAST:
							textX = 3;
							textY = 2;
							break;
						case SOUTH:
							textX = 1;
							textY = 2;
							break;
						case WEST:
							textX = 2;
							textY = 2;
							break;
					}
			}

			glTexCoord2d(textX*(40.0/256.0),textY*(40.0/128.0)); glVertex2d(-1.0+(x*(2.0/width)), +1.0-(y*(2.0/height))  );
			glTexCoord2d((textX+1.0)*(40.0/256.0),textY*(40.0/128.0)); glVertex2d((-1.0+(x*(2.0/width)))+(2.0/width),+1.0-(y*(2.0/height)));
			glTexCoord2d((textX+1.0)*(40.0/256.0),(textY+1)*(40.0/128.0)); glVertex2d((-1.0+(x*(2.0/width))) + (2.0/width),1-(y*(2.0/height))-(2.0/height)); 
			glTexCoord2d(textX*(40.0/256.0),(textY+1)*(40.0/128.0)); glVertex2d(-1.0+(x*(2.0/width)),1-(y*(2.0/height))-(2.0/height));			
		}
	}

	  
	  /*glVertex2d(-1.0,+1.0)
	  glVertex2d(+1.0,+1.0);
	  glVertex2d(+1.0,-1.0);
	  glVertex2d(-1.0,-1.0);*/

      glEnd();
	  glFlush();
}

//Methode die de WIN32 Calls afvangt.
LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ 
    static PAINTSTRUCT ps;

    switch(uMsg) {
		case WM_PAINT:
		display();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

    case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;

    case WM_CHAR:
		switch (wParam) {
			case 27:			/* ESC key */
				PostQuitMessage(0);
				break;
			case 119:	// w
				rrGame.moveRobot(FORWARD);
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
			case 115:	// s
				rrGame.moveRobot(BACKWARD);
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
			case 97:	// a
				rrGame.turnRobotLeft();
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
			case 100:	// d
				rrGame.turnRobotRight();
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
		}
		return 0;

    case WM_KEYDOWN:
		switch (wParam) {
			case VK_UP:
				rrGame.moveRobot(FORWARD);
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
			case VK_DOWN:
				rrGame.moveRobot(BACKWARD);
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
			case VK_RIGHT:
				rrGame.turnRobotRight();
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
			case VK_LEFT:
				rrGame.turnRobotLeft();
				PostMessage(hWnd, WM_PAINT, 0, 0);
				break;
		}
	}
    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

/* Creeërt een WIN32 Window, met daaraan gekoppeld een OPENGL Interface */
HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height, BYTE type, DWORD flags){
    
	int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    
	static HINSTANCE hInstance = 0;

    //Om te voorkomen dat er meerdere instanties worden gemaakt, wordt de hInstance gecontroleerd.
    if (!hInstance) {
		hInstance = GetModuleHandle(NULL);
		wc.style         = CS_OWNDC;
		wc.lpfnWndProc   = (WNDPROC)WindowProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = "OpenGL";

		if (!RegisterClass(&wc)) {
			MessageBox(NULL, "RegisterClass() failed:  "
				   "Cannot register window class.", "Error", MB_OK);
			return NULL;
		}
    }

    hWnd = CreateWindow("OpenGL", "RoboRally", WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
	return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
	MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
	return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	MessageBox(NULL, "SetPixelFormat() failed:  "
		   "Cannot set format specified.", "Error", MB_OK);
	return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hWnd, hDC);

    return hWnd;
}    

// load a 256x128 RGB .RAW file as a texture
GLuint LoadTextureRAW( const char * filename, int wrap )
{
  GLuint texture;
  int width, height;
  BYTE * data;
  FILE * file;

  // open texture data
  file = fopen( filename, "rb" );
  if ( file == NULL ) return 0;

  // allocate buffer
  width = 256;
  height = 128;
  data = (BYTE*)malloc( width * height * 3 );

  // read texture data
  fread( data, width * height * 3, 1, file );
  fclose( file );

  // allocate a texture name
  glGenTextures( 1, &texture );

  // select our current texture
  glBindTexture( GL_TEXTURE_2D, texture );

  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

  // when texture area is small, bilinear filter the closest MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  // if wrap is true, the texture wraps over at the edges (repeat)
  //       ... false, the texture ends at the edges (clamp)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                   wrap ? GL_REPEAT : GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                   wrap ? GL_REPEAT : GL_CLAMP );

  // build our texture MIP maps
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,
    height, GL_RGB, GL_UNSIGNED_BYTE, data );

  // free buffer
  free( data );

  return texture;
}


//Win32 Main methode (i.p.v. main());
int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,LPSTR lpszCmdLine, int nCmdShow){
    HDC hDC;				/* device context */
    HGLRC hRC;				/* opengl context */
    HWND  hWnd;				/* window */
    MSG   msg;				/* message */

	GLuint texture;

	
	width = rrGame.getCurLevel().getWidth();	//Elk object is 40pixels breed.
	height = rrGame.getCurLevel().getHeight();


	//Creeer het window
    hWnd = CreateOpenGLWindow("minimal", 0, 0, (width*40), (height*40), PFD_TYPE_RGBA, 0);
    if (hWnd == NULL)
		exit(1);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

	//Textures laden
	texture = LoadTextureRAW( "texture.raw", TRUE );
	glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture );	//Map de textures


    ShowWindow(hWnd, nCmdShow);

    while(GetMessage(&msg, hWnd, 0, 0)) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);

    return msg.wParam;
}
