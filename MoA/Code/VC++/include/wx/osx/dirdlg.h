/////////////////////////////////////////////////////////////////////////////
// Name:        dirdlg.h
// Purpose:     wxDirDialog class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: dirdlg.h 63291 2010-01-28 04:06:23Z KO $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DIRDLG_H_
#define _WX_DIRDLG_H_

class WXDLLIMPEXP_CORE wxDirDialog : public wxDirDialogBase
{
public:
    wxDirDialog(wxWindow *parent,
                const wxString& message = wxDirSelectorPromptStr,
                const wxString& defaultPath = wxT(""),
                long style = wxDD_DEFAULT_STYLE,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                const wxString& name = wxDirDialogNameStr);

    virtual int ShowModal();
    
#if wxOSX_USE_COCOA
    virtual void ShowWindowModal();
    virtual void ModalFinishedCallback(void* panel, int returnCode);
#endif    

protected:

    DECLARE_DYNAMIC_CLASS(wxDirDialog)
};

#endif
    // _WX_DIRDLG_H_
