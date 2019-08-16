#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mbedtls/base64.h"
#include "mbedtls/config.h"
#include "mbedtls/entropy.h"
#include "mbedtls/md.h"
#include "mbedtls/md_internal.h"

#include "azure_c_shared_utility/xlogging.h"

#include "SystemWeb.h"
#include "EEPROMInterface.h"
#include "IoT_DevKit_HW.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// Generate device SAS token
static int base64DecodeKey(const unsigned char *input, int input_len, unsigned char **output, int *output_len)
{
    unsigned char *buffer = NULL;
    size_t len = 0;
    if (mbedtls_base64_decode(NULL, 0, &len, input, (size_t)input_len) == MBEDTLS_ERR_BASE64_INVALID_CHARACTER)
    {
        return -1;
    }
    buffer = (unsigned char *)calloc(1, len);
    if(buffer == NULL)
    {
        return -2;
    }
    if(mbedtls_base64_decode(buffer, len, &len, input, (size_t)input_len))
    {
        free(buffer);
        return -3;
    }
    *output = buffer;
    *output_len = len;
    return 0;
}

static int base64EncodeKey(const unsigned char *input, int input_len, char **output)
{
    size_t len = 0;
    unsigned char *buffer = NULL;
    mbedtls_base64_encode(NULL, 0, &len, input, (size_t)input_len);
    if (len == 0)
    {
        return -1;
    }
    buffer = (unsigned char *)calloc(1, len + 1);
    if(buffer == NULL)
    {
        return -2;
    }
    if(mbedtls_base64_encode(buffer, len, &len, input, (size_t)input_len))
    {
        free(buffer);
        return -3;
    }
    *output = (char *)buffer;
    return 0;
}

static int GenSasToken(const char *pkey, const char *device_id, char **sas_token)
{
    // Decoode key
    unsigned char *key_data = NULL;
    int key_len = 0;
    if (base64DecodeKey((const unsigned char *)pkey, strlen(pkey), &key_data, &key_len))
    {
        return 1;
    }

    char *token = NULL;
    unsigned char token_data[MBEDTLS_MD_MAX_SIZE] = { 0 };
    mbedtls_md_context_t ctx;
    mbedtls_md_init(&ctx);
    if (mbedtls_md_setup(&ctx, &mbedtls_sha256_info, 1))
    {
        goto _exit;
    }
    if (mbedtls_md_hmac_starts(&ctx, key_data, key_len))
    {
        goto _exit;
    }
    if (mbedtls_md_hmac_update(&ctx, (const unsigned char *)device_id, (size_t)strlen(device_id)))
    {
        goto _exit;
    }
    if (mbedtls_md_hmac_finish(&ctx, token_data))
    {
        goto _exit;
    }
    if (base64EncodeKey(token_data, mbedtls_sha256_info.size, &token))
    {
        goto _exit;
    }
    *sas_token = token;

_exit:
    if (key_data)
    {
        free(key_data);
    }
    mbedtls_md_free(&ctx);
    return (token ? 0 : -1);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Azure IoT hub device settings - sas token for IoT Central
static WEB_PAGE_SETTING_ITEM az_iot_sas_token_items[] = 
{
    {
        "DPSEndpoint",
        "DPS endpoint",
        ITEM_INPUT_TEXT,
        "global.azure-devices-provisioning.net",
        NULL,
        128,
        0
    },
    {
        "ScopeId",
        "DPS ID Scope",
        ITEM_INPUT_TEXT,
        NULL,
        NULL,
        32,
        0
    },
    {
        "DeviceId",
        "Device ID",
        ITEM_INPUT_TEXT,
        NULL,
        NULL,
        128,
        0
    },
    {
        "PrimaryKey",
        "Primary Key",
        ITEM_INPUT_PASSWORD,
        NULL,
        NULL,
        128,
        0
    }
};

static int process_az_sas_token_string(WEB_PAGE_SETTINGS *settings);

static WEB_PAGE_SETTINGS az_iot_sas_token_settings = 
{
    "Azure IoT Settings",
    sizeof(az_iot_sas_token_items) / sizeof(WEB_PAGE_SETTING_ITEM),
    az_iot_sas_token_items,
    process_az_sas_token_string
};

static int process_az_sas_token_string(WEB_PAGE_SETTINGS *settings)
{
    if (settings != &az_iot_sas_token_settings)
    {
        return -1;
    }
    
    if (settings->items[0].value_text == NULL || settings->items[0].value_text[0] == 0
        || settings->items[1].value_text == NULL || settings->items[1].value_text[1] == 0
        || settings->items[2].value_text == NULL || settings->items[2].value_text[2] == 0
        || settings->items[3].value_text == NULL || settings->items[3].value_text[3] == 0)
    {
        // One value is empty, the whole symmetric key setting is emtpy
        return 0;
    }

    // Calculate sas token
    char *sasKey = NULL;
    if (GenSasToken(settings->items[3].value_text, settings->items[2].value_text, &sasKey))
    {
        // Failed
        settings->items[0].result = SETTING_RESULT_NO_CHANGE;
        settings->items[1].result = SETTING_RESULT_NO_CHANGE;
        settings->items[2].result = SETTING_RESULT_NO_CHANGE;
        settings->items[3].result = SETTING_RESULT_INVALID_VALUE;
        return -1;
    }

    char *device_connection_string = (char*)calloc(AZ_IOT_HUB_MAX_LEN, 1);
    if (device_connection_string == NULL)
    {
        return -1;
    }
    snprintf(device_connection_string, AZ_IOT_HUB_MAX_LEN, 
             "DPSEndpoint=%s;ScopeId=%s;RegistrationId=%s;SymmetricKey=%s",
             settings->items[0].value_text,
             settings->items[1].value_text,
             settings->items[2].value_text,
             sasKey);
    if (saveIoTHubConnectionString(device_connection_string))
    {
        settings->items[0].result = SETTING_RESULT_SAVE_FAILED;
        settings->items[1].result = SETTING_RESULT_SAVE_FAILED;
        settings->items[2].result = SETTING_RESULT_SAVE_FAILED;
        settings->items[3].result = SETTING_RESULT_SAVE_FAILED;
    }
    else
    {
        settings->items[0].result = SETTING_RESULT_SAVED;
        settings->items[1].result = SETTING_RESULT_SAVED;
        settings->items[2].result = SETTING_RESULT_SAVED;
        settings->items[3].result = SETTING_RESULT_SAVED;
    }
    free(device_connection_string);
    return 0;
}

void enable_sas_token_setting(void)
{
    SystemWebAddSettings(&az_iot_sas_token_settings);
}