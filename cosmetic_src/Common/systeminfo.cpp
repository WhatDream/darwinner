#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "systeminfo.h"

void systeminfo::ToHex(const unsigned char * szOrigin, int nSize, char * szHex)
{
	char szTemp[10];
	int nIndex = 0;
		for(nIndex = 0; nIndex < nSize; nIndex ++)
		{
			sprintf(szTemp, "%02X", szOrigin[nIndex]);
			 if(nIndex == 0)
			 {
				strcpy(szHex, szTemp);
			 }
			 else
			 {
				strcat(szHex, szTemp);
			 }
		}
}

unsigned char* systeminfo::getCPUID()
{
	char szCPUDesc[13];
	char szCPUSNHex[25];
	char szCPUID[37];
	unsigned char szCPUSN[12];

	memset(szCPUDesc, 0, 13);
	memset(szCPUSNHex, 0, 25);
	memset(szCPUID, 0, 37);
	memset(szCPUSN, 0, 12);

	unsigned long ulEAX = 0U, ulEBX = 0U, ulECX = 0U, ulEDX = 0U;

//	__try
//	{
		_asm
	 {
		mov eax, 1
		cpuid
		mov ulEDX, edx
		mov ulEAX, eax
	}

 //����Ƿ���CPU���к�
 //ע�⣬Intel�ĵ���˵���edx�ĵ�18λ�Ǵӵ�0λ��ʼ�����
	 if(!(ulEDX & (1 << 18)))
	  return 0;
	 //��ȡ���кŵĺ�����WORD
	 memcpy(&szCPUSN[8], &ulEAX, 4);

	 _asm
	 {
	  mov eax, 3
	  cpuid
	  mov ulECX, ecx
	  mov ulEDX, edx
	 }
	 //��ȡ���кŵ�ǰ4��WORD
	 memcpy(&szCPUSN[0], &ulECX, 4);
	 memcpy(&szCPUSN[4], &ulEDX, 4);

	 //��ȡCPU OEM��Ϣ
	 _asm
	 {
	  mov eax, 0
			   cpuid
			   mov dword ptr szCPUDesc[0], ebx
			   mov dword ptr szCPUDesc[4], edx
			   mov dword ptr szCPUDesc[8], ecx
	 }
//}
	ToHex(szCPUSN, 12, szCPUSNHex);
	sprintf(szCPUID, "%s%s", szCPUDesc, szCPUSNHex);

	return szCPUSN;
}
