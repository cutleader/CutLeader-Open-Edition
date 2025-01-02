#pragma once

#include "clUtilityCommon.h"
#include "baseConst.h"

DECLARE_CUTLEADER_CLASS(ExpressionCalc)

BEGIN_CUTLEADER_NAMESPACE()

typedef struct doubleRes
{
	double	Value;
	int		ErrorFlag;
} DOUBLE_RESULT;

const double BAD_NUMBER = 0xFFFFFFFF;

enum EXPRESSIONCALC_ERR_FLAG
{
	CALCULATE_SUCCESS		= 0,
	ERROR_STRING			= -1,
	PARA_ERROR				= -2,
	EPARA_ERROR				= -3,
	OVERFLOW_NUMBER			= -4,
	DIVIDE_BY_ZERO			= -5,
	INFINITE_NUMBER			= -6
};

// this class can evaluate the expression, and calculate the value.
class ClUtility_Export ExpressionCalc
{
public:
	~ExpressionCalc(void);

public:
	static ExpressionCalcPtr GetInstance();

private:
	ExpressionCalc(void);

private:
	//the singleton object
	static ExpressionCalcPtr expCalcPtr;

public:
	void CalculateStr(char* pInstr, double& ReturnValue, EXPRESSIONCALC_ERR_FLAG& ErrorFlag);

public:
	//calculate the value of input string pInStr
	DOUBLE_RESULT Calculate( char* pInStr, const char PreOperator );

	//Transform numerical string into value
	void TransStrToValue( char* pInStr, int& curPos, DOUBLE_RESULT& ReturnValue );

	//processing left bracket when current character is left bracket
	void LeftBracket( char* pInStr, int& curPos, DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//processing rignt bracket when current character is right bracket
	void RightBracket( char* pInStr, int& curPos, const char& PreOperator,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the sum of two values 
	void AddOperate( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the margin of two vallues
	void SubOperate( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the product of two values
	void MulOperate( char* pInStr, int& curPos,
		DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the quotient of two values
	void DivOperate( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the power of two values
	void PowOperate( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the logical(&) value
	void LogicalAnd( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the logical(|) value
	void LogicalOr( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the logical(!) value
	void LogicalNot( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the cos value of an angle(degrees)
	void CosOperator( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the sin value of an angle(degrees)
	void SinOperator( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the tan value of an angle(degrees)
	void TanOperator( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the absolute value 
	void AbsOperator( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	////calculate the arccos value of an angle(degrees)
	void AcosOperator( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the arcsin value of an angle(degrees)
	void ASinOperator( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue );

	//calculate the arctan value of an angle(degrees)
	void AtanOperator( char* pInStr, int& curPos,
						DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue);

	//compare two values
	void CompareOperator( char* pInStr, int& curPos, DOUBLE_RESULT& ReturnValue, DOUBLE_RESULT& GetValue);

	//processing when current character is '\0'
	void EndStr( int& curPos, DOUBLE_RESULT& ReturnValue, const char& PreOperator);
};

END_CUTLEADER_NAMESPACE()
