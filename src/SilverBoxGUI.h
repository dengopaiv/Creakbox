
#ifndef __SILVERBOXGUI_H
#define __SILVERBOXGUI_H

#include "SilverBox.h"
#include <string.h>

class SilverBoxGUI : public SilverBox
{
public:
	SilverBoxGUI (audioMasterCallback audioMaster);
	~SilverBoxGUI ();

	virtual void setParameter (long index, float value);
};

#endif
