;=== Test Cases/Test Data ===
; UseCase 1:		transmit J1939 Message Transmission - Data
; Critical (C):		Y
; TestCase No.:		TC_UC5_02
; TestCases:		Create or load Database(dbf) files
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

ConsoleWrite("****Start : TC_UC7_02.au3****"&@CRLF)
_launchApp()
if $app=0 Then
	_loadConfig("UseCase4")
EndIf
_loadJ1939Database("J1939Test",25,10,8)
send("!JDC")			; Close the database
_associateDB("&J1939","J1939Test.dbf")

ConsoleWrite("****End : TC_UC7_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)