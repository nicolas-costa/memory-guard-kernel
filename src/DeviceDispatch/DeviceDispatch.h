#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif // (_MSC_VER) && (_MSC_VER >= 1020)

#ifndef _DeviceDispatch_h__
#define _DeviceDispatch_h__

#define PROCESS_ARRAY_SIZE sizeof(PROCESS_CALLBACK_INFO) * 110)

DRIVER_DISPATCH PFKDIspatchCreateClose;

DRIVER_DISPATCH PFKDispatchDeviceControl;

DRIVER_UNLOAD PFKUnload;


#endif // _DeviceDispatch_h__
