#pragma once

#include "clGeometryBaseCommon.h"
#include <list>

BEGIN_CUTLEADER_NAMESPACE()

// this class keeps the "smallest" objects.
// notes:
//   1) when call "AddObj" function, we will compare the "newObj" with each object in the container.
//   2) if you define a class as "class ClassA : public SmallObjList<ClassB>", and if you want to export ClassA, you should implement operators(==,<) for ClassB.
template <class T> class SmallObjList : public std::list<T>
{
public:
	virtual BOOL Add(T pNewObj)
	{
		BOOL bCanBeAdded = TRUE;
		std::list<T>::iterator iter = begin();

		for (NULL; iter != end(); NULL)
		{
			T pOldObj = *iter;

			// if "newObj" is bigger than some obj, return.
			if (pOldObj.SmallerThan(pNewObj))
			{
				bCanBeAdded = FALSE;

				break;
			}
			// if some "oldObj" is bigger than "newObj", delete "oldObj".
			else if(pNewObj.SmallerThan(pOldObj))
			{
				iter = erase(iter);
			}
			// maybe "oldObj" is not bigger than "newObj", nor smaller than 
			// "newObj", check next "oldObj".
			else
			{
				iter++;
			}
		}

		if (bCanBeAdded)
		{
			push_back(pNewObj);
		}

		return bCanBeAdded;
	}
};

END_CUTLEADER_NAMESPACE()
