#define DEBUG

#include <wx/app.h>

#include "App.h"
#include "MainFrame.h"
#include <wx/msgdlg.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame();
	mainFrame->Center();
	mainFrame->Show();

	return true;
}