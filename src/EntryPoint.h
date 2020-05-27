#if defined (_MSC_VER) &&(_MSC_VER >= 1020)
#pragma once
#endif // (_MSC_VER) &&(_MSC_VER >= 1020)

#ifndef _EntryPoint_h__
#define _EntryPoint_h__

extern PDEVICE_OBJECT g_pDeviceObject;

DRIVER_INITIALIZE DriverEntry;

#endif // _EntryPoint_h__


