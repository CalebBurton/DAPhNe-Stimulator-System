// CR95HF DLL function import based on CR95HFDll.dll revision 0.5

__declspec(dllimport) int  __stdcall CR95HFDll_Echo(char *Stringreply);
__declspec(dllimport) int  __stdcall CR95HFDll_Idn(char *Stringreply);
__declspec(dllimport) int  __stdcall CR95HFDll_Select(char *StringCmd, char *Stringreply);
__declspec(dllimport) int  __stdcall CR95HFDll_SendReceive(char *StringCmd, char *StringReply);
__declspec(dllimport) int  __stdcall CR95HFDll_STCmd(char *StringCmd, char *StringReply);
__declspec(dllimport) int  __stdcall CR95HFDll_FieldOff(char *StringReply);
__declspec(dllimport) int  __stdcall CR95HFDll_GetDLLrev(char* stringreply);
__declspec(dllexport) int  __stdcall CR95HFDll_ResetSPI(char* stringreply);
__declspec(dllexport) int  __stdcall CR95HFDll_SendIRQPulse(char* stringreply);
__declspec(dllexport) int  __stdcall CR95HFDLL_getMCUrev(char* stringreply);
__declspec(dllexport) int  __stdcall CR95HFDLL_getInterfacePinState(char* stringreply);

//new in DLL 0.5
__declspec(dllexport) int  __stdcall CR95HFDLL_USBconnect(void);
__declspec(dllexport) int  __stdcall CR95HFDll_Polling_Reading(char* StringReply);
__declspec(dllexport) int  __stdcall CR95HFDll_SendNSSPulse(char* StringReply);

//new in DLL 0.6
__declspec(dllexport) int  __stdcall CR95HFDLL_USBhandlecheck(void);

//new in DLL 0.9
__declspec(dllexport) int  __stdcall CR95HFDLL_getHardwareVersion(char* StringReply);