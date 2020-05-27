#include "src/stdafx.h"


NTSTATUS PFKDIspatchCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS PFKDispatchDeviceControl(IN PDEVICE_OBJECT psDeviceObject, IN PIRP pIrp)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	PIO_STACK_LOCATION psIO_Stack_Location = IoGetCurrentIrpStackLocation(pIrp);
	PDEVICE_EXTENSION psExtension = (PDEVICE_EXTENSION)psDeviceObject->DeviceExtension;

	switch(psIO_Stack_Location->Parameters.DeviceIoControl.IoControlCode)
	{
		case IOCTL_MGK_ACTIVATE_MONITORING:
		{
			ntStatus = ActivateProcessMonitoring(psDeviceObject,pIrp);
			break;
		}
		case IOCTL_MGK_GET_PROCESS_INFO:
		{
			if(psIO_Stack_Location->Parameters.DeviceIoControl.OutputBufferLength >= DEVICE_EXTENSION_SIZE)
			{
	
				if(!IsListEmpty(&psExtension->sHead))
				{

					ntStatus = SendEntriesToUserLand(psExtension,pIrp->AssociatedIrp.SystemBuffer);

				}
				else ntStatus = STATUS_SUCCESS;
			}
			break;
		}
		case IOCTL_MGK_CLEAR_PROCESS_INFO:
		{
			if(!IsListEmpty(&psExtension->sHead))
			{

				ClearNotificationList(psExtension);

			}
			ntStatus = STATUS_SUCCESS;
			break;
		}
	}
	
	pIrp->IoStatus.Status = ntStatus;

	if(ntStatus == STATUS_SUCCESS)
	{
		pIrp->IoStatus.Information = psIO_Stack_Location->Parameters.DeviceIoControl.OutputBufferLength;
	}
	else pIrp->IoStatus.Information = 0;

	IoCompleteRequest(pIrp,IO_NO_INCREMENT);

	return ntStatus;
}

VOID PFKUnload(IN PDRIVER_OBJECT psDriverObject)
{

	UNICODE_STRING uszDeviceName;

	PDEVICE_EXTENSION psExtension = (PDEVICE_EXTENSION)psDriverObject->DeviceObject->DeviceExtension;

	if(psExtension->sActivateInfo.bActivated)
	{
		PsSetCreateProcessNotifyRoutine(ProcessNotify,TRUE);
	}

	ClearNotificationList(psExtension);

	IoDeleteDevice(psDriverObject->DeviceObject);

	RtlInitUnicodeString(&uszDeviceName,L"\\DosDevices\\MGK");

	IoDeleteSymbolicLink(&uszDeviceName);

}
