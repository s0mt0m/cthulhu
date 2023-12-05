#include <brick-assert>
#include <brick-unit>

#include "builder.hpp"
#include "eval.hpp"
#include "st_bvec.hpp"

int main()
{
    using namespace cthu;

    brq::test_case( "simple" ) = [ = ]()
    {
        cthu::builder::program b;

        auto insn = b.add_stack();
        insn.add( word_add( 1, 1, 1 ) );

        auto data = b.add_stack();
        data.add( 2 ).add( 5 );

        auto p = b.build( { insn, data } );

        auto e = eval< exec_tag >( p );
        e();

        ASSERT( p.template pop< word >( 1 ) == 7 );
    };
}

