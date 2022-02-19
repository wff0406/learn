/*******阿里云交互*******/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtthread.h"

#include "board.h"
#include "fal.h"

#include "dfs_posix.h"
#include "blink_led.h"
#include "dev_buzzer.h"

#ifdef   PKG_USING_AT_DEVICE
#include "at_device.h"
#else
#ifdef RT_USING_NETDEV
#include "lwip/netdb.h"
#include <netdev.h>
#endif /* RT_USING_NETDEV */
#endif  // PKG_USING_AT_DEVICE

#include "infra_compat.h"
#include "mqtt_api.h"
#include "ota_api.h"
#include "wrappers_defs.h"
#include "mqtt_wrapper.h"
#include "ota_wrapper.h"

#include "ParamMgr.h"
#include "TaskWatchdog.h"

#include "utils_global.h"
#include "utils_tools.h"

#include "platform_register.h"
#include "platform_parser.h"

#include "aliy_iotx.h"
#include "ulog.h"

#define OTA_BUF_LEN                     (2048)

#define IOTX_TOPIC_NAME_REPORT         ("pb_report")
#define IOTX_TOPIC_NAME_CONTROL        ("pb_control")

extern int  eth_device_reset(void); // 重启网口