#include"tm4c123gh6pm.h"
#include<stdio.h>
void CANinit(){
	             
	//Enable clock for CAN
	SYSCTL_RCGC0_R |=0x01000000;
	
	/******Initialize PortB for CAN******/
	
	//Activate port B
	SYSCTL_RCGC2_R |=0x00000002;
	while((SYSCTL_RCGC2_R & 0x00000002 )==0){};
	
	//Set ASFEL for PB4 & PB5
	GPIO_PORTB_AFSEL_R |=0x30;
	
	// Set PCTL Register PMC5 and PMC4 to 0x08 for CAN functionality
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFF00FFFF)+0x00880000;
	
	//Set the digital pins
	GPIO_PORTB_DEN_R |=0x30;
	
	//Disable analog function
	GPIO_PORTB_AMSEL_R &=0x00;
	
  // allow changes to PB4-5 
	GPIO_PORTB_CR_R |= 0x30;
	
	// Direction Output for PB5 and Input for PB4
	GPIO_PORTB_DIR_R |= 0x20;	
  
	/******Configuration of CAN******/
	
	//wait while CAN0 Module is BUSY
  while(CAN0_IF1CRQ_R & 0x00008000); 
	CAN0_BIT_R  |=0x00002301;
	
	//Enabling Loopback test mode
  CAN0_CTL_R |= 0x81;
  CAN0_TST_R |= 0x10;
	
	//Bit rate setting
	//Setting the CCE bit to edit CANBIT Register
  CAN0_CTL_R |= 0x40; 
  CAN0_BIT_R |= 0x0201;
	
	//No interrupts are used
  //CAN is enabled (Init = 0)
  CAN0_CTL_R &= ~0x41;
		
}
void CAN_Tranmitter(int ObjNum){
	//wait while CAN0 Module is BUSY
	while(CAN0_IF1CRQ_R & 0x00008000);
	
	//Actual Data to be sent
	CAN0_IF1DA1_R = 0x5453;
	
	//the first message
	CAN0_IF1DA2_R = 0x5241;
	CAN0_IF1DB1_R = 0x0054;
  CAN0_IF1DB2_R = 0x0000;
	
	//SET WRNRD, ARB, Control, DATAA & DATAB
	CAN0_IF1CMSK_R |=0xB3;
	CAN0_IF1MSK1_R =0;
	CAN0_IF1MSK2_R =0;
	
	//Set UMASK for filtering acceptance & txrqst
	CAN0_IF1MCTL_R |=0x1100;
	CAN0_IF1ARB1_R =0;
	
	//Set MSGVAL & DIR
	CAN0_IF1ARB2_R |=0xA000
	
	//Message object number
	CAN0_IF1CRQ_R = ObjNum;
	
}
void CAN_Reciever(int ObjNum){
	
   //wait while CAN0 Module is BUSY 
   while(CAN0_IF2CRQ_R & 0x00008000);
    
	 //WRNRD, Control, Mask, DATAA, DATAB & ARB
   CAN0_IF2CMSK_R |=0xF3 
   CAN0_IF2MSK1_R = 0;
   CAN0_IF2MSK2_R = 0;
   CAN0_IF2ARB1_R = 0;
		
 	 //Set MSGVal and Dir
   CAN0_IF2ARB2_R |= 0xA000;
    
   // Set no. of data bytes in MCTL Reg, EOB Set and UMASK set
	 CAN0_IF2MCTL_R |=0x1088;
 
	//Message object number
  CAN0_IF2CRQ_R = objNo; 
   
}



