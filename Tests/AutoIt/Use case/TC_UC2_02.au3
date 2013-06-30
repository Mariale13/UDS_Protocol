;=== Test Cases/Test Data ===
; UseCase 1:		transmit CAN Messages
; Critical (C):		Y
; TestCase No.:		TC_02
; Test Cases:		Create or load and Associate database(dbf) file
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC2_02.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase2.cfx")
EndIf
_loadDatabase("AutoitTest")
sleep(500)
send("!FDC")								; Close the database
_associateDB("&File","AutoitTest.dbf")
sleep(1000)

ConsoleWrite("$lDb : "&$lDb&@CRLF)
ConsoleWrite("$msgCount : "&$msgCount&@CRLF)
ConsoleWrite("$sigCount : "&$sigCount&@CRLF)
ConsoleWrite("$flag : "&$flag&@CRLF)

ConsoleWrite("****End : TC_UC2_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)