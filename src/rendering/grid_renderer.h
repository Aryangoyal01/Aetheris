#pragma once

class Viewport;

class GridRenderer
{
public:
    void Render(const Viewport& viewport);

private:
    float ChooseSpacing(float worldSpacing) const;
    void RenderBorder(const Viewport& viewport);
};
