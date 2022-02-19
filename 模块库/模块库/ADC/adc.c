单通道
uint16_t ADC_Value=0;
uint16_t dong_get_adc(){
    //开启ADC1
  HAL_ADC_Start(&hadc1);
    //等待ADC转换完成，超时为100ms
    HAL_ADC_PollForConversion(&hadc1,100);
    //判断ADC是否转换成功
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC)){
         //读取值
       return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
}
三通道

uint16_t ADC_Value[3]={0};
 
uint16_t dong_get_adc(){
    //开启ADC1
  HAL_ADC_Start(&hadc1);
    //等待ADC转换完成，超时为100ms
    HAL_ADC_PollForConversion(&hadc1,100);
    //判断ADC是否转换成功
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC)){
         //读取值
       return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
	for(uint8_t i=0;i<3;i++){
    //分别存放通道1、2、3的ADC值
    ADC_Value[i]=dong_get_adc();
}
ADC中断方式多通道采集

#define ADC_MAX_NUM 3*5 //3组ADC,每组最多存储5个值
uint16_t ADC_Values[ADC_MAX_NUM]={0};
uint16_t adc_value_flg=0;
 
//启动函数，需要在main中调用一次
void dong_start_adc(){
    //开启ADC1,使能中断
  HAL_ADC_Start_IT(&hadc1);
}
//ADC转换完成自动调用函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
    
     //获取值并存储
   ADC_Values[adc_value_flg++]=HAL_ADC_GetValue(hadc);
 
     if(adc_value_flg==ADC_MAX_NUM)
     {
         adc_value_flg=0;//清零下标
     }
}
DMA采集
#define ADC_MAX_NUM 3*5 //3组ADC,每组最多存储5个值
 
uint16_t ADC_Values[ADC_MAX_NUM]={0};
 
//启动函数，需要在main中调用一次
void dong_start_adc(){
    
    //启动DMA
  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Values,ADC_MAX_NUM);
    
}