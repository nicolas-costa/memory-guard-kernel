#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifdef __cplusplus
extern "C" 
{

#endif

// Standard DDK Headers

#include <ntddk.h>
#include <wdm.h>
#include <ntddstor.h>
#include <mountdev.h>
#include <ntddvol.h>


// Project Specific
#include "src/KernelCallbacks/KernelCallbacks.h"
#include "src/IoManagement/IoManagement.h"
#include "src/DeviceDispatch/DeviceDispatch.h"
#include "EntryPoint.h"


#ifdef __cplusplus
}
#endif
