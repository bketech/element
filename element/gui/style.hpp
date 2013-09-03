
#ifndef BKE_STYLE_HPP
#define BKE_STYLE_HPP

#include "element/juce.hpp"

namespace element {
namespace gui {

    class Style :  public LookAndFeel
    {
    public:

        Style();
        virtual ~Style();

        static void drawShinyButtonShape (Graphics& g,     float x, float y, float w, float h,
                                                           float maxCornerSize,
                                                           const Colour& baseColour,
                                                           const float strokeWidth,
                                                           const bool flatOnLeft,
                                                           const bool flatOnRight,
                                                           const bool flatOnTop,
                                                           const bool flatOnBottom) noexcept;

        void drawPopupMenuBackground (Graphics& g, int width, int height);

        virtual void drawComboBox (Graphics&, int width, int height,
                                   bool isButtonDown,
                                   int buttonX, int buttonY,
                                   int buttonW, int buttonH,
                                   ComboBox& box);

        void fillWithBackgroundTexture (Graphics&);
        static void fillWithBackgroundTexture (Component&, Graphics&);

        int getTabButtonOverlap (int tabDepth);
        int getTabButtonSpaceAroundImage();
        int getTabButtonBestWidth (TabBarButton& button, int tabDepth);
        static Colour getTabBackgroundColour (TabBarButton& button);
        void drawTabButton (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown);

        Rectangle<int> getTabButtonExtraComponentBounds (const TabBarButton& button, Rectangle<int>& textArea, Component& comp);
        void drawTabAreaBehindFrontButton (TabbedButtonBar&, Graphics&, int, int) {}

        void drawStretchableLayoutResizerBar (Graphics& g, int /*w*/, int /*h*/, bool /*isVerticalBar*/, bool isMouseOver, bool isMouseDragging);
        Rectangle<int> getPropertyComponentContentPosition (PropertyComponent&);

        bool areScrollbarButtonsVisible()   { return false; }

        void drawScrollbar (Graphics& g, ScrollBar& scrollbar, int x, int y, int width, int height, bool isScrollbarVertical,
                            int thumbStartPosition, int thumbSize, bool /*isMouseOver*/, bool /*isMouseDown*/);

        void drawConcertinaPanelHeader (Graphics& g, const Rectangle<int>& area,
                                        bool isMouseOver, bool isMouseDown,
                                        ConcertinaPanel& concertina, Component& panel);

        void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                                   bool isMouseOverButton, bool isButtonDown);

        static Colour getScrollbarColourForBackground (Colour background);


    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Style);

    };
}}

#endif /* BKE_STYLE_HPP */
