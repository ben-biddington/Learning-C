#include <iostream>

#include <string.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextTestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/TestSuccessListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

using namespace std;

class About_arrays_and_pointer : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_arrays_and_pointer);
  CPPUNIT_TEST(an_array_is_a_pointer_to_the_first_item);
  CPPUNIT_TEST(you_can_printf_to_a_char_buffer);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void) {}
  void tearDown(void) {} 

protected:
  void an_array_is_a_pointer_to_the_first_item(void) { 
    char text[] = "a b c d e f g h";
    char *ptr = text;

    CPPUNIT_ASSERT(*ptr == text[0]);
  }

  void you_can_printf_to_a_char_buffer(void) { 
    char buffer[1024];

    sprintf(buffer, "Jazz Kang is a %s", "twat");

    string expected = "Jazz Kang is a twat";

    string actual = string(buffer);

    CPPUNIT_ASSERT(expected == actual);
  }
};

struct AnExampleStruct {
  int left, right;
};

class About_structs : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_structs);
  CPPUNIT_TEST(you_can_create_them);
  CPPUNIT_TEST(there_is_a_short_cut_for_pointers_and_accessing_fields_so_dereferencing_is_automatic);
  CPPUNIT_TEST_SUITE_END();

 protected: 
  void you_can_create_them(void) {
    AnExampleStruct str = {1,2};
    CPPUNIT_ASSERT_MESSAGE("Failed to match the left value", str.left == 1);
    CPPUNIT_ASSERT_MESSAGE("Failed to match the right value", str.right == 2);
  }

  void there_is_a_short_cut_for_pointers_and_accessing_fields_so_dereferencing_is_automatic() {
    AnExampleStruct str = {1,2};

    AnExampleStruct *ptr = &str;

    CPPUNIT_ASSERT_MESSAGE(
      "Expected dereferencing pointer and the arrow notation to produce the same result", 
      (*ptr).left == ptr->left
    );
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_arrays_and_pointer);
CPPUNIT_TEST_SUITE_REGISTRATION(About_structs);

int main( int ac, char **av ) {
  //--- Create the event manager and test controller
  CPPUNIT_NS::TestResult controller;

  //--- Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener( &result );        

  CPPUNIT_NS::TextTestResult text_test_result;
  controller.addListener( &text_test_result );     

  CPPUNIT_NS::TestSuccessListener success;
  controller.addListener( &success );     

  //--- Add a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener( &progress );      

  //--- Add the top suite to the test runner
  CPPUNIT_NS::TextTestRunner runner;
  runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
  runner.run( controller );

  CPPUNIT_NS::TestResultCollector::TestFailures failures = result.failures();

  return result.wasSuccessful() ? 0 : 1;
}
