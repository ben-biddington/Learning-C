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
#include <signal.h> 
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

using std::string;

class About_processes : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_processes);
  CPPUNIT_TEST(you_can_fork);
  CPPUNIT_TEST(you_can_wait_for_a_child_process_to_terminate);
  CPPUNIT_TEST(a_fork_has_copy_on_write_access);
  CPPUNIT_TEST(you_can_tell_if_you_supply_invalid_options_to_waitpid);
  CPPUNIT_TEST(you_can_tell_if_a_process_exits_abnormally);
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

  void you_can_wait_for_a_child_process_to_terminate() {
    pid_t pid = fork();
    int status;

    bool i_am_a_child = (pid == 0);

    if (i_am_a_child) {
      sleep(1);
      exit(0);
    } else {
      int wait_result = waitpid(pid, &status, 0);

      bool the_child_was_signalled = WIFSIGNALED(status);
      bool the_child_exited_normally = WIFEXITED(status);

      CPPUNIT_ASSERT_MESSAGE("Did not expect the child to be signalled", the_child_was_signalled == false);
      CPPUNIT_ASSERT_MESSAGE("Expected waitpid to return > -1, because -1 means error.", wait_result);
      CPPUNIT_ASSERT_MESSAGE("Expected the child process to've exited normally", the_child_exited_normally);
    }
  }

  void you_can_tell_if_you_supply_invalid_options_to_waitpid() {
    pid_t pid = fork();
    int status;

    bool i_am_a_child = (pid == 0);

    int invalid_options = 1337;

    if (i_am_a_child) {
      exit(0);
    } else {
      int x = waitpid(pid, &status, invalid_options);

      bool waitpid_failed = (x == -1);
      
      CPPUNIT_ASSERT_MESSAGE("Expected waitpid to've failed but it succeeded, check the options", waitpid_failed);

      string actual;

      if (waitpid_failed) {
	if (errno == ECHILD) {
	  actual = "ECHILD";
	} else if (errno == EFAULT) {
	  actual = "EFAULT";
	} else if (errno == EINTR) {
	  actual = "EINTR";
	} else if (errno == EINVAL) {
	  actual = "EINVAL";	  
	} else if (errno == ENOSYS) {
	  actual = "ENOSYS";	  
	}
      }

      CPPUNIT_ASSERT_MESSAGE("Unexpected error, expected EINVAL", actual == "EINVAL");
    }
  }

  // See: /usr/include/bits/signum.h
  void you_can_tell_if_a_process_exits_abnormally() {
    pid_t pid = fork();
    int status;

    bool i_am_a_child = (pid == 0);

    int expected_exit_status = 2;

    if (i_am_a_child) {
      exit(2);
    } else {
      waitpid(pid, &status, 0);

      bool the_child_exited_normally = WIFEXITED(status);
      int the_exit_status = WEXITSTATUS(status);

      CPPUNIT_ASSERT_MESSAGE(
        "Expected the child process to've exited normally (, even though it exits with nonzero status)", 
        the_child_exited_normally
      );

      CPPUNIT_ASSERT_EQUAL_MESSAGE("Unexpected exit status", expected_exit_status, the_exit_status);
    }
  }

  const char *to_s(bool what) { return what ? "yes" : "no"; }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_processes);
