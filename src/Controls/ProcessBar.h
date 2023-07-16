#pragma once

#include <wx/window.h>
#include <wx/gauge.h>
#include <wx/sizer.h>

typedef void (*EventHandlerPointer)(wxCommandEvent&);

class ProcessBar {
public:
	wxBoxSizer* const sizer;
	wxGauge* const progressGauge;
	wxButton* const button;
	
	ProcessBar(wxWindow* parent, const wxString& buttonText, wxSize progressGaugeSize);

private:
	wxWindow* const parent;
};