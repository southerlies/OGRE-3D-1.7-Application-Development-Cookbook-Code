// WeatherControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WeatherControl.h"
#include "WeatherControlDlg.h"
#include "afxdialogex.h"


// CWeatherControlDlg dialog

IMPLEMENT_DYNAMIC(CWeatherControlDlg, CDialogEx)

CWeatherControlDlg::CWeatherControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWeatherControlDlg::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CWeatherControlDlg::~CWeatherControlDlg()
{
}

void CWeatherControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWeatherControlDlg, CDialogEx)
END_MESSAGE_MAP()


// CWeatherControlDlg message handlers
