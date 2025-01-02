#pragma once

#include "boost/shared_ptr.hpp"

#define BEGIN_CUTLEADER_NAMESPACE() namespace CutLeader {
#define END_CUTLEADER_NAMESPACE() }

// macros to declare class.
#define DECLARE_CUTLEADER_CLASS(className) \
	BEGIN_CUTLEADER_NAMESPACE() \
		class className; \
		typedef boost::shared_ptr<className> className##Ptr; \
	END_CUTLEADER_NAMESPACE() \
	using namespace CutLeader;

