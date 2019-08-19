#include "pnp_statistics.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// Setting change
static int last_setting_change = SETTING_CHANGE_NONE;
static int last_setting_change_value = 0;

void setting_change(int setting, int value)
{
    last_setting_change = setting;
    last_setting_change_value = value;
}

void get_last_setting_change(int *setting, int *value)
{
    *setting = last_setting_change;
    *value = last_setting_change_value;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Telemetry statistics
static int telemetry_sent = 0;

void telemetry_sent_inc(void)
{
    telemetry_sent++;
}

void telemetry_sent_reset(void)
{
    telemetry_sent = 0;
}

int get_telemetry_sent_number(void)
{
    return telemetry_sent;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Error
#define ERROR_VALVE_THRESHOLD_VALUE 20
static int error_valve = 0;

void error_reset(void)
{
    error_valve = 0;
}

void error_inc(void)
{
    error_valve ++;
}

int is_error_max(void)
{
    return (error_valve > ERROR_VALVE_THRESHOLD_VALUE ? 1 : 0);
}