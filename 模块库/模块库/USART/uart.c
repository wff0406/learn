串口查询模式
int main(void)
{
  /* USER CODE BEGIN 1 */
	char str[12] = "Hello World\n";
	char recv_buf[12] = {0};
  /* USER CODE END 1 */

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
	HAL_UART_Transmit(&huart1, (uint8_t*)str, 12, 0xFFFF);
  /* USER CODE END 2 */

  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
    //接收12个字节的数据，不超时
    if(HAL_OK == HAL_UART_Receive(&huart1, (uint8_t*)recv_buf, 12, 0xFFFF))
    {
      //将接收到的数据发送
      HAL_UART_Transmit(&huart1, (uint8_t*)recv_buf, 12, 0xFFFF);
    }
  }
  /* USER CODE END 3 */
}
串口中断模式
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t hello[] = "USART1 is ready...\n";
uint8_t recv_buf[13] = {0};
/* USER CODE END 0 */
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  //使能串口中断接收
  HAL_UART_Receive_IT(&huart1, (uint8_t*)recv_buf, 13);
  //发送提示信息
  HAL_UART_Transmit_IT(&huart1, (uint8_t*)hello, sizeof(hello));
  /* USER CODE END 2 */

  while (1)
  {
  }
}

/* USER CODE BEGIN 4 */
/* 中断回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* 判断是哪个串口触发的中断 */
	if(huart ->Instance == USART1)
	{
		//将接收到的数据发送
		HAL_UART_Transmit_IT(huart, (uint8_t*)recv_buf, 13);
		//重新使能串口接收中断
		HAL_UART_Receive_IT(huart, (uint8_t*)recv_buf, 13);
	}
}
/* USER CODE END 4 */

串口DMA模式
发送
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t dat[] = "Hello, I am Mculover666.\n";
/* USER CODE END 0 */
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dat, sizeof(dat));
  /* USER CODE END 2 */

  while (1)
  {
  }
}
接收
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t dat[] = "Hello, I am Mculover666.\n";
uint8_t recv_buf[13] = {0};		//串口接收缓冲区
/* USER CODE END 0 */
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart1, (uint8_t*)dat, sizeof(dat), 0xFFFF);
  HAL_UART_Receive_DMA(&huart1, recv_buf, 13);  //使能DMA接收
  /* USER CODE END 2 */

  while (1)
  {
  }
}
/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{ 
	//将接收到的数据再发送
	HAL_UART_Transmit(&huart1,recv_buf,13, 0xFFFF);
}
/* USER CODE END 4 */

IDIE空闲中断
#include "usart.h"
//下方为自己添加的代码
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //使能IDLE中断

//DMA接收函数，此句一定要加，不加接收不到第一次传进来的实数据，是空的，且此时接收到的数据长度为缓存器的数据长度
	HAL_UART_Receive_DMA(&huart1,rx_buffer,BUFFER_SIZE);
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
/* USER CODE BEGIN Private defines */
 
 
#define BUFFER_SIZE  100  
extern  volatile uint8_t rx_len ;  //接收一帧数据的长度
extern volatile uint8_t recv_end_flag; //一帧数据接收完成标志
extern uint8_t rx_buffer[100];  //接收数据缓存数组

void USART1_IRQHandler(void)
{
	uint32_t tmp_flag = 0;
	uint32_t temp;
	tmp_flag =__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
		//这两句和上面那句等效
		HAL_UART_DMAStop(&huart1); //  停止DMA传输，防止
		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);// 获取DMA中未传输的数据个数   
		//temp  = hdma_usart1_rx.Instance->NDTR;// 读取NDTR寄存器，获取DMA中未传输的数据个数，
		rx_len =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		recv_end_flag = 1;	// 接受完成标志位置1	
	 }
  HAL_UART_IRQHandler(&huart1);

}
 while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		 if(recv_end_flag == 1)  //接收完成标志
		{
			
			
			DMA_Usart_Send(rx_buffer, rx_len);
			rx_len = 0;//清除计数
			recv_end_flag = 0;//清除接收结束标志位
//			for(uint8_t i=0;i<rx_len;i++)
//				{
//					rx_buffer[i]=0;//清接收缓存
//				}
				memset(rx_buffer,0,rx_len);
  }
		HAL_UART_Receive_DMA(&huart1,rx_buffer,BUFFER_SIZE);//重新打开DMA接收
}

重定向printf
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
//同样USART1改为你的串口
  HAL_UART_Transmit(&huart1, (uint8_t*)&ch,1,HAL_MAX_DELAY);
    return ch;
}
重定向scanf
#include "stdio.h"

#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
    #define GETCHAR_PROTOTYPE int __io_getchar(FILE *f)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)   
    #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

//HAL库
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

GETCHAR_PROTOTYPE
{
  uint8_t  ch;
  HAL_UART_Receive(&huart3,(uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return  ch;
}

//寄存器
PUTCHAR_PROTOTYPE
{ 	
	while((USART3->ISR & UART_FLAG_TC) == RESET);
	USART3->TDR=(uint8_t)ch;      
	return ch;
}

GETCHAR_PROTOTYPE
{
	while((USART3->ISR & UART_FLAG_RXNE) == RESET);
	return (uint8_t)(USART3->RDR);
}
