#ifndef TRIANGULAR_SLIDER_HPP
#define TRIANGULAR_SLIDER_HPP

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <algorithm>

wxDECLARE_EVENT(wxEVT_TRIANGULAR_SLIDER_CHANGED, wxCommandEvent);

class TriangularSlider : public wxPanel {
public:
    TriangularSlider(wxWindow* parent, int id, int value, int minValue, int maxValue,
                     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    int GetValue() const;
    void SetValue(int value);

private:
    int m_value;
    int m_min;
    int m_max;

    void OnPaint(wxPaintEvent& event);
    void OnMouse(wxMouseEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif
