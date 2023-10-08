#pragma once
#include <vector>
#include <wx/frame.h>
#include "Modules/Module.h"

class MainFrame : public wxFrame {
public:
	MainFrame();
private:
	std::vector<Module*> modules;
	void onClose(wxCloseEvent& e);
};