#include "esp8266.h"
#include "usart3.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "ledkeybuzzer.h"
#include "DH11.h"
#include "MQ2.h"
#include "ledkeybuzzer.h"
#include "time.h"
static uint8_t buf[1024];
#define CMD_READ_DATA "read"

uint8_t stm32_mode=0;
void buf_clear(void)
{
	memset(buf,0,256);
}	
void debug_printf(void)
{
    while(usart3_head2!=usart3_tail2)
	{
        printf("%c",*usart3_head2);
        if(++usart3_head2>=usart3_buf2+256)
        usart3_head2=usart3_buf2;
    }
}
static void ring_flush(void)
{
    usart3_head = usart3_tail;
    if (usart3_head >= usart3_buf + 256)
        usart3_head = usart3_buf;
}
int send_esp_cmp(const char *pData,const char *comparedate,uint32_t timeout_ms)
{
    char a[128];
    int len=snprintf(a, sizeof(a), "%s\r\n", pData);
    if (len<=0||len>=sizeof(a))return 0;
    ring_flush();
    HAL_UART_Transmit(&husart3, (uint8_t *)a, len, 100);
	if (strcmp(comparedate, CMD_READ_DATA) == 0) 
	{
    HAL_Delay(500);
    return 1;         
	}
	strcpy(a,comparedate);
	int i=0;
	uint32_t c=HAL_GetTick();
	int a_len=strlen(a);
	while(i!=a_len)
	{
		if(HAL_GetTick()-c>=timeout_ms)return 0;
		if(usart3_head==usart3_tail)continue;
		if(*usart3_head!=a[i])
		{
			if(*usart3_head==a[0])i=1;
			else i=0;
		}
		else i++;
		if(++usart3_head>=usart3_buf+256)
		{usart3_head=usart3_buf;}
	}
	return 1;
}
uint8_t esp8266_sendbuf[256];
uint16_t mqtt_connect_send(uint8_t *buf);
uint16_t mqtt_publish_send(uint8_t *buf,const char *topic,const uint8_t *payload,uint16_t payload_len,uint8_t qos);
void mqtt_subscribe(const char *topic, uint8_t qos);
void up_state(void)
{
    
    const char *topic = "$sys/Ob1hpCUIu0/d1/thing/property/post";
    char json[512];
    snprintf(json, sizeof(json),
        "{\"id\":\"1\",\"version\":\"1.0\",\"params\":{"
        "\"buzzer\":{\"value\":%d},"
        "\"led1\":{\"value\":%s},"
        "\"led2\":{\"value\":%s},"
        "\"led3\":{\"value\":%s},"
        "\"led4\":{\"value\":%s},"
		"\"t_g\":{\"value\":%u},"
		"\"t_d\":{\"value\":%u},"
		"\"h_g\":{\"value\":%u},"
		"\"h_d\":{\"value\":%u},"
		"\"MQ2\":{\"value\":%u},"
		"\"mode\":{\"value\":%u}"
        "}}",
        buzzer_state,
        led1_state ? "true" : "false",
        led2_state ? "true" : "false",
        led3_state ? "true" : "false",
        led4_state ? "true" : "false",
		t_g,t_d,h_g,h_d,mq2_data,stm32_mode);
    mqtt_publish_send(buf,topic,(const uint8_t *)json,strlen(json),0);// QoS 0；要确认到达可改 1
}

void state_up(void)
{
	buf_clear();
	ring_flush();
	send_esp_cmp("AT+CIPRECVDATA=200",CMD_READ_DATA, 2000);
	if(usart3_head==usart3_tail)return;
	debug_printf();
	uint32_t t=HAL_GetTick();
	uint8_t* p=(uint8_t*)usart3_head;
	int i=0;
	int cnt=0;
	while(1)
	{
	if(p==usart3_tail){return;}
	if(*p=='{') break;
	if (++p >=usart3_buf+256) p = usart3_buf;
	if(HAL_GetTick()-t>=1000){return;}
	}
	buf[i++]=*p;  
	if(++p>=usart3_buf+256)p=usart3_buf;
	cnt++;
	while(cnt!=0)
	{
	if(HAL_GetTick()-t>=1000){return;}	
	if(p==usart3_tail){return;}
	if(*p=='{'){cnt++;buf[i]=*p;i++;if(++p>=usart3_buf+256)p=usart3_buf;continue;}
	if(*p=='}'){cnt--;buf[i]=*p;i++;if(++p>=usart3_buf+256)p=usart3_buf;continue;}
	buf[i]=*p;i++;if(++p>=usart3_buf+256)p=usart3_buf;
	}
	buf[i]='\0';
	usart3_head=p;
	
	char id[32] = {0};
    int buzzer=0;
    char *q;
    sscanf((const char*)buf, "{\"id\":\"%31[^\"]\"", id);
	const char *key = "\"uinx_time\":";
	q = strstr((const char*)buf, key);
	if (q) {
		q += strlen(key);              /* 跳过 "uinx_time": */
		while (*q == ' ') q++;         /* 跳过空格 */
		printf("q=[%s]\r\n", q);       /* 看 q 指向哪 */

		uinx_time = 0;
		while (*q >= '0' && *q <= '9') {
			uinx_time = uinx_time * 10 + (uint64_t)(*q - '0');
			q++;
		}
	}
    q = strstr((const char*)buf, "\"buzzer\":");
    if (q) sscanf(q, "\"buzzer\":%d", &buzzer);
	q = strstr((const char*)buf, "\"mode\":");
    if (q) {printf("2");sscanf(q, "\"mode\":%d",(int*)&stm32_mode);printf("2");}
    if (strstr((const char*)buf, "\"led1\":true"))	 {led1_on();}
    if (strstr((const char*)buf, "\"led1\":false")) {led1_off();}
    if (strstr((const char*)buf, "\"led2\":true"))  {led2_on();}
    if (strstr((const char*)buf, "\"led2\":false")) {led2_off();}
    if (strstr((const char*)buf, "\"led3\":true"))  {led3_on();}
    if (strstr((const char*)buf, "\"led3\":false")) {led3_off();}
    if (strstr((const char*)buf, "\"led4\":true"))  {led4_on();}
    if (strstr((const char*)buf, "\"led4\":false")) {led4_off();}
	beep_on(buzzer);
    if(id[0]) 
	{
        char reply[128];
		buf_clear();
        snprintf(reply, sizeof(reply),"{\"id\":\"%s\",\"code\":200,\"msg\":\"success\"}", id);
        mqtt_publish_send(buf,"$sys/Ob1hpCUIu0/d1/thing/property/set_reply",(const uint8_t*)reply, strlen(reply), 0);
    }
}

void esp8266_init()
{
	usart3_init(115200);
	HAL_Delay(1000);
	printf("buf=%p head=%p tail=%p\n", usart3_buf, usart3_head, usart3_tail);
	if(send_esp_cmp("AT","OK",500)==0)
	{printf("AT time out\n");}
	debug_printf();
	if(send_esp_cmp("AT+CIPRECVMODE=1","OK",500)==0) // 开启手动接收模式
	{printf("get time out\n");}	
	debug_printf();
	if(send_esp_cmp("ATE0","OK",500)==0)
	{printf("ATE0 time out\n");}
	debug_printf();
	if(send_esp_cmp("AT+CWMODE=1","OK",500)==0)
	{printf("mode time out\n");}
	debug_printf();
	if(send_esp_cmp("AT+CWJAP=\"410\",\"12345678\"","OK",5000)==0)
	{printf("wifi time out\n");}
	debug_printf();
	if(send_esp_cmp("AT+CIPSTART=\"TCP\",\"183.230.40.96\",1883","OK",1000)==0)
	{printf("tcp time out\n");}
	debug_printf();
	mqtt_connect_send(buf);
	HAL_Delay(500);
	up_state();
	debug_printf();
	HAL_Delay(500);
	send_esp_cmp("AT+CIPRECVDATA=150", "OK", 2000);       // ★ 读掉 CONNACK
	debug_printf();
	HAL_Delay(200);
	mqtt_subscribe("$sys/Ob1hpCUIu0/d1/thing/property/set",0);
	debug_printf();
	HAL_Delay(500);
	send_esp_cmp("AT+CIPRECVDATA=150", "OK", 2000);       // ★ 读掉 SUBACK
	debug_printf();
	HAL_Delay(200);
//	send_esp_cmp("AT+CW();SAP=\"This is a esp8266\",\"12345678\",11,3");
//	HAL_Delay(500);
//	send_esp_cmp("AT+CIPMUX=1");
//	HAL_Delay(500);
//	send_esp_cmp("AT+CIPSERVER=1,8080");
//	HAL_Delay(500);
//	send_esp_cmp("AT+CIFSR");
//	HAL_Delay(500);
}

#define CLIENT_ID    "d1"
#define USER_NAME    "Ob1hpCUIu0"
#define PASSWORD     "version=2018-10-31&res=products%2FOb1hpCUIu0%2Fdevices%2Fd1&et=2108357613&method=md5&sign=nPCT0JFh9uZ0X9TGtC%2BZgA%3D%3D"

uint16_t mqtt_connect_send(uint8_t *buf)
{
	buf_clear();
    uint16_t pos = 0;
    uint16_t client_len = strlen(CLIENT_ID);
    uint16_t user_len   = strlen(USER_NAME);
    uint16_t pass_len   = strlen(PASSWORD);

    // 可变头 10 字节 + 三个字段（每个 2 字节长度 + 内容）
    uint16_t remaining = 10
                       + 2 + client_len
                       + 2 + user_len
                       + 2 + pass_len;

    // 固定头：0x10 + MQTT 变长编码的剩余长度
    buf[pos++] = 0x10;
    uint16_t rem = remaining;
    do {
        uint8_t digit = rem % 128;
        rem /= 128;
        if (rem > 0) digit |= 0x80;
        buf[pos++] = digit;
    } while (rem > 0);

    // 可变头
    buf[pos++] = 0x00; buf[pos++] = 0x04;
    buf[pos++] = 'M';  buf[pos++] = 'Q';
    buf[pos++] = 'T';  buf[pos++] = 'T';
    buf[pos++] = 0x04;  // MQTT 3.1.1
    buf[pos++] = 0xC2;  // clean session + username + password
    buf[pos++] = 0x00; buf[pos++] = 0x3C;  // keepalive 60s
    // ClientID
    buf[pos++] = (client_len >> 8) & 0xFF;
    buf[pos++] = client_len & 0xFF;
    memcpy(&buf[pos], CLIENT_ID, client_len);
    pos += client_len;

    // Username
    buf[pos++] = (user_len >> 8) & 0xFF;
    buf[pos++] = user_len & 0xFF;
    memcpy(&buf[pos], USER_NAME, user_len);
    pos += user_len;

    // Password
    buf[pos++] = (pass_len >> 8) & 0xFF;
    buf[pos++] = pass_len & 0xFF;
    memcpy(&buf[pos], PASSWORD, pass_len);
    pos += pass_len;

    // AT+CIPSEND=<pos>
    uint8_t cmd[24];
    uint8_t cmd_idx = 0;
    cmd[cmd_idx++] = 'A'; cmd[cmd_idx++] = 'T';
    cmd[cmd_idx++] = '+'; cmd[cmd_idx++] = 'C';
    cmd[cmd_idx++] = 'I'; cmd[cmd_idx++] = 'P';
    cmd[cmd_idx++] = 'S'; cmd[cmd_idx++] = 'E';
    cmd[cmd_idx++] = 'N'; cmd[cmd_idx++] = 'D';
    cmd[cmd_idx++] = '=';

    uint16_t num = pos;
    uint8_t temp[6];
    int8_t t_idx = 0;
    if (num == 0) {
        temp[t_idx++] = '0';
    } else {
        while (num > 0) {
            temp[t_idx++] = (num % 10) + '0';
            num /= 10;
        }
    }
    while (t_idx > 0) {
        cmd[cmd_idx++] = temp[--t_idx];
    }
    cmd[cmd_idx++] = '\r';
    cmd[cmd_idx++] = '\n';

    HAL_UART_Transmit(&husart3, cmd, cmd_idx, 100);
    HAL_Delay(200);   // 等模块回 ">"，80ms 偏短

    HAL_UART_Transmit(&husart3, buf, pos, 100);
    return pos;
}
// 只负责构造 PUBLISH 报文并发送，QoS 0/1 都支持
uint16_t mqtt_publish_send(uint8_t *buf,
                           const char *topic,
                           const uint8_t *payload,
                           uint16_t payload_len,
                           uint8_t qos)
{
	buf_clear();
    uint16_t pos = 0;
    uint16_t topic_len = strlen(topic);

    // ---------- 计算剩余长度 ----------
    uint16_t remaining = 2 + topic_len + payload_len;
    if (qos > 0) remaining += 2;          // Packet Identifier

    // ---------- 固定头 ----------
    if (qos == 0)      buf[pos++] = 0x30; // PUBLISH, QoS 0
    else if (qos == 1) buf[pos++] = 0x32; // PUBLISH, QoS 1
    else               return 0;          // OneNET 不支持 QoS 2

    // 剩余长度：MQTT 变长编码（1~4 字节）
    uint16_t rem = remaining;
    do {
        uint8_t digit = rem % 128;
        rem /= 128;
        if (rem > 0) digit |= 0x80;
        buf[pos++] = digit;
    } while (rem > 0);

    // ---------- 可变头：Topic ----------
    buf[pos++] = (topic_len >> 8) & 0xFF;
    buf[pos++] = topic_len & 0xFF;
    memcpy(&buf[pos], topic, topic_len);
    pos += topic_len;

    // ---------- Packet Identifier（QoS > 0 才有） ----------
    if (qos > 0) {
        static uint16_t pkt_id = 0;
        pkt_id++;
        if (pkt_id == 0) pkt_id = 1;      // 0 是保留值
        buf[pos++] = (pkt_id >> 8) & 0xFF;
        buf[pos++] = pkt_id & 0xFF;
    }

    // ---------- Payload ----------
    memcpy(&buf[pos], payload, payload_len);
    pos += payload_len;

    // ---------- AT+CIPSEND=<pos> ----------
    uint8_t cmd[24];
    uint8_t cmd_idx = 0;
    cmd[cmd_idx++] = 'A'; cmd[cmd_idx++] = 'T';
    cmd[cmd_idx++] = '+'; cmd[cmd_idx++] = 'C';
    cmd[cmd_idx++] = 'I'; cmd[cmd_idx++] = 'P';
    cmd[cmd_idx++] = 'S'; cmd[cmd_idx++] = 'E';
    cmd[cmd_idx++] = 'N'; cmd[cmd_idx++] = 'D';
    cmd[cmd_idx++] = '=';

    uint16_t num = pos;
    uint8_t temp[6];
    int8_t t_idx = 0;
    if (num == 0) {
        temp[t_idx++] = '0';
    } else {
        while (num > 0) {
            temp[t_idx++] = (num % 10) + '0';
            num /= 10;
        }
    }
    while (t_idx > 0) {
        cmd[cmd_idx++] = temp[--t_idx];
    }
    cmd[cmd_idx++] = '\r';
    cmd[cmd_idx++] = '\n';
    HAL_UART_Transmit(&husart3, cmd, cmd_idx, 100);
    HAL_Delay(200);   // 等模块回 ">"
    HAL_UART_Transmit(&husart3, buf, pos, 100);
    return pos;
}
void mqtt_subscribe(const char *topic, uint8_t qos)
{
	buf_clear();
    uint16_t pos = 0;
    uint16_t topic_len = strlen(topic);
    uint16_t remaining = 2 + 2 + topic_len + 1;

    buf[pos++] = 0x82;
    uint16_t rem = remaining;
    do {
        uint8_t digit = rem % 128;
        rem /= 128;
        if (rem > 0) digit |= 0x80;
    buf[pos++] = digit;
    } while (rem > 0);

    buf[pos++] = 0x00;
    buf[pos++] = 0x01;
    buf[pos++] = (topic_len >> 8) & 0xFF;
    buf[pos++] = topic_len & 0xFF;
    memcpy(&buf[pos], topic, topic_len);
    pos += topic_len;
    buf[pos++] = qos;

    uint8_t cmd[24];
    uint8_t cmd_idx = 0;
    cmd[cmd_idx++] = 'A'; cmd[cmd_idx++] = 'T';
    cmd[cmd_idx++] = '+'; cmd[cmd_idx++] = 'C';
    cmd[cmd_idx++] = 'I'; cmd[cmd_idx++] = 'P';
    cmd[cmd_idx++] = 'S'; cmd[cmd_idx++] = 'E';
    cmd[cmd_idx++] = 'N'; cmd[cmd_idx++] = 'D';
    cmd[cmd_idx++] = '=';

    uint16_t num = pos;
    uint8_t temp[6];
    int8_t t_idx = 0;
    while (num > 0) {
        temp[t_idx++] = (num % 10) + '0';
        num /= 10;
    }
    while (t_idx > 0) {
        cmd[cmd_idx++] = temp[--t_idx];
    }
    cmd[cmd_idx++] = '\r';
    cmd[cmd_idx++] = '\n';
    HAL_UART_Transmit(&husart3, cmd, cmd_idx, 100);
    HAL_Delay(200);                       
    HAL_UART_Transmit(&husart3, buf, pos, 100);
}
