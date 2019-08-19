#ifndef PNP_STATISTICS_H
#define PNP_STATISTICS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define SETTING_CHANGE_NONE     0
#define SETTING_CHANGE_FANSPEED 1
#define SETTING_CHANGE_CURRENT  2
#define SETTING_CHANGE_VOLTAGE  3
#define SETTING_CHANGE_IRDA     4

void setting_change(int setting, int value);
void get_last_setting_change(int *setting, int *value);

void telemetry_sent_reset(void);
void telemetry_sent_inc(void);
int get_telemetry_sent_number(void);

void error_reset(void);
void error_inc(void);
int is_error_max(void);

#ifdef __cplusplus
}
#endif

#endif // PNP_STATISTICS_H