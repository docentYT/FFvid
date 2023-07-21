#include "FilePathCtrl.h"

FilePathCtrl::FilePathCtrl(wxWindow* parent, const wxString& staticBoxTitle, const wxString& wildcard, const long dialogStyle) :
	parent(parent),
	textCtrl(new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, parent->FromDIP(wxSize(500, -1)))),
	sizer(new wxStaticBoxSizer(wxHORIZONTAL, parent, staticBoxTitle)) {
	this->wildcard = wildcard;
	this->dialogStyle = dialogStyle;
	wxButton* outputFilePathButton = new wxButton(parent, wxID_ANY, "Browse");
	outputFilePathButton->Bind(wxEVT_BUTTON, &FilePathCtrl::selectFile, this);

	sizer->AddStretchSpacer();
	sizer->Add(textCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	sizer->AddStretchSpacer();
	sizer->Add(outputFilePathButton, 0, wxALL, 10);
	sizer->AddStretchSpacer();
}

void FilePathCtrl::selectFile(wxCommandEvent& evt) {
	wxFileDialog dialog(parent, wxEmptyString, wxEmptyString, wxEmptyString, wildcard, dialogStyle);
	if (dialog.ShowModal() == wxID_CANCEL) return;
	textCtrl->SetValue(dialog.GetPath());
}