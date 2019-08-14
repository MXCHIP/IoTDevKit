#ifndef SCREEN_ANIMATION_H
#define SCREEN_ANIMATION_H

#ifdef __cplusplus
extern "C"
{
#endif

void play_countdown_animation(int value);
void play_fanspeed_animation(int value);
void play_current_animation(int value);
void play_voltage_animation(int value);
void play_irda_animation(int value);

#ifdef __cplusplus
}
#endif

#endif // SCREEN_ANIMATION_H
