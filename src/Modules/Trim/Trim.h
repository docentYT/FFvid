#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>
#include "../Module.h"

class Trim : public Module {
public:
	wxPanel* panel;
	std::atomic<bool> busy{ false };
	wxPanel* createPanel(wxNotebook* parent);

private:
	wxTextCtrl* inputFilePathCtrl;
	wxTextCtrl* outputFilePathCtrl;
	wxTextCtrl* startTimeCtrl;
	wxTextCtrl* endTimeCtrl;
	wxGauge* progressGauge;
	void selectInput(wxCommandEvent& evt);
	void selectOutput(wxCommandEvent& evt);
	void trimVideo(wxCommandEvent& evt);
	void getTimeFromString(wxString& string, int& h, int& m, int& s);
};