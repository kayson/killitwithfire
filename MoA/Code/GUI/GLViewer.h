
#ifndef _GL_VIEWER_H
#define _GL_VIEWER_H

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "GLCamera.h"
#include "GLObject.h"
#include <list>
#include <string>

extern const wxEventType wxEVT_GL_OBJECT_SELECTED;

class GLViewer : public wxGLCanvas
{

public:
  GLViewer(wxWindow* parent, int* args);

  void Resized(wxSizeEvent& evt);

  int GetWidth();
  int GetHeight();

  void Render(bool swapBuffers = true);

  void ScreenCapture(const std::string & filename, float magnification = 1);

  bool AddObject(GLObject * object);
  GLObject * RemoveObject(const std::string & name);
  GLObject * RemoveObject(unsigned int i);
  void MoveObject(const std::string & name, int shift);
  std::list<GLObject *> GetObjects();
  GLObject * GetObject(const std::string & name);
  GLObject * GetObject(unsigned int i);
  void ReplaceObject(GLObject * oldObject, GLObject * newObject);

	static GLCamera & GetCamera() { return mCamera; }

  void DeselectAllObjects();
  void DeselectObject(const std::string & name);
  void SelectObject(const std::string & name);
  std::list<GLObject *> GetSelectedObjects();

  void SetLighting(bool lighting);

protected :

  static GLCamera mCamera;
  std::list<GLObject *> mObjects;

  long mMouseDown[2];
  long mMousePrevious[2];
  bool mLighting;

  GLuint mSelectBuffer[512];
  GLObject * PickObject(long x, long y);

  void InitGL();
  void SetupView();
  bool mIsInitialized;

  // events
  void MouseMoved(wxMouseEvent& event);
  void MouseDown(wxMouseEvent& event);
  void MouseWheelMoved(wxMouseEvent& event);
  void MouseReleased(wxMouseEvent& event);
  void RightClick(wxMouseEvent& event);
  void MouseLeftWindow(wxMouseEvent& event);
  void OnPaint(wxPaintEvent& event);
  void OnEraseBackground(wxEraseEvent& event) { }
  void KeyPressed(wxKeyEvent& event);


  DECLARE_EVENT_TABLE()
};

#endif
