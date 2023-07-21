#include "ProcessBar.h"

#include <wx/button.h>

ProcessBar::ProcessBar(wxWindow* parent, const wxString& buttonText) :
	parent(parent),
	gaugeSizer(new wxBoxSizer(wxVERTICAL)),
	sizer(new wxBoxSizer(wxHORIZONTAL)),
	progressGauge(new wxGauge(parent, wxID_ANY, 100)),
	button(new wxButton(parent, wxID_ANY, buttonText)) {

	gaugeSizer->Add(progressGauge, 1, wxEXPAND);
#ifdef _WIN32
	sizer->Add(gaugeSizer, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
	sizer->Add(button, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
#else
	sizer->Add(gaugeSizer, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxBOTTOM, 10);
	sizer->Add(button, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxBOTTOM, 10);
#endif
}
