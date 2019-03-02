# VUnbricker550
This is an Unofficial Despertar-Del-Cementerio mod with Support for Firmware 5.50  
This is an archive of the original Source Code released at lan.st

# How to compile
We can't yet compile it since we very likely need the SDK files provided with the M33 Firmware releases.  
Atleast that it my assumption from the research I've done so far.  
The makefile fails with the following:  
```
/usr/local/pspdev/lib/gcc/psp/4.9.3/../../../../psp/bin/ld: cannot find -lvlfgui
/usr/local/pspdev/lib/gcc/psp/4.9.3/../../../../psp/bin/ld: cannot find -lvlfgu
/usr/local/pspdev/lib/gcc/psp/4.9.3/../../../../psp/bin/ld: cannot find -lvlfutils
/usr/local/pspdev/lib/gcc/psp/4.9.3/../../../../psp/bin/ld: cannot find -lvlflibc
/usr/local/pspdev/lib/gcc/psp/4.9.3/../../../../psp/bin/ld: cannot find -lpspsystemctrl_user
/usr/local/pspdev/lib/gcc/psp/4.9.3/../../../../psp/bin/ld: cannot find -lpspkubridge
```

You need to have the [PSPSDK](https://github.com/pspdev/pspsdk) installed. Simply use the [PSPToolchain](https://github.com/pspdev/psptoolchain) script to quickly install the latest SDK.