/* 
 * File:   hal_i2c.c
 * Author: Aya Ali
 *
 * Created on December 11, 2023, 8:16 PM
 */
#include "hal_i2c.h"
static inline void MSSP_I2C_Mode_GPIO_CFG(void);
static inline void MSSP_I2C_Clock_CFG(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
 /*Disable MSSP module */
    MSSP_MODULE_DISABLE_CFG();
/*MSSP select mode*/
/*MSSP MASTER Mode Configuration*/    
if(i2c_obj->i2c_cfg.i2c_mode == MMSP_I2C_MASTER_MODE){
/*MSSP clock configuration*/
MSSP_I2C_Clock_CFG(i2c_obj);
}
/*MSSP Slave Mode Configuration*/
else if(i2c_obj->i2c_cfg.i2c_mode == MMSP_I2C_SLAVE_MODE){
/*General call configuration*/
    if(i2c_obj->i2c_cfg.i2c_general_call == I2C_GENERAL_CALL_ENABLE){
        I2C_GENERAL_CALL_ENABLE_CFG();
    }
    else if(i2c_obj->i2c_cfg.i2c_general_call == I2C_MASTER_RECEIVE_DISABLE){
        I2C_SLEW_RATE_DISABLE_CFG();
    } 
    else{}   
}
else{}    
/*MSSP GPIO pin direction as input*/    
    MSSP_I2C_Mode_GPIO_CFG();
/*MSSP I2C Slaw rate control*/
    if(i2c_obj->i2c_cfg.i2c_slew_rate == I2C_SLAW_RATE_ENABLE){
        I2C_SLEW_RATE_ENABLE_CFG();
    }
    else if(i2c_obj->i2c_cfg.i2c_slew_rate == I2C_SLAW_RATE_DISABLE){
        I2C_SLEW_RATE_DISABLE_CFG();
    } 
    else{}   
/*MSSP I2C SMBUS control*/    
     if(i2c_obj->i2c_cfg.i2c_SMBus_Control == I2C_SMBUS_ENABLE){
        I2C_SMBUS_ENABLE_CFG();
    }
    else if(i2c_obj->i2c_cfg.i2c_SMBus_Control == I2C_SMBUS_DISABLE){
        I2C_SMBUS_DISABLE_CFG();
    } 
    else{}    
 /*Enable MSSP module */   
    MSSP_MODULE_ENABLE_CFG();
    ret = E_OK;
}
return ret;
}
Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
    ret = E_OK;
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
   /*Initialize start condition*/
  SSPCON2bits.SEN = 1;
/*wait for the completion of the start condition*/  
  while(SSPCON2bits.SEN);
  /*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;
  if(SSPSTATbits.S == START_BIT_DETECTED){
    ret = E_OK;
  }
  else{
     ret = E_NOT_OK;
  }
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
 /*Initialize Repeated start condition*/
  SSPCON2bits.RSEN = 1;
/*wait for the completion of the stop condition*/  
  while(SSPCON2bits.RSEN);
  /*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;
    ret = E_OK;
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
 /*Initialize stop condition*/
  SSPCON2bits.PEN = 1;
/*wait for the completion of the stop condition*/  
  while(SSPCON2bits.PEN);
  /*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;
  if(SSPSTATbits.P == STOP_BIT_DETECTED){
    ret = E_OK;
  }
  else{
     ret = E_NOT_OK;
  }
    
}
return ret;
}
Std_ReturnType MSSP_I2C_Write_Blocking(const mssp_i2c_t *i2c_obj, uint_8 i2c_data, uint_8 *ack){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
/*write data to the data register*/    
    SSPBUF = i2c_data;
/*wait for transmetion to complete*/    
    while(!PIR1bits.SSPIF);
/*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;    
if(SSPCON2bits.ACKSTAT == I2C_ACK_RECEVED_FROM_SLAVE){
    *ack = I2C_ACK_RECEVED_FROM_SLAVE;
}
else{
 *ack = I2C_ACK_NOT_RECEVED_FROM_SLAVE;
}
  ret = E_OK;
}
return ret;
}
Std_ReturnType MSSP_I2C_Read(const mssp_i2c_t *i2c_obj, uint_8 ack ,uint_8 *i2c_data){
Std_ReturnType ret = E_NOT_OK;
if((i2c_obj == NULL) || (i2c_data == NULL)){
ret = E_NOT_OK;
}
else{
    ret = E_OK;
}
return ret;
}
static inline void MSSP_I2C_Mode_GPIO_CFG(void){
    TRISCbits.TRISC3 = 1;    /*SCL pin is Input*/
    TRISCbits.TRISC4 = 1;    /*SDA pin is Input*/
}

static inline void MSSP_I2C_Clock_CFG(const mssp_i2c_t *i2c_obj){
    SSPCON1bits.SSPM = I2C_MASTER_MODE_DEFINED_CLOCK;
    SSPADD = (uint_8)(((_XTAL_FREQ/ 4.0)/i2c_obj->i2c_clock) - 1);
}