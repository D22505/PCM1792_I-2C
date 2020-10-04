#ifndef __DEFINE_H
#define __DEFINE_H

#include <REGX52.H>

#define uchar unsigned char
#define uint  unsigned int

#define PCM1792Addr 0x9C   //PCM1792��IIC�����ַ,RWλĬ��Ϊ0,��д��
#define READ_ERR    0xAAAA //�Ĵ����޷�������ֵ
#define WRITE_ERR   (-1)   //�Ĵ����޷�д����ֵ

//I^2C�������Ŷ���
//89C52   PCM1792
//P2^0---_>SDA
//P2^1---_>SCL
sbit SDA = P2^0;
sbit SCL = P2^1;

#endif