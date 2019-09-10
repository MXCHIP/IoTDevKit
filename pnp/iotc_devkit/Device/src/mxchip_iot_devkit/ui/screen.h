#ifndef SCREEN_H
#define SCREEN_H

#ifdef __cplusplus
extern "C"
{
#endif

void screen_main(void);
void screen_showInfo(const char* text);
void screen_echo(const char* text);

#ifdef __cplusplus
}
#endif

#endif // SCREEN_H
