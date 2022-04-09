/*
 * File:   main.c
 * Author: nrabi
 *
 * Created on 1 April, 2022, 1:59 PM
 */


#include "main.h"
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)

char *gear[7]={"GN","GR","G1","G2","G3","G4"};
static void init_config(void) {
      init_clcd(); // Initialization of clcd
           
       init_i2c(100000); //100K
       init_ds1307();
       
        init_adc(); // initialize adc
        
       init_digital_keypad(); // initialize digital key pad
     
       init_timer2();
       PEIE=1;
       GIE=1;
 
     
     
    
  
   
}

void main(void) {
    char event[]="ON";
    unsigned char speed=0;
    unsigned char control_flag=DASH_BOARD_FLAG;
    unsigned char key,reset_flag,menu_pos;
    unsigned char gr=0;
    
    init_config();
    log_car_event(event,speed);
    eeprom_write(0x00,'2');
    eeprom_write(0x01,'4');
    eeprom_write(0x02,'2');
    eeprom_write(0x03,'4');
    while(1)
    {   
        speed=read_adc();
        if(speed>99)
        {
            speed=99;
        }
        key= read_digital_keypad(STATE);
        
        for(int j=100;j>0;j--);
        
        if(key==SW1)
        {
            strcpy(event,"C ");
            log_car_event(event,speed);
        }
        else if(key==SW2 && gr<6)
        {
            strcpy(event,gear[gr]);
            gr++;
            log_car_event(event,speed);
        }
        else if(key==SW3 && gr>0)
        {    gr--;
            strcpy(event,gear[gr]);
            log_car_event(event,speed);
        }
        else if(control_flag==DASH_BOARD_FLAG &&(key==SW4 || key==SW5))
        {    
            clear_scr();
            clcd_print(" ENTER PASSWORD",LINE1(0));
            clcd_write(CURSOR_POS,INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_us(100);
            
            control_flag=LOGIN_FLAG;
            
            reset_flag=RESET_PASSWORD;
            TMR2ON = 1;
        }
        else if(control_flag==LOGIN_MENU_FLAG && (key==SW6))
        {
            switch(menu_pos)
            {
                case 0:
                    clear_scr();
                    clcd_print("# TIME     E  SP",LINE1(0));
                    control_flag=VIEW_LOG_FLAG;
                    reset_flag=VIEW_LOG_RESET;
                            
                    break;
                case 1:
                    log_car_event("CL",speed);
                    clear_scr();
                    control_flag=CLEAR_LOG_FLAG;
                    reset_flag=RESET_MEMORY;
                    break;
                case 2:
                    log_car_event("CP",speed);
                    clear_scr();
                    control_flag=CHANGE_PASSWORD_FLAG;
                    reset_flag=RESET_CHANGE_PASSWORD;
                    break;
            }
        }
       
       switch(control_flag)
       {
           case DASH_BOARD_FLAG : display_dash_board(event,speed);
           break;
           case LOGIN_FLAG :
               switch(login(key,reset_flag))
               {
                   case RETURN_BACK :
                       control_flag=DASH_BOARD_FLAG;
                       TMR2ON = 0;
                       clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                       __delay_us(100);
                       break;
                   case TASK_SUCCESS :
                       control_flag=LOGIN_MENU_FLAG;
                       reset_flag=RESET_LOGIN_MENU;
                       clear_scr();
                       TMR2ON = 0;
                       clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                       __delay_us(100);
                       continue;
                       break;
               }
               break;
           case LOGIN_MENU_FLAG :
               menu_pos=login_menu(key,reset_flag);
               break;
           case VIEW_LOG_FLAG :
               view_log(key,reset_flag);
               break;
           case CLEAR_LOG_FLAG:
               if(clear_log(reset_flag)==TASK_SUCCESS);
               {   
                   clear_scr();
                   control_flag=LOGIN_MENU_FLAG;
                   reset_flag=RESET_LOGIN_MENU;
                   continue;
               }
               break;
           case CHANGE_PASSWORD_FLAG :
               switch(change_password(key,reset_flag))
               {
                   case TASK_SUCCESS:
                   clear_scr();
                   clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                       __delay_us(100);
                   control_flag=LOGIN_MENU_FLAG;
                   reset_flag=RESET_LOGIN_MENU;
                   continue; 
                   break;
               }
               break;
       }
       reset_flag=RESET_NOTHING;
    }
 
}
