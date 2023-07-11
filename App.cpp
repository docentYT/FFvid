#define DEBUG

#include <wx/app.h>

#include "App.h"
#include "MainFrame.h"
#include <wx/msgdlg.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame();
	mainFrame->SetClientSize(800, 200);
	mainFrame->Center();
	mainFrame->Show();

	return true;
}