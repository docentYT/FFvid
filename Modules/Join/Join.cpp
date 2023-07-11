#include "Join.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/button.h>

wxPanel* Join::createPanel(wxNotebook* parent) {
	wxPanel* mainPanel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	/* Input file */
	wxButton* inputFilePathButton = new wxButton(mainPanel, wxID_ANY, "Browse");
	inputFilePathButton->Bind(wxEVT_BUTTON, &Join::selectInput, this);

	/* Main sizer setup */
	mainPanel->SetSizerAndFit(mainSizer);

	Join::panel = mainPanel;
	return mainPanel;
}

void Join::selectInput(wxCommandEvent& evt) {
	wxFileDialog dialog(Trim::panel, wxEmptyString, wxEmptyString, wxEmptyString, WILDCARD, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_CANCEL) return;
	Trim::inputFilePathCtrl->SetValue(dialog.GetPath());
	wxLogStatus(dialog.GetPath());
}