#pragma once
#include "i_panel.h"
#include <string>

class EditorState;

class LeftSidebar : public IPanel
{
public:
    LeftSidebar(EditorState* state);
    void Render() override;
    const std::string& GetName() const override { return m_name; }
private:
    std::string m_name = "Navigation";
    EditorState* m_state;
};