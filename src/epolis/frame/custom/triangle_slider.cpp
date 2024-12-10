#include "triangle_slider.hpp"


wxDEFINE_EVENT(wxEVT_TRIANGULAR_SLIDER_CHANGED, wxCommandEvent);

wxBEGIN_EVENT_TABLE(TriangularSlider, wxPanel)

wxEND_EVENT_TABLE()

TriangularSlider::TriangularSlider(wxWindow* parent, int id, int value, int minValue, int maxValue,
                                   const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, id, pos, size, wxBORDER_NONE), m_value(value), m_min(minValue), m_max(maxValue) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &TriangularSlider::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &TriangularSlider::OnMouse, this);
    Bind(wxEVT_MOTION, &TriangularSlider::OnMouse, this);
}

int TriangularSlider::GetValue() const {
    return m_value;
}

void TriangularSlider::SetValue(int value) {
    m_value = std::clamp(value, m_min, m_max);
    Refresh();
}

void TriangularSlider::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    wxSize size = GetClientSize();


    double ratio = double(m_value - m_min) / (m_max - m_min);
    int fillWidth = static_cast<int>(ratio * size.x);
    int dynamicY = static_cast<int>((1.0 - ratio) * size.y);

    // White
    wxPoint backgroundTriangle[3] = {
        {0, size.y},
        {size.x, 0},
        {size.x, size.y}
    };
    dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawPolygon(3, backgroundTriangle);

    // Blue
    wxPoint triangle[3] = {
        {0, size.y},
        {fillWidth, dynamicY},
        {fillWidth, size.y}
    };
    dc.SetBrush(wxBrush(wxColour(0, 0, 255)));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawPolygon(3, triangle);
}

void TriangularSlider::OnMouse(wxMouseEvent& event) {
    if (event.LeftIsDown() || event.Dragging()) {
        wxSize size = GetClientSize();
        int mouseX = event.GetX();
        double ratio = std::clamp(static_cast<double>(mouseX) / size.GetWidth(), 0.0, 1.0);
        SetValue(m_min + static_cast<int>(ratio * (m_max - m_min)));

        wxCommandEvent event(wxEVT_TRIANGULAR_SLIDER_CHANGED);
        event.SetInt(m_value);
        wxPostEvent(this, event);
    }
}
