#pragma once

#ifndef _GL_OBJECT
#define _GL_OBJECT

#ifdef __WXMAC__
#include "GLUT/glut.h"
#else
#include <GL/glut.h>
#endif

#include "Util/BlackWhiteColorMap.h"

#include <string>
#include <cstring>
#include <list>
#include <typeinfo>

class GLObject
{
public:

  class VisualizationMode {
    friend class GLObject;

  public :
    inline unsigned int GetID() const { return mID; }
    inline const std::string & GetName() const { return mName; }

    bool operator==(const VisualizationMode & mode) {
      return mID == mode.mID;
    }

  protected :
    VisualizationMode(unsigned int ID=0, const std::string & name="<not set>")
      : mID(ID), mName(name) { }

    unsigned int mID;
    std::string mName;
  };


  GLObject() : mName("no-name") {
    mHover = mSelected = false;
    mWireframe = false;
    mShowNormals = false;
    mOpacity = 1;
    mAutoMinMax = true;

    std::list<std::string> colormaps = ColorMapFactory::GetColorMaps();
    if (colormaps.size() > 0)
      mColorMap = ColorMapFactory::New(colormaps.front());
    else
      mColorMap = NULL;
  }

  GLObject(const std::string & name) : mName(name) {
    mHover = mSelected = false;
    mWireframe = false;
    mShowNormals = false;
    mOpacity = 1;
    mAutoMinMax = true;

    std::list<std::string> colormaps = ColorMapFactory::GetColorMaps();
    if (colormaps.size() > 0)
      mColorMap = ColorMapFactory::New(colormaps.front());
    else
      mColorMap = NULL;
  }

  virtual ~GLObject(){ }

  virtual void Render();

  void SetName(const std::string & name) { mName = name; }
  const std::string & GetName() { return mName; }
  virtual const char * GetTypeName() = 0;

  virtual void SetColorMap(ColorMap * colormap) { mColorMap = colormap; }
  virtual ColorMap * GetColorMap() { return mColorMap; }

  virtual void SetVisualizationMode(const VisualizationMode & source) { mVisualizationMode = source; }
  virtual const VisualizationMode & GetVisualizationMode() const { return mVisualizationMode; }

  virtual void SetWireframe(bool enable=true) { mWireframe = enable; }
  virtual bool GetWireframe() { return mWireframe; }

  virtual void SetShowNormals(bool enable=true) { mShowNormals = enable; }
  virtual bool GetShowNormals() { return mShowNormals; }

  virtual void SetOpacity(float opacity) { mOpacity = opacity; }
  inline float GetOpacity() const { return mOpacity; }

  virtual std::list<VisualizationMode> GetVisualizationModes() { return std::list<VisualizationMode>(); }

  void Hover() { mHover = true; }
  void UnHover() { mHover = false; }
  void ToggleHover() { mHover = !mHover; }
  bool IsHovering() { return mHover; }

  void Select() { mSelected = true; }
  void DeSelect() { mSelected = false; }
  void ToggleSelect() { mSelected = !mSelected; }
  bool IsSelected() { return mSelected; }

  virtual void PickChildren(GLuint * objects, int numberOfObjects) { }

  float mMinCMap, mMaxCMap;
  bool mAutoMinMax;

protected:

  std::string mName;
  bool mHover, mSelected;

  ColorMap * mColorMap;

  VisualizationMode mVisualizationMode;

  float mOpacity;

  bool mWireframe;

  bool mShowNormals;

  void CheckGLError();

  void RenderString(float x, float y, float z, char *string) {
    int len, i;
    glRasterPos3f(x, y, z);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
  }

  static VisualizationMode NewVisualizationMode(const std::string & name) {
    static unsigned int mVisualizationModeCounter = 0;
    mVisualizationModeCounter++;
    return VisualizationMode(mVisualizationModeCounter, name);
  }

};

#endif
