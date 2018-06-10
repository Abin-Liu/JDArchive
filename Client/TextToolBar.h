#ifndef __TEXTTOOLBAR_H__
#define __TEXTTOOLBAR_H__

#ifndef __CNewMenu_H_
typedef CToolBar CNewToolBar;
#endif

class CTextToolBar : public CNewToolBar
{
public: 

	CTextToolBar();
	virtual ~CTextToolBar();	
	virtual BOOL LoadToolBar(UINT nIDResource); 
	virtual BOOL LoadToolBar(LPCTSTR lpszResourceName);	
};

#endif // __TEXTTOOLBAR_H__