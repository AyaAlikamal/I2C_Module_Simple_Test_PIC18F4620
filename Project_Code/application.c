/* 
 * File:   application.c
 * Author: AyaAli
 * Created on September 22, 2023, 10:24 AM
 */
#include "application.h"

mssp_i2c_t i2c_obj;
uint_8 slave_ack;
void I2C_init(){
Std_ReturnType ret = E_OK;    
   
   i2c_obj.I2C_Defualt_Interrupt_Hundeler = NULL;
   i2c_obj.I2c_Receive_Overflow = NULL;
   i2c_obj.I2c_Write_Collision = NULL;
   i2c_obj.i2c_clock = 100000;
   i2c_obj.i2c_cfg.i2c_mode = MMSP_I2C_MASTER_MODE;
   i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK;
   i2c_obj.i2c_cfg.i2c_SMBus_Control = I2C_SMBUS_DISABLE;
   i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SLAW_RATE_DISABLE;
   ret = MSSP_I2C_Init(&i2c_obj);  
};

int main() {
Std_ReturnType ret = E_NOT_OK;
I2C_init();

while(1){
ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
//ret = MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);
ret = MSSP_I2C_Write_Blocking(&i2c_obj, 0x30 ,&slave_ack);
ret = MSSP_I2C_Write_Blocking(&i2c_obj, 0x35 ,&slave_ack);
ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
__delay_ms(1000);
}
 return (EXIT_SUCCESS);
}
void application_initialize(void){
   Std_ReturnType ret = E_NOT_OK;
}
