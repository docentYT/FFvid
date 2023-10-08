#include "MainFrame.h"
#include "FFmpeg.h"
#include "Modules/Trim/Trim.h"
#include "Modules/Join/Join.h"
#include "Modules/Watermark/Watermark.h"
#include "Modules/RemoveData/RemoveData.h"

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/window.h>
#include <wx/sizer.h>

#include <wx/msgdlg.h>

MainFrame::MainFrame()
	: wxFrame(nullptr, wxID_ANY, "FFvid", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE^ wxRESIZE_BORDER^ wxMAXIMIZE_BOX)
	, modules{ 
		new Trim(_("Trim")),
		new Join(_("Join")),
		new Watermark(_("Watermark")),
		new RemoveData(_("Remove data"))
	}
{
	if (!FFmpeg::GetInstance()->isInstalled()) {
		wxMessageBox("Please install ffmpeg! https://ffmpeg.org");
		this->Destroy();
	}
	
	wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(notebook, 1, wxEXPAND);
	this->SetSizer(sizer);

	for (auto& m : this->modules) {
		wxPanel* panel = m->createPanel(notebook);
		notebook->AddPage(panel, m->name);
	}

	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::onClose, this);

#ifdef _WIN32
	this->SetIcon(wxIcon("appicon"));
#endif

	auto bestSize = GetBestSize();
#ifndef _WIN32
	bestSize.SetHeight(bestSize.GetY() + 100);
#endif
	SetMinSize(bestSize);
	SetMaxSize(bestSize);
	SetSize(bestSize);
}

void MainFrame::onClose(wxCloseEvent& e) {
	for (auto& m : this->modules) {
		if (m->busy) {
			e.Veto();
			wxMessageBox("FFmpeg is working! Please wait or stop ffmpeg first.");
			return;
		}
	}
	this->Destroy();
}
