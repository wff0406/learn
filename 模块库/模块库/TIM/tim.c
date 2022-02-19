//开启PWM输出
HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
//设置默认的占空比值
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,10);
HAL_Delay(30);//延时30ms
 
//变量修改
if(i<100) i++;
else i=0;
 
//设置占空比值
__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,i);


输入捕获
//开启定时器溢出中断
HAL_TIM_Base_Start_IT(&htim2);  
//开启输入捕获中断，设置下降沿触发中断
__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING); 
HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);    //启动输入捕获
//变量存储
typedef struct 
{   
    uint8_t   flg; //0为未开始，1已经开始，2为结束
    uint16_t  num;//计数值
    uint16_t  num_period;//溢出次数
}COUNT_TEMP;
 
COUNT_TEMP count_temp={0};
 
//捕获中断发送时的回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
      //判断定时器2
        if(TIM2 == htim->Instance){
            if ( count_temp.flg == 0 )
            {   
                // 清零定时器计数
                __HAL_TIM_SET_COUNTER(htim,0); 
                //设置上升沿触发
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
                count_temp .flg = 1;    //设置已经开始    
                count_temp .num_period = 0;    //溢出计数清零        
                count_temp .num = 0; //计数清零
            }        
            else
            {
                // 获取定时器计数值
                count_temp .num = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);
                //设置下降沿触发
                __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
                count_temp .flg = 2;
            }
        }
}    
 
//定时器溢出回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(TIM2 == htim->Instance){
    //每次溢出时间为65536us
    if(count_temp.flg==1)//还未成功捕获
    {
                if(count_temp.num_period==0XFFFF)//电平太长了
                {
                    count_temp.flg=2;        //标记成功捕获了一次
                    count_temp .num=0XFFFF;
                }else count_temp .num_period ++;
    }
    }
}
//等待测量完毕
if(count_temp.flg == 2 )
{
    //计数计数值，0xFFFF为最大计数
    uint32_t ulTime = (uint32_t)count_temp .num_period * 0xFFFF + count_temp .num;
    //输出测量的值
    printf ( "低电平时间：%d us\n",ulTime); 
    count_temp .flg = 0;            
}