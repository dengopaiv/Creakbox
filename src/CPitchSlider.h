#ifndef __CPitchSlider__
#define __CPitchSlider__

#include "vstcontrols.h"

class CPitchSlider : public CSlider
{
public:
	CPitchSlider(const CRect &rect, CControlListener *listener, long tag,
                  CPoint   &offsetHandle,    // handle offset
                  long     _rangeHandle, // size of handle range
                  CBitmap  *handle,     // bitmap of slider
                  CBitmap  *background, // bitmap of background
                  CPoint   &offset,     // offset in the background
                  const long style);     // style (kBottom,kRight,kTop,kLeft,kHorizontal,kVertical)

	~CPitchSlider();
	virtual void draw (CDrawContext*);
	virtual void mouse (CDrawContext *pContext, CPoint &where, long button = -1);
	int getPos();
	void setPos(int pos);

protected:

	int iPos;
};

#endif