#include "ProcessBar.h"

#include <wx/button.h>

ProcessBar::ProcessBar(wxWindow* parent, const wxString& buttonText, wxSize progressGaugeSize) :
	parent(parent),
	sizer(new wxBoxSizer(wxHORIZONTAL)),
	progressGauge(new wxGauge(parent, wxID_ANY, 100, wxDefaultPosition, progressGaugeSize)),
	button(new wxButton(parent, wxID_ANY, buttonText)) {

	sizer->AddStretchSpacer();
	sizer->Add(progressGauge, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	sizer->AddStretchSpacer();
	sizer->Add(button, 0, wxALL, 10);
	sizer->AddStretchSpacer();
}
