#include "filterdirtywords.h"
#include "MapGroup.h" 

CFilterDirtyWords*	g_pFilterDirtyWords = NULL ;

MYHEAP_IMPLEMENTATION(CFilterDirtyWords,s_heap)

CFilterDirtyWords::CFilterDirtyWords()
{
	m_phead = NULL ;
}

CFilterDirtyWords::~CFilterDirtyWords()
{
	releasedirtytree(m_phead);
}
bool CFilterDirtyWords::CreateAll()
{
	return loaddirtywords() ;
}

bool CFilterDirtyWords::loaddirtywords()
{
	return true ;
	FILE * f = fopen(CONFIG_DIRTY_WORDS,"r");
	if(NULL == f){
		return false;
	}
	char szbuf[256] ;
	PDIRTYTREE phead = NULL;
	while(NULL != fgets(szbuf,256,f)){
		insertdirtywords(phead,szbuf);
	}
	fclose(f);
	m_phead = phead ;
	if(NULL == m_phead) ::LOGWARNING("CFilterDirtyWords::loaddirtywords is NULL");
	return true ;
//	return m_phead?true:false ;
}
void CFilterDirtyWords::filterdirtywords(const PDIRTYTREE pHead, char * pstring)
{
	if(!pHead) return ;
	PDIRTYTREE pTree = pHead;
	unsigned char ch ='\0';
	int pos = 0;
	char * pTemp  = pstring ;
	bool bBegin = false;
	while(*pTemp != '\0')
	{
		ch = isupper( *pTemp )?_tolower( *pTemp ): *pTemp;
		if(pTree->subtree[ch]){
			if(!bBegin){
				bBegin = true;  pos = pTemp - pstring ;
			}
			pTree = pTree->subtree[ch];

			if(pTree->bend){
				while(pos <= pTemp - pstring) *(pstring+pos++)='*';
			}

		}else if(bBegin && pHead->subtree[ch]){
			pos = pTemp - pstring ; pTree =  pHead->subtree[ch];
			if(pTree->bend){
				while(pos <= pTemp - pstring) *(pstring+pos++)='*';
			}
		}else{
			pTree = pHead ;  bBegin = false;
		}
		++pTemp;
	}
}
void CFilterDirtyWords::insertdirtywords(PDIRTYTREE& pHead,const char *  pstring)
{
	if(!pstring) return;
	if(!pHead) pHead = new DIRTYTREE ;
	const char * pTemp =(char*) pstring ;
	PDIRTYTREE pTree = pHead ;
	unsigned char ch ='\0';
	while(*pTemp != '\0' && *pTemp !='\r' && *pTemp != '\n'){
		ch = isupper( *pTemp )?_tolower( *pTemp ): *pTemp;
		if(!pTree->subtree[ch]) pTree->subtree[ch] = new DIRTYTREE ;
		pTree = pTree->subtree[ch] ;  
		++pTemp;
	}
	pTree->bend = true ;
}
void CFilterDirtyWords::releasedirtytree(PDIRTYTREE pHead)
{
	if(!pHead) return ;
	for(unsigned int i =0;i< 256;i++){
		releasedirtytree(pHead->subtree[i]);
	}
	delete pHead;
}

bool CFilterDirtyWords::hasdirtywords(const PDIRTYTREE pHead,const char *  pstring)
{
	if(!pHead) return false;
	PDIRTYTREE pTree = pHead;
	unsigned char ch ='\0';
	char * pTemp  =(char*) pstring ;
	while(*pTemp != '\0')
	{
		ch = isupper( *pTemp )?_tolower( *pTemp ): *pTemp;
		if(pTree->subtree[ch]){
			pTree = pTree->subtree[ch];
			if(pTree->bend){
				return true;
			}
		}else{
			pTree = pHead ;
		}
		++pTemp;
	}
	return false ;
}

bool CFilterDirtyWords::HasDirtyWords(const char *  pstring)
{
	return hasdirtywords(m_phead,pstring);
}

void CFilterDirtyWords::FilterDirtyWords(char * pstring)
{
	filterdirtywords(m_phead,pstring);
}