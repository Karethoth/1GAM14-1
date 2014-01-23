#ifndef _TEMPLATESTACK_HH_
#define _TEMPLATESTACK_HH_

#include <vector>
#include <memory>


template<typename T>
class TemplateStack
{
	protected:
	std::vector< std::shared_ptr<T> > stack;


	public:
	virtual ~TemplateStack()
	{
		stack.clear();
	}



	virtual bool Push( std::shared_ptr<T> item )
	{
		stack.insert( stack.begin(), item );
		return true;
	}



	virtual std::shared_ptr<T> Get( unsigned int index )
	{
		return stack.at( index );
	}



	virtual std::shared_ptr<T> Pop()
	{
		std::shared_ptr<T> ret = stack.front();
		stack.clear();
		return ret;
	}
};

#endif
