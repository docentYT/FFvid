#pragma once

#include "../Module.h"

#include <wx/textctrl.h>
#include <wx/gauge.h>

#include "../../Controls/FilePathCtrl.h"
#include "../../Controls/ProcessBar.h"

class Trim : public Module {
public:
	wxPanel* createPanel(wxNotebook* parent) override;

	using Module::Module;

private:
	FilePathCtrl* inputFilePathCtrl;
	FilePathCtrl* outputFilePathCtrl;
	wxTextCtrl* startTimeCtrl;
	wxTextCtrl* endTimeCtrl;
	ProcessBar* processBar;
	void trimVideo(wxCommandEvent& evt);
	void getTimeFromString(wxString& string, int& h, int& m, int& s);
};