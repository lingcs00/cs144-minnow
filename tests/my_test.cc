#include "reassembler_test_harness.hh"

#include <exception>
#include <iostream>

using namespace std;

int main()
{
  try {
    {
      ReassemblerTestHarness test { "all within capacity", 2 };

      test.execute( Insert { "ab", 0 } );
      test.execute( BytesPushed( 2 ) );
      test.execute( BytesPending( 0 ) );
      test.execute( ReadAll( "ab" ) );

      test.execute( Insert { "cd", 2 } );
      test.execute( BytesPushed( 4 ) );
      test.execute( BytesPending( 0 ) );
      test.execute( ReadAll( "cd" ) );

      test.execute( Insert { "ef", 4 } );
      test.execute( BytesPushed( 6 ) );
      test.execute( BytesPending( 0 ) );
      test.execute( ReadAll( "ef" ) );
    }

  } catch ( const exception& e ) {
    cerr << "Exception: " << e.what() << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
