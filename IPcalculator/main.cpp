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
		case IDOK:
			break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
	}
	break;
	case WM_NOTIFY:
	{
		NMHDR* p_nmhdr = (NMHDR*)lParam;
		if (p_nmhdr->hwndFrom == GetDlgItem(hwnd, IDC_IP_MASK))
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