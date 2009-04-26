///////////////////////////////////////////////////////////
//  common.h
//  Implementation of the common
//  Created on:      08-Apr-2009 10:06:32 AM
//  Original author: Tim Kuo
///////////////////////////////////////////////////////////

#ifndef EA_9F454DB8_A969_4830_9B7B_1DCC06D3333D6__INCLUDED_
#define EA_9F454DB8_A969_4830_9B7B_1DCC06D3333D6__INCLUDED_

#define _CONSOLE_DEBUG

#ifdef _CONSOLE_DEBUG
#include <iostream>
#include <iomanip>
#define DBINFO(info, para)\
					{\
					std::cout<<__FILE__<<" Line:"<<std::dec<<__LINE__<<" "<<info<<" "<<std::hex<<para<<std::endl;\
					}
#else
#define DBINFO(info, para) {}
#endif

typedef unsigned int uint32;
typedef unsigned char byte;

#ifndef NULL
#define NULL 0
#endif

#define F_OK (0)
#define W_OK (2)
#define R_OK (4)
#define RW_OK (6)
  
#define MB (1024*1024)
#define MESSAGE_BUFFER_SIZE (4096)
#define ID_MAX_LEN (16)
#define PW_MAX_LEN (16)
#define NAME_MAX_LEN (32)
#define PHONE_MAX_LEN (16)
#define PIC_MAX_SIZE (1.5)
#define ACTION_FLAG (0x80)
#define EVENT_FLAG (0x00)

#define DBCONNECTION_COMMON "COMMONDB"
#define DBCONNECTION_SM "SMDB"

//GROUP should be less than 127
enum GROUP {
	GROUP_COMMON = 0,
	GROUP_STAFFMGNT = 1,
	GROUP_GOODSMGNT = 2,
	GROUP_SHOPMGNT = 3,
	GROUP_CLIENTMGNT = 4,
	GROUP_END
};

//SUBGROUP should be less than 255
enum SUBGROUP_SF {
	SUBGROUP_SF_STAFFBROWSE = 1,
	SUBGROUP_SF_INFOMGNT, // 2
	SUBGROUP_SF_SALARYMGNT, // 3
	SUBGROUP_SF_PASSWORDMGNT, // 4
	SUBGROUP_SF_ADVANCEDMGNT, // 5
	SUBGROUP_SF_END
};

enum ERROR {
	ERROR_NO_ERROR = 0,
	ERROR_DBERROR = -1,
	ERROR_PASSWORD_WRONG = -2,
	ERROR_END
};

enum SINFO {
	SINFO_BROWSE = 0,
	SINFO_MYINFO,
	SINFO_MODIFY,
	SINFO_NEW,
	SINFO_END
};

//Message box warning strings
#define loginWindowTitle "��¼����"
#define smWindowTitle "Ա������"
#define loginErrorWarning "���Ż�����������������롣"
#define zeroSelectionWarning "����û��ѡ��ѡ��Ա����"
#define emptyNameWarnning "Ա�������������գ� \n�뷵�ز���дԱ��������"
#define removeStaffConfirm "ȷ��ɾ��ѡ��Ա����%1����\n���Ա����ְ�����������ĸ�Ա��״̬����������ϵͳ��ɾ����Ա����"
#define removeStaffError "δ�ܳɹ�ɾ��ѡ��Ա���������ԡ�"
#define addStaffError "δ�ܳɹ����Ա���������ԡ�"
#define modifyStaffError "δ�ܳɹ��޸�Ա����Ϣ�������ԡ�"
#define jobString "ְ��"
#define jobRemoveWarning "δ�ܳɹ�ɾ���������ǻ���Ա��������Ϊ��ְ��"
#define levelString "�ȼ�"
#define levelRemoveWarning "δ�ܳɹ�ɾ���������ǻ���Ա��������Ϊ�õȼ���"
#define modifyWaring "δ�ܳɹ��޸ģ������ԡ�"
#define leftMark "��"
#define rightMark "��"




#endif
