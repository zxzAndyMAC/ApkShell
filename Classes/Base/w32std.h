// w32std.h
//
// Created 2005-04-08, ER1C
//

#ifndef __WSTD_H__
#define __WSTD_H__

#include "Base/e32std.h"

struct TKeyEvent
{
	TUint iCode;
	TInt iScanCode;
	TUint iModifiers;	// State of modifier keys and pointing device
	TInt iRepeats;		// Count of auto repeats generated
};

enum TEventCode
{
	EEventNull,
	EEventKey,
	EEventKeyUp,
	EEventKeyDown,
	EEventModifiersChanged,
	EEventPointer,			//5
	EEventPointerEnter,
	EEventPointerExit,
	EEventPointerBufferReady,
	EEventDragDrop,
	EEventFocusLost,		//10
	EEventFocusGained,
	EEventSwitchOn,
	EEventPassword,
	EEventWindowGroupsChanged,
	EEventErrorMessage,		//15
	EEventMessageReady,
	EEventMarkInvalid,	// For internal use only
	EEventSwitchOff,
	EEventKeySwitchOff,
	EEventScreenDeviceChanged, //20
	EEventFocusGroupChanged,
	EEventCaseOpened,
	EEventCaseClosed,
	EEventUser=1000,	// Event codes from EEventUser upwards may be used for non-wserv events
						// No event codes below this should be defined except by the window server
};

struct TPointerEvent
{
	enum TType
	{
		EButton1Down,
		EButton1Up,
		EButton2Down,
		EButton2Up,
		EButton3Down,
		EButton3Up,
		EDrag,
		EMove,
		EButtonRepeat,
		ESwitchOn,
	};
	TType iType;	// Type of pointer event
	TUint iModifiers;	// State of pointing device and associated buttons
	TPoint iPosition; 	// Window co-ordinates of mouse event
	TPoint iParentPosition; 	// position relative to parent window
};

#endif	//__WSTD_H__
