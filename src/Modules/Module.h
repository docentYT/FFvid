#pragma once

#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/textctrl.h> // for std::atomic without C4996 error

class Module {
public:
	std::atomic<bool> busy{ false };
	virtual wxPanel* createPanel(wxNotebook* parent) = 0;
};