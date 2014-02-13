#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <cmath>
using namespace std;
//Global variables
Display *dpy;
Window win;
XVisualInfo *vi;
Colormap cmap;
GLXContext glc;
XSetWindowAttributes swa;
XWindowAttributes gwa;
XEvent xev;
float angle1,angle2;

float z_index=0.0;
float y_index=0.0;
float x_index=0.0;
float z2_index=1.0;
float y2_index=1.0;
float x2_index=1.0;
float definition=.25;
typedef struct{
 float x,y,z;
}CIRCLE;

GLuint texture[1];

const int spacing=72;//SPACE OUT VERTICES

CIRCLE circ[360/spacing][360/spacing];
static int GLAttr[]={
 GLX_RGBA,
 GLX_RED_SIZE,6,
 GLX_GREEN_SIZE, 6,                                                 
 GLX_BLUE_SIZE, 6,
 GLX_DEPTH_SIZE,24,
 GLX_DOUBLEBUFFER,
 None
};
float angle=1;

//////////////////////////
//        PROTOTYPES
//////////////////////////
void setupWindow();
void shapeView();
void renderLoop();
void drawThis(int);
void checkKeyEvents();
void reshapeView();
void makeSphere();
void makeCircle();

//////////////////////////
//        MAIN FUNCTION
//////////////////////////
int main(){
 setupWindow();
 shapeView();
 renderLoop();//calls shapeView
}

//////////////////////////
//        SETUP GLX WINDOW
//////////////////////////
void setupWindow(){
 dpy=XOpenDisplay(0);
 vi=glXChooseVisual(dpy,DefaultScreen(dpy),GLAttr);
 cmap=XCreateColormap(dpy,RootWindow(dpy,vi->screen),vi->visual,AllocNone);
 glc=glXCreateContext(dpy,vi,NULL,GL_TRUE);

 swa.colormap=cmap;
 swa.border_pixel=0;
 swa.event_mask=ExposureMask|ButtonPressMask|StructureNotifyMask|KeyPressMask;
 win=XCreateWindow(dpy,RootWindow(dpy,vi->screen),
  0,0,600,600,0,
  vi->depth,
  InputOutput,
  vi->visual,
  CWBorderPixel|CWColormap|CWEventMask,
  &swa);
 XSetStandardProperties(dpy,win,
  "SPHERE ASSIGNMENT",
  "SPHERE ASSIGNMENT",
  None,
  None,
  None,
  NULL);
 glXMakeCurrent(dpy,win,glc);
 XMapWindow(dpy,win);
 
}


///////////////////////////////////
//        SHAPE VIEW && RESHAPE VIEW
//////////////////////////////////////
void shapeView(){//setup initial camera view

 
 glEnable(GL_DEPTH_TEST);
 glCullFace(GL_BACK);//

 XGetWindowAttributes(dpy,win,&gwa);
 glViewport(0,0,gwa.width,gwa.height);

 glClearColor(0,0,0,1);//cyan clear color
 glClearDepth(100.0);

 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(30,1,0.001,100.0);


 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(0,0,-10);


 makeSphere();
 makeCircle();
 glLineWidth(50);
 //

}

void reshapeView(){
 
 
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

 glPushMatrix();
 glTranslatef(x_index,0,z_index);
 glScalef(.5,.5,.5); 
 glRotatef(angle,0,1,0);
 drawThis(2);
 glPopMatrix();

 glPushMatrix();
 glTranslatef(x2_index,0,z2_index);
 //glScalef(.5,.5,.5); 
 glRotatef(-angle,1,0,0);
 drawThis(2);
 glPopMatrix();

 glXSwapBuffers(dpy,win);
 angle++;
}

//////////////////////////
//        RENDER LOOP
//////////////////////////
void renderLoop(){

 while(1){
  if(XPending(dpy)){
   XNextEvent(dpy,&xev);
   if(xev.type==Expose){
    shapeView();
   }//end Expose
   if(xev.type==KeyPress){
    char buffer[31];
    KeySym keysym;
    XLookupString(&xev.xkey,buffer,30,&keysym,NULL);
    if(keysym==XK_Escape){
     glXMakeCurrent(dpy,None,NULL);
     glXDestroyContext(dpy,glc);
     XDestroyWindow(dpy,win);
     XCloseDisplay(dpy);
     exit(0);
    }
    switch(keysym){
     case XK_Up:
      z_index-=1;
      break;
     case XK_Down:
      z_index+=1;
      break;
     case XK_Left:
      x_index-=1;
      break;
     case XK_Right:
      x_index+=1;
      break;

     case XK_w:
      z2_index-=1;
      break;
     case XK_s:
      z2_index+=1;
      break;
     case XK_a:
      x2_index-=1;
      break;
     case XK_d:
      x2_index+=1;
      break;


    }

    
   }//end KeyPress
  }//end XPEnding
  reshapeView();
 }//end while(1)
}

//////////////////////////
//        HANDLE KEYBOARD
//////////////////////////
void checkKeyEvents(){
 
}

//////////////////////////
//        DRAWING FUNCTION
////////////////////////// 
void drawThis(int which){
 switch(which){
  case 0:
   glBegin(GL_QUADS);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex3f( 1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f( 1.0f, 1.0f,-1.0f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);

    glColor3f(1.0f,0.0f,1.0f);
    glVertex3f( 1.0f, 1.0f,-1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);
   glEnd();
   break;
  case 1:

   glBegin(GL_QUADS);
    glColor3f(.7, 0., 0.); glVertex3f(-.75, -.75, 0.);
    glColor3f(.7, 0., 0.); glVertex3f( .75, -.75, 0.);
    glColor3f(.7, 0., 0.); glVertex3f( .75,  .75, 0.);
    glColor3f(.7, 0., 0.); glVertex3f(-.75,  .75, 0.);
   glEnd();
   break;
  case 2:
   
   for(angle2=-M_PI/2;angle2<M_PI/2;angle2+=definition){
    glBegin(GL_QUAD_STRIP); 
     for(angle1=0.0;angle1<=M_PI*2+definition;angle1+=definition){
      glColor3f(cos(angle2)*cos(angle1),sin(angle2),cos(angle2)*sin(angle1));
      glVertex3f(cos(angle2)*cos(angle1),sin(angle2),cos(angle2)*sin(angle1));
      glVertex3f(cos(angle2+definition)*cos(angle1),sin(angle2+definition),cos(angle2+definition)*sin(angle1));
     }    
    glEnd();
   }
   
   
   
   break;
  case 3:
   for(int a=0;a<360/spacing;++a){
    int anext=a+1;
    if(a==359){
     anext=0;
    }
    glBegin(GL_TRIANGLE_STRIP);
     
     for(int b=0;b<360/spacing;++b){
      glColor3f(1,0,0);
      glVertex3f(circ[a][b].x,circ[a][b].y,circ[a][b].z);
      glColor3f(0,1,0);
      glVertex3f(circ[a][b+1].x,circ[a][b+1].y,circ[a][b+1].z);
      glColor3f(0,0,1);
      glVertex3f(circ[a+1][b+1].x,circ[anext][b+1].y,circ[a+1][b+1].z);
     }
    glEnd();
   }    
   break;
   case 4:

    

   break;
 }//end switch statement
}
//////////////////////////
//        CIRCLE GEOMETRY
//////////////////////////
void makeCircle(){
 float circleradius=2;
 for(int a=0;a<360/spacing;++a){
  for(int b=0;b<360/spacing;++b){
   circ[a][b].x=circleradius*-cos(b*M_PI*spacing/180)*sin(a);
   circ[a][b].y=circleradius*sin(b*M_PI*spacing/180);
   circ[a][b].z=circleradius*cos(b*M_PI*spacing/180)*cos(a);
  }
 }
}
//////////////////////////
//        SPHERE GEOMETRY
//////////////////////////
//(number of subdivisions,xpos,ypos,zpos)
void makeSphere(){
 
}