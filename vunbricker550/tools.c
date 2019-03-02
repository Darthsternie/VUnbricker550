#include <pspsdk.h>
#include <pspkernel.h>
#include <pspiofilemgr.h>
#include <psputils.h>

#include <pspusb.h>
#include <pspusbstor.h>
#include <pspusbdevice.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <vlf.h>
#include "./h/main.h"

#define USB_DISCONNECT -1
#define USB_MEMORY_STICK 0
#define USB_FLASH0 1
#define USB_FLASH1 2
#define USB_FLASH2 3
#define USB_FLASH3 4

static int OnBackToMainMenu(int enter){
	if(!enter){
		vlfGuiCancelCentralMenu();
		vlfGuiCancelBottomDialog();
		MainMenu(5);		
	}
	return VLF_EV_RET_NOTHING;
}

int OnBackToToolsMenu(int enter){
	if(!enter){
		vlfGuiCancelCentralMenu();
		vlfGuiCancelBottomDialog();
		ToolsMenu();		
	}
	return VLF_EV_RET_NOTHING;
}

void LoadStartModule(const char* file){
	SceUID mod = sceKernelLoadModule(file, 0, NULL);
	if(mod >= 0){
		mod = sceKernelStartModule(mod, 0, NULL, NULL, NULL);
	}else{
		char tmp[128]=""; sprintf(tmp, "Cannot load module \"%s\": 0x%08X", file, mod);
		vlfGuiMessageDialog(tmp, VLF_MD_TYPE_ERROR | VLF_MD_BUTTONS_NONE);
	}
}

int usb_init = 0;
void ToggleUsb(int which){
	if(!usb_init){
		LoadStartModule("flash0:/kd/semawm.prx");
		LoadStartModule("flash0:/kd/usbstor.prx");
		LoadStartModule("flash0:/kd/usbstormgr.prx");
		LoadStartModule("flash0:/kd/usbstorms.prx");
		LoadStartModule("flash0:/kd/usbstorboot.prx");
		LoadStartModule("flash0:/kd/usbdevice.prx");
//		sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
//		sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
//		sceUsbstorBootSetCapacity(0x800000);
		usb_init = 1;
	}

	int devices[] = {
		-1,
		PSP_USBDEVICE_FLASH0,
		PSP_USBDEVICE_FLASH1,
		PSP_USBDEVICE_FLASH2,
		PSP_USBDEVICE_FLASH3
	};

	if(which < 0){
//		sceUsbDeactivate(0x1c8);
//		sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
//		sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
//		pspUsbDeviceFinishDevice();
	}else{
		ToggleUsb(-1); //Disable USB
		if(which >= 1){
//			pspUsbDeviceSetDevice(devices[which], 0, which);
		}
//		sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
//		sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
//		sceUsbActivate(0x1c8);
	}
}

int OnUsbMenuSelect(int sel){
	switch (sel){
		case 0:
			ToggleUsb(USB_MEMORY_STICK);
			break;

		case 1:
			ToggleUsb(USB_FLASH0);
			break;

		case 2:
			ToggleUsb(USB_FLASH1);
			break;

		case 3:
			ToggleUsb(USB_FLASH2);
			break;

		case 4:
			ToggleUsb(USB_FLASH3);
			break;

		case 5:
			ToggleUsb(USB_DISCONNECT);
			break;
	}
	return VLF_EV_RET_NOTHING;
}

void UsbMenu(){
	char *items[] =	{
		"Toggle MemoryStick",
		"Toggle Flash0",
		"Toggle Flash1",
		"Toggle Flash2",
		"Toggle Flash3",
		"Disconnect"
	};

	vlfGuiCentralMenu(6, items, 0, OnUsbMenuSelect, 0, 0);
	vlfGuiBottomDialog(VLF_DI_CANCEL, VLF_DI_ENTER, 1, 0, VLF_DEFAULT, OnBackToToolsMenu);
}

int OnToolsMenuSelect(int sel){
	switch (sel){
		case 0:
			vlfGuiCancelCentralMenu();
			vlfGuiCancelBottomDialog();
			UsbMenu();
			return VLF_EV_RET_NOTHING;
			break;
	}
	return VLF_EV_RET_NOTHING;
}

void ToolsMenu(){
	char *items[] =	{
		"USB"
	};

	vlfGuiCentralMenu(1, items, 0, OnToolsMenuSelect, 0, 0);
	vlfGuiBottomDialog(VLF_DI_CANCEL, VLF_DI_ENTER, 1, 0, VLF_DEFAULT, OnBackToMainMenu);
}
