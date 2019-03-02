#ifndef __VLF_H__
#define __VLF_H__

typedef int VlfText;
typedef int VlfPicture;
typedef int VlfProgressBar;
typedef int VlfSpin;
typedef void* VlfShadowedPicture;
typedef void *VlfBatteryIcon;


#define VLF_DEFAULT	-1

#define VLF_BATTERY_ICON_HIGH		0
#define VLF_BATTERY_ICON_MEDIUM		1
#define VLF_BATTERY_ICON_LOW		2
#define VLF_BATTERY_ICON_LOWEST		3

#define VLF_ERROR_INVALID_INPUT			(-1)
#define VLF_ERROR_INVALID_INPUT_DATA	(-2)
#define VLF_ERROR_UNSUPPORTED_FORMAT	(-3)
#define VLF_ERROR_OBJECT_OVERFLOW		(-4)
#define VLF_ERROR_OBJECT_NOT_FOUND		(-5)
#define VLF_ERROR_NO_MEMORY				(-6)
#define VLF_ERROR_SYSTEM				(-7)
#define VLF_ERROR_DUPLICATED			(-8)

/** Fade modes */
#define VLF_FADE_MODE_IN		1
#define VLF_FADE_MODE_OUT		2
#define VLF_FADE_MODE_REPEAT	4

/** Fade effect */
#define VLF_FADE_EFFECT_STANDARD	0
#define VLF_FADE_EFFECT_FAST		1
#define VLF_FADE_EFFECT_VERY_FAST	2
#define VLF_FADE_EFFECT_SLOW		3
#define VLF_FADE_EFFECT_SUPER_FAST	4


/* Alignment */
#define VLF_ALIGNMENT_LEFT			0
#define VLF_ALIGNMENT_CENTER	0x200
#define VLF_ALIGNMENT_RIGHT		0x400

enum VlfObjects
{
	VLF_TEXT = 0,
	VLF_PIC = 1,
	VLF_SHADOWED_PIC = 2,
	VLF_PROGRESS_BAR = 3
};

enum VlfButtonIcon
{
	VLF_ENTER = 0,
	VLF_CANCEL = 1,
	VLF_CROSS = 2,
	VLF_CIRCLE = 3,
	VLF_TRIANGLE = 4,
	VLF_SQUARE = 5
};

enum RCOType
{
	RCO_GRAPHIC,
	RCO_OBJECT,
	RCO_SOUND,
	RCO_LABEL,
	RCO_FILEPARAM,
	RCO_ANIMPARAM
};

enum VLF_MDType
{
	VLF_MD_TYPE_ERROR,
	VLF_MD_TYPE_NORMAL,
};

enum VLF_MD_Buttons
{
	VLF_MD_BUTTONS_NONE = 0,
	VLF_MD_BUTTONS_YESNO = 0x10,
};

enum VLF_MD_InitalCursor
{
	VLF_MD_INITIAL_CURSOR_YES = 0,
	VLF_MD_INITIAL_CURSOR_NO = 0x100,
};

enum VLF_MD_ButtonRes
{
	VLF_MD_NONE,
	VLF_MD_YES,
	VLF_MD_NO,
	VLF_MD_BACK
};

enum VLF_DialogItem
{
	VLF_DI_ENTER,
	VLF_DI_CANCEL,
	VLF_DI_BACK,
	VLF_DI_YES,
	VLF_DI_NO,
	VLF_DI_EDIT,
};

enum VLF_SpinState
{
	VLF_SPIN_STATE_NOT_FOCUS, // Spin control has not focus, buttons are not listened
	VLF_SPIN_STATE_FOCUS, // Spin control text has focus but arrow is not shown, buttons are not listened
	VLF_SPIN_STATE_ACTIVE, // Spin control has focus, and it is active  (arrows are shown, up and down buttons are listened)
};

#define VLF_EV_RET_NOTHING				0
#define VLF_EV_RET_REMOVE_EVENT			1 
#define VLF_EV_RET_REMOVE_OBJECTS		2
#define VLF_EV_RET_REMOVE_HANDLERS		4
#define VLF_EV_RET_REFRESH_ON_DELAY		8
#define VLF_EV_RET_DELAY				0x80000000 /* Delay VLF_EV_RET_DELAY | (X << 16), 0 <= X <= 32767 milisecs */


enum PspCtrlExtension
{
	PSP_CTRL_ENTER = 0x40000000,
	PSP_CTRL_CANCEL = 0x80000000
};

/**
 * Inits VLF library
*/
void vlfGuiInit(int heap_size, int (* app_main)(int argc, char *argv[]));

int  vlfGuiSystemSetup(int battery, int clock, int notuserwp);
int  vlfGuiGetLanguage();
void vlfGuiSetLanguage(int lang);
int  vlfGuiGetButtonConfig();
void vlfGuiSetButtonConfig(int config);
void vlfGuiSetResourceDir(char *dir);

/**
 * Performs the draw of the current frame
*/
void vlfGuiDrawFrame();

/**
 * Loads resources from a rco file
 *
 * @param rco - It can be one of following things:
 * - path relative to the flash0:/vsh/resource directory without extension (e.g. "system_plugin_bg")
 * - path relative to the flash0:/vsh/resource directory with extension (e.g. "system_plugin_bg.rco")
 * - path to a file (e.g. "flash0:/vsh/resource/system_plugin_bg.rco", "ms0:/myresfile.rco")
 *
 * RCO param is evaluated in the order given above, so if a rco file exists in current directory with name 
 * "system_plugin_bg.rco", it would load the one of flash0 and not the one of current directory. (in such a case, use "./system_plugin_bg.rco")
 * 
 * @param n - The number of resources to loads
 * @param names (IN) - An array with the names of resources
 * @param types (IN) - An array with the types of the resources (one of RCOType)
 * @param datas (OUT) - A pointer to a variable that will receive an array of pointers to the content of each resource,
 * or NULL if a specific resource has not been found.
 * Pointers returned are allocated with malloc, and should be deallocated by the application.
 *
 * @param sizes (OUT) - It will receive the sizes of the resources
 * @param pntable (OUT) - A pointer that will receive the string table. Pass NULL if no required.
 * Returned pointer is allocated with malloc and should be deallocated by the application.
 *
 * @returns - the number of resources loaded, or < 0 if there is an error.
 *
 * @Sample: Load battery icon pic and shadow
 *
 * char *names[2];
 * void *datas[2];
 * int types[2], sizes[2];
 *
 * names[0] = "tex_battery";
 * names[1] = "tex_battery_shadow";
 * types[0] = types[1] = RCO_GRAPHIC;
 *
 * int res = vlfGuiLoadResources("system_plugin_fg", 2, names, types, datas, sizes, NULL);
 * if (res != 2) // error or not all resources loaded
 * {
 *    if (res > 0)
 *    {
 *       if (datas[0])
 *          free(datas[0]);
 *       if (datas[1])
 *          free(datas[1]);
 *    }
 * }
 * else
 * {
 *    void *bat;    
 *    vlfGuiAddShadowedPicture(&bat, datas[0], sizes[0], datas[1], sizes[1], 441, 4, 1, 1, 1); 
 *    free(datas[0]);
 *    free(datas[1]);
 * }
 *
*/
int  vlfGuiLoadResources(char *rco, int n, char **names, int *types, void **datas, int *sizes, char **pntable);

int  vlfGuiCacheResource(char *rco);
int  vlfGuiUncacheResource(char *rco);


int  vlfGuiGetResourceSubParam(void *entry, int insize, char *ntable, char *name, void **data, int *size);


int  vlfGuiLoadLabel(u16 *str, char *rco, char *name);

/**
 * Sets the background from 8888 texture data
 *
 * @param texture - The texture data in 8888 format
 * @param width - The width of texture. Must be a power of 2.
 * @param height - The height of texture. Must be multiple of 8.
 * @param swizzled - Indicates if the texture is already in the psp GE fast texture format
 * @param scale_x - The x scale to apply
 * @param scale_y - The y scale to apply
 *
 * @returns 0 on success, or < 0 on error (params invalid)
*/
int  vlfGuiSetBackground(u32 *texture, int width, int height, int swizzled, float scale_x, float scale_y);

/**
 * Sets the background from a file buffer.
 * Supported formats are currently: BMP, TIM and GIM, with a depth of 24 or 32 bits.
 *
 * @param data - The buffer with the file data
 * @param size - The size of the data
 *
 * @returns - 0 on success, < 0 on error.
*/
int  vlfGuiSetBackgroundFileBuffer(void *data, int size);

/**
 * Sets the background from a file
 * Supported formats are currently: BMP, TIM and GIM, with a depth of 24 or 32 bits.
 *
 * @param file - Path to the file.
 *
 * @returns - 0 on success, < 0 on error.
*/
int  vlfGuiSetBackgroundFile(char *file);

/**
 * Sets one of system backgrounds based on the index.
 *
 * @param index - The index of the background, valid values are 1-27 
 * (note that 13-27 is only available on slim and will return an error on old psp)
 *
 * @returns 0 on success, < 0 on error
*/
int  vlfGuiSetBackgroundIndex(int index);

/**
 * Sets one of system backgrounds based on the current date
 *
 * @returns 0 on success, < 0 on error
*/
int  vlfGuiSetBackgroundDate();

/** 
 * Sets a background of a single color
 *
 * @param color - The color in XXBBGGRR format (XX is ignored).
 *
 * @returns - this functions always succeeds returning 0
*/
int  vlfGuiSetBackgroundPlane(u32 color);

/**
 * Sets the background according to the system configuration
 *
 * @returns - 0 on success, < 0 on error.
*/
int  vlfGuiSetBackgroundSystem(int notuserwp);

/**
 * Sets the system color, used in titlebars or menus
 *
 * @param index - the index of the color, 1-27
*/
void vlfGuiSetSystemColor(int index); 

/**
 * Sets the background model from a buffer.
 *
 * @param data - The buffer with the model in GMO format
 * @param size - The size of the model
 *
 * @returns - 0 on success, < 0 on error.
*/
int  vlfGuiSetModel(void *data, int size);

/**
 * Sets the background model from a file.
 *
 * @param file - The file with the model in GMO format
 *
 * @returns - 0 on success, < 0 on error.
*/
int  vlfGuiSetModelFile(char *file);

/**
 * Sets the background model from a resource.
 *
 * @param rco - The path to the RCO file
 * @param name - The name of the resource
 *
 * @returns - 0 on success, < 0 on error.
*/
int  vlfGuiSetModelResource(char *rco, char *name);

/**
 * Sets the background model of the system, and applies the proper world matrix to it.
 *
 * @returns 0 on success, < 0 on error.
*/
int  vlfGuiSetModelSystem();

/**
 * Sets the world matrix for the model. (by default, the world matrix is the identity 
 * after a model has been loaded, except when calling vlfGuiSetModelSystem).
 *
 * @param matrix - The matrix to set.
 *
 * @Sample: Load waves (this sample assumes the default scale of 8.5)
 *
 * int res = vlfGuiSetModelResource("system_plugin_bg", "mdl_bg");
 * if (res < 0) process_error;
 *
 * ScePspFMatrix4 matrix;
 * ScePspFVector3 scale;
 *
 * scale.x = scale.y = scale.z = 8.5f;
 * gumLoadIdentity(&matrix);
 * gumScale(&matrix, &scale);
 * vlfGuiSetModelWorldMatrix(&matrix);
*/
void vlfGuiSetModelWorldMatrix(ScePspFMatrix4 *matrix);

/**
 * Gets the world matrix of the model
 *
 * @returns a pointer to the model world matrix
*/
ScePspFMatrix4 *vlfGuiGetModelWorldMatrix();

/**
 * Sets the model speed 
 *
 * @param speed - The speed, default model speed is 1.0f/60.0f
*/
void vlfGuiSetModelSpeed(float speed);

void vlfGuiSetTitleBar(VlfText text, VlfPicture pic, int visible, int hideobj);
void vlfGuiSetTitleBarEx(VlfText text, VlfPicture pic, int visible, int hideobj, u32 color);
void vlfGuiSetTitleBarVisibility(int visible);

VlfText vlfGuiAddText(int x, int y, char *string);
VlfText vlfGuiAddTextW(int x, int y, wchar_t *string);
VlfText vlfGuiAddTextF(int x, int y, char *fmt, ...);
VlfText vlfGuiAddTextResource(char *rco, char *name, int x, int y);
int  vlfGuiRemoveText(VlfText text);
int  vlfGuiSetText(VlfText text, char *string);
int  vlfGuiSetTextW(VlfText text, wchar_t *string);
int  vlfGuiSetTextF(VlfText text, char *string, ...);
int  vlfGuiSetTextResource(VlfText text, char *rco, char *name);
int  vlfGuiSetTextFocus(VlfText text);
int  vlfGuiRemoveTextFocus(VlfText text, int keepres);
int  vlfGuiSetTextVisibility(VlfText text, int visible);
int  vlfGuiSetTextBlinking(VlfText text, u32 nshow, u32 nhide);
int  vlfGuiSetTextFade(VlfText text, int mode, int effect, int direction_out);
int  vlfGuiCancelTextFade(VlfText text);
int  vlfGuiSetTextFadeFinishCallback(VlfText text, void (* callback)(void *), void *param, int delay);
int  vlfGuiSetTextAlignment(VlfText text, int alignment);
int  vlfGuiSetTextXY(VlfText text, int x, int y);
int  vlfGuiSetTextSize(VlfText text, float size);
int  vlfGuiChangeCharacterByButton(u16 ch, int button);

VlfPicture vlfGuiAddPicture(void *data, int size, int x, int y);
VlfPicture vlfGuiAddPictureFile(char *file, int x, int y);
VlfPicture vlfGuiAddPictureResource(char *rco, char *name, int x, int y);
int  vlfGuiRemovePicture(VlfPicture pic);
int  vlfGuiSetPictureXY(VlfPicture pic, int x, int y);
int  vlfGuiGetPictureSize(VlfPicture pic, int *width, int *height);
int  vlfGuiSetPictureDisplayArea(VlfPicture pic, u32 x, u32 y, u32 width, u32 height);
int  vlfGuiSetPictureAlphaBlend(VlfPicture pic, int op, int src, int dst, u32 srcfix, u32 dstfix);
int  vlfGuiClonePicture(VlfPicture pic, int real, int x, int y);
int  vlfGuiSetPictureVisibility(VlfPicture pic, int visible);
int  vlfGuiSetPictureBlinking(VlfPicture pic, u32 nshow, u32 nhide);
int  vlfGuiAnimatePicture(VlfPicture pic, int w, int h, int frames, int vertical);
int  vlfGuiSetPictureFade(VlfPicture pic, int mode, int effect, int direction_out);
int  vlfGuiCancelPictureFade(VlfPicture pic);
int  vlfGuiSetPictureFadeFinishCallback(VlfPicture pic, void (* callback)(void *), void *param, int delay);

int  vlfGuiAddShadowedPicture(VlfShadowedPicture *sp, void *pic, int pic_size, void *shpic, int shpic_size, int x, int y, int sh_offsx, int sh_offsy, int shadow_before);
int  vlfGuiAddShadowedPictureFile(VlfShadowedPicture *sp, char *pic, char *shpic, int x, int y, int sh_offsx, int sh_offsy, int shadow_before);
int  vlfGuiAddShadowedPictureResource(VlfShadowedPicture *sp, char *rco, char *pic, char *shpic, int x, int y, int sh_offsx, int sh_offsy, int shadow_before);
int  vlfGuiRemoveShadowedPicture(VlfShadowedPicture sp);
int  vlfGuiSetShadowedPictureVisibility(VlfShadowedPicture sp, int visible);
int  vlfGuiSetShadowedPictureBlinking(VlfShadowedPicture sp, u32 nshow, u32 nhide);
int  vlfGuiAnimateShadowedPicture(VlfShadowedPicture sp, int w, int h, int ws, int hs, int frames, int vertical);
int  vlfGuiSetShadowedPictureFade(VlfShadowedPicture sp, int mode, int effect, int direction_out);
int  vlfGuiCancelShadowedPictureFade(VlfShadowedPicture sp);
int  vlfGuiSetShadowedPictureFadeFinishCallback(VlfShadowedPicture sp, void (* callback)(void *), void *param, int delay);

int  vlfGuiAddBatteryIcon(VlfBatteryIcon *baticon, u32 status, int blink);
int  vlfGuiAddBatteryIconEx(VlfBatteryIcon *baticon, int x, int y, u32 status, int blink);
int  vlfGuiAddBatteryIconSystem(VlfBatteryIcon *baticon, int timer_ms);
int  vlfGuiSetBatteryIconStatus(VlfBatteryIcon baticon, int status, int blink);
int  vlfGuiRemoveBatteryIcon(VlfBatteryIcon baticon);

int  vlfGuiAddClock();

int  vlfGuiAddWaitIcon(VlfShadowedPicture *waiticon);
int  vlfGuiAddWaitIconEx(VlfShadowedPicture *waiticon, int x, int y);

int  vlfGuiAddCross(VlfShadowedPicture *cross, int x, int y);
int  vlfGuiAddCircle(VlfShadowedPicture *circle, int x, int y);
int  vlfGuiAddTriangle(VlfShadowedPicture *triangle, int x, int y);
int  vlfGuiAddSquare(VlfShadowedPicture *square, int x, int y);
int  vlfGuiAddEnter(VlfShadowedPicture *enter, int x, int y);
int  vlfGuiAddCancel(VlfShadowedPicture *cancel, int x, int y);
int  vlfGuiAddSpinUp(VlfShadowedPicture *spinup, int x, int y);
int  vlfGuiAddSpinDown(VlfShadowedPicture *spindown, int x, int y);
int  vlfGuiAddArrowLeft(VlfShadowedPicture *arrowleft, int x, int y);
int  vlfGuiAddArrowRight(VlfShadowedPicture *arrowright, int x, int y);

int  vlfGuiAddGameIcon(VlfShadowedPicture *game, int x, int y);

VlfProgressBar vlfGuiAddProgressBar(int y);
VlfProgressBar vlfGuiAddProgressBarEx(int x, int y);
int  vlfGuiRemoveProgressBar(VlfProgressBar pb);
int  vlfGuiProgressBarSetProgress(VlfProgressBar pb, u32 perc);
int  vlfGuiSetProgressBarFade(VlfProgressBar pb, int mode, int effect, int direction_out);
int  vlfGuiCancelProgressBarFade(VlfProgressBar pb);
int  vlfGuiSetProgressBarFadeFinishCallback(VlfProgressBar pb, void (* callback)(void *), void *param, int delay);

int  vlfGuiSetRectangleVisibility(int x, int y, int w, int h, int visible);
void *vlfGuiSaveRectangleVisibilityContext(int x, int y, int w, int h);
void vlfGuiRestoreVisibilityContext(void *ctx);
void vlfGuiFreeVisibilityContext(void *ctx);

int  vlfGuiSetRectangleFade(int x, int y, int w, int h, int mode, int effect, int direction_out, void (* callback)(void *), void *param, int delay);
int  vlfGuiSetRectangleBlinking(int x, int y, int w, int h, u32 nshow, u32 nhide);
int  vlfGuiSetSynchronizedBlinking(void *dst, int dst_type, void *src, int src_type);

int  vlfGuiMessageDialog(char *msg, u32 flags);
int  vlfGuiErrorDialog(int error);
int  vlfGuiNetConfDialog();

int  vlfGuiBottomDialog(int button1, int button2, int automatic, int enter_is_left, int distance, int (* handler)(int enter));
int  vlfGuiCustomBottomDialog(char *button1, char *button2, int automatic, int enter_is_left, int distance, int (* handler)(int enter));
void vlfGuiCancelBottomDialog();

int  vlfGuiCentralMenu(int noptions, char **items, int defaultsel, int (* handler)(int sel), int dispx, int dispy);
void vlfGuiCancelCentralMenu();
int  vlfGuiCentralMenuSelection();

VlfSpin vlfGuiAddIntegerSpinControl(int x, int y, int min, int max, int cur, int step, int loop, int speed, int initstate, char *prefix, char *suffix);
int  vlfGuiRemoveSpinControl(VlfSpin spin);
int  vlfGuiSetSpinState(VlfSpin spin, int state);
int  vlfGuiSetIntegerSpinMinMax(VlfSpin spin, int min, int max);
int  vlfGuiGetIntegerSpinValue(VlfSpin spin, int *value);
int  vlfGuiSetIntegerSpinValue(VlfSpin spin, int value);

int  vlfGuiPreviousPageControl(int (* handler)(int page));
int  vlfGuiNextPageControl(int (* handler)(int page));
void vlfGuiCancelPreviousPageControl();
void vlfGuiCancelNextPageControl();
void vlfGuiSetPageControlEnable(int enable);

int  vlfGuiAddEventHandler(int buttons, int wait, int (* func)(void *), void *param);
int  vlfGuiAddNegativeEventHandler(int buttons, int wait, int (* func)(void *), void *param);
int  vlfGuiRemoveEventHandler(int (* func)(void *));
int  vlfGuiRemoveEventHandlerEx(int (* func)(void *), void *param);
int  vlfGuiIsEventRegistered(int (* func)(void *));
int  vlfGuiSetEventDelay(int (* func)(void *), u32 delay);
int  vlfGuiSetEventDelayEx(int (* func)(void *), void * param, u32 delay);
int  vlfGuiDelayAllEvents(u32 delay);


#endif

