#pragma once
#include <wx/frame.h>
#include "modules/Trim/Trim.h"
#include "modules/Join/Join.h"
#include "modules/Watermark/Watermark.h"

class MainFrame : public wxFrame {
public:
	MainFrame();
private:
	Trim* trim;
	Join* join;
	Watermark* watermark;
	void onClose(wxCloseEvent& e);
};