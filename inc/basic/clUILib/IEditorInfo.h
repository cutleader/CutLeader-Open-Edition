#pragma once

#include "clUILibCommon.h"
#include "EditorData.h"

BEGIN_CUTLEADER_NAMESPACE()

// һЩ����Ľӿڣ�����Щ��������Ҫ�õ���ǰ���༭������Ϣ��
// ע��
//  1) ���綯������������
class ClUILib_Export IEditorInfo
{
public:
    virtual ~IEditorInfo() {}

public:
	// �õ��༭����Ϣ��
	virtual EditorData GetEditorData() const = 0;
};

END_CUTLEADER_NAMESPACE()
