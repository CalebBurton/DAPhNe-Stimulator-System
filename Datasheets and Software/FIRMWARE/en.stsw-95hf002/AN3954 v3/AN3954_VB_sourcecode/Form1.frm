VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   8790
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   13620
   LinkTopic       =   "Form1"
   ScaleHeight     =   8790
   ScaleWidth      =   13620
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtHardwareRevAscii 
      Height          =   285
      Left            =   4560
      TabIndex        =   47
      Top             =   1200
      Width           =   1875
   End
   Begin VB.TextBox txtHardwareRev 
      Height          =   285
      Left            =   2640
      TabIndex        =   46
      Top             =   1200
      Width           =   1875
   End
   Begin VB.CommandButton cmdGetHardwareRev 
      Caption         =   "Get HARDWARE rev *"
      Height          =   375
      Left            =   360
      TabIndex        =   45
      Top             =   1080
      Width           =   2175
   End
   Begin VB.TextBox txtUSBHandlecheckAnswer 
      Height          =   285
      Left            =   10920
      TabIndex        =   44
      Top             =   1500
      Width           =   2295
   End
   Begin VB.CommandButton cmdUSBHandlecheck 
      Caption         =   "DEMO-CR95HF-A USB Handle Check"
      Height          =   375
      Left            =   7560
      TabIndex        =   42
      Top             =   1380
      Width           =   3255
   End
   Begin VB.CommandButton cmdUSBconnect 
      Caption         =   "DEMO-CR95HF-A USB connect"
      Height          =   375
      Left            =   7560
      TabIndex        =   38
      Top             =   420
      Width           =   3255
   End
   Begin VB.TextBox txtUSBconnectAnswer 
      Height          =   285
      Left            =   10920
      TabIndex        =   37
      Top             =   540
      Width           =   2295
   End
   Begin VB.TextBox txtPollingReadingAnswer 
      Height          =   285
      Left            =   9420
      TabIndex        =   36
      Top             =   7260
      Width           =   2655
   End
   Begin VB.CommandButton cmdPollingReading 
      Caption         =   "Polling Reading sequence"
      Height          =   375
      Left            =   6660
      TabIndex        =   35
      Top             =   7140
      Width           =   2535
   End
   Begin VB.TextBox txtNSSpulseAnswer 
      Height          =   285
      Left            =   9420
      TabIndex        =   34
      Top             =   7740
      Width           =   2775
   End
   Begin VB.CommandButton cmdSendNSSPulse 
      Caption         =   "Send interrupt pulse on NSS pin"
      Height          =   375
      Left            =   6660
      TabIndex        =   33
      Top             =   7620
      Width           =   2535
   End
   Begin VB.CheckBox chkInterfacePinState 
      Caption         =   "communication in SPI mode"
      Height          =   195
      Index           =   1
      Left            =   3840
      TabIndex        =   32
      Top             =   8220
      Width           =   2535
   End
   Begin VB.CheckBox chkInterfacePinState 
      Caption         =   "communication in UART mode"
      Height          =   195
      Index           =   0
      Left            =   3840
      TabIndex        =   31
      Top             =   7980
      Width           =   2535
   End
   Begin VB.TextBox txtGetInterfacePinStateAnswer 
      Height          =   285
      Left            =   3120
      TabIndex        =   30
      Top             =   7680
      Width           =   3255
   End
   Begin VB.CommandButton cmdGetInterfacePinState 
      Caption         =   "Get Interface Pin State"
      Height          =   375
      Left            =   360
      TabIndex        =   29
      Top             =   7560
      Width           =   2535
   End
   Begin VB.CommandButton cmdSendIRQPulse 
      Caption         =   "Send interrupt pulse on IRQ pin"
      Height          =   375
      Left            =   360
      TabIndex        =   28
      Top             =   7080
      Width           =   2535
   End
   Begin VB.TextBox txtSendIRQPulseAnswer 
      Height          =   285
      Left            =   3120
      TabIndex        =   27
      Top             =   7200
      Width           =   2655
   End
   Begin VB.CommandButton cmdResetSPI 
      Caption         =   "Reset SPI"
      Height          =   375
      Left            =   360
      TabIndex        =   26
      Top             =   6600
      Width           =   2535
   End
   Begin VB.TextBox txtResetSPIAnswer 
      Height          =   285
      Left            =   3120
      TabIndex        =   25
      Top             =   6720
      Width           =   2655
   End
   Begin VB.CommandButton cmdSTCmd_Idle 
      Caption         =   "Idle ""01070E03A200F801980001606000003F28"" with CR95HFDll_STCmd"
      Height          =   375
      Left            =   360
      TabIndex        =   24
      Top             =   5640
      Width           =   6375
   End
   Begin VB.TextBox txtSTCmd_Idle 
      Height          =   285
      Left            =   6960
      TabIndex        =   23
      Top             =   5760
      Width           =   2175
   End
   Begin VB.CommandButton cmdSTCmd_ISO15693 
      Caption         =   "ISO15693 ""010202010D"" with CR95HFDll_STCmd"
      Height          =   375
      Left            =   360
      TabIndex        =   22
      Top             =   5160
      Width           =   6375
   End
   Begin VB.TextBox txtSTCmd_ISO15693 
      Height          =   285
      Left            =   6960
      TabIndex        =   21
      Top             =   5280
      Width           =   2175
   End
   Begin VB.CommandButton cmdSTCmd_RdReg 
      Caption         =   "RgReg ""010803620100"" with CR95HFDll_STCmd"
      Height          =   375
      Left            =   360
      TabIndex        =   20
      Top             =   4680
      Width           =   6375
   End
   Begin VB.TextBox txtSTCmd_RdReg 
      Height          =   285
      Left            =   6960
      TabIndex        =   19
      Top             =   4800
      Width           =   2175
   End
   Begin VB.TextBox txtISO14443B_Initiate_Answer 
      Height          =   285
      Left            =   7020
      TabIndex        =   18
      Top             =   4200
      Width           =   2055
   End
   Begin VB.CommandButton cmdSendReceive_ISO14443B_Initiate 
      Caption         =   "ISO14443B Initiate"
      Height          =   375
      Left            =   4740
      TabIndex        =   17
      Top             =   4080
      Width           =   2175
   End
   Begin VB.TextBox txtISO15693_Inventory_Answer 
      Height          =   285
      Left            =   2640
      TabIndex        =   16
      Top             =   4200
      Width           =   1755
   End
   Begin VB.CommandButton cmdSendReceive_ISO15693_Inventory 
      Caption         =   "ISO15693 Inventory"
      Height          =   375
      Left            =   360
      TabIndex        =   15
      Top             =   4080
      Width           =   2175
   End
   Begin VB.TextBox txtISO14443B_SelectAnswer 
      Height          =   285
      Left            =   7020
      TabIndex        =   14
      Top             =   3600
      Width           =   1995
   End
   Begin VB.CommandButton cmdSelect_ISO14443B_protocol 
      Caption         =   "Select ISO14443B"
      Height          =   375
      Left            =   4740
      TabIndex        =   13
      Top             =   3480
      Width           =   2175
   End
   Begin VB.CommandButton cmdSelect_ISO15693_protocol 
      Caption         =   "Select ISO15693"
      Height          =   375
      Left            =   360
      TabIndex        =   12
      Top             =   3480
      Width           =   2175
   End
   Begin VB.TextBox txtISO15693_SelectAnswer 
      Height          =   285
      Left            =   2640
      TabIndex        =   11
      Top             =   3600
      Width           =   1755
   End
   Begin VB.TextBox txtIdnAnswer 
      Height          =   285
      Left            =   2640
      TabIndex        =   10
      Top             =   2640
      Width           =   4335
   End
   Begin VB.CommandButton cmdIdnRequest 
      Caption         =   "Idn"
      Height          =   375
      Left            =   360
      TabIndex        =   9
      Top             =   2520
      Width           =   2175
   End
   Begin VB.CommandButton cmdFieldOff 
      Caption         =   "Field Off"
      Height          =   375
      Left            =   360
      TabIndex        =   8
      Top             =   2040
      Width           =   2175
   End
   Begin VB.TextBox txtFieldOffAnswer 
      Height          =   285
      Left            =   2640
      TabIndex        =   7
      Top             =   2160
      Width           =   4335
   End
   Begin VB.TextBox txtEchoAnswer 
      Height          =   285
      Left            =   2640
      TabIndex        =   6
      Top             =   1680
      Width           =   4335
   End
   Begin VB.CommandButton cmdEcho 
      Caption         =   "Echo"
      Height          =   375
      Left            =   360
      TabIndex        =   5
      Top             =   1560
      Width           =   2175
   End
   Begin VB.CommandButton cmdGetMCUrev 
      Caption         =   "Get MCU rev"
      Height          =   375
      Left            =   360
      TabIndex        =   4
      Top             =   660
      Width           =   2175
   End
   Begin VB.TextBox txtMCUrev 
      Height          =   285
      Left            =   2640
      TabIndex        =   3
      Top             =   780
      Width           =   1695
   End
   Begin VB.TextBox txtDllRev 
      Height          =   285
      Left            =   2640
      TabIndex        =   2
      Top             =   300
      Width           =   1695
   End
   Begin VB.CommandButton cmdGetDllRev 
      Caption         =   "Get DLL rev"
      Height          =   375
      Left            =   360
      TabIndex        =   1
      Top             =   180
      Width           =   2175
   End
   Begin VB.Label Label6 
      Caption         =   $"Form1.frx":0000
      BeginProperty Font 
         Name            =   "Calibri"
         Size            =   11.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1995
      Left            =   9300
      TabIndex        =   51
      Top             =   5100
      Width           =   4275
   End
   Begin VB.Label Label5 
      Caption         =   "Note : this software is only some examples to explain how to write functions to communicate with the DEMO-CR95HF and Visual Basic"
      BeginProperty Font 
         Name            =   "Calibri"
         Size            =   11.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   -1  'True
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1155
      Left            =   9300
      TabIndex        =   50
      Top             =   3840
      Width           =   4275
   End
   Begin VB.Label Label4 
      Caption         =   "* only available from Dll revision 0.9 && latest boards"
      Height          =   195
      Left            =   9540
      TabIndex        =   49
      Top             =   8460
      Width           =   3975
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      Caption         =   "Dll 0.9 included"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   9900
      TabIndex        =   48
      Top             =   3240
      Width           =   2415
   End
   Begin VB.Label lblUSBHandlecheck 
      Caption         =   "To check USB connection"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   8040
      TabIndex        =   43
      Top             =   1020
      Width           =   3615
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "Visual Basic 6 source code example for "
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   8760
      TabIndex        =   41
      Top             =   2400
      Width           =   4695
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "AN3954 version 3"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   9960
      TabIndex        =   40
      Top             =   2040
      Width           =   2415
   End
   Begin VB.Label lblUSBconnect 
      Caption         =   "To be done first !"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   8040
      TabIndex        =   39
      Top             =   60
      Width           =   2175
   End
   Begin VB.Label lblWarning 
      Alignment       =   2  'Center
      Caption         =   "DEMO-CR95HF-A"
      BeginProperty Font 
         Name            =   "Comic Sans MS"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   9960
      TabIndex        =   0
      Top             =   2760
      Width           =   2415
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdGetDllRev_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDll_GetDLLrev(strAnswer)

If (lngStatus = 0) Then
    txtDllRev.Text = strAnswer
Else
    txtDllRev.Text = "Error = no Dll found"
End If
  
End Sub

Private Sub cmdGetInterfacePinState_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

chkInterfacePinState(0).Value = vbUnchecked
chkInterfacePinState(1).Value = vbUnchecked

lngStatus = CR95HFDLL_getInterfacePinState(strAnswer)

If (lngStatus = 0) Then
    txtGetInterfacePinStateAnswer.Text = strAnswer
    If (Mid(strAnswer, 6, 1) = "0") Then
        chkInterfacePinState(0).Value = vbChecked
        chkInterfacePinState(1).Value = vbUnchecked
    ElseIf (Mid(strAnswer, 6, 1) = "1") Then
        chkInterfacePinState(0).Value = vbUnchecked
        chkInterfacePinState(1).Value = vbChecked
    Else
        DoEvents
        'error
    End If
Else
    txtGetInterfacePinStateAnswer.Text = "No answer from the DEMO-CR95HF-A board"
End If
    
End Sub

Private Sub cmdGetMCURev_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDLL_getMCUrev(strAnswer)

If (lngStatus = 0) Then
    txtMCUrev.Text = strAnswer
Else
    txtMCUrev.Text = "Error = no communication with STM32 MCU"
End If
  
End Sub

Private Sub cmdGetHardwareRev_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long
Dim i As Integer

txtHardwareRevAscii.Text = ""

lngStatus = CR95HFDLL_getHardwareVersion(strAnswer)

If (lngStatus = 0) Then
    If (Mid(strAnswer, 1, 2) = "00") Then
        txtHardwareRev.Text = strAnswer
        For i = 0 To Mid(strAnswer, 3, 2) - 1
            txtHardwareRevAscii.Text = txtHardwareRevAscii.Text & Chr(CLng("&h" & Mid(strAnswer, (i * 2) + 5, 2)))
        Next i
    Else
        txtHardwareRev.Text = strAnswer
        txtHardwareRevAscii.Text = "Hardware doesn't contain Hardware Revision"
    End If
Else
    txtHardwareRevAscii.Text = "Error = no communication with STM32 MCU"
End If
  
End Sub

Private Sub cmdEcho_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDll_Echo(strAnswer)

If (lngStatus = 0) Then
    txtEchoAnswer.Text = strAnswer
Else
    txtEchoAnswer.Text = "No communication with DEMO-CR95HF-A board"
End If
  
End Sub

Private Sub cmdFieldOff_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDll_FieldOff(strAnswer)

If (lngStatus = 0) Then
    txtFieldOffAnswer.Text = strAnswer
Else
    txtFieldOffAnswer.Text = "Field Off request not executed"
End If
  
End Sub

Private Sub cmdIDnRequest_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDll_Idn(strAnswer)

If (lngStatus = 0) Then
    txtIdnAnswer.Text = strAnswer
Else
    txtIdnAnswer.Text = "Idn not sent back by the CR95HF IC"
End If
  
End Sub

Private Sub cmdSelect_ISO15693_protocol_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

    'CR95HF put in ISO15693 HIGHDATARATE ONE SUBCARRIER mode 10%
    lngStatus = CR95HFDll_Select("010D", strAnswer)
    
    If (lngStatus = 0) Then
        txtISO15693_SelectAnswer.Text = strAnswer
    Else
        txtISO15693_SelectAnswer.Text = "Select protocol ISO15693 not executed"
    End If
  
End Sub

Private Sub cmdSelect_ISO14443B_protocol_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

    'Select protocol ISO14443-B
    lngStatus = CR95HFDll_Select("03010180", strAnswer)
    
    'CR95HF INCREASE DEMO GAIN for ISO14443-B
    lngStatus = lngStatus + CR95HFDll_STCmd("01" & "090468010130", strAnswer)     'modified 20120914
        
    If (lngStatus = 0) Then
        txtISO14443B_SelectAnswer.Text = strAnswer
    Else
        txtISO14443B_SelectAnswer.Text = "Select protocol ISO14443B not executed"
    End If
  
End Sub

Private Sub cmdSendReceive_ISO15693_Inventory_Click()
Dim strTagResponse  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDll_SendReceive("260100", strTagResponse)

If (lngStatus = 0) Then
    txtISO15693_Inventory_Answer.Text = strTagResponse
Else
    txtISO15693_Inventory_Answer.Text = "No answer from the Tag"
End If
  
End Sub

Private Sub cmdSendReceive_ISO14443B_Initiate_Click()
Dim strTagResponse  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDll_SendReceive("0600", strTagResponse)

If (lngStatus = 0) Then
    txtISO14443B_Initiate_Answer.Text = strTagResponse
Else
    txtISO14443B_Initiate_Answer.Text = "No answer from the Tag"
End If
  
End Sub


Private Sub cmdSTCmd_RdReg_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

lngStatus = CR95HFDll_STCmd("010803620100", strAnswer)

If (lngStatus = 0) Then
    txtSTCmd_RdReg.Text = strAnswer
Else
    txtSTCmd_RdReg.Text = "Read register not executed properly"
End If

End Sub

Private Sub cmdSTCmd_ISO15693_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

txtSTCmd_ISO15693.Text = ""

lngStatus = CR95HFDll_STCmd("010202010D", strAnswer)

If (lngStatus = 0) Then
    txtSTCmd_ISO15693.Text = strAnswer
Else
    txtSTCmd_ISO15693.Text = "ISO15693 protocol not selected"
End If

End Sub

Private Sub cmdSTCmd_Idle_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

txtSTCmd_Idle.Text = ""

lngStatus = CR95HFDll_STCmd("01070E03A200F801980001606000003F28", strAnswer)

If (lngStatus = 0) Then
    txtSTCmd_Idle.Text = strAnswer
Else
    txtSTCmd_Idle.Text = "No Idn returned from CR95HF IC"
End If

End Sub

Private Sub cmdResetSPI_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

txtResetSPIAnswer.Text = ""

lngStatus = CR95HFDll_ResetSPI(strAnswer)

If (lngStatus = 0) Then
    txtResetSPIAnswer.Text = strAnswer
Else
    txtResetSPIAnswer.Text = "No answer from the DEMO-CR95HF-A board"
End If
    
End Sub

Private Sub cmdSendIRQPulse_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

txtSendIRQPulseAnswer.Text = ""

lngStatus = CR95HFDll_SendIRQPulse(strAnswer)

If (lngStatus = 0) Then
    txtSendIRQPulseAnswer.Text = strAnswer
Else
    txtSendIRQPulseAnswer.Text = "No answer from the DEMO-CR95HF-A board"
End If
    
End Sub

Private Sub cmdUSBconnect_Click()
Dim lngStatus As Long

txtUSBconnectAnswer.Text = ""

lngStatus = CR95HFDLL_USBconnect()

If (lngStatus = 0) Then
    txtUSBconnectAnswer.Text = "DEMO-CR95HF-A connected and ready to be used"
Else
    txtUSBconnectAnswer.Text = "Warning : DEMO-CR95HF-A not connected"
End If
End Sub

Private Sub cmdPollingReading_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

txtPollingReadingAnswer.Text = ""

lngStatus = CR95HFDll_Polling_Reading(strAnswer)

If (lngStatus = 0) Then
    txtPollingReadingAnswer.Text = strAnswer
Else
    txtPollingReadingAnswer.Text = "No answer from the DEMO-CR95HF-A board"
End If
    
End Sub

Private Sub cmdSendNSSPulse_Click()
Dim strAnswer  As String * 256
Dim lngStatus As Long

txtNSSpulseAnswer.Text = ""

lngStatus = CR95HFDll_SendNSSPulse(strAnswer)

If (lngStatus = 0) Then
    txtNSSpulseAnswer.Text = strAnswer
Else
    txtNSSpulseAnswer.Text = "No answer from the DEMO-CR95HF-A board"
End If
    
End Sub

Private Sub cmdUSBHandlecheck_Click()
Dim lngStatus As Long

txtUSBHandlecheckAnswer.Text = ""

lngStatus = CR95HFDLL_USBhandlecheck()

If (lngStatus = 0) Then
    txtUSBHandlecheckAnswer.Text = "DEMO-CR95HF-A connected"
ElseIf (lngStatus = 5) Then
    txtUSBHandlecheckAnswer.Text = "DEMO-CR95HF-A not connected (use CR95HFDLL_USBconnect to connect it again"
End If

End Sub

