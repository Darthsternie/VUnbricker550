#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsuspend.h>
#include <psppower.h>
#include <pspreg.h>
#include <systemctrl.h>
#include <pspusb.h>
#include <pspusbstor.h>
#include <pspusbdevice.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <vlf.h>

#include "./h/main.h"
#include "./h/dcman.h"

extern int status;
int usbStarted=0;
char dev[12];
int pic;

int LoadStartModule(char *module)
{
	SceUID mod = sceKernelLoadModule(module, 0, NULL);

	if (mod < 0 && mod != SCE_KERNEL_ERROR_EXCLUSIVE_LOAD)
	return mod;

	if (mod >= 0)
	{
		mod = sceKernelStartModule(mod, strlen(module)+1, module, NULL, NULL);
		if (mod < 0)
			return mod;
	}

	return 0;
}

void enableUsbStore()
{
	LoadStartModule("flash0:/kd/semawm.prx");
	LoadStartModule("flash0:/kd/usbstor.prx");
	LoadStartModule("flash0:/kd/usbstormgr.prx");
	LoadStartModule("flash0:/kd/usbstorms.prx");
	LoadStartModule("flash0:/kd/usbstorboot.prx");

	sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
	sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
	sceUsbstorBootSetCapacity(0x800000);
	sceUsbActivate(0x1c8);
	usbStarted = 1;

	status = vlfGuiAddTextF(0, 0, "Connexion USB %s",dev);
	vlfGuiSetTitleBar(status, -1, 1, 0);
	vlfGuiRemovePicture(pic);
	pic = vlfGuiAddPictureResource("sysconf_plugin", "tex_bar_usb_icon", 4, -2);
}

void DisableUsbStore()
{
	if (usbStarted)
	{
	    sceUsbDeactivate(0x1c8);
	    sceUsbStop(PSP_USBSTOR_DRIVERNAME,0,0);
	    sceUsbStop(PSP_USBBUS_DRIVERNAME,0,0);
		usbStarted = 0;
		vlfGuiRemoveText(status);
		vlfGuiSetTitleBarVisibility(0);
		vlfGuiRemovePicture(pic);
		pspUsbDeviceFinishDevice();
	}
}

int OnUsbOperationsSelect(int sel)
{
	LoadStartModule("flash0:/kd/usbdevice.prx");
	int res;

	switch (sel)
	{
		case 0:
			if(usbStarted == 0)
				{
				  sprintf(dev,"MS");
				  enableUsbStore();
				} else {
		          DisableUsbStore();
		       }
			return VLF_EV_RET_NOTHING;
		break;

		case 1:
			if(!usbStarted) {
			    sprintf(dev,"Flash0");
				res = dcPatchModuleString("pspUsbDev_Driver", "flash", "flach");
	            if(res < 0){vlfGuiMessageDialog("Impossible de patcher le module flash0:/kd/usbdevice.prx", 0);}
				pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH0, 0, NULL);
				enableUsbStore();
			} else {
				DisableUsbStore();
			}
			return VLF_EV_RET_NOTHING;
		break;

		case 2:
		    res = dcPatchModuleString("pspUsbDev_Driver", "flash", "flach");
	        if(res < 0){vlfGuiMessageDialog("Impossible de patcher le module flash0:/kd/usbdevice.prx", 0);}
		    else
			{
			    if(!usbStarted){
			        sprintf(dev,"Flash1");
				    pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH1, 0, NULL);
				    enableUsbStore();
			    } else{
				    DisableUsbStore();
			    }
			}
			return VLF_EV_RET_NOTHING;
		break;

		case 3:
		    if(!usbStarted){
			    sprintf(dev,"Flash2");
				res = dcPatchModuleString("pspUsbDev_Driver", "flash", "flach");
	            if(res < 0){vlfGuiMessageDialog("Impossible de patcher le module flash0:/kd/usbdevice.prx", 0);}

				pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH2, 0, NULL);
				enableUsbStore();
			} else{
				DisableUsbStore();
			}
			return VLF_EV_RET_NOTHING;
		break;

		case 4:
		    if(!usbStarted){
			    sprintf(dev,"Flash3");
				res = dcPatchModuleString("pspUsbDev_Driver", "flash", "flach");
	            if(res < 0){vlfGuiMessageDialog("Impossible de patcher le module flash0:/kd/usbdevice.prx", 0);}

				pspUsbDeviceSetDevice(PSP_USBDEVICE_FLASH3, 0, NULL);
				enableUsbStore();
			} else{
				DisableUsbStore();
			}
			return VLF_EV_RET_NOTHING;
		break;

		case 5:
		    if(!usbStarted){
			    sprintf(dev,"UMD9660");
				pspUsbDeviceSetDevice(PSP_USBDEVICE_UMD9660, 0, NULL);
				enableUsbStore();
			} else{
				DisableUsbStore();
			}
			return VLF_EV_RET_NOTHING;
		break;
	}	

	return VLF_EV_RET_REMOVE_OBJECTS | VLF_EV_RET_REMOVE_HANDLERS;
}

int OnBackToMainMenuFromN(int enter)
{
	if (!enter)
	{
		vlfGuiCancelCentralMenu();
		vlfGuiCancelBottomDialog();
		vlfGuiRemoveText(status);
		MainMenu(4);
	}

	return VLF_EV_RET_NOTHING;
}

void UsbMenu(int sel)
{
	char *items[] =
	{
		"MS",
		"Flash0",
		"Flash1",
		"Flash2",
		"Flash3",
		"UMD"
	};

	vlfGuiCentralMenu(6, items, sel, OnUsbOperationsSelect, 0, -8);
	vlfGuiBottomDialog(VLF_DI_BACK, VLF_DI_ENTER, 1, 0, VLF_DEFAULT, OnBackToMainMenuFromN);
}
