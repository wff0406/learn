//------------------------------------------------------------------------------
//                                  头文件包含                                 
//------------------------------------------------------------------------------
#include "flash.h"

/*-------------------------------------------------------------------
 * @name:         STMFLASH_ReadWord()
 * @brief:        读取指定地址的字(32位数据) 
 * @num1:         1.faddr 地址
 * @return:       *(unsigned int*)faddr 对应数据  
 * @notice:       
-------------------------------------------------------------------*/
static uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
	return *(unsigned int*)faddr; 
}

/*-------------------------------------------------------------------
 * @name:         STMFLASH_GetFlashSector()
 * @brief:        获取某个地址所在的flash扇区
 * @num1:         1.faddr 地址
 * @return:       FLASH_SECTOR_11 0~11  addr所在的扇区  
 * @notice:       
-------------------------------------------------------------------*/
uint8_t STMFLASH_GetFlashSector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10; 
	else if(addr<ADDR_FLASH_SECTOR_12)return FLASH_SECTOR_11; 

	return FLASH_SECTOR_11;	
}

/*-------------------------------------------------------------------
 * @name:         STMFLASH_Write()
 * @brief:        从指定地址开始写入指定长度的数据
 * @num1:         1.WriteAddr:起始地址(此地址必须为4的倍数!!)
                  2.pBuffer:数据指针
                  3.NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
 * @return:       
 * @notice:       因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
                  写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
                  写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
                  没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
                  该函数对OTP区域也有效!可以用来写OTP区!
OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
-------------------------------------------------------------------*/
void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
{ 
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t SectorError=0;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
    
	HAL_FLASH_Unlock();                                 //解锁	
	addrx=WriteAddr;				                    //写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	                    //写入的结束地址
    
	if(addrx<0X1FFF0000)
	{
		while(addrx<endaddr)		                                    //扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			 if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)                   //有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
				FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
				{
					break;//发生错误了	
				}
				}else addrx+=4;
				FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
		}
	}
	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
	if(FlashStatus==HAL_OK)
	{
		 while(WriteAddr<endaddr)//写数据
		 {
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		}  
	}
	HAL_FLASH_Lock();           //上锁
} 
/*-------------------------------------------------------------------
 * @name:         STMFLASH_Read()
 * @brief:        从指定地址开始读出指定长度的数据
 * @num1:          1.ReadAddr:起始地址
                   2.pBuffer:数据指针
                   3.NumToRead:字(32位)数
 * @return:       void
 * @notice:       
-------------------------------------------------------------------*/
void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}



