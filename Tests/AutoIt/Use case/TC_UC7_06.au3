;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; Test Cases:		Message Transmission
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC7_06.au3****"&@CRLF)
$fPath=0
if winexists("BUSMASTER") Then
	WinMenuSelectItem("BUSMASTER","","&J1939","&Configure","&Log")
	if winexists("Configure Logging for J1939") Then
		$fPath=ControlListView("Configure Logging for J1939","","SysListView321","GetText",0,0)
		ControlClick("Configure Logging for J1939","&OK","[CLASS:Button; INSTANCE:14]")
		sleep(1000)
	EndIf
EndIf
ConsoleWrite("File Path : " &$fPath&@CRLF)
ConsoleWrite("****End : TC_UC7_06.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("***********UseCase 7 Script Execution Ended************"&@CRLF)