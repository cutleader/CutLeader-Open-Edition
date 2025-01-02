#pragma once

#include "clGeometryBaseCommon.h"
#include "Vector2D.h"
#include "Path2D.h"
#include "baseConst.h"
#include <map>

BEGIN_CUTLEADER_NAMESPACE()

// the data type which only used for polygon algorithm.
class AlgAngle
{
public:
	AlgAngle();
	AlgAngle(double dDX, double dDY);

public:
	bool operator < (const AlgAngle& Angle) const;
	bool operator ==(const AlgAngle& Angle) const;

public:
	void SetVect(double dDX, double dDY);
	void GetVect(double* dDX, double* dDY) const;

public:
	double m_dRate;
	BOOL m_bLessThanPI;
};

template <class T> class AlgAngleLoop : public std::multimap<AlgAngle, T>
{
public:
	iterator Find_Pre_Angle(const AlgAngle& Angle) 
	{
		iterator iter = lower_bound(Angle) ;
		if ( iter == begin() )
		{
			iter = end() ;
		}
		-- iter;

		return iter;
	}

	iterator Find_Next_Angle(const AlgAngle& Angle)
	{
		iterator iter = upper_bound(Angle);

		return iter == end() ? begin() : iter;
	}
};

struct AlgPoint
{
	bool operator()(const Point2D& lPt, const Point2D& rPt) const
	{
		if (lPt.X() - rPt.X() < -GEOM_TOLERANCE)
		{
			return true;
		}
		else if (lPt.X() - rPt.X() > GEOM_TOLERANCE)
		{
			return false;
		}
		else if (lPt.Y() - rPt.Y() < -GEOM_TOLERANCE)
		{
			return true;
		}

		return false;
	}
};

class DistMap : public std::map<double, bool>
{
public:
	void Add(double X, bool IsIn) 
	{
		iterator Iter = upper_bound(X - SYSTEM_TOLERANCE);

		if (Iter == end() || Iter->first > X + SYSTEM_TOLERANCE)
		{
			insert(std::make_pair(X, IsIn));
		}
		else
		{
			if (IsIn != Iter->second)
			{
				erase(Iter);
			}
		}
	}
};

struct Dist : public std::pair<double, double>
{
	Dist(){};

	Dist(const std::pair<double, double>& pair)
	{ 
		first = pair.first;  
		second = pair.second;
	};

	bool operator < (const Dist& dist) const 
	{
		if (first < dist.first - SYSTEM_TOLERANCE)
		{
			return true;
		}
		else if (first > dist.first + SYSTEM_TOLERANCE)
		{
			return false;
		}
		else
		{
			return second < dist.second - SYSTEM_TOLERANCE;
		}
	}
};

END_CUTLEADER_NAMESPACE()
