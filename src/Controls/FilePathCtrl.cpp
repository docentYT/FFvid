#include "FilePathCtrl.h"

FilePathCtrl::FilePathCtrl(wxWindow* parent, const wxString& staticBoxTitle, const wxString& wildcard, const long dialogStyle) :
	parent(parent),
	textCtrl(new wxTextCtrl(parent, wxID_ANY, wxEmptyString)),
	textCtrlSizer(new wxBoxSizer(wxVERTICAL)),
	sizer(new wxStaticBoxSizer(wxHORIZONTAL, parent, staticBoxTitle)) {
	this->wildcard = wildcard;
	this->dialogStyle = dialogStyle;
	wxButton* outputFilePathButton = new wxButton(parent, wxID_ANY, "Browse");
	outputFilePathButton->Bind(wxEVT_BUTTON, &FilePathCtrl::selectFile, this);

	textCtrlSizer->Add(textCtrl, 1, wxEXPAND);
#ifdef _WIN32
	sizer->Add(textCtrlSizer, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	sizer->Add(outputFilePathButton, 0, wxALIGN_CENTER_VERTICAL);
#else
	sizer->Add(textCtrlSizer, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxBOTTOM | wxRIGHT, 10);
	sizer->Add(outputFilePathButton, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM | wxRIGHT, 10);
#endif
}

void FilePathCtrl::selectFile(wxCommandEvent& evt) {
	wxFileDialog dialog(parent, wxEmptyString, wxEmptyString, wxEmptyString, wildcard, dialogStyle);
	if (dialog.ShowModal() == wxID_CANCEL) return;
	textCtrl->SetValue(dialog.GetPath());
}