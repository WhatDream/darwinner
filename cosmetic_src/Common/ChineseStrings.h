#ifndef CHINESESTRINGS_H
#define CHINESESTRINGS_H
#include "common.h"

namespace UiStr {
    //group name
    static const char* smString = "Ա������";
    static const char* bmString = "ҵ�����";
    static const char* soString = "������Ӫ";
    static const char* logoffString = "ע��";
    static const char* exitString = "�˳�";
    static const char* zeroSelectionWarning = "����û��ѡ��ѡ���κ��б��";

    //common strings
    static const char* undefineStr = "δ����";
    static const char* femaleStr = "Ů";
    static const char* maleStr = "��";
    static const char* cosmeticNameStr = "Cosmetic����˼������������ҵ��Ӫϵͳ";
    static const char* emptyStr = "";
    static const char* staffEntraceStr = "Ա�����";
    static const char* passwordStr = "����";
    static const char* LoginStr = "��¼";
    static const char* idStr = "���";
    static const char* okStr = "ȷ��";
    static const char* cancleStr = "ȡ��";
    static const char* backStr = "����";
    static const char* expandSideBarStr = "չ�������";
    static const char* shrinkSideBarStr = "��������";
    static const char* sexUndefinedStr = "δ�趨";
    static const char* sexMaleStr = "��";
    static const char* sexFemaleStr = "Ů";
    static const char* modifyWaring = "δ�ܳɹ��޸ģ������ԡ�";
    static const char* abandonModifyWarning = "����δ�ύ���޸���Ϣ�������޸�?";
    static const char* leftMark = "��";
    static const char* rightMark = "��";
    static const char* removeError = "δ�ܳɹ�ɾ��ѡ����Ŀ�������ԡ�";
    static const char* choosePicStr = "ѡ����Ƭ";
    static const char* editStr = "�༭";
    static const char* submitStr = "�ύ";
    static const char* imageFileStr = "ͼ���ļ�(*.png *.jpg *.bmp *.gif)";
    static const char* imageSizeWarning = "�ǳ���Ǹ����Ƭ�ļ���С���ܳ���%1�ף�������ѡ��";
	static const char* sexStr[] = { sexUndefinedStr, sexMaleStr, sexFemaleStr};

    //common mgnt strings
    static const char* loginWindowTitle = "��¼����";
    static const char* loginErrorWarning = "���Ż�����������������롣";

    //staff mgnt strings
    static const char* staffIDStr ="����";
    static const char* staffNameStr ="����";
    static const char* staffSexStr ="�Ա�";
    static const char* staffJobStr ="ְ��";
    static const char* staffLevelStr ="����";
    static const char* staffStatusStr ="״̬";
    static const char* staffRatingStr ="�ͻ������";
    static const char* staffCellStr ="�ֻ�";
    static const char* staffPhoneStr ="����";
    static const char* staffAddressStr ="��ַ";
    static const char* staffDescriptionStr ="��ע";
    static const char* staffJobSalaryStr = "ְ����";
    static const char* staffLevelSalaryStr = "������";
    static const char* staffProfitStr = "���(%)";
    static const char* staffBrowseStr = "Ա�����";
	static const char* staffJobSettingStr = "ְ������";
	static const char* staffLevelSettingStr = "��������";
    //***********staff mgnt message box strings
    static const char* smEditModeWarning = "Ŀǰ�����½�Ա��״̬������ɺ����޸����ϡ�";
    static const char* smEmptyNameWarnning = "Ա�������������գ� \n�뷵�ز���дԱ��������";
    static const char* smRemoveStaffConfirm = "ȷ��ɾ��ѡ��Ա����%1����\n���Ա����ְ�����������ĸ�Ա��״̬����������ϵͳ��ɾ����Ա����";
    static const char* smAddStaffError = "δ�ܳɹ����Ա���������ԡ�";
    static const char* smModifyStaffError = "δ�ܳɹ��޸�Ա����Ϣ�������ԡ�";
    static const char* smJobRemoveWarning = "δ�ܳɹ�ɾ���������ǻ���Ա��������Ϊ��ְ��";
    static const char* smLevelRemoveWarning = "δ�ܳɹ�ɾ���������ǻ���Ա��������Ϊ�õȼ���";
    static const char* smChangePwSucces = "��������Ѿ��ɹ����ġ�";
    static const char* smChangePwFailure = "ԭʼ��������������δ�ܸ��ġ�";

//business mgnt string
	static const char* bmBrandStr = "Ʒ��";
	static const char* bmBuysStr = "�ܽ���";
	static const char* bmCostStr = "�ɱ�";
	static const char* bmDescriptionStr = "ҵ������";
	static const char* bmDiscountStr = "�ۿ�";
	static const char* bmIdStr = "ҵ�����";
	static const char* bmNameStr = "ҵ����";
	static const char* bmRatingStr ="�ͻ������";
	static const char* bmPriceStr = "�۸�";
	static const char* bmSalesStr = "������";
	static const char* bmSpecificationStr = "���";
	static const char* bmStocksStr = "���";
	static const char* bmTypeStr = "ҵ������";
	static const char* bmAdjustableStr = "����ʱ�۸�ɵ���";
	static const char* bmBusinessTypeCategoryStr = "ҵ������";
	static const char* bmBusinessTypeDescriptionStr = "��������";
	static const char* bmBusinessTypeNameStr = "��������";
	static const char* bmBusinessTypeIdStr = "�������";
	static const char* bmNonProfitableStr = "������ҵ��";
	static const char* bmProfitableServiceStr = "Ӫ���Է���";
	static const char* bmProfitableSaleStr = "Ӫ������Ʒ";
	static const char* bmBrowseStr = "ҵ�����";
	static const char* BusinessCategoryStr[] = {bmNonProfitableStr, bmProfitableServiceStr, bmProfitableSaleStr};

}
#endif
