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
float nearfar;
bool move=false;
bool sphereup=false;
bool spheredown=false;
bool sphereleft=false;
bool sphereright=false;
float z_index=0.0;
float y_index=0.0;
float x_index=0.0;
int clear=0;
typedef struct{
  int X,Y,Z;
  float U,V;
  
}VERTICES;
VERTICES vert;
typedef struct{
 float x,y,z;
}CIRCLE;

GLuint texture[1];
float sphereang=0;
const int spacing=18;//SPACE OUT VERTICES
const int VertexCount=(90/spacing)*(360/spacing)*4;
VERTICES VERTEX[VertexCount];//
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
GLuint vao,buffers[2],matrixlocation;


//////////////////////////
//        PROTOTYPES
//////////////////////////
void setupWindow();
void shapeView();
void renderLoop();
void drawThis(int);
void checkKeyEvents();
void reshapeView();
void makeSphere(float,float,float,float);
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
 nearfar=-2;
 
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
 makeSphere(30,0,0,0);
 makeCircle();
 glLineWidth(50);
 //

}

void reshapeView(){
 
 
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


 drawThis(2);
 glScalef(.5,.5,.5);
 

 glTranslatef(0,0,-0.001);
 glFlush();
 glXSwapBuffers(dpy,win);

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
    if(keysym==XK_Up){
     z_index+=1;
     sphereup=true;
    }
    if(keysym==XK_Left){
     x_index-=1;
     sphereleft=true;
    }
    if(keysym==XK_Right){
     x_index+=1;
     sphereright=true;
    }
    if(keysym==XK_Down){
     z_index-=1;
     spheredown=true;
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
    glVertex3f( 1.0f+x_index, 1.0f,-1.0f-z_index);
    glVertex3f(-1.0f+x_index, 1.0f,-1.0f-z_index);
    glVertex3f(-1.0f+x_index, 1.0f, 1.0f-z_index);
    glVertex3f( 1.0f+x_index, 1.0f, 1.0f-z_index);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f( 1.0f+x_index,-1.0f, 1.0f-z_index);
    glVertex3f(-1.0f+x_index,-1.0f, 1.0f-z_index);
    glVertex3f(-1.0f+x_index,-1.0f,-1.0f-z_index);
    glVertex3f( 1.0f+x_index,-1.0f,-1.0f-z_index);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f( 1.0f+x_index, 1.0f, 1.0f-z_index);
    glVertex3f(-1.0f+x_index, 1.0f, 1.0f-z_index);
    glVertex3f(-1.0f+x_index,-1.0f, 1.0f-z_index);
    glVertex3f( 1.0f+x_index,-1.0f, 1.0f-z_index);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f( 1.0f+x_index,-1.0f,-1.0f-z_index);
    glVertex3f(-1.0f+x_index,-1.0f,-1.0f-z_index);
    glVertex3f(-1.0f+x_index, 1.0f,-1.0f-z_index);
    glVertex3f( 1.0f+x_index, 1.0f,-1.0f-z_index);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f(-1.0f+x_index, 1.0f, 1.0f-z_index);
    glVertex3f(-1.0f+x_index, 1.0f,-1.0f-z_index);
    glVertex3f(-1.0f+x_index,-1.0f,-1.0f-z_index);
    glVertex3f(-1.0f+x_index,-1.0f, 1.0f-z_index);

    glColor3f(1.0f,0.0f,1.0f);
    glVertex3f( 1.0f+x_index, 1.0f,-1.0f-z_index);
    glVertex3f( 1.0f+x_index, 1.0f, 1.0f-z_index);
    glVertex3f( 1.0f+x_index,-1.0f, 1.0f-z_index);
    glVertex3f( 1.0f+x_index,-1.0f,-1.0f-z_index);
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
   //float R=5;

   
   
   glBegin(GL_TRIANGLE_STRIP);
    for(int b=0; b<=VertexCount;++b){
     glColor3f(1,0,0);
     glVertex3f(VERTEX[b].X,VERTEX[b].Y,-VERTEX[b].Z);
    }
    for(int b=0; b<=VertexCount;++b){
     glColor3f(1,0,0);
     glVertex3f(VERTEX[b].X,VERTEX[b].Y,VERTEX[b].Z);
    }
   glEnd();
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
      glVertex3f(circ[a][b].x+x_index,circ[a][b].y,circ[a][b].z-z_index);
      glColor3f(0,1,0);
      glVertex3f(circ[a][b+1].x+x_index,circ[a][b+1].y,circ[a][b+1].z-z_index);
      glColor3f(0,0,1);
      glVertex3f(circ[a+1][b+1].x+x_index,circ[anext][b+1].y,circ[a+1][b+1].z-z_index);
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
void makeSphere(float R,float H, float K, float Z){
 int n;//the current vertex being worked on
 float a,b;//loop counters
 n=0;//start with first vertex
 for(b=0;b<=90-spacing;b+=spacing){
  for(a=0;a<=360-spacing;a+=spacing){
   VERTEX[n].X=R*sin((a)/180*M_PI)*sin((b)/180*M_PI)-H;
   VERTEX[n].Y=R*cos((a)/180*M_PI)*cos((b)/180*M_PI)+K;
   VERTEX[n].Z=R*cos((b)/180*M_PI)-Z;

   VERTEX[n].V=(2*b)/360;
   VERTEX[n].U=(a)/360;
   ++n;

   VERTEX[n].X=R*sin((a)/180*M_PI)*sin((b+spacing)/180*M_PI)-H;
   VERTEX[n].Y=R*cos((a)/180*M_PI)*cos((b+spacing)/180*M_PI)+K;
   VERTEX[n].Z=R*cos((b+spacing)/180*M_PI)-Z;

   VERTEX[n].V=(2*(b+spacing))/360;
   VERTEX[n].U=(a)/360;
   ++n;

   VERTEX[n].X=R*sin((a+spacing)/180*M_PI)*sin((b)/180*M_PI)-H;
   VERTEX[n].Y=R*cos((a+spacing)/180*M_PI)*cos((b)/180*M_PI)+K;
   VERTEX[n].Z=R*cos((b)/180*M_PI)-Z;

   VERTEX[n].V=(2*b)/360;
   VERTEX[n].U=(a+spacing)/360;
   ++n;
  }
 }
}