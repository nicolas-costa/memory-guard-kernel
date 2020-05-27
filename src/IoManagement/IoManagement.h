#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif // (_MSC_VER) && (_MSC_VER >= 1020)

#ifndef _IoManagement_h__
#define _IoManagement_h__

NTSTATUS 
	ActivateProcessMonitoring(
		IN PDEVICE_OBJECT pDeviceObject,
			IN PIRP pIrp
				);




#pragma deprecated( /*NTSTATUS*/ GetDeviceExtPointer/*( IN PDEVICE_EXTENSION *pDeviceExt )*/ )

NTSTATUS 
	SendEntriesToUserLand(
		IN PDEVICE_EXTENSION psDevExt,
			IN PVOID pSysBuffer
				);

VOID 
	ClearNotificationList(
		PDEVICE_EXTENSION psDevExt
			);



#endif // _IoManagement_h__
