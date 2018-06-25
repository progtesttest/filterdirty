// Ôà×Ö¹ýÂË»òÔà×Ö¼ì²â [2/1/2013 HDP]
#if !defined(AFX_FILTERDIRTYWORDS_H__13CC4509_75B1_91B4_D84D_E2C662263BDA__INCLUDED_)
#define AFX_FILTERDIRTYWORDS_H__13CC4509_75B1_91B4_D84D_E2C662263BDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyHeap.h"

class CFilterDirtyWords
{
public:
	CFilterDirtyWords();
	~CFilterDirtyWords();
	static CFilterDirtyWords* CreateNew()		{ return (new CFilterDirtyWords); }
	UINT	ReleaseByOwner()		{ delete this; return 0; }
private:
	typedef struct _dirtytree
	{
		bool bend;
		struct _dirtytree * subtree[256];
		_dirtytree(){
			bend = false;
			memset(subtree,0,sizeof(_dirtytree*)*256);
		}
	}DIRTYTREE,*PDIRTYTREE;

	PDIRTYTREE m_phead  ;
private:
	bool loaddirtywords() ;
	bool hasdirtywords(const PDIRTYTREE pHead,const char *  pstring);
	void filterdirtywords(const PDIRTYTREE pHead, char * pstring);
	void insertdirtywords(PDIRTYTREE& pHead,const char *  pstring);
	void releasedirtytree(PDIRTYTREE pHead);
public:
	bool CreateAll() ;
	bool HasDirtyWords(const char* lpstr);
	void FilterDirtyWords(char * pstring);
public:
	MYHEAP_DECLARATION(s_heap)
};
extern CFilterDirtyWords*	g_pFilterDirtyWords  ;
#endif