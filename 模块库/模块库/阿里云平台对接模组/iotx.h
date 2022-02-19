
#ifndef __ALIY_IOTX_H__
#define __ALIY_IOTX_H__

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define LOG_TAG              "iotx"
#define LOG_LVL              LOG_LVL_DBG

extern int param_report_flag;

int aliy_iotx_init(void);

int aliy_iotx_publish_report(uint8_t* payload, uint16_t len);
int aliy_iotx_publish(char *topic_name, char* payload, uint16_t payload_len);
int aliy_iotx_subscribe(char *topic_name);

int iotx_link_state(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __ALIY_IOTX_H__ */
