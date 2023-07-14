#pragma once
#include <wx/frame.h>
#include "modules/Trim/Trim.h"
#include "modules/Join/Join.h"

class MainFrame : public wxFrame {
public:
	MainFrame();
private:
	Trim* trim;
	Join* join;
	void onClose(wxCloseEvent& e);
};