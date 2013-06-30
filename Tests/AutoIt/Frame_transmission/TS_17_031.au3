; Feature:			Frame Transmission
#Include <GuiListView.au3>
#Include <GuiToolBar.au3>

if WinExists("BUSMASTER") then
	sleep(2000)
	ControlClick("BUSMASTER","",1226)  							; add a block
	ControlCommand("BUSMASTER","",1008,"Check")					; Select 'Monoshot'
	ControlCommand("BUSMASTER","",1237,"Check")					; Select 'Key Trigger'
	Controlsettext("BUSMASTER","",1137,"B")						; Edit Key trigger value
	ControlCommand("BUSMASTER","",1010,"SelectString","All")	; Select All msgs from 'msg' combo box
	if (ControlCommand("BUSMASTER","",17000,"IsChecked")=0) Then
		ControlCommand("BUSMASTER","",17000,"Check")				; check 'Autoupdate' check box
	EndIf
	Controlsettext("BUSMASTER","",1001,300)						; Add '300' to 'Name/ID' edit box
	Controlsettext("BUSMASTER","",1126,100)						; change the DB1 value
	Controlsettext("BUSMASTER","",1127,100)						; change the DB2 value
	Controlsettext("BUSMASTER","",1128,100)						; change the DB3 value
	Controlsettext("BUSMASTER","",1129,100)						; change the DB4 value
	Controlsettext("BUSMASTER","",1130,100)						; change the DB5 value
	Controlsettext("BUSMASTER","",1131,100)						; change the DB6 value
	Controlsettext("BUSMASTER","",1132,100)						; change the DB7 value
	Controlsettext("BUSMASTER","",1133,100)						; change the DB8 value
	ControlClick("BUSMASTER","",1122)  							; Click Add message button
	Controlsettext("BUSMASTER","",1001,200)						; Add '200' to 'Name/ID' edit box
	ControlClick("BUSMASTER","",1122)  							; Click Add message button
	Controlsettext("BUSMASTER","",1001,100)						; Add '100' to 'Name/ID' edit box
	ControlClick("BUSMASTER","",1122)  							; Click Add message button
	$sigDetHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysListView32; INSTANCE:1]")	; Get handle of Signal details list view control
	_GUICtrlListView_ClickItem($sigDetHWD,0,"Left","",2)							; Click on the first item
	sleep(1000)
	if winexists("Signal Details") Then
		ControlClick("Signal Details","",2)											; Click Close button
	EndIf
	$hWnd = ControlGetHandle("BUSMASTER","",1232)
	$sigDetPos=_GUICtrlListView_GetItemPosition($hWnd, 0)							; Get position of Signal details list view control
	ControlClick("BUSMASTER","",1232,"Left",2,$sigDetPos[0]+100,$sigDetPos[1])		; Double click on the raw value
	sleep(500)
	send("{DOWN}")
	sleep(500)
	ControlClick("BUSMASTER","",1232,"Left",2,$sigDetPos[0]+200,$sigDetPos[1])		; Double click on the physical value
	sleep(500)
	send("{DOWN}")
	send("{ENTER}")
	$cntToolhWd=ControlGetHandle("BUSMASTER","",128)
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							;connect
	WinMenuSelectItem("BUSMASTER","","F&unctions","&Transmit","&Normal Blocks")	;transmit
	$txMsgListHWD=ControlGetHandle("BUSMASTER","","[CLASS:SysListView32; INSTANCE:2]")
	_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,False)				; uncheck all items in 'Tx msg List'
	sleep(3000)
 	_GUICtrlListView_SetItemChecked($txMsgListHWD,-1,True)				; check all items in 'Tx msg List'
	sleep(3000)
	_GUICtrlListView_ClickItem($txMsgListHWD,0,"Right")					; Right click on first msg and 'send'
	send("s")
	sleep(2000)
	send("+b")
	_GUICtrlListView_ClickItem($txMsgListHWD,4,"Right")
	_GUICtrlToolbar_ClickIndex($cntToolhWd,4)							; disconnect
	; Close the Tx Window
	ControlClick("BUSMASTER","",1015)
EndIf




