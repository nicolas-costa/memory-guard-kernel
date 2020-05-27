#include "src/stdafx.h"


NTSTATUS GetDeviceExtPointer(IN PDEVICE_EXTENSION *pDeviceExt)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	UNICODE_STRING uszDeviceName;
	PDEVICE_OBJECT psDevice = NULL;
	PFILE_OBJECT sFileObj;

	if(pDeviceExt == NULL)
	{
		return ntStatus;
	}

	RtlInitUnicodeString(&uszDeviceName,L"\\DosDevices\\MGK");
	
	ntStatus = IoGetDeviceObjectPointer(&uszDeviceName, FILE_GENERIC_READ,&sFileObj,&psDevice);

	if(ntStatus != STATUS_SUCCESS)
	{
		*pDeviceExt = NULL;

		return ntStatus;
	}
	else 
	{
		*pDeviceExt = (PDEVICE_EXTENSION)psDevice->DeviceExtension;

		return ntStatus;
	}

}

NTSTATUS ActivateProcessMonitoring(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	
	PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(pIrp);

	if(irpStack->Parameters.DeviceIoControl.InputBufferLength >= sizeof(ACTIVATE_INFO))
	{

		PACTIVATE_INFO psActivateInfo = NULL;
		PDEVICE_EXTENSION psDeviceExt = NULL;

		psActivateInfo = (PACTIVATE_INFO)pIrp->AssociatedIrp.SystemBuffer;

		psDeviceExt = (PDEVICE_EXTENSION)pDeviceObject->DeviceExtension;

		if(psDeviceExt->sActivateInfo.bActivated != psActivateInfo->bActivated )
		{
			if(psActivateInfo->bActivated)
			{
				ntStatus = PsSetCreateProcessNotifyRoutine(ProcessNotify,FALSE);

				if(ntStatus != STATUS_SUCCESS)
				{
					return ntStatus;
				}

				psDeviceExt->sActivateInfo.bActivated = psActivateInfo->bActivated;
			}
			else
			{
				ntStatus = PsSetCreateProcessNotifyRoutine(ProcessNotify,TRUE);

				if(ntStatus != STATUS_SUCCESS)
				{
					return ntStatus;
				}
				else
				{
					psDeviceExt->sActivateInfo.bActivated = FALSE;
				}
			}
		}
	}
	return ntStatus;
}

NTSTATUS SendEntriesToUserLand(PDEVICE_EXTENSION psDevExt, PVOID pSysBuffer)
{

	LONG lCount = (LONG)NULL;
	PPROCESS_CALLBACK_INFO pKernelBuffer = NULL, pUserBuffer = (PPROCESS_CALLBACK_INFO)pSysBuffer;
	PLIST_ENTRY psCurrentEntry = NULL;

	RtlZeroMemory(pSysBuffer,sizeof(DEVICE_EXTENSION_SIZE));

	psCurrentEntry = psDevExt->sHead.Flink;

	ExAcquireFastMutex(&psDevExt->sFMutex);

	while(psCurrentEntry != &psDevExt->sHead)
	{
		PLIST_ENTRY psDamnedEntry = NULL;
		PPROCESS_CALLBACK_INFO pDamnedEntry = NULL;

		pKernelBuffer = (PPROCESS_CALLBACK_INFO) CONTAINING_RECORD(psCurrentEntry,PROCESS_CALLBACK_INFO,Node);

		RtlCopyMemory(&pUserBuffer[lCount],pKernelBuffer,sizeof(PROCESS_CALLBACK_INFO));

		// erase the DLink list info

		RtlZeroMemory(&pUserBuffer[lCount].Node,sizeof(LIST_ENTRY));

		psDamnedEntry = psCurrentEntry;

		psCurrentEntry = psCurrentEntry->Flink;

		RemoveEntryList(psDamnedEntry);

		pDamnedEntry = pKernelBuffer;

		ExFreePool(pDamnedEntry);

		InterlockedDecrement(&psDevExt->lEntry_Count);

		lCount++;

	}

	ExReleaseFastMutex(&psDevExt->sFMutex);

	RtlFillMemory(&pUserBuffer[lCount],(DEVICE_EXTENSION_SIZE - (sizeof(PROCESS_CALLBACK_INFO) * lCount)),'�');

	return STATUS_SUCCESS;

}

NTSTATUS ValidateUserEvent()
{
	return STATUS_SUCCESS;
}

VOID ClearNotificationList(PDEVICE_EXTENSION psDevExt)
{
	PLIST_ENTRY psEntry = psDevExt->sHead.Flink;

	if(!IsListEmpty(&psDevExt->sHead))
	{
		while(psEntry != &psDevExt->sHead)
		{
			PPROCESS_CALLBACK_INFO psProcessInfoToRemove = NULL;
			PLIST_ENTRY psListEntryToBeRemoved = NULL;

			psProcessInfoToRemove = (PPROCESS_CALLBACK_INFO) CONTAINING_RECORD(psEntry,PROCESS_CALLBACK_INFO,Node);

			psEntry = psEntry->Flink;

			psListEntryToBeRemoved = psEntry;

			RemoveEntryList(psListEntryToBeRemoved);

			ExFreePool(psProcessInfoToRemove);

			psDevExt->lEntry_Count--;


		}
	}
	else return;
}