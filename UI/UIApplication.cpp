// Michael Taufen 2012
#include <iostream>
#include <fstream>
using namespace std;

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>
#include "GlobalState.h"
#include "TabBarController.h"
#include "EventDispatcher.h"
#include "TextInputView.h"
#include "TitledTextInputView.h"
#include "ToggleButton.h"
#include "TitledToggleButton.h"
#include "SetupViewController.h"
#include "StatusViewController.h"
#include "SearchViewController.h"
/*
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0, windowWidth, 0.0, windowHeight, znear, zfar);

*/

/*

TODO:
moving on to creating special UI elements for each of our views,
and going to add text rendering



*/


bool mouseIsDragging = false;

int WIDTH = 1024;  // width of the user window (640 + 80)
int HEIGHT = 768;  // height of the user window (480 + 60)
char programName[] = "Web Crawler UI Application";


void drawWindow()
{
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_SCISSOR_TEST);
  //glScissor(300,300,500,500);
  // draw stuff
  GlobalState::tabInterfaceController->drawViews();
  glDisable(GL_SCISSOR_TEST);

  // tell the graphics card that we're done-- go ahead and draw!
  //   (technically, we are switching between two color buffers...)
  glutSwapBuffers();
}

// process keyboard events
void keyboard( unsigned char c, int x, int y )
{
  int win = glutGetWindow();
  GlobalState::eventDisp->pushKeyboardEvent(KeyboardEvent(KEY_PRESS, c));
  switch(c) {
    case 27:
      // get rid of the window (as part of shutting down)
      glutDestroyWindow(win);
      exit(0);
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

// the reshape function handles the case where the user changes the size
//   of the window.  We need to fix the coordinate
//   system, so that the drawing area is still the unit square.
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   WIDTH = w;  HEIGHT = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
}

// the mouse function is called when a mouse button is pressed down or released
void mouse(int button, int state, int x, int y)
{
  if ( GLUT_LEFT_BUTTON == button ) {
    if ( GLUT_DOWN == state )
    {
      mouseIsDragging = true;
      // the user just pressed down on the mouse-- do something
      GlobalState::eventDisp->pushMouseEvent(
        MouseEvent(
        MOUSE_DOWN,
        LEFT_MOUSE_BUTTON,
        CGPoint(x, y) ));
    }
    else
    {
      // the user just let go the mouse-- do something
      mouseIsDragging = false;
      GlobalState::eventDisp->pushMouseEvent(
        MouseEvent(
        MOUSE_UP,
        LEFT_MOUSE_BUTTON,
        CGPoint(x, y) ));
      
      
      GlobalState::tabInterfaceController->mouseClickHandler( CGPoint(x, y) );
      GlobalState::eventDisp->pushMouseEvent(
        MouseEvent(
        MOUSE_CLICK,
        LEFT_MOUSE_BUTTON,
        CGPoint(x, y) ));
    }
  } 
  else if ( GLUT_RIGHT_BUTTON == button )
  {
    // right click stuff here
    if (GLUT_DOWN == state)
    {
      GlobalState::eventDisp->pushMouseEvent(
        MouseEvent(
        MOUSE_DOWN,
        RIGHT_MOUSE_BUTTON,
        CGPoint(x, y) ));
    }
    else
    {
      // the user just let go the mouse-- do something
      GlobalState::eventDisp->pushMouseEvent(
        MouseEvent(
        MOUSE_UP,
        RIGHT_MOUSE_BUTTON,
        CGPoint(x, y) ));
      
      GlobalState::eventDisp->pushMouseEvent(
        MouseEvent(
        MOUSE_CLICK,
        RIGHT_MOUSE_BUTTON,
        CGPoint(x, y) ));
    }
  }
  glutPostRedisplay();
}

// the mouse_motion function is called when the mouse is being dragged,
//   and gives the current location of the mouse
void mouse_motion(int x,int y)
{
  // the mouse button is pressed, and the mouse is moving....
  GlobalState::eventDisp->pushMouseEvent(
    MouseEvent(
      MOUSE_DRAG,
      RIGHT_MOUSE_BUTTON,
      CGPoint(x, y) ));
  glutPostRedisplay();
}

void mouse_motion_passive(int x,int y)
{
  // the mouse button is pressed, and the mouse is moving....
  GlobalState::eventDisp->pushMouseEvent(
    MouseEvent(
      MOUSE_OVER,
      RIGHT_MOUSE_BUTTON,
      CGPoint(x, y) ));
  glutPostRedisplay();
}

void idle()
{
  // run the event loop
  GlobalState::eventDisp->eventLoop();
  // see if we need to force a redraw
  if (GlobalState::forceRedraw)
  {
    GlobalState::forceRedraw = false;
    drawWindow();
  }
    
}

// the init function sets up the graphics card to draw properly
void init(void)
{
  // clear the window to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // set up the coordinate system:  number of pixels along x and y
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);

  // allow alpha blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  //glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);

  // welcome message
  cout << "Welcome to " << programName << "." << endl;
}

// init_gl_window is the function that starts the ball rolling, in
//  terms of getting everything set up and passing control over to the
//  glut library for event handling.  It needs to tell the glut library
//  about all the essential functions:  what function to call if the
//  window changes shape, what to do to redraw, handle the keyboard,
//  etc.
void init_gl_window()
{
  char *argv[] = {programName};
  int argc = sizeof(argv) / sizeof(argv[0]);
  glutInit(&argc, argv);
  //glEnable (GL_BLEND);
  //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
  glutInitWindowSize(WIDTH,HEIGHT);
  glutInitWindowPosition(100,100);
  glutCreateWindow(programName);
  init();

  glutDisplayFunc(drawWindow);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  glutPassiveMotionFunc(mouse_motion_passive);
  glutMainLoop();
}

void loadUIComponents()
{
  GlobalState::tabInterfaceController = new TabBarController( CGRect(0,0,WIDTH,HEIGHT) );
  GlobalState::eventDisp = new EventDispatcher();; // woooo, now everything can get to it, deprecate the old way
  GlobalState::winWidth = WIDTH;
  GlobalState::winHeight = HEIGHT;

  SetupViewController * content1 = new SetupViewController(); // TODO: deprecate this constructor
  StatusViewController * content2 = new StatusViewController();
  SearchViewController * content3 = new SearchViewController();
  
  GlobalState::tabInterfaceController->addTab("Setup", content1);
  GlobalState::tabInterfaceController->addTab("Status", content2);
  GlobalState::tabInterfaceController->addTab("Search", content3);


  // Gets the receiving enabled in this tab
  GlobalState::tabInterfaceController->tabSelectedWithTitle("Setup");

  //content3->getMasterView()->setCanRecieveRecursive(true); // a test
}

int main()
{
  loadUIComponents();
  init_gl_window();
}
