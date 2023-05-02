#pragma once

#include "colors.hpp"

#include <QPalette>

namespace DialogueFromVideo :: Palettes {

QPalette createLightPalette()
{
    QPalette lightPalette;

    lightPalette.setColor(QPalette::Window, lightGray);
    lightPalette.setColor(QPalette::WindowText, black);
    lightPalette.setColor(QPalette::Base, white);
    lightPalette.setColor(QPalette::AlternateBase, lightGray);
    lightPalette.setColor(QPalette::ToolTipBase, blue.lighter(150));
    lightPalette.setColor(QPalette::ToolTipText, black);
    lightPalette.setColor(QPalette::Text, black);
    lightPalette.setColor(QPalette::Button, lightGray);
    lightPalette.setColor(QPalette::ButtonText, black);
    lightPalette.setColor(QPalette::Link, blue.darker(150));
    lightPalette.setColor(QPalette::Highlight, blue.lighter(150));
    lightPalette.setColor(QPalette::HighlightedText, white);

    lightPalette.setColor(QPalette::Active, QPalette::Button, gray.lighter(150));
    lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
    lightPalette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
    lightPalette.setColor(QPalette::Disabled, QPalette::Text, gray);
    lightPalette.setColor(QPalette::Disabled, QPalette::Light, lightGray.darker(150));

    return lightPalette;
}

} // namespace DialogueFromVideo :: Palettes
