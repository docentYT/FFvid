#pragma once
#include <wx/frame.h>
#include "Modules/Trim/Trim.h"
#include "Modules/Join/Join.h"
#include "Modules/Watermark/Watermark.h"

class MainFrame : public wxFrame {
public:
	MainFrame();
private:
	Trim* trim;
	Join* join;
	Watermark* watermark;
	void onClose(wxCloseEvent& e);
};