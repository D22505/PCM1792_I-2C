#include "IIC.h"

/*******************/
//���豸: 89C52RC
//���豸: PCM1792A
/*******************/

//I^2C����,���߳�ʼ��
//���:SDA-->1
//     SCL-->1
//PS:����δ�Ķ�(������_P182)
void IIC_BUS_Init(void)
{
	SCL=1;   //SCL����
	delay(); //��ʱ
	SDA=1;   //SDA����
	delay();
}

//I^2C����,���������ź�
//���:SDA-->0
//     SCL-->1
//PS:����δ�Ķ�(������_P182)
void IIC_BUS_Start(void)
{
	SDA=1;  //SDA����
	delay();//��ʱ
	SCL=1;  //SCL����
	delay();//��ʱ
	SDA=0;  //SDA����
	delay();//��ʱ
}

//I^2C����,����ֹͣ�ź�
//���:SDA-->1
//     SCL-->1
//PS:����δ�Ķ�(������_P183)
void IIC_BUS_Stop(void)
{
	SDA=0;  //SDA����
	delay();//��ʱ
	SCL=1;  //SDL����
	delay();//��ʱ
	SDA=1;  //SDA����
	delay();//��ʱ
}

//I^2C����,���豸�ȴ����豸Ӧ��
//���:SDA-->1�����豸δӦ��
//     SDA-->0�����豸Ӧ��
//     SCL-->0
//PS:����ʼû���ͷ�SDA,��������ǰ����á������д�ֽڡ������ͷ�SDA,����δ�Ķ�(������_P182)
void IIC_BUS_MasterWaitResp(void)
{
	uchar i=0;
	
//  SDA=1;  //�ͷ�SDA
	SCL=1;  //SCL����
	delay();//��ʱ
	while((SDA==1) && (i<255))
		i++;  //�ȴ����豸��SDA����
	SCL=0;  //���豸��SCL����
	delay();//��ʱ
}

//I^2C���ߣ����豸����豸Ӧ��
//���:SDA-->0
//     SCL-->0
//PS:�������û���ͷ�SDA,֮����á�����Ӷ����Ŀ�ʼ���ͷ�SDA,����δ�Ķ�(https://blog.csdn.net/xwwwj/article/details/105333223?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param)
void IIC_BUS_MasterRespToSlave(void)
{
	SCL=0;  //SCL����
	SDA=0;  //SDA����
	delay();//��ʱ
	SCL=1;  //SCL����
	delay();//��ʱ
	SCL=0;  //SCL����
//  SDA=1;  //�ͷ�SDA
}

//I^2C���ߣ����豸������豸Ӧ��
//���:SDA-->1
//     SCL-->0
//PS:����δ�Ķ�(https://blog.csdn.net/xwwwj/article/details/105333223?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param)
void IIC_BUS_MasterNotResp(void)
{
	SCL=0;  //SCL����
	SDA=1;  //SDA����
	delay();//��ʱ
	SCL=1;  //SCL����
	delay();//��ʱ
	SCL=0;  //SCL����
}

//I^2C���ߣ����豸����豸дһ���ֽ�
//���:SDA-->1
//     SCL-->0
//PS:����δ�Ķ�(������_P183)
void IIC_BUS_MasterWriteByte(uchar byte)
{
	uchar i,temp=byte;
	
	for(i=0; i<8; i++)
	{
		temp=temp<<1;//�������ֽ����λ����CYλ,�ȷ������λ
		SCL=0;       //SCL����
		delay();     //��ʱ
		SDA=CY;      //SDA�ͳ�CY
		delay();     //��ʱ
		SCL=1;       //SCL����
		delay();     //��ʱ
	}
	SCL=0;         //SCL����
	delay();       //��ʱ
	SDA=1;         //�ͷ�SDA
	delay();       //��ʱ
}

//I^2C���ߣ����豸����豸��һ���ֽ�
//���:SDA-->X
//     SCL-->0
//PS:����δ�Ķ�(������_P184)
uchar IIC_BUS_MasterReadByte(void)
{
	uchar i,k=0x00;
	
	SCL=0;  //SCL����
	delay();//��ʱ
	SDA=1;  //�ͷ�SDA
	for(i=0; i<8; i++)
	{
		SCL=1;  //SCL����
		delay();//��ʱ
		k=(k<<1)|SDA;//�����ȡ�ı���
		SCL=0;  //SCL����
		delay();//��ʱ
	}
	delay();  //��ʱ
	return k; //���ػ�ȡ���ֽ�
}

//I^2C���ߣ���PCM1792��һ���Ĵ���д��16λ���ݣ�����MSB��LSB��ɡ�
//���ô˺���֮ǰ��Ҫ��ʼ��I^2C����
//����ֵ -1 ʧ��,�üĴ����޷�д���üĴ�����Ч
//        0 �ɹ�
char IIC_BUS_WrtiePCM1792Reg(uchar addr, uchar MSB, uchar LSB)
{
	if((addr<0x10) || (addr>0x15))//���Ĵ���дȨ��
		return WRITE_ERR;
	
	MSB &= 0x7F;                  //���λ��0
	IIC_BUS_Start();              //��ʼ�ź�
	IIC_BUS_MasterWriteByte(PCM1792Addr & 0xFE);//дPCM1792��I^2C�����ַ + Wλ
	IIC_BUS_MasterWaitResp();     //�ȴ��ӻ�Ӧ��
	IIC_BUS_MasterWriteByte(addr);//д���д���Ŀ��Ĵ�����ַ
	IIC_BUS_MasterWaitResp();     //�ȴ��ӻ�Ӧ��
	IIC_BUS_MasterWriteByte(MSB); //д���ݣ����ֽ�
	IIC_BUS_MasterWaitResp();     //�ȴ��ӻ�Ӧ��
	IIC_BUS_MasterWriteByte(LSB); //д���ݣ����ֽ�
	IIC_BUS_MasterWaitResp();     //�ȴ��ӻ�Ӧ��
	IIC_BUS_Stop();               //ֹͣ�ź�
	delay();                      //��ʼ�ź���ֹͣ�ź�֮�䱣��һ�����
	return 0;
}

//I^2C���ߣ���PCM1792��һ���Ĵ�������16λ���ݣ�����MSB��LSB��ɡ�
//���ô˺���֮ǰ��Ҫ��ʼ��I^2C����
//����ֵ READ_ERR    ʧ�ܣ��üĴ����޷�����üĴ�����Ч
//       ����        ������16λ����
uint IIC_BUS_ReadPCM1792Reg(uchar addr)
{
	uchar MSB,LSB;
	uint date=0x0000;
	
	if((addr<0x10) || (addr>0x17))//���Ĵ�����Ȩ��
		return READ_ERR;
	
	IIC_BUS_Start();              //��ʼ�ź�
	IIC_BUS_MasterWriteByte(PCM1792Addr & 0xFE);//дPCM1792��I^2C�����ַ + Wλ
	IIC_BUS_MasterWaitResp();     //�ȴ��ӻ�Ӧ��
	IIC_BUS_MasterWriteByte(addr);//д�����ȡ��Ŀ��Ĵ�����ַ
	IIC_BUS_MasterWaitResp();     //�ȴ��ӻ�Ӧ��
	IIC_BUS_Start();              //�ظ���ʼ�ź�
	IIC_BUS_MasterWriteByte(PCM1792Addr | 0x01);//дPCM1792��I^2C�����ַ + Rλ
	IIC_BUS_MasterWaitResp();     //�ȴ��ӻ�Ӧ��
	MSB = IIC_BUS_MasterReadByte();//�����ݣ����ֽ�
	IIC_BUS_MasterRespToSlave();  //���豸����豸Ӧ��
	LSB = IIC_BUS_MasterReadByte();//�����ݣ����ֽ�
	IIC_BUS_MasterNotResp();      //���豸������豸Ӧ��
	IIC_BUS_Stop();               //ֹͣ�ź�
	
  date = (((date|MSB)<<8) | LSB);	//��װΪ16λ
	return date;
}









