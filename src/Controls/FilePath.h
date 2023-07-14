#pragma once

#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

class FilePath {
public:
	wxTextCtrl* textCtrl;
	wxStaticBoxSizer* sizer;
	FilePath(wxWindow* parent, const wxString& staticBoxTitle, const wxString& wildcard, const long dialogStyle) {
		FilePath::parent = parent;
		FilePath::wildcard = wildcard;
		FilePath::dialogStyle = dialogStyle;
		textCtrl = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(600, -1));
		wxButton* outputFilePathButton = new wxButton(parent, wxID_ANY, "Browse");
		outputFilePathButton->Bind(wxEVT_BUTTON, &FilePath::selectFile, this);

		FilePath::sizer = new wxStaticBoxSizer(wxHORIZONTAL, parent, staticBoxTitle);
		sizer->AddStretchSpacer();
		sizer->Add(textCtrl, 0, wxALL, 10);
		sizer->AddStretchSpacer();
		sizer->Add(outputFilePathButton, 0, wxALL, 10);
		sizer->AddStretchSpacer();
	}

private:
	wxWindow* parent;
	wxString wildcard;
	long dialogStyle;
	void selectFile(wxCommandEvent& evt) {
		wxFileDialog dialog(parent, wxEmptyString, wxEmptyString, wxEmptyString, wildcard, dialogStyle);
		if (dialog.ShowModal() == wxID_CANCEL) return;
		FilePath::textCtrl->SetValue(dialog.GetPath());
	}
};