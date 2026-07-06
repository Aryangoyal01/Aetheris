#pragma once
#include "i_panel.h"
#include <string>

class EditorState;

class RightSidebar : public IPanel
{
public:
    RightSidebar(EditorState* state);
    void Render() override;
    const std::string& GetName() const override { return m_name; }
private:
    std::string m_name = "Inspector";
    EditorState* m_state;
    char m_capacityInputBuffer[64] = {0};
    bool m_isEditingMaxParticles = false;
    char m_spawnRateInputBuffer[64] = {0};
    bool m_isEditingSpawnRate = false;
    bool m_showClampWarning = false;
    float m_clampWarningTimer = 0.0f;
    float m_smoothFraction = 0.0f;
    std::string m_clampMessage = "";
};