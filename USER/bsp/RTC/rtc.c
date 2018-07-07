#include "sys.h"
#include "rtc.h"
#include "delay.h"

typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    //������������
    uint16_t w_year;
    uint8_t  w_month;
    uint8_t  w_date;
    uint8_t  week;
}tm;

//ʱ��ľֲ�����
static tm timer;

const uint8_t *COMPILED_DATE=__DATE__;//��ñ�������
const uint8_t *COMPILED_TIME=__TIME__;//��ñ���ʱ��

const uint8_t Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
//�Զ�����ʱ��Ϊ������ʱ��
static void Auto_Time_Set(void);
static uint8_t RTC_Get(void);
static uint8_t Is_Leap_Year(uint16_t year);
static uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
static uint8_t str_cmpx(uint8_t*s1,uint8_t*s2,uint8_t len);
 
static void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;               //RTCȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    //��ռ���ȼ�1λ,�����ȼ�3λ
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //��ռ���ȼ�0λ,�����ȼ�4λ
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //ʹ�ܸ�ͨ���ж�
    NVIC_Init(&NVIC_InitStructure);                               //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

uint8_t RTC_Init(bool force_set_compile_time)
{
    //����ǲ��ǵ�һ������ʱ��
    uint8_t temp=0;
    RTC_NVIC_Config();

    //��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
    if ((BKP_ReadBackupRegister(BKP_DR1) != 0x5050) \
        || (force_set_compile_time))                \
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //ʹ��PWR��BKP����ʱ��
        PWR_BackupAccessCmd(ENABLE);                                                //ʹ��RTC�ͺ󱸼Ĵ�������
        BKP_DeInit();                                                               //������BKP��ȫ���Ĵ�������Ϊȱʡֵ
        RCC_LSEConfig(RCC_LSE_ON);                                                  //�����ⲿ���پ���(LSE),ʹ��������پ���
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)                         //���ָ����RCC��־λ�������,�ȴ����پ������
        {
            temp++;
            delayMs(10);
        }
        if(temp>=250)
            return 1;//��ʼ��ʱ��ʧ��,����������

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);     //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��
        RCC_RTCCLKCmd(ENABLE);                      //ʹ��RTCʱ��
        RTC_WaitForSynchro();                       //�ȴ����һ�ζ�RTC�Ĵ�����д�������
        RTC_WaitForLastTask();                      //�ȴ����һ�ζ�RTC�Ĵ�����д�������
        RTC_ITConfig(RTC_IT_SEC, ENABLE);           //ʹ��RTC���ж�
        RTC_WaitForLastTask();                      //�ȴ����һ�ζ�RTC�Ĵ�����д�������
        RTC_SetPrescaler(32767);                    //����RTCԤ��Ƶ��ֵ
        RTC_WaitForLastTask();                      //�ȴ����һ�ζ�RTC�Ĵ�����д�������
        Auto_Time_Set();                            //����ʱ��
        BKP_WriteBackupRegister(BKP_DR1, 0X5050);   //��ָ���ĺ󱸼Ĵ�����д���û���������
    }
    else//ϵͳ������ʱ
    {

        RTC_WaitForSynchro();   //�ȴ����һ�ζ�RTC�Ĵ�����д�������
        RTC_ITConfig(RTC_IT_SEC, ENABLE);   //ʹ��RTC���ж�
        RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    }
    RTC_Get();          //����ʱ��
    RCC_ClearFlag();    //���RCC�ĸ�λ��־λ
    return 0;           //ok
}


//RTC�жϷ�����
void RTC_IRQHandler(void)
{
    if(RTC->CRL&0x0001)//�����ж�
    {
        RTC_Get();//����ʱ��
    }
    if(RTC->CRL&0x0002)//�����ж�
    {
        RTC->CRL&=~(0x0002);//�������ж�
        //���Ӵ���
    }
    RTC->CRL&=0X0FFA;         //�������������жϱ�־
    while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������
}


//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
static uint8_t Is_Leap_Year(uint16_t year)
{
    if(year%4==0) //�����ܱ�4����
    {
        if(year%100==0)
        {
            if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400����
            else return 0;
        }
        else
            return 1;
    }
    else 
        return 0;
}

//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�
static uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�
//ƽ����·����ڱ�
static const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
    uint16_t t;
    uint32_t seccount=0;
    if(syear<1970||syear>2099)return 1;
    for(t=1970;t<syear;t++) //��������ݵ��������
    {
        if(Is_Leap_Year(t))seccount+=31622400;//�����������
        else seccount+=31536000;              //ƽ���������
    }
    smon-=1;
    for(t=0;t<smon;t++)    //��ǰ���·ݵ����������
    {
        seccount+=(uint32_t)mon_table[t]*86400;//�·����������
        if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������
    }

    seccount+=(uint32_t)(sday-1)*86400;//��ǰ�����ڵ����������
    seccount+=(uint32_t)hour*3600;//Сʱ������
    seccount+=(uint32_t)min*60;   //����������
    seccount+=sec;//�������Ӽ���ȥ

    RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������

    RTC_SetCounter(seccount);   //����RTC��������ֵ

    RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    return 0;
}

//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
static uint8_t RTC_Get(void)
{
    static uint16_t daycnt=0;
    uint32_t timecount=0;
    uint32_t temp=0;
    uint16_t temp1=0;

    timecount=RTC->CNTH;//�õ��������е�ֵ(������)
    timecount<<=16;
    timecount+=RTC->CNTL;

    temp=timecount/86400;   //�õ�����(��������Ӧ��)
    if(daycnt!=temp)//����һ����
    {
        daycnt=temp;
        temp1=1970; //��1970�꿪ʼ
        while(temp>=365)
        {
            if(Is_Leap_Year(temp1))//������
            {
                if(temp>=366)temp-=366;//�����������
                else {temp1++;break;}
            }
            else temp-=365;   //ƽ��
            temp1++;
        }
        timer.w_year=temp1;//�õ����
        temp1=0;
        while(temp>=28)//������һ����
        {
            if(Is_Leap_Year(timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
            {
                if(temp>=29)temp-=29;//�����������
                else break;
            }
            else
            {
                if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
                else break;
            }
            temp1++;
        }
        timer.w_month=temp1+1;//�õ��·�
        timer.w_date=temp+1;  //�õ�����
    }
    temp=timecount%86400;     //�õ�������
    timer.hour=temp/3600;     //Сʱ
    timer.min=(temp%3600)/60; //����
    timer.sec=(temp%3600)%60; //����
    timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//��ȡ����
    return 0;
}
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//�������������������
//����ֵ�����ں�
static uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
{
    uint16_t temp2;
    uint8_t yearH,yearL;

    yearH=year/100; yearL=year%100;
    // ���Ϊ21����,�������100
    if (yearH>19)yearL+=100;
    // ����������ֻ��1900��֮���
    temp2=yearL+yearL/4;
    temp2=temp2%7;
    temp2=temp2+day+table_week[month-1];
    if (yearL%4==0&&month<3)
        temp2--;

    return(temp2%7);
}
//�Ƚ������ַ���ָ�����ȵ������Ƿ����
//����:s1,s2Ҫ�Ƚϵ������ַ���;len,�Ƚϳ���
//����ֵ:1,���;0,�����

static uint8_t str_cmpx(uint8_t*s1,uint8_t*s2,uint8_t len)
{
    uint8_t i;
    for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
    return 1;
}

//�Զ�����ʱ��Ϊ������ʱ��
static void Auto_Time_Set(void)
{
    uint8_t temp[3];
    uint8_t mon,date;
    uint16_t year;
    uint8_t sec,min,hour;
    for(int i=0;i<3;i++)
    {
        temp[i]=COMPILED_DATE[i];
    }

    for(int i=0;i<12;i++)
    {
        if(str_cmpx((uint8_t*)Month_Tab[i],temp,3))
        {
            mon=i+1;//�õ��·�
            break;
        }
    }

    if(COMPILED_DATE[4]==' ')
    {
        date=COMPILED_DATE[5]-'0';
    }
    else
    {
        date=10*(COMPILED_DATE[4]-'0')+COMPILED_DATE[5]-'0';
    }
    year=1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')+10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';
    hour=10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';
    min=10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';
    sec=10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';
    RTC_Set(year,mon,date,hour,min,sec) ;
}

void get_wall_clock_time(UTCTimeStruct * utc_time)
{
    utc_time->year = timer.w_year;
    utc_time->month = timer.w_month;
    utc_time->day = timer.w_date;
    utc_time->hour = timer.hour;
    utc_time->minute = timer.min;
    utc_time->second = timer.sec;
}

void set_system_clock(time_union_t time)
{
    RTC_Set(time.time.year,time.time.month,time.time.day,time.time.hour,time.time.minute,time.time.second);
}

uint8_t get_current_weak(void)
{
    return timer.week;
}

