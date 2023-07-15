#include "MainFrame.h"
#include "Modules/Trim/Trim.h"
#include "Modules/Join/Join.h"

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/window.h>
#include <wx/sizer.h>

#include <wx/msgdlg.h>

MainFrame::MainFrame()
	: wxFrame(nullptr, wxID_ANY, "FFvid", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER) {
	wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(notebook, 1, wxEXPAND);
	this->SetSizer(sizer);
	this->SetBackgroundColour(wxColor("red"));

	trim = new Trim();
	wxPanel* trimPanel = trim->createPanel(notebook);
	notebook->AddPage(trimPanel, "Trim", true);

	join = new Join();
	wxPanel* joinPanel = join->createPanel(notebook);
	notebook->AddPage(joinPanel, "Join");

	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::onClose, this);
	this->SetIcon(wxIcon("appicon"));

	//CreateStatusBar();
	SetMinSize(GetBestSize());
	SetMaxSize(GetBestSize());
	SetSize(GetBestSize());
}

void MainFrame::onClose(wxCloseEvent& e) {
	if (trim->busy) {
		e.Veto();
		wxMessageBox("FFmpeg is working! Please wait or stop ffmpeg first.");
		return;
	}
	else {
		this->Destroy();
	}
}
