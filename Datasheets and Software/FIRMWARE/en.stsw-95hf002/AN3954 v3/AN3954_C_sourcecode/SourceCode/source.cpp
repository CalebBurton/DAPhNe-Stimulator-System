#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "source.h"
#include "CR95HFDLL.h"


//User menu: exit emulator
#define		exit_cmd						'0'

//User menu: CR95HFDll.h related functions
#define		Get_Dll_rev_cmd					'a'
#define		DEMO_CR95HF_A_USBconnect_cmd	'b'
#define		DEMO_CR95HF_A_USBhandlecheck_cmd 'c'
#define		Get_MCU_rev_cmd					'd'		
#define		Echo_cmd						'e'
#define		FieldOff_cmd					'f'
#define		Idn_cmd							'g'
#define		Select_ISO15693_cmd				'h'
#define		Send_ISO15693_Inventory_cmd		'i'
#define		Select_ISO14443B_cmd			'j'
#define		Send_ISO14443B_Initiate_cmd		'k'
#define		STCmd_RdReg_cmd					'l'
#define		STCmd_ISO15693_cmd				'm'
#define		STCmd_Idle_cmd					'n'
#define		ResetSPI_cmd					'o'
#define		Send_IRQ_Pulse_cmd				'p'
#define		Get_Interface_Pin_State_cmd		'q'
#define		Set_Polling_Reading_config_cmd	'r'
#define		Send_NSS_Pulse_cmd				's'
#define		Get_Hardware_rev_cmd			't'

/* MAIN USER INTERFACE */
void main (void)
{
	char entry=0;	

	do
	{
		printf("\n");
		printf("\n");
		printf("\n---------------------------------------------------");
		printf("\n-                USER MENU release %s           -", release_xy);
		printf("\n---------------------------------------------------");

		printf("\n");	
		printf("\n  %c)   -> CR95HFDll_GetDLLrev : get DLL revision", Get_Dll_rev_cmd);
		//printf("\n");
		printf("\n  %c)   -> CR95HFDLL_USBconnect : DEMO-CR95HF-A USB connection  <-- TBD 1st ", DEMO_CR95HF_A_USBconnect_cmd);
		//printf("\n");			
		printf("\n  %c)   -> CR95HFDLL_USBhandlecheck : DEMO-CR95HF-A USB handle check ", DEMO_CR95HF_A_USBhandlecheck_cmd);
		//printf("\n");
		printf("\n  %c)   -> CR95HFDll_GetMCUrev : get MCU revision",Get_MCU_rev_cmd);
		//printf("\n");
		printf("\n  %c)   -> CR95HFDll_Echo : send Echo command",Echo_cmd);
		//printf("\n");
		printf("\n  %c)   -> CR95HFDll_FieldOff : send Field Off request ",FieldOff_cmd);
		//printf("\n");	
		printf("\n  %c)   -> CR95HFDll_Idn : send Idn command",Idn_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_SELECT : select ISO15693 protocol",Select_ISO15693_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_SendRec : send ISO15693 Inventory request",Send_ISO15693_Inventory_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_SELECT : select ISO14443B protocol",Select_ISO14443B_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_SendRec : send ISO14443B Initiate request",Send_ISO14443B_Initiate_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_STCmd : read register",STCmd_RdReg_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_STCmd : ISO15693 protocol select",STCmd_ISO15693_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_STCmd : send Idle command",STCmd_Idle_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_ResetSPI : reset SPI",ResetSPI_cmd);
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_SendIRQPulse : send IRQ pulse",Send_IRQ_Pulse_cmd);	
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_GetInterfacePinState : get interface pin state",Get_Interface_Pin_State_cmd);	
		//printf("\n");		
		printf("\n  %c)   -> CR95HFDll_Polling_Reading : Polling Reading state",Set_Polling_Reading_config_cmd);	
		//printf("\n");	
		printf("\n  %c)   -> CR95HFDll_SendNSSPulse : send NSS pulse",Send_NSS_Pulse_cmd);	
		//printf("\n");	
		printf("\n  %c)   -> CR95HFDLL_getHardwareVersion : get Hardware version (dll 0.9 or upper)",Get_Hardware_rev_cmd);
		//printf("\n");
		printf("\n");

		printf("\n  %c)   -> Exit\n\n", exit_cmd);

		printf("your choice:?");
		scanf("%s", &entry);  

		switch (entry)
		{

		case exit_cmd:
			break;			

		case Get_Dll_rev_cmd: 
			{
				Get_Dll_rev();
			}; break;

		case DEMO_CR95HF_A_USBconnect_cmd: 
			{
				DEMO_CR95HF_A_USBconnect();
			}; break;

		case DEMO_CR95HF_A_USBhandlecheck_cmd: 
			{
				DEMO_CR95HF_A_USBhandlecheck();
			}; break;

		case Get_MCU_rev_cmd:
			{
				Get_MCU_rev();
			}; break;
			
		case Echo_cmd:
			{
				Echo();
			}; break;

		case FieldOff_cmd:
			{
				FieldOff();
			}; break;				

		case Idn_cmd:
			{
				Idn();
			}; break;							

		case Select_ISO15693_cmd:
			{
				Select_ISO15693();
			}; break;

		case Send_ISO15693_Inventory_cmd:
			{
				Send_ISO15693_Inventory();
			}; break;

		case Select_ISO14443B_cmd:
			{
				Select_ISO14443B();
			}; break;

		case Send_ISO14443B_Initiate_cmd:
			{
				Send_ISO14443B_Initiate();
			}; break;

		case STCmd_RdReg_cmd:
			{
				STCmd_RdReg();
			}; break;

		case STCmd_ISO15693_cmd:
			{
				STCmd_ISO15693();
			}; break;

		case STCmd_Idle_cmd:
			{
				STCmd_Idle();
			}; break;

		case ResetSPI_cmd:
			{
				ResetSPI();
			}; break;

		case Send_IRQ_Pulse_cmd:
			{
				Send_IRQ_Pulse();
			}; break;

		case Get_Interface_Pin_State_cmd:
			{
				Get_Interface_Pin_State();
			}; break;

		case Set_Polling_Reading_config_cmd:
			{
				Set_Polling_Reading_config();
			}; break;

		case Send_NSS_Pulse_cmd:
			{
				Send_NSS_Pulse();
			}; break;

		case Get_Hardware_rev_cmd:
			{
				Get_Hardware_rev();
			}; break;

		}
	} while (entry!='0');

}

int Get_Dll_rev (void)
{
  char strAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");

  iresult = CR95HFDll_GetDLLrev(strAnswer);
  
  printf("\nGet Dll revision using CR95HFDll_GetDLLrev function:\n");	
  printf("\n    --> request : CR95HFDll_GetDLLrev(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = DLL version\n\n\r", strAnswer);
  else
		printf("%s = Error : no communication with MCU\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;
}

int DEMO_CR95HF_A_USBconnect (void)
{
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");

  iresult= CR95HFDLL_USBconnect ();
  
  printf("\nCR95HFDll_USBconnect function:\n");
  printf("\n    --> request : CR95HFDll_USBconnect()");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("DEMO-CR95HF-A is connected and ready to be used\n\n\r");
  else
		printf("Error : DEMO-CR95HF-A not ready to be used\n\n\r");

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;
}

int DEMO_CR95HF_A_USBhandlecheck (void)
{
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");

  iresult= CR95HFDLL_USBhandlecheck ();
  
  printf("\nCR95HFDll_USBhandlecheck function:\n");
  printf("\n    --> request : CR95HFDll_USBhandlecheck()");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("DEMO-CR95HF-A is well connected\n\n\r");
  else
		printf("Error : DEMO-CR95HF-A not connected (use CR95HFDLL_USBConnect)\n\n\r");

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;
}

int Get_MCU_rev (void)
{
  char strAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");

  iresult = CR95HFDLL_getMCUrev(strAnswer);

  printf("\nGet MCU revision using CR95HFDLL_getMCUrev function:\n");	
  printf("\n    --> request : CR95HFDLL_getMCUrev(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = MCU version\n\n\r", strAnswer);
  else
		printf("%s = Error : no communication with MCU\n\n\r", strAnswer);
  
  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;
}

int Echo (void)
{
  char strAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");

  iresult= CR95HFDll_Echo (strAnswer);	

  printf("\nEcho using CR95HFDll_Echo function:\n");
  printf("\n    --> request : CR95HFDll_Echo(strAnswer)");
  printf("\n    <-- answer  : ");
  
  if (iresult == 0) 
		printf("%s = Echo command answer\n\n\r", strAnswer);
  else
		printf("%s = No answer from MCU\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);
  
  return iresult;
}

int FieldOff (void)
{
  int iresult;
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n");

  iresult= CR95HFDll_FieldOff (strAnswer);
  
  printf("\nField Off using CR95HFDll_FieldOff function:\n");
  printf("\n    --> request : CR95HFDll_FieldOff(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = RF Field Off ok\n\n\r", strAnswer);
  else
		printf("%s = Error : RF Field Off command error\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);
  
  return iresult;
}

int Idn (void)
{
  int iresult;
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n");

  iresult= CR95HFDll_Idn (strAnswer);	
  
  printf("\nIdn using CR95HFDll_Idn function:\n");
  printf("\n    --> request : CR95HFDll_Idn(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Idn command response\n\n\r", strAnswer);
  else
		printf("%s = Error : no Idn returned\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;	
}

int Select_ISO15693 (void)
{
  int iresult;
  char strRequest[50]="";
  char strAnswer[50]="";
  
  char entry3;	 

  printf("\n\n\n\n");
  
  strcpy(strRequest,"010D");

  iresult = CR95HFDll_Select(strRequest ,strAnswer);
  
  printf("\nProtocol select using CR95HFDll_Select function:\n");
  printf("\n    --> request : CR95HFDll_Select(%s, strAnswer)", strRequest);
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = ISO15693 protocol selected\n\n\r", strAnswer);
  else
		printf("%s = Select command error\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);
 
  return iresult;
}

int Send_ISO15693_Inventory (void)
{
  int iresult;
  char strRequest[50]="";
  char strTagAnswer[50]="";  
  char entry3;	 

  printf("\n\n\n\n");
  
  strcpy(strRequest,"260100");

  iresult=CR95HFDll_SendReceive(strRequest,strTagAnswer);
  
  printf("\nISO15695 inventory using CR95HFDll_SendReceive function:\n");
  printf("\n    --> request : CR95HFDll_SendReceive(%s, strTagAnswer)",strRequest);
  printf("\n    <-- answer  : ");

  if ((strTagAnswer[0] == '8') & (strTagAnswer[1] == '0')) //CR95HF Tag answer OK
  {
	printf("%s = Tag answer",strTagAnswer);	
  }
  else
  {
	printf("%s = No tag answer received\n");
  }	

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);  	

  return iresult;
}

int Select_ISO14443B (void)
{
  char strRequest[50]="";
  char strAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");
  
  strcpy(strRequest,"03010180");

  iresult = CR95HFDll_Select(strRequest ,strAnswer);
  
  printf("\nISO14443B protocol select using CR95HFDll_Select function:\n");
  printf("\n    --> request : CR95HFDll_Select(%s, strAnswer)",strRequest);
  printf("\n    <-- answer  : ");
  if (iresult == 0) 
		printf("%s = ISO14443B protocol selected\n\n\r", strAnswer);
  else
		printf("%s = Select command error\n\n\r", strAnswer);
 
  strcpy(strRequest,"01090468010130");

  iresult = CR95HFDll_STCmd(strRequest ,strAnswer);

  printf("\nImproving performance : INCREASE DEMO GAIN\n");
  printf("\n    --> request : CR95HFDll_STCmd(%s, strAnswer)",strRequest);
  printf("\n    <-- answer  : ");
  if (iresult == 0) 
		printf("%s = CR95HF performance improved\n\n\r", strAnswer);
  else
		printf("%s = Error return\n\n\r", strAnswer);


  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;	
}

int Send_ISO14443B_Initiate (void)
{
  char strRequest[50]="";
  char strTagAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");
  
  strcpy(strRequest,"0600");

  iresult=CR95HFDll_SendReceive(strRequest,strTagAnswer);
  
  printf("\nISO14443B Initiate using CR95HFDll_SendReceive function:\n");
  printf("\n    --> request : CR95HFDll_SendReceive(%s, strTagAnswer)",strRequest);
  printf("\n    <-- answer  : ");

  if ((strTagAnswer[0] == '8') & (strTagAnswer[1] == '0')) //CR95HF Tag answer OK
  {
	printf("%s = Tag answer",strTagAnswer);	
  }
  else
  {
	printf("No tag answer received\n");
	printf("CR95HF answer : %s",strTagAnswer);
  }	
  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);  	

  return iresult;
}

int STCmd_RdReg (void)
{
  int iresult;
  char strRequest[50]="";
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n");
  
  strcpy(strRequest,"010803620100");

  iresult = CR95HFDll_STCmd(strRequest ,strAnswer);

  printf("\nRead register using CR95HFDll_STCmd function:\n");
  printf("\n    --> request : CR95HFDll_STCmd(%s, strAnswer)",strRequest);
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Read Register answer\n\n\r", strAnswer);
  else
		printf("%s = No answer from CR95HF IC\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

 return iresult;
}

int STCmd_ISO15693 (void)
{
  char strRequest[50]="";
  char strAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");
  
  strcpy(strRequest,"010202010D");

  iresult = CR95HFDll_STCmd(strRequest ,strAnswer);
  
  printf("\nISO15693 protocol select using CR95HFDll_STCmd function:\n");
  printf("\n    --> request : CR95HFDll_STCmd(%s, strAnswer)",strRequest);
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = ISO15693 protocol selected successfully\n\n\r", strAnswer);
  else
		printf("%s = Protocol not selected\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;
}

int STCmd_Idle (void)
{
  char strRequest[50]="";
  char strAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");
  
  strcpy(strRequest,"01070E0B21007801180020606064743F01");

  iresult = CR95HFDll_STCmd(strRequest ,strAnswer);
  
  printf("\nIdle using CR95HFDll_STCmd function:\n");
  printf("\n    --> request : CR95HFDll_STCmd(%s, strAnswer)",strRequest);
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Idle answer\n\n\r", strAnswer);
  else
		printf("%s = No answer from CR95HF IC\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);
  
  return iresult;
}

int ResetSPI (void)
{
  int iresult;
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n"); 

  iresult = CR95HFDll_ResetSPI(strAnswer);

  printf("\nReset SPI using CR95HFDll_ResetSPI function:\n");
  printf("\n    --> request : CR95HFDll_ResetSPI(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Reset SPI successfully\n\n\r", strAnswer);
  else
		printf("%s = No answer from DEMO-CR95HF-A\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);
  
  return iresult;
}

int Send_IRQ_Pulse (void)
{
  int iresult;
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n"); 

  iresult = CR95HFDll_SendIRQPulse(strAnswer);

  printf("\nSend IRQ Pulse using CR95HFDll_SendIRQPulse function:\n");
  printf("\n    --> request : CR95HFDll_SendIRQPulse(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Send IRQ pulse successfully\n\n\r", strAnswer);
  else
		printf("%s = No answer from DEMO-CR95HF-A\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;	
}

int Get_Interface_Pin_State (void)
{
  int iresult;
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n"); 

  iresult = CR95HFDLL_getInterfacePinState(strAnswer);

  printf("\nGet Interface Pin State CR95HFDLL_getInterfacePinState function:\n");
  printf("\n    --> request : CR95HFDLL_getInterfacePinState(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
  {
	  printf("%s = Get Interface Pin State successfully\n\n\r", strAnswer);
	  if (strAnswer[3] == '0')
		printf("\n        %c = communication in UART mode",strAnswer[3]);	
	  else
		printf("\n        %c = communication in SPI mode",strAnswer[3]);
  }
  else
  	  printf("\n        %s = No answer from DEMO-CR95HF-A\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;	
}

int Set_Polling_Reading_config (void)
{
  int iresult;
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n"); 

  iresult = CR95HFDll_Polling_Reading(strAnswer);

  printf("\nSend IRQ Pulse using CR95HFDll_Polling_Reading function:\n");
  printf("\n    --> request : CR95HFDll_Polling_Reading(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Set Polling Reading successfully\n\n\r", strAnswer);
  else
		printf("%s = No answer from DEMO-CR95HF-A\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;	
}

int Send_NSS_Pulse (void)
{
  int iresult;
  char strAnswer[50]="";
  char entry3;	 

  printf("\n\n\n\n"); 

  iresult = CR95HFDll_SendNSSPulse(strAnswer);

  printf("\nSend IRQ Pulse using CR95HFDll_SendNSSPulse function:\n");
  printf("\n    --> request : CR95HFDll_SendNSSPulse(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Send NSS pulse successfully\n\n\r", strAnswer);
  else
		printf("%s = No answer from DEMO-CR95HF-A\n\n\r", strAnswer);

  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;	
}

int Get_Hardware_rev (void)
{
  char strAnswer[50]="";
  int iresult;
  char entry3;	 

  printf("\n\n\n\n");

  iresult = CR95HFDLL_getHardwareVersion(strAnswer);

  printf("\nGet Hardware revision using CR95HFDLL_getHardwareVersion function:\n");	
  printf("\n    --> request : CR95HFDLL_getHardwarerev(strAnswer)");
  printf("\n    <-- answer  : ");

  if (iresult == 0) 
		printf("%s = Hardware revision\n\n\r", strAnswer);
  else
		printf("%s = Error : no communication with MCU\n\n\r", strAnswer);
  
  printf("\n\n\n\n");
  printf("\npress any key to continue");
  printf("\n");
  scanf("%s", &entry3);

  return iresult;
}