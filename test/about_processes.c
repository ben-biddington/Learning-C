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

class About_processes : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_processes);
  CPPUNIT_TEST(you_can_fork);
  CPPUNIT_TEST(a_fork_has_copy_on_write_access);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void) {}
  void tearDown(void) {} 

protected:
  void you_can_fork() {
    pid_t pid = fork();
    
    pid_t parent,child;

    if (pid == 0) {
      exit(0);
    } else {
      child = pid;
    }
  }

  void a_fork_has_copy_on_write_access() {
    int number = 1337;
    int another_number = 16;

    int *ptr = &number;
    int *another_ptr = &another_number;

    pid_t pid = fork();
    
    if (pid == 0) {
      *ptr = 1338;
      CPPUNIT_ASSERT_EQUAL(1338, *ptr);
      CPPUNIT_ASSERT_EQUAL(16, *another_ptr);
      exit(0);
    } else {
      CPPUNIT_ASSERT_EQUAL(1337, *ptr);
    }
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_processes);
