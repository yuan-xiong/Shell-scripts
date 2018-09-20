#include <iostream>
#include "backtrace.h"


int MyFunc_A()
{
        showBacktrace(3);
	std::cout << std::endl;

        showBacktrace(4);
	std::cout << std::endl;

        showBacktrace(5);
	std::cout << std::endl;

        showBacktrace(15);
	std::cout << std::endl;

        return 10;
}

int MyFunc_B()
{
        return MyFunc_A();
}

int func_c()
{
	
        return MyFunc_B();
}

int main()
{
	func_c();
        return 0;
}
