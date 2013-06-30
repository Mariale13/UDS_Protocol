;Test Case Number:	TS_17_013
;Feature:			Frame Transmission
; Critical (C):		Y
; Test Cases:		Adding/Modifying Cyclic Key Event Message Block
; Test Strategy:	Black Box
; Test Data:		-
; === Test Procedure ===

#include <Array.au3>
#include <GuiComboBox.au3>
#include <GUIConstantsEx.au3>

WinWaitActive("BUSMASTER","",5)
if (ControlClick("BUSMASTER","Add",1226))=1 then
	ControlCommand("BUSMASTER","",1237,"Check")
	ControlCommand("BUSMASTER","",1236,"UnCheck")
	$listViewCount=ControlListView("BUSMASTER", "", "SysListView323", "GetItemCount")  					 ;fetch the item count in the listview
	local $bValue = ControlListView("BUSMASTER", "", "SysListView323", "GetText", $listViewCount-1, "2") ;fetch last column 3rd row text
	$hWnd = ControlGetHandle("BUSMASTER","","[CLASS:ComboBox; INSTANCE:3]")
	$count=_GUICtrlComboBox_GetCount($hWnd)									; fetch the item count in combobox
	if $count=2 Then
		$RtArray =_GUICtrlComboBox_GetListArray($hWnd)							; fetch the items in combobox
		$item1=$RtArray[1]
		$item2=$RtArray[2]
	EndIf
EndIf
	