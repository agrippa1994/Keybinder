#ifndef TEXTOVERLAY_H
#define TEXTOVERLAY_H
#include "Overlay.h"

class TextOverlay : public OverlayBase
{
public:
    int init(const char *Font, int FontSize, bool bBold, bool bItalic, int x, int y, unsigned int color, const char *text, bool bShadow, bool bShow)
    {
        _id = TextCreate(Font, FontSize, bBold, bItalic, x, y, color, text, bShadow, bShow);
        return _id;
    }

    ~TextOverlay()
    {
        destroy();
    }

    int destroy()
    {
        auto ret = TextDestroy(_id);
        _id = -1;
        return ret;
    }

    int setShadow(bool b)
    {
        return TextSetShadow(_id, b);
    }

    int setShown(bool b)
    {
        return TextSetShown(_id, b);
    }

    int setColor(unsigned int color)
    {
        return TextSetColor(_id, color);
    }

    int setPos(int x, int y)
    {
        return TextSetPos(_id, x, y);
    }

    int setString(const char *str)
    {
        return TextSetString(_id, str);
    }

    int update(const char *Font, int FontSize, bool bBold, bool bItalic)
    {
        return TextUpdate(_id, Font, FontSize, bBold, bItalic);
    }
};

#endif // TEXTOVERLAY_H
