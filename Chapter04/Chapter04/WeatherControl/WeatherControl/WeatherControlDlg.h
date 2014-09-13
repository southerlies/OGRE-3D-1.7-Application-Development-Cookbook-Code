#pragma once


// CWeatherControlDlg dialog

class CWeatherControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWeatherControlDlg)

public:
	CWeatherControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWeatherControlDlg();

// Dialog Data
	enum { IDD = IDD_WEATHER_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
