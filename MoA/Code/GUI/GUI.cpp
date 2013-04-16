///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

BaseFrameMain::BaseFrameMain( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar2 = new wxMenuBar( 0 );
	mMenuAddObject = new wxMenu();
	wxMenuItem* mAddObjectSimpleMesh;
	mAddObjectSimpleMesh = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Simple mesh") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectSimpleMesh );

	wxMenuItem* mAddObjectHalfEdgeMesh;
	mAddObjectHalfEdgeMesh = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Half-edge mesh") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectHalfEdgeMesh );

	wxMenuItem* mAddObjectSimpleDecimationMesh;
	mAddObjectSimpleDecimationMesh = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Simple decimation mesh") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectSimpleDecimationMesh );

	wxMenuItem* mAddObjectQuadricDecimationMesh;
	mAddObjectQuadricDecimationMesh = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Quadric decimation mesh") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectQuadricDecimationMesh );

	wxMenuItem* m_separator1;
	m_separator1 = mMenuAddObject->AppendSeparator();

	wxMenuItem* mAddObjectCubicSpline;
	mAddObjectCubicSpline = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Cubic spline") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectCubicSpline );

	wxMenuItem* mAddObjectSubdivisionCurve;
	mAddObjectSubdivisionCurve = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Subdivision curve") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectSubdivisionCurve );

	wxMenuItem* mAddObjectLoopSubdivisionMesh;
	mAddObjectLoopSubdivisionMesh = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Loop subdivision mesh") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectLoopSubdivisionMesh );

	wxMenuItem* mAddObjectStrangeSubdivisionMesh;
	mAddObjectStrangeSubdivisionMesh = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Strange subdivision mesh") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectStrangeSubdivisionMesh );

	wxMenuItem* m_separator2;
	m_separator2 = mMenuAddObject->AppendSeparator();

	wxMenuItem* mAddObjectImplicitSphere;
	mAddObjectImplicitSphere = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Implicit sphere") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectImplicitSphere );

	wxMenuItem* mAddObjectImplicitMesh;
	mAddObjectImplicitMesh = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Implicit mesh") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectImplicitMesh );

	m_menu1 = new wxMenu();
	wxMenuItem* mAddObjectQuadricPlane;
	mAddObjectQuadricPlane = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Plane") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( mAddObjectQuadricPlane );

	wxMenuItem* mAddObjectQuadricCylinder;
	mAddObjectQuadricCylinder = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Cylinder") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( mAddObjectQuadricCylinder );

	wxMenuItem* mAddObjectQuadricEllipsoid;
	mAddObjectQuadricEllipsoid = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Ellipsoid") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( mAddObjectQuadricEllipsoid );

	wxMenuItem* mAddObjectQuadricCone;
	mAddObjectQuadricCone = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Cone") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( mAddObjectQuadricCone );

	wxMenuItem* mAddObjectQuadricParaboloid;
	mAddObjectQuadricParaboloid = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Paraboloid") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( mAddObjectQuadricParaboloid );

	wxMenuItem* mAddObjectQuadricHyperboloid;
	mAddObjectQuadricHyperboloid = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("Hyperboloid") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( mAddObjectQuadricHyperboloid );

	mMenuAddObject->Append( -1, wxT("Implicit quadric"), m_menu1 );

	wxMenuItem* m_separator3;
	m_separator3 = mMenuAddObject->AppendSeparator();

	wxMenuItem* mAddObjectScalarCutPlane;
	mAddObjectScalarCutPlane = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Scalar cut plane") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectScalarCutPlane );

	wxMenuItem* mAddObjectVectorCutPlane;
	mAddObjectVectorCutPlane = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Vector cut plane") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( mAddObjectVectorCutPlane );

	wxMenuItem* m_separator4;
	m_separator4 = mMenuAddObject->AppendSeparator();

	wxMenuItem* m_menuItem21;
	m_menuItem21 = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Load levelset") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( m_menuItem21 );

	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Convert implicit to levelset") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( m_menuItem8 );

	wxMenuItem* m_separator5;
	m_separator5 = mMenuAddObject->AppendSeparator();

	wxMenuItem* m_menuItem22;
	m_menuItem22 = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Add template 1") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( m_menuItem22 );

	wxMenuItem* m_menuItem23;
	m_menuItem23 = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Add template 2") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( m_menuItem23 );

	wxMenuItem* m_menuItem24;
	m_menuItem24 = new wxMenuItem( mMenuAddObject, wxID_ANY, wxString( wxT("Add template 3") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAddObject->Append( m_menuItem24 );

	m_menubar2->Append( mMenuAddObject, wxT("Add object") );

	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Capture screen") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem14 );

	wxMenuItem* m_menuItem26;
	m_menuItem26 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Save mesh") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem26 );

	m_menubar2->Append( m_menu2, wxT("Tools") );

	this->SetMenuBar( m_menubar2 );

	m_statusBar2 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	mPanelSideBar = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	mPanelSideBar->SetScrollRate( 5, 5 );
	mPanelSideBar->SetMinSize( wxSize( 320,-1 ) );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	mPanelObjects = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( mPanelObjects, wxID_ANY, wxT("Object list") ), wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	mObjectList = new wxListBox( mPanelObjects, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_EXTENDED|wxLB_NEEDED_SB );
	bSizer5->Add( mObjectList, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_button14 = new wxButton( mPanelObjects, wxID_ANY, wxT("Move up"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_button14, 0, wxALL, 5 );

	m_button15 = new wxButton( mPanelObjects, wxID_ANY, wxT("Move down"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_button15, 0, wxALL, 5 );

	mDeleteObjects = new wxButton( mPanelObjects, wxID_ANY, wxT("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( mDeleteObjects, 0, wxALL, 5 );

	bSizer5->Add( bSizer9, 1, wxEXPAND, 5 );

	sbSizer2->Add( bSizer5, 1, wxEXPAND, 5 );

	mPanelObjects->SetSizer( sbSizer2 );
	mPanelObjects->Layout();
	sbSizer2->Fit( mPanelObjects );
	bSizer2->Add( mPanelObjects, 0, wxEXPAND | wxALL, 5 );

	mPanelFluid = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mPanelFluid->Hide();

	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( mPanelFluid, wxID_ANY, wxT("Fluid solver") ), wxVERTICAL );

	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxHORIZONTAL );

	m_button18 = new wxButton( mPanelFluid, wxID_ANY, wxT("Set solid"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_button18, 0, wxALL, 5 );

	m_button19 = new wxButton( mPanelFluid, wxID_ANY, wxT("Set fluid"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_button19, 0, wxALL, 5 );

	m_button21 = new wxButton( mPanelFluid, wxID_ANY, wxT("Propagate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer131->Add( m_button21, 0, wxALIGN_CENTER|wxALL, 5 );

	sbSizer8->Add( bSizer131, 0, 0, 5 );

	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText16 = new wxStaticText( mPanelFluid, wxID_ANY, wxT("Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer141->Add( m_staticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mFluidTime = new wxTextCtrl( mPanelFluid, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	bSizer141->Add( mFluidTime, 0, wxALL, 5 );

	m_staticText17 = new wxStaticText( mPanelFluid, wxID_ANY, wxT("Iterations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	bSizer141->Add( m_staticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mFluidIterations = new wxTextCtrl( mPanelFluid, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	bSizer141->Add( mFluidIterations, 0, wxALL, 5 );

	sbSizer8->Add( bSizer141, 0, 0, 5 );

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );

	mFluidRecord = new wxCheckBox( mPanelFluid, wxID_ANY, wxT("Record"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer17->Add( mFluidRecord, 0, wxALL, 5 );

	mFluidSlider = new wxSlider( mPanelFluid, wxID_ANY, 0, 0, 0, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	mFluidSlider->Enable( false );

	bSizer17->Add( mFluidSlider, 0, wxALL|wxEXPAND, 5 );

	sbSizer8->Add( bSizer17, 0, wxEXPAND, 5 );

	mFluidSaveButton = new wxButton( mPanelFluid, wxID_ANY, wxT("Save recorded sequence"), wxDefaultPosition, wxDefaultSize, 0 );
	mFluidSaveButton->Enable( false );

	sbSizer8->Add( mFluidSaveButton, 0, wxALL, 5 );

	m_button20 = new wxButton( mPanelFluid, wxID_ANY, wxT("Visualize velocity field"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer8->Add( m_button20, 0, wxALL, 5 );

	m_button22 = new wxButton( mPanelFluid, wxID_ANY, wxT("Visualize voxel classifications"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer8->Add( m_button22, 0, wxALL, 5 );

	m_button24 = new wxButton( mPanelFluid, wxID_ANY, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button24->Hide();

	sbSizer8->Add( m_button24, 0, wxALL, 5 );

	mPanelFluid->SetSizer( sbSizer8 );
	mPanelFluid->Layout();
	sbSizer8->Fit( mPanelFluid );
	bSizer2->Add( mPanelFluid, 0, wxEXPAND | wxALL, 5 );

	mPanelDecimation = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mPanelDecimation->Hide();

	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( mPanelDecimation, wxID_ANY, wxT("Decimation") ), wxHORIZONTAL );

	mButtonDecimate = new wxButton( mPanelDecimation, wxID_ANY, wxT("Decimate"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer7->Add( mButtonDecimate, 0, wxALL, 5 );

	m_DecimationTargetTxtBox = new wxTextCtrl( mPanelDecimation, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	sbSizer7->Add( m_DecimationTargetTxtBox, 0, wxALL, 5 );

	mPanelDecimation->SetSizer( sbSizer7 );
	mPanelDecimation->Layout();
	sbSizer7->Fit( mPanelDecimation );
	bSizer2->Add( mPanelDecimation, 0, wxEXPAND | wxALL, 5 );

	mPanelSubdivision = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mPanelSubdivision->Hide();

	wxStaticBoxSizer* sbSizer71;
	sbSizer71 = new wxStaticBoxSizer( new wxStaticBox( mPanelSubdivision, wxID_ANY, wxT("Subdivision") ), wxHORIZONTAL );

	mButtonSubdivide = new wxButton( mPanelSubdivision, wxID_ANY, wxT("Subdivide"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer71->Add( mButtonSubdivide, 0, wxALL, 5 );

	mPanelSubdivision->SetSizer( sbSizer71 );
	mPanelSubdivision->Layout();
	sbSizer71->Fit( mPanelSubdivision );
	bSizer2->Add( mPanelSubdivision, 0, wxEXPAND | wxALL, 5 );

	mPanelLevelset = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mPanelLevelset->Hide();

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( mPanelLevelset, wxID_ANY, wxT("Levelset") ), wxVERTICAL );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 2, 0, 0 );

	m_button7 = new wxButton( mPanelLevelset, wxID_ANY, wxT("Reinitialize"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button7, 0, wxALL, 5 );

	m_button8 = new wxButton( mPanelLevelset, wxID_ANY, wxT("Advect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button8, 0, wxALL, 5 );

	m_button4 = new wxButton( mPanelLevelset, wxID_ANY, wxT("Dilate"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button4, 0, wxALL, 5 );

	m_button5 = new wxButton( mPanelLevelset, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button5, 0, wxALL, 5 );

	m_button6 = new wxButton( mPanelLevelset, wxID_ANY, wxT("Smooth"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button6, 0, wxALL, 5 );

	m_button13 = new wxButton( mPanelLevelset, wxID_ANY, wxT("Morph"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_button13, 0, wxALL, 5 );

	sbSizer5->Add( gSizer1, 1, wxEXPAND, 5 );

	mReinitializeFastMarching = new wxCheckBox( mPanelLevelset, wxID_ANY, wxT("Fast marching reinitialization"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer5->Add( mReinitializeFastMarching, 0, wxALL, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText15 = new wxStaticText( mPanelLevelset, wxID_ANY, wxT("Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer8->Add( m_staticText15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mPropagationTime = new wxTextCtrl( mPanelLevelset, wxID_ANY, wxT("0.01"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	bSizer8->Add( mPropagationTime, 0, wxALL, 5 );

	m_staticText13 = new wxStaticText( mPanelLevelset, wxID_ANY, wxT("Iterations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer8->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mLevelsetIterations = new wxTextCtrl( mPanelLevelset, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	bSizer8->Add( mLevelsetIterations, 0, wxALL, 5 );

	sbSizer5->Add( bSizer8, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	mLabelNarrowBand = new wxStaticText( mPanelLevelset, wxID_ANY, wxT("Narrowband"), wxDefaultPosition, wxDefaultSize, 0 );
	mLabelNarrowBand->Wrap( -1 );
	bSizer14->Add( mLabelNarrowBand, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mNarrowBandWidth = new wxTextCtrl( mPanelLevelset, wxID_ANY, wxT("16"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	bSizer14->Add( mNarrowBandWidth, 0, wxALL, 5 );

	mNarrowBandButton = new wxButton( mPanelLevelset, wxID_ANY, wxT("Set width"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( mNarrowBandButton, 0, wxALL, 5 );

	sbSizer5->Add( bSizer14, 0, 0, 5 );

	mPanelLevelset->SetSizer( sbSizer5 );
	mPanelLevelset->Layout();
	sbSizer5->Fit( mPanelLevelset );
	bSizer2->Add( mPanelLevelset, 0, wxEXPAND | wxALL, 5 );

	mPanelImplicit = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer72;
	sbSizer72 = new wxStaticBoxSizer( new wxStaticBox( mPanelImplicit, wxID_ANY, wxT("Implicit") ), wxVERTICAL );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText131 = new wxStaticText( mPanelImplicit, wxID_ANY, wxT("Mesh sampling"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	bSizer13->Add( m_staticText131, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mMeshSampling = new wxTextCtrl( mPanelImplicit, wxID_ANY, wxT("0.05"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	bSizer13->Add( mMeshSampling, 0, wxALL, 5 );

	m_button16 = new wxButton( mPanelImplicit, wxID_ANY, wxT("Resample"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_button16, 0, wxALL, 5 );

	sbSizer72->Add( bSizer13, 0, 0, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	mUnion = new wxButton( mPanelImplicit, wxID_ANY, wxT("Union"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mUnion, 0, wxALL, 5 );

	mIntersection = new wxButton( mPanelImplicit, wxID_ANY, wxT("Intersection"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mIntersection, 0, wxALL, 5 );

	mDifference = new wxButton( mPanelImplicit, wxID_ANY, wxT("Difference"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( mDifference, 0, wxALL, 5 );

	bSizer12->Add( bSizer6, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	mBlend = new wxCheckBox( mPanelImplicit, wxID_ANY, wxT("Blended CSG"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( mBlend, 0, wxALL, 5 );

	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxHORIZONTAL );

	mBlendLabel = new wxStaticText( mPanelImplicit, wxID_ANY, wxT("Blend"), wxDefaultPosition, wxDefaultSize, 0 );
	mBlendLabel->Wrap( -1 );
	mBlendLabel->Enable( false );

	bSizer51->Add( mBlendLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mBlendParameter = new wxTextCtrl( mPanelImplicit, wxID_ANY, wxT("10"), wxDefaultPosition, wxSize( 40,-1 ), 0 );
	mBlendParameter->Enable( false );

	bSizer51->Add( mBlendParameter, 0, wxALL, 5 );

	bSizer7->Add( bSizer51, 1, wxEXPAND, 5 );

	bSizer12->Add( bSizer7, 0, wxALIGN_CENTER_VERTICAL, 5 );

	sbSizer72->Add( bSizer12, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText191 = new wxStaticText( mPanelImplicit, wxID_ANY, wxT("Differential scale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText191->Wrap( -1 );
	bSizer18->Add( m_staticText191, 0, wxALL, 5 );

	mDifferentialScale = new wxSlider( mPanelImplicit, wxID_ANY, 5, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer18->Add( mDifferentialScale, 0, wxALL, 5 );

	sbSizer72->Add( bSizer18, 0, 0, 5 );

	mPanelImplicit->SetSizer( sbSizer72 );
	mPanelImplicit->Layout();
	sbSizer72->Fit( mPanelImplicit );
	bSizer2->Add( mPanelImplicit, 0, wxEXPAND | wxALL, 5 );

	mPanelVisualization = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer51;
	sbSizer51 = new wxStaticBoxSizer( new wxStaticBox( mPanelVisualization, wxID_ANY, wxT("Visualization") ), wxVERTICAL );

	mVisualizeWireframe = new wxCheckBox( mPanelVisualization, wxID_ANY, wxT("Wireframe"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer51->Add( mVisualizeWireframe, 0, wxALL, 5 );

	mVisualizeMeshNormals = new wxCheckBox( mPanelVisualization, wxID_ANY, wxT("Visualize mesh normals"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer51->Add( mVisualizeMeshNormals, 0, wxALL, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText9 = new wxStaticText( mPanelVisualization, wxID_ANY, wxT("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer4->Add( m_staticText9, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	mVisualizeOpacity = new wxSlider( mPanelVisualization, wxID_ANY, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer4->Add( mVisualizeOpacity, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	sbSizer51->Add( bSizer4, 1, wxEXPAND, 5 );

	wxString mColorMapChoiceChoices[] = { wxT("Choose color map...") };
	int mColorMapChoiceNChoices = sizeof( mColorMapChoiceChoices ) / sizeof( wxString );
	mColorMapChoice = new wxChoice( mPanelVisualization, wxID_ANY, wxDefaultPosition, wxDefaultSize, mColorMapChoiceNChoices, mColorMapChoiceChoices, 0 );
	mColorMapChoice->SetSelection( 0 );
	sbSizer51->Add( mColorMapChoice, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText51 = new wxStaticText( mPanelVisualization, wxID_ANY, wxT("Min"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer11->Add( m_staticText51, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	mMin = new wxTextCtrl( mPanelVisualization, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	mMin->SetMaxLength( 5 );
	mMin->Enable( false );

	fgSizer11->Add( mMin, 0, wxALL, 5 );

	m_staticText61 = new wxStaticText( mPanelVisualization, wxID_ANY, wxT("Max"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer11->Add( m_staticText61, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	mMax = new wxTextCtrl( mPanelVisualization, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	mMax->SetMaxLength( 5 );
	mMax->Enable( false );

	fgSizer11->Add( mMax, 0, wxALL, 5 );

	sbSizer51->Add( fgSizer11, 1, wxEXPAND, 5 );

	mAutoMinMax = new wxCheckBox( mPanelVisualization, wxID_ANY, wxT("Automatic min/max"), wxDefaultPosition, wxDefaultSize, 0 );
	mAutoMinMax->SetValue(true);
	sbSizer51->Add( mAutoMinMax, 0, wxALL, 5 );

	wxString mVisualizationModeChoiceChoices[] = { wxT("Choose visualization mode...") };
	int mVisualizationModeChoiceNChoices = sizeof( mVisualizationModeChoiceChoices ) / sizeof( wxString );
	mVisualizationModeChoice = new wxChoice( mPanelVisualization, wxID_ANY, wxDefaultPosition, wxDefaultSize, mVisualizationModeChoiceNChoices, mVisualizationModeChoiceChoices, 0 );
	mVisualizationModeChoice->SetSelection( 0 );
	sbSizer51->Add( mVisualizationModeChoice, 0, wxALL, 5 );

	mPanelVisualization->SetSizer( sbSizer51 );
	mPanelVisualization->Layout();
	sbSizer51->Fit( mPanelVisualization );
	bSizer2->Add( mPanelVisualization, 0, wxEXPAND | wxALL, 5 );

	mPanelTransform = new wxPanel( mPanelSideBar, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( mPanelTransform, wxID_ANY, wxT("Transform") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 4, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( mPanelTransform, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );

	m_staticText2 = new wxStaticText( mPanelTransform, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );

	m_staticText3 = new wxStaticText( mPanelTransform, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALIGN_CENTER|wxALL, 5 );

	m_staticText4 = new wxStaticText( mPanelTransform, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALIGN_CENTER|wxALL, 5 );

	m_staticText5 = new wxStaticText( mPanelTransform, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	mScaleX = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer1->Add( mScaleX, 0, wxALL, 5 );

	mScaleY = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	mScaleY->Enable( false );

	fgSizer1->Add( mScaleY, 0, wxALL, 5 );

	mScaleZ = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	mScaleZ->Enable( false );

	fgSizer1->Add( mScaleZ, 0, wxALL, 5 );

	m_staticText6 = new wxStaticText( mPanelTransform, wxID_ANY, wxT("Translate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	mTranslateX = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer1->Add( mTranslateX, 0, wxALL, 5 );

	mTranslateY = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer1->Add( mTranslateY, 0, wxALL, 5 );

	mTranslateZ = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer1->Add( mTranslateZ, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( mPanelTransform, wxID_ANY, wxT("Rotate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	mRotateX = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer1->Add( mRotateX, 0, wxALL, 5 );

	mRotateY = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer1->Add( mRotateY, 0, wxALL, 5 );

	mRotateZ = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer1->Add( mRotateZ, 0, wxALL, 5 );

	sbSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );

	mUniformScaling = new wxCheckBox( mPanelTransform, wxID_ANY, wxT("Uniform scaling"), wxDefaultPosition, wxDefaultSize, 0 );
	mUniformScaling->SetValue(true);
	sbSizer1->Add( mUniformScaling, 0, wxALIGN_RIGHT|wxALL, 5 );

	mButtonTransform = new wxButton( mPanelTransform, wxID_ANY, wxT("Transform"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( mButtonTransform, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	mDilate = new wxButton( mPanelTransform, wxID_ANY, wxT("Dilate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( mDilate, 0, wxALL, 5 );

	mErode = new wxButton( mPanelTransform, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( mErode, 0, wxALL, 5 );

	m_button27 = new wxButton( mPanelTransform, wxID_ANY, wxT("Smooth"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button27, 0, wxALL, 5 );

	sbSizer1->Add( bSizer16, 0, 0, 5 );

	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText19 = new wxStaticText( mPanelTransform, wxID_ANY, wxT("Amount"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer171->Add( m_staticText19, 0, wxALL, 5 );

	mAmount = new wxTextCtrl( mPanelTransform, wxID_ANY, wxT("0.1"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer171->Add( mAmount, 0, wxALL, 5 );

	sbSizer1->Add( bSizer171, 0, 0, 5 );

	mPanelTransform->SetSizer( sbSizer1 );
	mPanelTransform->Layout();
	sbSizer1->Fit( mPanelTransform );
	bSizer2->Add( mPanelTransform, 0, wxEXPAND | wxALL, 5 );

	mPanelSideBar->SetSizer( bSizer2 );
	mPanelSideBar->Layout();
	bSizer2->Fit( mPanelSideBar );
	bSizer11->Add( mPanelSideBar, 0, wxALL|wxEXPAND, 3 );

	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
	mGLViewer = new GLViewer(this, args);
	bSizer11->Add( mGLViewer, 1, wxALL|wxEXPAND, 0 );

	this->SetSizer( bSizer11 );
	this->Layout();

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( BaseFrameMain::OnClose ) );
	this->Connect( mAddObjectSimpleMesh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectSimpleMesh ) );
	this->Connect( mAddObjectHalfEdgeMesh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectHalfEdgeMesh ) );
	this->Connect( mAddObjectSimpleDecimationMesh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectSimpleDecimationMesh ) );
	this->Connect( mAddObjectQuadricDecimationMesh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricDecimationMesh ) );
	this->Connect( mAddObjectCubicSpline->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectCubicSpline ) );
	this->Connect( mAddObjectSubdivisionCurve->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectSubdivisionCurve ) );
	this->Connect( mAddObjectLoopSubdivisionMesh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectLoopSubdivisionMesh ) );
	this->Connect( mAddObjectStrangeSubdivisionMesh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectStrangeSubdivisionMesh ) );
	this->Connect( mAddObjectImplicitSphere->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectImplicitSphere ) );
	this->Connect( mAddObjectImplicitMesh->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectImplicitMesh ) );
	this->Connect( mAddObjectQuadricPlane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricPlane ) );
	this->Connect( mAddObjectQuadricCylinder->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricCylinder ) );
	this->Connect( mAddObjectQuadricEllipsoid->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricEllipsoid ) );
	this->Connect( mAddObjectQuadricCone->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricCone ) );
	this->Connect( mAddObjectQuadricParaboloid->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricParaboloid ) );
	this->Connect( mAddObjectQuadricHyperboloid->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricHyperboloid ) );
	this->Connect( mAddObjectScalarCutPlane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectScalarCutPlane ) );
	this->Connect( mAddObjectVectorCutPlane->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectVectorCutPlane ) );
	this->Connect( m_menuItem21->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::LoadLevelset ) );
	this->Connect( m_menuItem8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::ConvertToLevelset ) );
	this->Connect( m_menuItem22->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddTemplate1 ) );
	this->Connect( m_menuItem23->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddTemplate2 ) );
	this->Connect( m_menuItem24->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddTemplate3 ) );
	this->Connect( m_menuItem14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::CaptureScreen ) );
	this->Connect( m_menuItem26->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::SaveMesh ) );
	mObjectList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( BaseFrameMain::SelectObjects ), NULL, this );
	m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::MoveObjectsUp ), NULL, this );
	m_button15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::MoveObjecsDown ), NULL, this );
	mDeleteObjects->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::DeleteObjects ), NULL, this );
	m_button18->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidSetSolid ), NULL, this );
	m_button19->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidSetFluid ), NULL, this );
	m_button21->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidSolve ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSaveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::SaveSimulationFrames ), NULL, this );
	m_button20->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidVisualizeVelocities ), NULL, this );
	m_button22->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidVisualizeVoxelsClassification ), NULL, this );
	m_button24->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::PlaySimulation ), NULL, this );
	mButtonDecimate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::DecimateObjects ), NULL, this );
	mButtonSubdivide->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::SubdivideObjects ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetReinitialize ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetAdvect ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetDilate ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetErode ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetSmooth ), NULL, this );
	m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetMorph ), NULL, this );
	mNarrowBandButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::EnableNarrowband ), NULL, this );
	m_button16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::ResampleImplicit ), NULL, this );
	mUnion->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Union ), NULL, this );
	mIntersection->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Intersection ), NULL, this );
	mDifference->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Difference ), NULL, this );
	mBlend->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::SwitchBlending ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mVisualizeWireframe->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::VisualizeWireframe ), NULL, this );
	mVisualizeMeshNormals->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::VisualizeMeshNormals ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mColorMapChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BaseFrameMain::SetColormap ), NULL, this );
	mMin->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mMin->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BaseFrameMain::ScaleChanged ), NULL, this );
	mMax->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mAutoMinMax->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::ToggleAutoMinMax ), NULL, this );
	mVisualizationModeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BaseFrameMain::SetVisualizationMode ), NULL, this );
	mScaleX->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mScaleX->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BaseFrameMain::ScaleChanged ), NULL, this );
	mScaleY->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mScaleZ->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mTranslateX->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mTranslateY->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mTranslateZ->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mRotateX->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mRotateY->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mRotateZ->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mUniformScaling->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::ToggleUniformScaling ), NULL, this );
	mButtonTransform->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::TransformObjects ), NULL, this );
	mDilate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Dilate ), NULL, this );
	mErode->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Erode ), NULL, this );
	m_button27->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Smooth ), NULL, this );
}

BaseFrameMain::~BaseFrameMain()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( BaseFrameMain::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectSimpleMesh ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectHalfEdgeMesh ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectSimpleDecimationMesh ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricDecimationMesh ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectCubicSpline ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectSubdivisionCurve ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectLoopSubdivisionMesh ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectStrangeSubdivisionMesh ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectImplicitSphere ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectImplicitMesh ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricPlane ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricCylinder ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricEllipsoid ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricCone ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricParaboloid ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectQuadricHyperboloid ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectScalarCutPlane ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddObjectVectorCutPlane ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::LoadLevelset ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::ConvertToLevelset ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddTemplate1 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddTemplate2 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::AddTemplate3 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::CaptureScreen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BaseFrameMain::SaveMesh ) );
	mObjectList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( BaseFrameMain::SelectObjects ), NULL, this );
	m_button14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::MoveObjectsUp ), NULL, this );
	m_button15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::MoveObjecsDown ), NULL, this );
	mDeleteObjects->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::DeleteObjects ), NULL, this );
	m_button18->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidSetSolid ), NULL, this );
	m_button19->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidSetFluid ), NULL, this );
	m_button21->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidSolve ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BaseFrameMain::FluidPlayback ), NULL, this );
	mFluidSaveButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::SaveSimulationFrames ), NULL, this );
	m_button20->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidVisualizeVelocities ), NULL, this );
	m_button22->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::FluidVisualizeVoxelsClassification ), NULL, this );
	m_button24->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::PlaySimulation ), NULL, this );
	mButtonDecimate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::DecimateObjects ), NULL, this );
	mButtonSubdivide->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::SubdivideObjects ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetReinitialize ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetAdvect ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetDilate ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetErode ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetSmooth ), NULL, this );
	m_button13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::LevelsetMorph ), NULL, this );
	mNarrowBandButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::EnableNarrowband ), NULL, this );
	m_button16->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::ResampleImplicit ), NULL, this );
	mUnion->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Union ), NULL, this );
	mIntersection->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Intersection ), NULL, this );
	mDifference->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Difference ), NULL, this );
	mBlend->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::SwitchBlending ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mDifferentialScale->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BaseFrameMain::DifferentialScaleChanged ), NULL, this );
	mVisualizeWireframe->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::VisualizeWireframe ), NULL, this );
	mVisualizeMeshNormals->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::VisualizeMeshNormals ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mVisualizeOpacity->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( BaseFrameMain::OpacityChanged ), NULL, this );
	mColorMapChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BaseFrameMain::SetColormap ), NULL, this );
	mMin->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mMin->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BaseFrameMain::ScaleChanged ), NULL, this );
	mMax->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mAutoMinMax->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::ToggleAutoMinMax ), NULL, this );
	mVisualizationModeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BaseFrameMain::SetVisualizationMode ), NULL, this );
	mScaleX->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mScaleX->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( BaseFrameMain::ScaleChanged ), NULL, this );
	mScaleY->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mScaleZ->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mTranslateX->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mTranslateY->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mTranslateZ->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mRotateX->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mRotateY->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mRotateZ->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( BaseFrameMain::TextCtrlFocus ), NULL, this );
	mUniformScaling->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BaseFrameMain::ToggleUniformScaling ), NULL, this );
	mButtonTransform->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::TransformObjects ), NULL, this );
	mDilate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Dilate ), NULL, this );
	mErode->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Erode ), NULL, this );
	m_button27->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BaseFrameMain::Smooth ), NULL, this );

}
