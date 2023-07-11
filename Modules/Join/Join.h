#pragma once
#include "../Module.h"

#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/textctrl.h> // for std::atomic without C4996 error

class Join : public Module {
public:
	wxPanel* panel;
	std::atomic<bool> busy{ false };
	wxPanel* createPanel(wxNotebook* parent);

private:
	void selectInput(wxCommandEvent& evt);
};