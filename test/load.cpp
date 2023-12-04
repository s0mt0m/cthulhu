#include <brick-assert>
#include <brick-unit>

int main()
{
    brq::test_case( "simple" ) = [ = ]
    {
        ASSERT( true );
    };
}

