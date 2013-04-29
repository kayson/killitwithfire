///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI__
#define __GUI__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include "GLViewer.h"
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class BaseFrameMain
///////////////////////////////////////////////////////////////////////////////
class BaseFrameMain : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar2;
		wxMenu* mMenuAddObject;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxStatusBar* m_statusBar2;
		wxScrolledWindow* mPanelSideBar;
		wxPanel* mPanelObjects;
		wxListBox* mObjectList;
		wxButton* m_button14;
		wxButton* m_button15;
		wxButton* mDeleteObjects;
		wxPanel* mPanelFluid;
		wxButton* m_button18;
		wxButton* m_button19;
		wxButton* m_button21;
		wxStaticText* m_staticText16;
		wxTextCtrl* mFluidTime;
		wxStaticText* m_staticText17;
		wxTextCtrl* mFluidIterations;
		wxCheckBox* mFluidRecord;
		wxSlider* mFluidSlider;
		wxButton* mFluidSaveButton;
		wxButton* m_button20;
		wxButton* m_button22;
		wxButton* m_button24;
		wxPanel* mPanelDecimation;
		wxButton* mButtonDecimate;
		wxTextCtrl* m_DecimationTargetTxtBox;
		wxPanel* mPanelSubdivision;
		wxButton* mButtonSubdivide;
		wxPanel* mPanelLevelset;
		wxButton* m_button7;
		wxButton* m_button8;
		wxButton* m_button4;
		wxButton* m_button5;
		wxButton* m_button6;
		wxButton* m_button13;
		wxCheckBox* mReinitializeFastMarching;
		wxStaticText* m_staticText15;
		wxTextCtrl* mPropagationTime;
		wxStaticText* m_staticText13;
		wxTextCtrl* mLevelsetIterations;
		wxStaticText* mLabelNarrowBand;
		wxTextCtrl* mNarrowBandWidth;
		wxButton* mNarrowBandButton;
		wxPanel* mPanelImplicit;
		wxStaticText* m_staticText131;
		wxTextCtrl* mMeshSampling;
		wxButton* m_button16;
		wxButton* mUnion;
		wxButton* mIntersection;
		wxButton* mDifference;
		wxCheckBox* mBlend;
		wxStaticText* mBlendLabel;
		wxTextCtrl* mBlendParameter;
		wxStaticText* m_staticText191;
		wxSlider* mDifferentialScale;
		wxPanel* mPanelVisualization;
		wxCheckBox* mVisualizeWireframe;
		wxCheckBox* mVisualizeMeshNormals;
		wxStaticText* m_staticText9;
		wxSlider* mVisualizeOpacity;
		wxChoice* mColorMapChoice;
		wxStaticText* m_staticText51;
		wxTextCtrl* mMin;
		wxStaticText* m_staticText61;
		wxTextCtrl* mMax;
		wxCheckBox* mAutoMinMax;
		wxChoice* mVisualizationModeChoice;
		wxPanel* mPanelTransform;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxTextCtrl* mScaleX;
		wxTextCtrl* mScaleY;
		wxTextCtrl* mScaleZ;
		wxStaticText* m_staticText6;
		wxTextCtrl* mTranslateX;
		wxTextCtrl* mTranslateY;
		wxTextCtrl* mTranslateZ;
		wxStaticText* m_staticText7;
		wxTextCtrl* mRotateX;
		wxTextCtrl* mRotateY;
		wxTextCtrl* mRotateZ;
		wxCheckBox* mUniformScaling;
		wxButton* mButtonTransform;
		wxButton* mDilate;
		wxButton* mErode;
		wxButton* m_button27;
		wxStaticText* m_staticText19;
		wxTextCtrl* mAmount;
		GLViewer * mGLViewer;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void AddObjectSimpleMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectHalfEdgeMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectSimpleDecimationMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectQuadricDecimationMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectCubicSpline( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectSubdivisionCurve( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectLoopSubdivisionMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectStrangeSubdivisionMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectImplicitSphere( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectImplicitMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectQuadricPlane( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectQuadricCylinder( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectQuadricEllipsoid( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectQuadricCone( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectQuadricParaboloid( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectQuadricHyperboloid( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectScalarCutPlane( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddObjectVectorCutPlane( wxCommandEvent& event ) { event.Skip(); }
		virtual void LoadLevelset( wxCommandEvent& event ) { event.Skip(); }
		virtual void ConvertToLevelset( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddTemplate1( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddTemplate2( wxCommandEvent& event ) { event.Skip(); }
		virtual void AddTemplate3( wxCommandEvent& event ) { event.Skip(); }
		virtual void CaptureScreen( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveMesh( wxCommandEvent& event ) { event.Skip(); }
		virtual void SelectObjects( wxCommandEvent& event ) { event.Skip(); }
		virtual void MoveObjectsUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void MoveObjecsDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void DeleteObjects( wxCommandEvent& event ) { event.Skip(); }
		virtual void FluidSetSolid( wxCommandEvent& event ) { event.Skip(); }
		virtual void FluidSetFluid( wxCommandEvent& event ) { event.Skip(); }
		virtual void FluidSolve( wxCommandEvent& event ) { event.Skip(); }
		virtual void FluidPlayback( wxScrollEvent& event ) { event.Skip(); }
		virtual void SaveSimulationFrames( wxCommandEvent& event ) { event.Skip(); }
		virtual void FluidVisualizeVelocities( wxCommandEvent& event ) { event.Skip(); }
		virtual void FluidVisualizeVoxelsClassification( wxCommandEvent& event ) { event.Skip(); }
		virtual void PlaySimulation( wxCommandEvent& event ) { event.Skip(); }
		virtual void DecimateObjects( wxCommandEvent& event ) { event.Skip(); }
		virtual void SubdivideObjects( wxCommandEvent& event ) { event.Skip(); }
		virtual void LevelsetReinitialize( wxCommandEvent& event ) { event.Skip(); }
		virtual void LevelsetAdvect( wxCommandEvent& event ) { event.Skip(); }
		virtual void LevelsetDilate( wxCommandEvent& event ) { event.Skip(); }
		virtual void LevelsetErode( wxCommandEvent& event ) { event.Skip(); }
		virtual void LevelsetSmooth( wxCommandEvent& event ) { event.Skip(); }
		virtual void LevelsetMorph( wxCommandEvent& event ) { event.Skip(); }
		virtual void EnableNarrowband( wxCommandEvent& event ) { event.Skip(); }
		virtual void ResampleImplicit( wxCommandEvent& event ) { event.Skip(); }
		virtual void Union( wxCommandEvent& event ) { event.Skip(); }
		virtual void Intersection( wxCommandEvent& event ) { event.Skip(); }
		virtual void Difference( wxCommandEvent& event ) { event.Skip(); }
		virtual void SwitchBlending( wxCommandEvent& event ) { event.Skip(); }
		virtual void DifferentialScaleChanged( wxScrollEvent& event ) { event.Skip(); }
		virtual void VisualizeWireframe( wxCommandEvent& event ) { event.Skip(); }
		virtual void VisualizeMeshNormals( wxCommandEvent& event ) { event.Skip(); }
		virtual void OpacityChanged( wxScrollEvent& event ) { event.Skip(); }
		virtual void SetColormap( wxCommandEvent& event ) { event.Skip(); }
		virtual void TextCtrlFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void ScaleChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void ToggleAutoMinMax( wxCommandEvent& event ) { event.Skip(); }
		virtual void SetVisualizationMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void ToggleUniformScaling( wxCommandEvent& event ) { event.Skip(); }
		virtual void TransformObjects( wxCommandEvent& event ) { event.Skip(); }
		virtual void Dilate( wxCommandEvent& event ) { event.Skip(); }
		virtual void Erode( wxCommandEvent& event ) { event.Skip(); }
		virtual void Smooth( wxCommandEvent& event ) { event.Skip(); }


	public:

		BaseFrameMain( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Modeling & Animation"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~BaseFrameMain();

};

#endif //__GUI__
