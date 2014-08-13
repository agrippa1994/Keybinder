#ifndef BOXOVERLAY_H
#define BOXOVERLAY_H
#include "Overlay.h"

class BoxOverlay : public OverlayBase
{
public:
    int init(int x, int y, int w, int h, unsigned int dwColor, bool bShow)
    {
        _id = BoxCreate(x, y, w, h, dwColor, bShow);
        return _id;
    }

    ~BoxOverlay()
    {
        destroy();
    }

    int destroy()
    {
        auto ret = BoxDestroy(_id);
        _id = -1;
        return ret;
    }

    int setShown(bool bShown)
    {
        return BoxSetShown(_id, bShown);
    }

    int setBorder(int height, bool bShown)
    {
        return BoxSetBorder(_id, height, bShown);
    }

    int setBorderColor(unsigned int dwColor)
    {
        return BoxSetBorderColor(_id, dwColor);
    }

    int setColor(unsigned int dwColor)
    {
        return BoxSetColor(_id, dwColor);
    }

    int setHeight(int height)
    {
        return BoxSetHeight(_id, height);
    }

    int setPos(int x, int y)
    {
        return BoxSetPos(_id, x, y);
    }

    int setWidth(int width)
    {
        return BoxSetWidth(_id, width);
    }
};

#endif // BOXOVERLAY_H
