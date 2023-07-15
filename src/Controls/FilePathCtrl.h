#pragma once

#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

class FilePathCtrl {
public:
	wxTextCtrl* const textCtrl;
	wxStaticBoxSizer* const sizer;
	FilePathCtrl(wxWindow* parent, const wxString& staticBoxTitle, const wxString& wildcard, const long dialogStyle);

private:
	wxWindow* const parent;
	wxString wildcard;
	long dialogStyle;
	void selectFile(wxCommandEvent& evt);
};