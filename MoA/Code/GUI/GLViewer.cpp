

#include "GLViewer.h"
#include <iostream>
#include <algorithm>
#include <limits>

#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include "GLUT/glut.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif

#include "Math/Vector3.h"
#include "Util/Image.h"

#include <GL/freeglut.h> // Takes care of everything GL-related

const wxEventType wxEVT_GL_OBJECT_SELECTED = wxNewEventType();

GLCamera GLViewer::mCamera;

BEGIN_EVENT_TABLE(GLViewer, wxGLCanvas)
EVT_MOTION(GLViewer::MouseMoved)
EVT_LEFT_DOWN(GLViewer::MouseDown)
EVT_LEFT_UP(GLViewer::MouseReleased)
EVT_RIGHT_DOWN(GLViewer::MouseDown)
EVT_LEAVE_WINDOW(GLViewer::MouseLeftWindow)
EVT_SIZE(GLViewer::Resized)
EVT_MOUSEWHEEL(GLViewer::MouseWheelMoved)
EVT_PAINT(GLViewer::OnPaint)
EVT_ERASE_BACKGROUND(GLViewer::OnEraseBackground)
EVT_KEY_DOWN(GLViewer::KeyPressed)
END_EVENT_TABLE()

bool CompareOpacity(const GLObject * obj1, const GLObject * obj2)
{
  return obj1->GetOpacity() > obj2->GetOpacity();
}


GLViewer::GLViewer(wxWindow* parent, int* args) :
  wxGLCanvas(parent, wxID_ANY,  wxDefaultPosition, wxDefaultSize, 0, wxT("GLCanvas"),  args), mIsInitialized(false)
{
	glutInit(args, NULL);
  mCamera.MoveForward(-3.0);
}

void GLViewer::InitGL()
{

  std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
  std::cout << "OpenGL vendor " << glGetString(GL_VENDOR) << std::endl;
  SetupView();

  // Setup OpenGL environment
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClearDepth(1.0f);

  GLfloat matSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat matShininess[] = { 50.0 };
  GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat whiteLight[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lightModelAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
  glShadeModel(GL_SMOOTH);

  //glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
  //glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  //glEnable(GL_LINE_SMOOTH);
  //glEnable(GL_POLYGON_SMOOTH);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);

  glEnable(GL_NORMALIZE);

  glSelectBuffer(512, mSelectBuffer);
}

void GLViewer::SetupView()
{
  // Setup project matrix and viewport
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat ratio = (GLfloat)GetWidth()/(GLfloat)GetHeight();
  gluPerspective(45.0, ratio, 0.1, 200);
  glViewport(0, 0, GetWidth(), GetHeight());
}

void GLViewer::Resized(wxSizeEvent& evt)
{
  wxGLCanvas::OnSize(evt);

  if (!IsShownOnScreen()) return;
  SetCurrent();
  SetupView();
}

int GLViewer::GetWidth()
{
  return GetSize().x;
}

int GLViewer::GetHeight()
{
  return GetSize().y;
}

void GLViewer::OnPaint(wxPaintEvent& event)
{
  Render();
}

void GLViewer::Render(bool swapBuffers)
{
  if (!IsShownOnScreen()) return;

  wxClientDC DC(this);
  SetCurrent();

  if (!mIsInitialized) {
    InitGL();
    mIsInitialized = true;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  mCamera.Render();

  // Sort the objects to get descent opacity rendering
  mObjects.sort(CompareOpacity);
  std::list<GLObject *>::iterator iter = mObjects.begin();
  std::list<GLObject *>::iterator iend = mObjects.end();
  GLuint ID = 0;
  while (iter != iend) {
    if ((*iter)->GetOpacity() > 0) {
      glPushName(ID++);
      (*iter)->Render();
      glPopName();
    }

    ++iter;
  }

  glFlush();
  if (swapBuffers) SwapBuffers();
}


void GLViewer::ScreenCapture(const std::string & filename, float magnification)
{
  if (!IsShownOnScreen()) return;
  SetCurrent();

  int width = GetWidth();
  int height = GetHeight();

  SetSize(width*magnification, height*magnification);
  SetupView();
  Render(false);

  glReadBuffer(GL_BACK);
  Image<GLfloat> img;
  img.CaptureScreen();
  img.SaveToFile(filename);

  SetSize(width, height);
  SetupView();
}


bool GLViewer::AddObject(GLObject * object)
{
  std::list<GLObject *>::iterator iter = mObjects.begin();
  std::list<GLObject *>::iterator iend = mObjects.end();
  while (iter != iend) {
    if ((*iter)->GetName() == object->GetName()) {
      std::cerr << "Error: GLObject '" << object->GetName() << " already added to scene" << std::endl;
      return false;
    }
    ++iter;
  }

  mObjects.push_back(object);
  std::cerr << "Object '" << object->GetName() << "' added to scene" << std::endl;

  return true;
}

GLObject * GLViewer::GetObject(const std::string & name)
{
  std::list<GLObject *>::iterator iter = mObjects.begin();
  std::list<GLObject *>::iterator iend = mObjects.end();
  while (iter != iend) {
    if ((*iter)->GetName() == name)
      return (*iter);
    ++iter;
  }

  return NULL;
}

GLObject * GLViewer::GetObject(unsigned int i)
{
  if (i >= mObjects.size())  return NULL;

  std::list<GLObject *>::iterator iter = mObjects.begin();
  for (unsigned int j = 0; j < i; j++)
    ++iter;

  return (*iter);
}

std::list<GLObject *> GLViewer::GetObjects()
{
  return mObjects;
}

GLObject * GLViewer::RemoveObject(const std::string & name)
{
  std::list<GLObject *>::iterator iter = mObjects.begin();
  std::list<GLObject *>::iterator iend = mObjects.end();
  while (iter != iend) {
    GLObject * object = *iter;
    if (object->GetName() == name) {
      mObjects.erase(iter);
      return object;
    }
    ++iter;
  }

  return NULL;
}

GLObject * GLViewer::RemoveObject(unsigned int i)
{
  if (i >= mObjects.size())  return NULL;

  std::list<GLObject *>::iterator iter = mObjects.begin();
  for (unsigned int j = 0; j < i; j++)
    ++iter;

  GLObject * object = *iter;
  mObjects.erase(iter);
  return object;
}

void GLViewer::MoveObject(const std::string & name, int shift)
{
  std::list<GLObject *>::iterator iter = mObjects.begin();
  std::list<GLObject *>::iterator iend = mObjects.end();
  while (iter != iend) {
    GLObject * object = *iter;
    if (object->GetName() == name) {
      std::list<GLObject *>::iterator iter2 = iter;
      while (iter2 != iend && shift > 0) {
        iter2++;
        shift--;
      }
      while (iter2 != mObjects.begin() && shift < 0) {
        iter2--;
        shift++;
      }

      if (iter2 != iend)
        std::swap(*iter, *iter2);

      return;
    }
    ++iter;
  }
}

void GLViewer::ReplaceObject(GLObject * oldObject, GLObject * newObject)
{
  std::list<GLObject *>::iterator iter = std::find(mObjects.begin(), mObjects.end(), oldObject);
  if (iter == mObjects.end()) return;

  mObjects.insert(iter, newObject);
  mObjects.erase(iter);
}

std::list<GLObject *> GLViewer::GetSelectedObjects()
{
  std::list<GLObject *> selected;

  std::list<GLObject *>::iterator iter = mObjects.begin();
  std::list<GLObject *>::iterator iend = mObjects.end();
  while (iter != iend) {
    if ((*iter)->IsSelected()) {
      selected.push_back(*iter);
    }
    iter++;
  }

  return selected;
}

void GLViewer::DeselectAllObjects()
{
  std::list<GLObject *>::iterator iter = mObjects.begin();
  std::list<GLObject *>::iterator iend = mObjects.end();
  while (iter != iend) {
    (*iter)->DeSelect();
    ++iter;
  }
}

void GLViewer::DeselectObject(const std::string & name)
{
  std::cerr << "Deselecting '" << name << "'..." << std::endl;
  GLObject * object = GetObject(name);
  if (object == NULL)  return;

  object->DeSelect();
}

void GLViewer::SelectObject(const std::string & name)
{
  std::cerr << "Selecting '" << name << "'..." << std::endl;
  GLObject * object = GetObject(name);
  if (object == NULL)  return;

  object->Select();

  wxCommandEvent event(wxEVT_GL_OBJECT_SELECTED);
  event.SetClientData(object);
  wxPostEvent(GetParent(), event);
}


GLObject * GLViewer::PickObject(long x, long y)
{
  if (!IsShownOnScreen()) return NULL;
  SetCurrent();

  // Enter select mode and initalize select buffer
  glRenderMode(GL_SELECT);

  if( GL_INVALID_OPERATION == glGetError()) {
    glRenderMode(GL_RENDER);
    return NULL;
  }

  glInitNames();

  // Get viewport and projection matrix
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  GLfloat projection[16];
  glGetFloatv(GL_PROJECTION_MATRIX, projection);

  // Switch to projection matrix stack
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  // Focus on the region around the cursor
  gluPickMatrix(x, GetHeight()-y, 4.0, 4.0, viewport);    //x, y, width, height is the picking area

  // Load the projection matrix
  glMultMatrixf(projection);

  // Render the scene
  glMatrixMode(GL_MODELVIEW);
  Render(false);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  // Read the hits
  GLint hits = glRenderMode(GL_RENDER);

  //std::cerr << "Number of hits: " << hits << std::endl;

  GLuint minDepth = (std::numeric_limits<GLuint>::max)();
  int i = 0;
  int pickedObject = -1;
  for (GLuint hit = 0; hit < (GLuint) hits; hit++) {
    int stackSize = mSelectBuffer[i++];         // stack size
    GLuint depth = mSelectBuffer[i++];          // minimum depth
    i++;                                                                        // ignore maximum depth

    // Check if object is closer than the current one
    if (minDepth > depth) {
      minDepth = depth;
      pickedObject = i;
    }
    i += stackSize; // proceed to next hit
  }

  if (pickedObject == -1)  return NULL;

  GLObject * object = GetObject(mSelectBuffer[pickedObject]);
  object->PickChildren(mSelectBuffer+pickedObject+1, mSelectBuffer[pickedObject-3] - 1);
  return object;
}

void GLViewer::MouseMoved(wxMouseEvent& event) {
  SetFocus();
  long x = event.GetX();
  long y = event.GetY();

  float diffY = (float)y - (float)mMousePrevious[1];
  float diffX = (float)x - (float)mMousePrevious[0];
  //std::cout << diffX << " " << diffY << std::endl;

  mCamera.SetCameraRotation(event.ShiftDown());

  if (event.LeftIsDown()) {
    if(event.AltDown()){
      mCamera.MoveForward(diffY*.01);

      mMousePrevious[0] = x;
      mMousePrevious[1] = y;
    } else {

      Vector3<GLfloat> right = mCamera.GetRightVector();
      Vector3<GLfloat> forward = mCamera.GetLookAtVector();
      Vector3<GLfloat> up = mCamera.GetUpVector();

      float halfWidth = GetWidth()/2;
      float ratioX = (x-halfWidth) / halfWidth;
      float blendX = std::abs(ratioX);

      float halfHeight = GetHeight()/2;
      float ratioY = (y-halfHeight) / halfHeight;
      float blendY = std::abs(ratioY);

      mCamera.RotateX(diffY*(1.0-blendX));
      mCamera.RotateZ(-diffY*ratioX);

      mCamera.RotateY(diffX*(1.0-blendY));
      mCamera.RotateZ(diffX*ratioY);

      mCamera.QuatRotate((mMousePrevious[0] / halfWidth) - 1, 1 - (mMousePrevious[1]/halfHeight),
                         (x/halfWidth) - 1, 1 - (y/halfHeight));

      mMousePrevious[0] = x;
      mMousePrevious[1] = y;
      //std::cout << "rot" << std::endl;
    }
  }
  else if (event.RightIsDown()){

    mCamera.MoveRight( -diffX*0.01);
    mCamera.MoveUpward( diffY*0.01);

    mMousePrevious[0] = x;
    mMousePrevious[1] = y;
  }
  else{

    std::list<GLObject *>::iterator iter = mObjects.begin();
    std::list<GLObject *>::iterator iend = mObjects.end();
    while (iter != iend) {
      (*iter)->UnHover();
      ++iter;
    }

    GLObject * object = PickObject(x,y);
    if (object != NULL)
      object->Hover();
  }

  Render();
}

void GLViewer::MouseDown(wxMouseEvent& event) {
  mMousePrevious[0] = mMouseDown[0] = event.GetX();
  mMousePrevious[1] = mMouseDown[1] = event.GetY();
}

void GLViewer::MouseWheelMoved(wxMouseEvent& event) {
  int scroll = event.GetWheelRotation();
  mCamera.MoveForward((float)scroll*0.001);
  Render();
}

void GLViewer::MouseReleased(wxMouseEvent& event)
{
  if (event.GetX() == mMouseDown[0] && event.GetY() == mMouseDown[1]) {
    std::cerr << "Pick at (" <<  event.GetX() << "," <<  event.GetY() << ")" << std::endl;

    if (!event.ShiftDown()) {
      std::list<GLObject *>::iterator iter = mObjects.begin();
      std::list<GLObject *>::iterator iend = mObjects.end();
      while (iter != iend) {
        (*iter)->DeSelect();
        ++iter;
      }
    }

    GLObject * object = PickObject( event.GetX(), event.GetY());
    if (object != NULL) {
      object->Select();
      std::cout << "Selecting object '" << object->GetName() << "'" << std::endl;
    }

    wxCommandEvent event(wxEVT_GL_OBJECT_SELECTED);
    event.SetClientData(object);
    wxPostEvent(GetParent(), event);

    Render();
  }
}

void GLViewer::RightClick(wxMouseEvent& event) {
}

void GLViewer::MouseLeftWindow(wxMouseEvent& event) {}


//Key capture function. Just add functionallity in the switch
void GLViewer::KeyPressed(wxKeyEvent& event) {

  switch(event.GetKeyCode()) {
  case WXK_SPACE:
    mCamera.Reset();
    mCamera.MoveForward(-3);
    break;
  case 87: //W
    mCamera.MoveForward(0.05);
    break;
  case 83: //S
    mCamera.MoveForward(-0.05);
    break;
  case 65: //A
    mCamera.MoveRight(-0.05);
    break;
  case 68: //D
    mCamera.MoveRight(0.05);
    break;
  case 81: //Q
    mCamera.MoveUpward(0.05);
    break;
  case 69: //E
    mCamera.MoveUpward(-0.05);
    break;
  }
  if(event.GetKeyCode() != WXK_SHIFT)
    Render();
}
