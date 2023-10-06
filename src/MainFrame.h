#pragma once
#include <wx/frame.h>
#include "Modules/Trim/Trim.h"
#include "Modules/Join/Join.h"
#include "Modules/Watermark/Watermark.h"
#include "Modules/RemoveData/RemoveData.h"

class MainFrame : public wxFrame {
public:
	MainFrame();
private:
	Trim* trim;
	Join* join;
	Watermark* watermark;
	RemoveData* removeData;
	void onClose(wxCloseEvent& e);
};