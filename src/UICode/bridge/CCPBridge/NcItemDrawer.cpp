#include "StdAfx.h"
#include "NcItemDrawer.h"

#include "NcItemEx.h"
#include "NcItemList.h"
#include "NcItem.h"
#include "GlViewPort.h"
#include <gl\gl.h>
#include "basicGeometryDrawer.h"
#include <sstream>
#include "StringUtil.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

void NcItemDrawer::DrawNcItemList(GlViewPortEx^ glViewPort, NcItemListEx^ ncItemList, Color cutLineColor, Color traceLineColor)
{
	GlViewPortPtr pGlViewPort = glViewPort->GetGlViewPort();
	BasicGeometryDrawer basicGeometryDrawer(pGlViewPort);
	const NcItemList* pNcItemList = ncItemList->GetNcItemList().get();

	// �Ȱѻ�ͼ��ɫ����Ϊ�и�����ɫ����;�����ƶ��߻���ʱ�޸���ɫ���ָ���
	COLORREF dwOldDrawColor;
	pGlViewPort->GetDrawColor(dwOldDrawColor);

	// �����и��ߺͿ����ƶ��ߡ�
	pGlViewPort->SetDrawColor(ColorTranslator::ToOle(cutLineColor));
	for (unsigned int i = 0; i < pNcItemList->size(); i++)
	{
		const NcItem* pNcItem = pNcItemList->operator[](i).get();
		if (pNcItem->GetNcItemType() == NcItem_TraceLine) // ���ƿ����ƶ���
		{
			const ILineArc2D* pLineArc2D = pNcItem->GetLineArc2D().get();

			// �ȼ�¼�ϵ����ã����ú�ָ���
			COLORREF dwOldDrawColor_tmp;
			pGlViewPort->GetDrawColor(dwOldDrawColor_tmp);
			ROP_MODE iOldRopMode = ROP_NORMAL;
			pGlViewPort->GetROP(iOldRopMode);
			bool bOldStipple = false;
			int iOldRepeat = 1;
			unsigned short iOldPattern = 0xffff;
			pGlViewPort->GetLineStipple(bOldStipple, iOldRepeat, iOldPattern);

			// ���ƿ����ƶ��ߡ�
			pGlViewPort->SetDrawColor(ColorTranslator::ToOle(traceLineColor));
			pGlViewPort->SetROP(ROP_NORMAL);
			pGlViewPort->SetLineStipple(true, 2, 0xcccc);
			basicGeometryDrawer.Draw2DLineArc(pLineArc2D);

			// �ָ�����
			pGlViewPort->SetDrawColor(dwOldDrawColor_tmp);
			pGlViewPort->SetROP(iOldRopMode);
			pGlViewPort->SetLineStipple(bOldStipple, iOldRepeat, iOldPattern);
		}
		else if (pNcItem->GetNcItemType() == NcItem_CutLine) // �����и���
		{
			const ILineArc2D* pLineArc2D = pNcItem->GetLineArc2D().get();

			// ������ǵ�һ��NcItem������һ��NcItem�����и��ߣ���ô����Ҫ��ʼ���ƶ���ߵ�ģʽ��
			if (i == 0 || pNcItemList->operator[](i - 1)->GetNcItemType() != NcItem_CutLine)
				glBegin(GL_LINE_STRIP);

			// ��������и��ߡ�
			if (const Line2D* pLine2D = dynamic_cast<const Line2D*>(pLineArc2D))
			{
				const Point2D& startPt = pLine2D->GetStartPoint();
				glVertex2d(startPt.X(), startPt.Y());
			}
			else if (const Arc2D* pArc2D = dynamic_cast<const Arc2D*>(pLineArc2D))
			{
				DrawArc(pGlViewPort, pArc2D);
			}

			// ����������һ��NcItem������һ��NcItem�����и��ߣ���ô����Ҫ�������ƶ���ߵ�ģʽ��
			if (i == pNcItemList->size() - 1 || pNcItemList->operator[](i + 1)->GetNcItemType() != NcItem_CutLine)
			{
				const Point2D& endPt = pLineArc2D->GetEndPoint();
				glVertex2d(endPt.X(), endPt.Y());
				glEnd();
			}
		}
		else if (pNcItem->GetNcItemType() == NcItem_CutSegment_OrderNum) // �����и�����
		{
			CString strNum = StringUtil::Integer2String(pNcItem->GetOrderNum());
			const Point2D& currentCutPt = pNcItem->GetCurrentCutPt();
			pGlViewPort->DrawString(strNum.GetString(), currentCutPt.X() + 1.0, currentCutPt.Y() + 1.0, 0, 0);
		}
	}

	// �ָ�����
	pGlViewPort->SetDrawColor(dwOldDrawColor);
}

void NcItemDrawer::DrawNcItemListPartially(GlViewPortEx^ glViewPort, NcItemListEx^ ncItemList, int iEndNcItemIndex, Color drawColor)
{
	GlViewPortPtr pGlViewPort = glViewPort->GetGlViewPort();
	BasicGeometryDrawer basicGeometryDrawer(pGlViewPort);

	// �Ȱѻ�ͼ��ɫ����Ϊ�и�����ɫ����;�����ƶ��߻���ʱ�޸���ɫ���ָ���
	COLORREF dwOldDrawColor;
	pGlViewPort->GetDrawColor(dwOldDrawColor);
	pGlViewPort->SetDrawColor(ColorTranslator::ToOle(drawColor));

	const NcItemList* pNcItemList = ncItemList->GetNcItemList().get();
	for (unsigned int i = 0; i <= iEndNcItemIndex; i++)
	{
		const NcItem* pNcItem = pNcItemList->operator[](i).get();
		const ILineArc2D* pLineArc2D = pNcItem->GetLineArc2D().get();

		if (pNcItem->GetNcItemType() == NcItem_TraceLine) // ���ƿ����ƶ���
		{
			// �ȼ�¼�ϵ����ã����ú�ָ���
			ROP_MODE iOldRopMode = ROP_NORMAL;
			pGlViewPort->GetROP(iOldRopMode);
			bool bOldStipple = false;
			int iOldRepeat = 1;
			unsigned short iOldPattern = 0xffff;
			pGlViewPort->GetLineStipple(bOldStipple, iOldRepeat, iOldPattern);

			// ���ƿ����ƶ��ߡ�
			pGlViewPort->SetROP(ROP_NORMAL);
			pGlViewPort->SetLineStipple(true, 2, 0xcccc);
			basicGeometryDrawer.Draw2DLineArc(pLineArc2D);

			// �ָ�����
			pGlViewPort->SetROP(iOldRopMode);
			pGlViewPort->SetLineStipple(bOldStipple, iOldRepeat, iOldPattern);
		}
		else if (pNcItem->GetNcItemType() == NcItem_CutLine) // �����и���
		{
			// ������ǵ�һ��NcItem������һ��NcItem�����и��ߣ���ô����Ҫ��ʼ���ƶ���ߵ�ģʽ��
			if (i == 0 || pNcItemList->operator[](i - 1)->GetNcItemType() != NcItem_CutLine)
				glBegin(GL_LINE_STRIP);

			// ��������и��ߡ�
			if (const Line2D* pLine2D = dynamic_cast<const Line2D*>(pLineArc2D))
			{
				const Point2D& startPt = pLine2D->GetStartPoint();
				glVertex2d(startPt.X(), startPt.Y());
			}
			else if (const Arc2D* pArc2D = dynamic_cast<const Arc2D*>(pLineArc2D))
			{
				DrawArc(pGlViewPort, pArc2D);
			}

			// ����������һ��NcItem������һ��NcItem�����и��ߣ���ô����Ҫ�������ƶ���ߵ�ģʽ��
			if (i == iEndNcItemIndex || pNcItemList->operator[](i + 1)->GetNcItemType() != NcItem_CutLine)
			{
				const Point2D& endPt = pLineArc2D->GetEndPoint();
				glVertex2d(endPt.X(), endPt.Y());
				glEnd();
			}
		}
	}

	// �����и�ͷ
	{
		const NcItem* pNcItem = pNcItemList->operator[](iEndNcItemIndex).get();
		if (pNcItem->GetNcItemType() != NcItem_CutSegment_OrderNum)
		{
			const Point2D& position = pNcItem->GetLineArc2D()->GetEndPoint();
			pGlViewPort->DrawCrossPoint2D(position.X(), position.Y(), 20, 0);
			CString str = _T("(");
			str += StringUtil::Double2String(position.X(), _T("%.3f"));
			str += _T(", ");
			str += StringUtil::Double2String(position.Y(), _T("%.3f"));
			str += _T(")");
			pGlViewPort->DrawString(str.GetString(), position.X() + 5.0, position.Y() + 5.0, 0, 0);
		}
	}

	// �ָ�����
	pGlViewPort->SetDrawColor(dwOldDrawColor);
}

void NcItemDrawer::DrawArc(GlViewPortPtr pGlViewPort, const Arc2D* pArc2D)
{
	const Point2D& centerPt = pArc2D->GetBasePoint();
	double dRadius = pArc2D->GetRadius();
	double dStartAngle = pArc2D->GetStartAngle();
	double dMoveAngle = pArc2D->GetSweepAngle();
	
	// ����Բ���ĳ��ȣ�ȡ������Ӿ��ο���нϴ�ֵ����תΪ�豸����ϵ�µĳ��ȡ�
	double dLength_device = .0;
	{
		double dXMin = .0, dXMax = .0, dYMin = .0, dYMax = .0;
		double dStartX = centerPt.X() + dRadius * cos(dStartAngle);
		double dStartY = centerPt.Y() + dRadius * sin(dStartAngle);
		double dEndX = centerPt.X() + dRadius * cos(dStartAngle + dMoveAngle);
		double dEndY = centerPt.Y() + dRadius * sin(dStartAngle + dMoveAngle);
		if (MathEx::CheckAngleRange(PI / 2, dStartAngle, dMoveAngle, GEOM_TOLERANCE))
			dYMax = centerPt.Y() + dRadius;
		else
			dYMax = max(dStartY, dEndY);
		if (MathEx::CheckAngleRange(PI, dStartAngle, dMoveAngle, GEOM_TOLERANCE))
			dXMin = centerPt.X() - dRadius;
		else
			dXMin = min(dStartX, dEndX);
		if (MathEx::CheckAngleRange(-PI / 2, dStartAngle, dMoveAngle, GEOM_TOLERANCE))
			dYMin = centerPt.Y() - dRadius;
		else
			dYMin = min(dStartY, dEndY);
		if (MathEx::CheckAngleRange(.0, dStartAngle, dMoveAngle, GEOM_TOLERANCE))
			dXMax = centerPt.X() + dRadius;
		else
			dXMax = max(dStartX, dEndX);

		dLength_device = max(dXMax - dXMin, dYMax - dYMin);
		pGlViewPort->WToD(dLength_device);
	}

	// ȷ����Բ����ϵĶ�����
	int iPieces = 30;
	if (dLength_device < 2) // Բ�����2��
		iPieces = 2;
	else if (dLength_device < 10) // Բ�����5��
		iPieces = 5;
	else if (dLength_device < 40) // Բ�����10��
		iPieces = 10;
	else
	{
		if (abs(dMoveAngle) <= PI / 2)
			iPieces = 20;
		else if (abs(dMoveAngle) > PI / 2 && abs(dMoveAngle) <= PI)
			iPieces = 25;
		else if (abs(dMoveAngle) > PI && abs(dMoveAngle) <= 3 * PI / 2)
			iPieces = 30;
		else if (abs(dMoveAngle) > 3 * PI / 2 && abs(dMoveAngle) <= 2 * PI)
			iPieces = 35;
	}

	// ��ɢԲ����ע�⣬����һ��Բ��������Σ�����ֻ���������㣬Բ�����յ��ɺ�����������ʱ�����ϡ�
	for (int i = 0; i < iPieces; ++i)
	{
		double d1 = centerPt.X() + dRadius * cos(i*dMoveAngle / iPieces + dStartAngle);
		double d2 = centerPt.Y() + dRadius * sin(i*dMoveAngle / iPieces + dStartAngle);
		glVertex2d(d1, d2);
	}
}

END_CUTLEADER_NAMESPACE()
