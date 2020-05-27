#include "src/stdafx.h"


PDEVICE_OBJECT g_pDeviceObject;

NTSTATUS DriverEntry(IN PDRIVER_OBJECT psDriverObject, IN PUNICODE_STRING puszRegPath)
{

	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	UNICODE_STRING uszDeviceName, uszDriverName;/*, uszEventName;*/
	PDEVICE_OBJECT psDeviceObject;
	PDEVICE_EXTENSION psExtension;
	//HANDLE hEvent;

	RtlInitUnicodeString(&uszDriverName,L"\\Device\\MGK");

	RtlInitUnicodeString(&uszDeviceName,L"\\DosDevices\\MGK");

	ntStatus = IoCreateDevice(psDriverObject,
							  sizeof(DEVICE_EXTENSION),
							  &uszDriverName,
							  FILE_DEVICE_UNKNOWN,
							  (ULONG)NULL,
							  FALSE,
							  &psDeviceObject);

	if(ntStatus != STATUS_SUCCESS)
	{
		return ntStatus;
	}

	ntStatus = IoCreateSymbolicLink(&uszDeviceName,
									&uszDriverName);

	if(ntStatus != STATUS_SUCCESS)
	{
		DbgPrint("MGK ERROR: SYMBOLIC LINK CREATION");

		IoDeleteDevice(psDeviceObject);

		return ntStatus;
	}

	// Setup our Dispatch routines

	psDriverObject->DriverUnload							= PFKUnload;
	psDriverObject->MajorFunction[IRP_MJ_CREATE]			= PFKDIspatchCreateClose;
	psDriverObject->MajorFunction[IRP_MJ_CLOSE]				= PFKDIspatchCreateClose;
	psDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]	= PFKDispatchDeviceControl;

	psExtension = (PDEVICE_EXTENSION)psDeviceObject->DeviceExtension;

	psExtension->psDeviceObject = psDeviceObject;

	// init global device object pointer

	g_pDeviceObject = psDeviceObject;

	// Init our internal synchronization mechanism

	ExInitializeFastMutex(&psExtension->sFMutex);

	// Set our callback as deactivated

	psExtension->sActivateInfo.bActivated = FALSE;

	// Set our process notify data counter as zero

	psExtension->lEntry_Count = 0;

	// Init our DLinked-List Head

	InitializeListHead(&psExtension->sHead);

	return ntStatus;

}