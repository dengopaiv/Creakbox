#include "SilverBoxGUI.h"

#ifndef __SILVERBOX_EDITOR__
#include "SilverBoxEditor.h"
#endif

#include <string.h>

extern bool oome;

//-----------------------------------------------------------------------------
SilverBoxGUI::SilverBoxGUI (audioMasterCallback audioMaster)
 : SilverBox (audioMaster)
{
//	setUniqueID ('ADlE');
	editor = new SilverBoxEditor (this);
	if (!editor)
		oome = true;
}

//-----------------------------------------------------------------------------
SilverBoxGUI::~SilverBoxGUI ()
{
	// the editor gets deleted by the
	// AudioEffect base class
}

//-----------------------------------------------------------------------------
void SilverBoxGUI::setParameter (long index, float value)
{
	SilverBox::setParameter (index, value);

	if (editor)
		((AEffGUIEditor*)editor)->setParameter (index, value);
}
