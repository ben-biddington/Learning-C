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
#include <stdlib.h> 
#include <errno.h>
#include <stdio.h>

class About_sockets : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_sockets);
  CPPUNIT_TEST(you_can_create_one);
  CPPUNIT_TEST_SUITE_END();

private:
  int x;

public:
  void setUp(void) {
    errno = 0;
  }

  void tearDown(void) {} 

protected:
  void you_can_create_one() {
    CPPUNIT_ASSERT(1==0);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_sockets);
