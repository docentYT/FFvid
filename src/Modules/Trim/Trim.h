#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>
#include "../Module.h"
#include "../../Controls/FilePathCtrl.h"

class Trim : public Module {
public:
	wxPanel* panel;
	std::atomic<bool> busy{ false };
	wxPanel* createPanel(wxNotebook* parent) override;

private:
	FilePathCtrl* inputFilePathCtrl;
	FilePathCtrl* outputFilePathCtrl;
	wxTextCtrl* startTimeCtrl;
	wxTextCtrl* endTimeCtrl;
	wxGauge* progressGauge;
	void trimVideo(wxCommandEvent& evt);
	void getTimeFromString(wxString& string, int& h, int& m, int& s);
};