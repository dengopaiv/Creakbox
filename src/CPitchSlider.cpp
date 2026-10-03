
#include "CPitchSlider.h"


//2.1
//------------------------------------------------------------------------
// CVerticalSlider
//------------------------------------------------------------------------
CPitchSlider::CPitchSlider(const CRect &rect, CControlListener *listener, long tag,
                  CPoint   &offsetHandle,    // handle offset
                  long     _rangeHandle, // size of handle range
                  CBitmap  *handle,     // bitmap of slider
                  CBitmap  *background, // bitmap of background
                  CPoint   &offset,     // offset in the background
                  const long style)     // style (kBottom,kRight,kTop,kLeft,kHorizontal,kVertical)
  :	CSlider(rect, listener, tag, offsetHandle, _rangeHandle, handle, background, offset, style)
{
	iPos = 0;
}

//------------------------------------------------------------------------
CPitchSlider::~CPitchSlider()
{
}
//------------------------------------------------------------------------
void CPitchSlider::draw (CDrawContext *pContext)
{
	float fValue;
	if (style & kLeft || style & kTop)
		fValue = value;
	else 
		fValue = 1.f - value;
	
	// (re)draw background
	CRect rect (0, 0, widthControl, heightControl);
	if (pBackground)
	{
		if (bTransparencyEnabled)
			pBackground->drawTransparent (pOScreen, rect, offset);
		else
			pBackground->draw (pOScreen, rect, offset);
	}
	
	// calc new coords of slider
	CRect   rectNew;
	if (style & kHorizontal)
	{
		rectNew.top    = offsetHandle.v;
		rectNew.bottom = rectNew.top + heightOfSlider;	

		rectNew.left   = offsetHandle.h + (int)(fValue * rangeHandle);
		rectNew.left   = (rectNew.left < minTmp) ? minTmp : rectNew.left;

		rectNew.right  = rectNew.left + widthOfSlider;
		rectNew.right  = (rectNew.right > maxTmp) ? maxTmp : rectNew.right;
	}
	else
	{
	        rectNew.left   = offsetHandle.h;
	        rectNew.right  = rectNew.left + widthOfSlider;	
        
	        rectNew.top    = offsetHandle.v + iPos + size.top;//offsetHandle.v + (int)(fValue * rangeHandle);
	        rectNew.top    = (rectNew.top < minTmp) ? minTmp : rectNew.top;
        
	        rectNew.bottom = rectNew.top + heightOfSlider;
	        rectNew.bottom = (rectNew.bottom > maxTmp) ? maxTmp : rectNew.bottom;        
	        rectNew.top = iPos + offsetHandle.v ;
	        rectNew.right = 27 + rectNew.left; rectNew.bottom = rectNew.top + 14;
        }

	// draw slider at new position
	if (pHandle)
	{
		if (bDrawTransparentEnabled)
			pHandle->drawTransparent (pOScreen, rectNew);
		else 
			pHandle->draw (pOScreen, rectNew);
	}

	pOScreen->copyFrom (pContext, size);

	setDirty (false);
}

//------------------------------------------------------------------------
void CPitchSlider::mouse(CDrawContext *pContext, CPoint &where, long button)
{
	if (!bMouseEnabled)
		return;

	if (button == -1) button = pContext->getMouseButtons ();

	// set the default value
	if (button == (kControl|kLButton))
	{
		value = getDefaultValue ();
		if (isDirty () && listener)
			listener->valueChanged (pContext, this);
		return;
	}
	
	// allow left mousebutton only
	if (!(button & kLButton))
		return;

	long delta;
	if (style & kHorizontal)
		delta = size.left + offsetHandle.h;
	else
		delta = size.top + offsetHandle.v;
	if (!bFreeClick)
	{
		float fValue;
		if (style & kLeft || style & kTop)
			fValue = value;
		else 
			fValue = 1.f - value;
		long actualPos;
		CRect rect;

		if (style & kHorizontal)
		{
			actualPos = offsetHandle.h + (int)(fValue * rangeHandle) + size.left;

			rect.left   = actualPos;
			rect.top    = size.top  + offsetHandle.v;
			rect.right  = rect.left + widthOfSlider;
			rect.bottom = rect.top  + heightOfSlider;

			if (!where.isInside (rect))
				return;
			else
				delta += where.h - actualPos;
		}
		else
		{
			actualPos = offsetHandle.v + (int)(fValue * rangeHandle) + size.top;
		
			rect.left   = size.left  + offsetHandle.h;
			rect.top    = actualPos;
		rect.right  = rect.left + widthOfSlider;
		rect.bottom = rect.top  + heightOfSlider;

		if (!where.isInside (rect))
			return;
		else
				delta += where.v - actualPos;
	} 
	} 
	else
	{
		if (style & kHorizontal)
			delta += widthOfSlider / 2 - 1;
	else
			delta += heightOfSlider / 2 - 1;
	}
	
	float oldVal    = value;
	long   oldButton = button;

	// begin of edit parameter
	getParent ()->beginEdit (tag);
	getParent ()->setEditView (this);

	while (1)
	{
		button = pContext->getMouseButtons ();
		if (!(button & kLButton))
			break;

		if ((oldButton != button) && (button & kShift))
		{
			oldVal = value;
			oldButton = button;
		}
		else if (!(button & kShift))
			oldVal = value;

		int newPos = where.v - size.top;
//		if (style & kBottom)
//			newPos = 60 - newPos;
		if (newPos < 0) newPos = 0;
		if (newPos > 60) newPos = 60;
		if (iPos != newPos)
		{
			iPos = newPos;
			setDirty(true);
			listener->valueChanged (pContext, this);
		}

		if (style & kHorizontal)
			value = (float)(where.h - delta) / (float)rangeHandle;
		else
			value = (float)(where.v - delta) / (float)rangeHandle;
		
		if (style & kRight || style & kBottom)
			value = 1.f - value;
		
		if (button & kShift)
			value = oldVal + ((value - oldVal) / zoomFactor);
		bounceValue ();
    	    
		if (isDirty () && listener)
			listener->valueChanged (pContext, this);

		pContext->getMouseLocation (where);

		doIdleStuff ();
	}

	// end of edit parameter
	getParent ()->endEdit (tag);
}

//------------------------------------------------------------------------
int CPitchSlider::getPos()
{
	return 60 - iPos;
}
//------------------------------------------------------------------------
void CPitchSlider::setPos(int pos)
{
	int newPos = 60 - pos;
	if (newPos < 0) newPos = 0;
	if (newPos > 60) newPos = 60;
	if (iPos != newPos)
	{
		iPos = newPos;
		setDirty(true);
	}
}
