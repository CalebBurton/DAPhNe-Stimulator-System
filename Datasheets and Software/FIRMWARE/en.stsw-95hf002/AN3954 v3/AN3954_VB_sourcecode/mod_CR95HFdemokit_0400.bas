Attribute VB_Name = "mod_CR95HFdemokit"
'---------------------------------------------------------
'|                                                       |
'|                      MOD_CR95HFdemokit.bas            |
'|                                                       |
'---------------------------------------------------------
'
'
'


Option Explicit

Public Declare Function CR95HFDll_Echo Lib "CR95HF.dll" (ByVal mystring As String) As Long

Public Declare Function CR95HFDll_Idn Lib "CR95HF.dll" (ByVal mystring As String) As Long

Public Declare Function CR95HFDll_Select Lib "CR95HF.dll" (ByVal mycmdstring As String, ByVal mystring As String) As Long

Public Declare Function CR95HFDll_SendReceive Lib "CR95HF.dll" (ByVal mycmdstring As String, ByVal mystring As String) As Long

Public Declare Function CR95HFDll_STCmd Lib "CR95HF.dll" (ByVal mycmdstring As String, ByVal mystring As String) As Long

Public Declare Function CR95HFDll_FieldOff Lib "CR95HF.dll" (ByVal mystring As String) As Long

Public Declare Function CR95HFDll_GetDLLrev Lib "CR95HF.dll" (ByVal mystring As String) As Long

Public Declare Function CR95HFDll_ResetSPI Lib "CR95HF.dll" (ByVal mystring As String) As Long

Public Declare Function CR95HFDll_SendIRQPulse Lib "CR95HF.dll" (ByVal mystring As String) As Long

Public Declare Function CR95HFDLL_getMCUrev Lib "CR95HF.dll" (ByVal mystring As String) As Long

'added in Dll revision 0.4
Public Declare Function CR95HFDLL_getInterfacePinState Lib "CR95HF.dll" (ByVal mystring As String) As Long

'added in Dll revision 0.5
Public Declare Function CR95HFDLL_USBconnect Lib "CR95HF.dll" () As Long

Public Declare Function CR95HFDll_Polling_Reading Lib "CR95HF.dll" (ByVal mystring As String) As Long

Public Declare Function CR95HFDll_SendNSSPulse Lib "CR95HF.dll" (ByVal mystring As String) As Long

'added in DLL revision 0.6
Public Declare Function CR95HFDLL_USBhandlecheck Lib "CR95HF.dll" () As Long

'added in DLL revision 0.9
Public Declare Function CR95HFDLL_getHardwareVersion Lib "CR95HF.dll" (ByVal mystring As String) As Long


Public Const CR95HF_STM32_error = &H1
Public Const CR95HF_empty_argument_error = &H2
Public Const CR95HF_cmd_parameter_error = &H3
Public Const CR95HF_communication_error = &H4
Public Const CR95HF_USB_communication_error = &H5

Public Const CR95HF_ERRORCODE_DEFAULT = &HFE
Public Const CR95HF_ERRORCODE_TIMEOUT = &HFD

