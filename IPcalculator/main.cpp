#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<CommCtrl.h>
#include<iostream>
#include"resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, (DLGPROC)DlgProc, NULL);
	return 0;
}

VOID PrintInfo(HWND hwnd);

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hIPaddress = GetDlgItem(hwnd, IDC_IP_ADDRESS);
	HWND hIPmask = GetDlgItem(hwnd, IDC_IP_MASK);
	HWND hIPprefix = GetDlgItem(hwnd, IDC_EDIT_PREFIX);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		//InitCommonControls();
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		std::cout << "Init" << std::endl;
		SetFocus(GetDlgItem(hwnd, IDC_IP_ADDRESS));
		//https://learn.microsoft.com/en-us/windows/win32/controls/udm-setrange
		//https://learn.microsoft.com/en-us/windows/win32/winmsg/makeword
		SendMessage(GetDlgItem(hwnd, IDC_SPIN_PREFIX), UDM_SETRANGE, 0, MAKEWORD(32, 0));
		break;

	case WM_COMMAND:
	{


		switch (LOWORD(wParam))
		{
		case IDC_IP_ADDRESS:
		{
			DWORD dwIPaddress = 0;
			DWORD dwIPprefix = 0;
			DWORD dwIPmask = 0;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
				//std::cout << FIRST_IPADDRESS(dwIPaddress) << std::endl;
				if (FIRST_IPADDRESS(dwIPaddress) < 128)dwIPmask = 0xFF000000, dwIPprefix = 8;
				else if (FIRST_IPADDRESS(dwIPaddress) < 192)dwIPmask = 0xFFFF0000, dwIPprefix = 16;
				else if (FIRST_IPADDRESS(dwIPaddress) < 224)dwIPmask = 0xFFFFFF00, dwIPprefix = 24;
				//std::cout << dwIPmask << std::endl;
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPmask);
				CHAR szIPprefix[3] = {};
				sprintf(szIPprefix, "%i", dwIPprefix);
				SendMessage(hIPprefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
			}
		}
		//break;
		case IDC_IP_MASK:
		{
			DWORD dwIPaddress = 0;
			DWORD dwIPprefix = 0;
			DWORD dwIPmask = 0;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				/*SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
				for (dwIPprefix = 0; dwIPmask; dwIPprefix++)dwIPmask <<= 1;
				CHAR szIPprefix[3] = {};
				sprintf(szIPprefix, "%i", dwIPprefix);
				std::cout << szIPprefix << std::endl;
				SendMessage(hIPprefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);*/
				//if(HIWORD(wParam) == EN_KILLFOCUS)SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPaddress);
			}
		}
		break;
		case IDC_EDIT_PREFIX:
		{
			DWORD dwIPaddress = 0;
			DWORD dwIPprefix = 0;
			DWORD dwIPmask = 0;
			if (HIWORD(wParam) == EN_CHANGE)
			{
				CHAR szIPprefix[3] = {};
				SendMessage(hIPprefix, WM_GETTEXT, 3, (LPARAM)szIPprefix);
				DWORD dwIPprefix = std::atoi(szIPprefix);
				DWORD dwIPmask = UINT_MAX;
				for (int i = 0; i < 32 - dwIPprefix; i++)
					dwIPmask <<= 1;
				SendMessage(hIPmask, IPM_SETADDRESS, 0, dwIPmask);
			}
		}
		break;
		case IDC_BUTTON_RESET:
			SendMessage(hIPaddress, IPM_CLEARADDRESS, 0, 0);
			SendMessage(hIPmask, IPM_CLEARADDRESS, 0, 0);
			SendMessage(hIPprefix, WM_SETTEXT, 0, (LPARAM)"");
			break;
		case IDOK:
			PrintInfo(hwnd);
			break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
	}
	break;
	case WM_NOTIFY:
	{
		NMHDR* p_nmhdr = (NMHDR*)lParam;
		std::cout << p_nmhdr->idFrom << "\t" << wParam << std::endl;
		if (wParam == IDC_IP_MASK || wParam == IDC_IP_ADDRESS)
			//if (p_nmhdr->hwndFrom == GetDlgItem(hwnd, IDC_IP_MASK))
		{
			DWORD dwIPmask = 0;
			DWORD dwIPprefix = 0;
			SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
			for (dwIPprefix = 0; dwIPmask; dwIPprefix++)dwIPmask <<= 1;
			CHAR szIPprefix[3] = {};
			sprintf(szIPprefix, "%i", dwIPprefix);
			std::cout << szIPprefix << std::endl;
			SendMessage(hIPprefix, WM_SETTEXT, 0, (LPARAM)szIPprefix);
		}
	}
	break;
	case WM_CLOSE:	EndDialog(hwnd, 0);
	}
	return FALSE;
}
LPSTR FormatAddress(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwIPaddress)
{
	sprintf
	(
		szBuffer,
		"%s%i.%i.%i.%i",
		szMessage,
		FIRST_IPADDRESS(dwIPaddress),
		SECOND_IPADDRESS(dwIPaddress),
		THIRD_IPADDRESS(dwIPaddress),
		FOURTH_IPADDRESS(dwIPaddress)
	);
	return szBuffer;
}
LPSTR FormatCount(CHAR szBuffer[], CONST CHAR szMessage[], DWORD dwCount)
{
	sprintf(szBuffer, "%s%i", szMessage, dwCount);
	return szBuffer;
}
VOID PrintInfo(HWND hwnd)
{
	HWND hIPaddress = GetDlgItem(hwnd, IDC_IP_ADDRESS);
	HWND hIPmask = GetDlgItem(hwnd, IDC_IP_MASK);
	HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
	DWORD dwIPaddress = 0;
	DWORD dwIPmask = 0;
	SendMessage(hIPaddress, IPM_GETADDRESS, 0, (LPARAM)&dwIPaddress);
	SendMessage(hIPmask, IPM_GETADDRESS, 0, (LPARAM)&dwIPmask);
	DWORD dwNetworkAddress = dwIPaddress & dwIPmask;
	DWORD dwBroadcastAddress = dwIPaddress | ~dwIPmask;

	CHAR szInfo[1024] = {};
	CHAR szNetworkAddress[1024] = {};
	CHAR szBroadcastAddress[1024] = {};
	CHAR szIPcount[1024] = {};
	CHAR szHostCount[1024] = {};
	sprintf
	(
		szInfo,
		"%s;\n%s;\n%s;\n%s;\n",
		FormatAddress(szNetworkAddress, "Адрес сети:\t\t\t", dwNetworkAddress),
		FormatAddress(szBroadcastAddress, "Широковещательный адрес:\t", dwBroadcastAddress),
		FormatCount(szIPcount, "Количество адресов:\t\t", dwBroadcastAddress - dwNetworkAddress + 1),
		FormatCount(szHostCount, "Количество узлов:\t\t", dwBroadcastAddress - dwNetworkAddress - 1)
	);
	SendMessage(hStaticInfo, WM_SETTEXT, 0, (LPARAM)szInfo);
}