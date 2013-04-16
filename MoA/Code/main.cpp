
#include <wx/wx.h>
#include "GUI/FrameMain.h"

// Open a console if we're on Windows
#ifdef WIN32
#include "Util/Console.h"
  Console con;
#endif

class wxWidgetsApp : public wxApp
{
public:
    wxWidgetsApp();
    virtual ~wxWidgetsApp();
    virtual bool OnInit();
};

DECLARE_APP(wxWidgetsApp)

IMPLEMENT_APP(wxWidgetsApp)

wxWidgetsApp::wxWidgetsApp()
{
}

wxWidgetsApp::~wxWidgetsApp()
{
}

bool wxWidgetsApp::OnInit()
{
    FrameMain * frame = new FrameMain( (wxWindow*)NULL );
    frame->Show();
    SetTopWindow(frame);
    return true;
}
