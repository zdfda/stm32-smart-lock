#include "stm32f1xx_hal.h"
#include "IIC1.h"
#include "OLED.h"
#include "stdio.h"
#include "OLED_LIB.h"
#include "string.h"
int oled_is_ok()
{
	return HAL_I2C_IsDeviceReady(&hi2c1,oled_address,3,100);
}
void oled_send_cmd(uint8_t a)
{
    uint8_t b[2];
    b[0]=0x00; 
    b[1]=a;
    if(iic1_send(oled_address,b,2)!=HAL_OK)
	{
		printf("cmd %#x fail, ErrorCode=0x%lx\n",a,hi2c1.ErrorCode);
		HAL_Delay(1000);
	}	
}

void oled_send_data(uint8_t a)
{
	uint8_t b[2];
    b[0]=0x40; 
    b[1]=a;
    if(iic1_send(oled_address,b,2)!=HAL_OK)
	{
		printf("date %#x send fail\n",a);
		HAL_Delay(1000);
	}
}

void oled_init(void)
{
	if(oled_is_ok()!=HAL_OK)
	{
		printf("oled address error\n");
	}
    HAL_Delay(100);
    oled_send_cmd(0xAE); // 关闭显示
    oled_send_cmd(0xD5); // 设置时钟分频因子,震荡频率
    oled_send_cmd(0x80);
    oled_send_cmd(0xA8); // 设置驱动路数
    oled_send_cmd(0x3F); // 128*64:0x3F
    oled_send_cmd(0xD3); // 显示偏移
    oled_send_cmd(0x00);
    oled_send_cmd(0x40); // 显示开始行
    oled_send_cmd(0x8D); // 电荷泵设置
    oled_send_cmd(0x14); // 开启电荷泵
    oled_send_cmd(0x20); // 内存地址模式
    oled_send_cmd(0x02); // 页地址模式
    oled_send_cmd(0xA1); // 段重定向 A0/A1
    oled_send_cmd(0xC8); // COM扫描方向
    oled_send_cmd(0xDA); // COM硬件配置
    oled_send_cmd(0x12);
    oled_send_cmd(0x81); // 对比度设置
    oled_send_cmd(0xCF);
    oled_send_cmd(0xD9); // 预充电周期
    oled_send_cmd(0xF1);
    oled_send_cmd(0xDB); // VCOMH取消选择电平
    oled_send_cmd(0x30);
    oled_send_cmd(0xA4); // 全局显示开启
    oled_send_cmd(0xA6); // 正常显示(0xA7反显)
    oled_send_cmd(0xAF); // 开启显示
}
void oled_clear()
{
    uint8_t i,j;
    for(i=0;i<8;i++)
    {
        oled_send_cmd(0xB0+i);
        oled_send_cmd(0x00);       
        oled_send_cmd(0x10);        
        for(j=0;j<128;j++)
        {
            oled_send_data(0x00);
        }
    }
}
void oled_test()
{
	
	oled_send_cmd(0xB0);
	oled_send_cmd(0x00);       
	oled_send_cmd(0x10);
	for(int j=0;j<8;j++)
	{
		oled_send_data(OLED_F8x16[16][j]);
	}
	oled_send_cmd(0xB1);
	oled_send_cmd(0x00);       
	oled_send_cmd(0x10);
	for(int j=8;j<16;j++)
	{
		oled_send_data(OLED_F8x16[16][j]);
	}
}
void oled_set_pos(uint8_t page,uint8_t col)
{
    oled_send_cmd(0xB0+page);                   
    oled_send_cmd(0x00|(col & 0x0F));           
    oled_send_cmd(0x10|((col>>4)&0x0F));     
}

void oled_putc(uint8_t x,uint8_t y,char ch)
{
    uint8_t i;
    uint8_t idx;
    if(ch <' '||ch >'~')ch = ' '; 
    idx=(uint8_t)(ch - ' ');            
    oled_set_pos(y,x);
    for(i=0;i<8;i++)
    {
        oled_send_data(OLED_F8x16[idx][i]);
    }
    oled_set_pos(y+1,x);
    for(i=0;i<8;i++)
    {
        oled_send_data(OLED_F8x16[idx][i + 8]);
    }
}
void oled_printf(uint8_t x, uint8_t y, const char *buf)
{
    while (*buf) 
	{
        oled_putc(x,y,*buf);
        x+=8;
        if(x>120){x=0;y+=2;if(y>6)y=0;}
        buf++;
    }
}
void oled_show_image(const uint8_t *img)
{
    uint8_t page;
    uint16_t col;

    for (page=0;page<8;page++)
    {
        oled_set_pos(page, 0);
        for(col=0;col<128;col++)
        {
            oled_send_data(img[col*8+page]);
        }
    }
}

