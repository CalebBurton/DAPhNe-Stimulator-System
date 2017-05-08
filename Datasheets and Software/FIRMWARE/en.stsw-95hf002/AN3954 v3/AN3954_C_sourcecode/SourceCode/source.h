
#define release_xy "3.00"


int Get_Dll_rev(void);
int Get_MCU_rev(void);
int Echo(void);
int FieldOff(void);
int Idn(void);
int Select_ISO15693(void);
int Send_ISO15693_Inventory(void);
int Select_ISO14443B(void);
int Send_ISO14443B_Initiate(void);
int STCmd_RdReg(void);
int STCmd_ISO15693(void);
int STCmd_Idle(void);
int ResetSPI(void);
int Send_IRQ_Pulse(void);
int Get_Interface_Pin_State (void);

int DEMO_CR95HF_A_USBconnect (void);
int Set_Polling_Reading_config (void);
int Send_NSS_Pulse (void);

int DEMO_CR95HF_A_USBhandlecheck (void);

int Get_Hardware_rev(void);