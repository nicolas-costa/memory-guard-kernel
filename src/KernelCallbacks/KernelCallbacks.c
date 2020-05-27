#include "src/stdafx.h"


VOID ProcessNotify(IN HANDLE hParent, IN HANDLE hProcess, IN BOOLEAN bCreate)
{
	PDEVICE_EXTENSION pDevExt = NULL;
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	PPROCESS_CALLBACK_INFO psProcCallbackInfo = NULL;

	pDevExt = (PDEVICE_EXTENSION)g_pDeviceObject->DeviceExtension;

	psProcCallbackInfo = (PPROCESS_CALLBACK_INFO) ExAllocatePoolWithTag(PagedPool,sizeof(PROCESS_CALLBACK_INFO),(ULONG)0);

	if(psProcCallbackInfo != NULL)
	{

		RtlZeroMemory(psProcCallbackInfo,sizeof(PROCESS_CALLBACK_INFO));

		psProcCallbackInfo->bCreate = bCreate;

		psProcCallbackInfo->hParent = hParent;

		psProcCallbackInfo->hProcess = hProcess;

		// use a thread-safe, multiprocessor safe sync mechanism
		// to insert process notification data...

		ExAcquireFastMutex(&pDevExt->sFMutex);

		InsertTailList(&pDevExt->sHead,&psProcCallbackInfo->Node);

		// and increment the counter

		InterlockedIncrement(&pDevExt->lEntry_Count);

		ExReleaseFastMutex(&pDevExt->sFMutex);

		//ObDereferenceObject(pExt->psDeviceObject);

		return;

	}
	else
	{

		DbgPrint("ALLOC FAIL: PID %d",hProcess);

		return;
	}

}