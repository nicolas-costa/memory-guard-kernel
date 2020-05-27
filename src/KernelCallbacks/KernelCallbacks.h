#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once	
#endif // (_MSC_VER) && (_MSC_VER >= 1020)

#ifndef _KernelCallbacks_h__
#define _KernelCallbacks_h__

//
// Defines
//

#define IOCTL_UNKNOWN_BASE FILE_DEVICE_UNKNOWN

#define IOCTL_MGK_ACTIVATE_MONITORING CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_MGK_GET_PROCESS_INFO CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_MGK_CLEAR_PROCESS_INFO CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0802, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define DEVICE_EXTENSION_SIZE (sizeof(PROCESS_CALLBACK_INFO) * 110)

//----------------------------------------


//
// Structure used for activation and deactivation of the process monitoring mechanism
//

typedef struct _ActivateInfo
{
	BOOLEAN  bActivated;
}ACTIVATE_INFO, *PACTIVATE_INFO;


//
// Device Extension structure
//

typedef struct tagDEVICE_EXTENSION
{

	LONG lEntry_Count;

	PDEVICE_OBJECT psDeviceObject;

	FAST_MUTEX sFMutex;

	PKEVENT sProcessEvent;

	LIST_ENTRY sHead;

	ACTIVATE_INFO sActivateInfo;

}DEVICE_EXTENSION,*PDEVICE_EXTENSION;


//
// Process Information Structure
//

typedef struct tagPROCESSCALLBACKINFO
{

	HANDLE hParent;
	
	HANDLE hProcess;

	BOOLEAN bCreate;

	LIST_ENTRY Node;

}PROCESS_CALLBACK_INFO,*PPROCESS_CALLBACK_INFO;


// Callback responsible for getting process creation/termination information

VOID 
	ProcessNotify(
		IN HANDLE hParent, 
			IN HANDLE hProcess, 
				IN BOOLEAN bCreate
					);


#endif // _KernelCallbacks_h__
