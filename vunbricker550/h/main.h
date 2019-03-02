#ifndef __MAIN_H__
#define __MAIN_H__

#define SMALL_BUFFER_SIZE	2000000
extern int BIG_BUFFER_SIZE;

void *ReadFileAllocEx(char *file, int seek, int size, int *retSize);
int ReadFile(char *file, int offset, void *buf, int size);
int WriteFile(char *file, void *buf, int size);
void *malloc64(int size);

void DumpMenu(int sel);
void ToolsMenu();
void MainMenu(int sel);
void ClearProgress();
void SetProgress(int percentage, int force);
void SetStatus(char *status);
void SetGenericProgress(int value, int max, int force);
void AddShutdownRebootBD(int shutdownonly);


#endif

