/*
 * system_interface_mamage.h
 *
 *  Created on: Apr 15, 2022
 *      Author: OWNER
 */

#ifndef MIDWARE_SYSTEM_INTERFACE_MAMAGE_H_
#define MIDWARE_SYSTEM_INTERFACE_MAMAGE_H_

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned long
#endif

typedef uint8_t (*commu_fn)(uint8_t * send_ptr , uint16_t send_length);

typedef struct
{
    uint8_t   index;
    uint8_t   stutus;
    uint16_t  unread_length;
    uint16_t  recv_buff_length;
    uint16_t  send_buff_length;
    uint8_t * recv_buff;
    uint8_t * send_buff;
    uint8_t * last_send_ptr;
    uint8_t * last_recv_ptr;
    commu_fn   send_fn;
    commu_fn   recv_fn;
    commu_fn   send_finish_cb_fn;
    commu_fn   recv_finish_cb_fn;
}if_mg_t;

/*
 * 通讯接口
 * */
typedef struct
{
    struct com_if_t * next;
    if_mg_t * if_mg;

}com_if_t;

typedef struct
{
    com_if_t * uart;
    com_if_t * iic;
    com_if_t * spi;
    com_if_t * usb;
    com_if_t * eth;
}globe_com_if_t;

#endif /* MIDWARE_SYSTEM_INTERFACE_MAMAGE_H_ */
