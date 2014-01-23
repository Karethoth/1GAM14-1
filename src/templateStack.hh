#ifndef _TEMPLATESTACK_HH_
#define _TEMPLATESTACK_HH_

#include <vector>
#include <memory>


template<typename T>
class TemplateStack
{
 public:
	virtual ~TemplateStack()
	{
		stack.clear();
	}


	virtual bool Push( const std::shared_ptr<T>& item )
	{
		stack.push_back( item );
		return true;
	}


	virtual std::shared_ptr<T> Get( unsigned int index )
	{
		return stack.at( index );
	}


	virtual std::shared_ptr<T> Pop()
	{
		auto ret = stack.back();
		stack.pop_back();
		return ret;
	}


	virtual std::shared_ptr<T> Top()
	{
		return stack.back();
	}


 protected:
	std::vector<std::shared_ptr<T>> stack;
};

#endif
