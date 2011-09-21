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
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/inotify.h>
#include <unistd.h>

using std::string;

class About_io : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_io);
  CPPUNIT_TEST(you_can_monitor_a_file_for_changes_using_inotify);
  CPPUNIT_TEST(you_can_add_watches);
  CPPUNIT_TEST(you_cannot_watch_a_file_that_does_not_exist);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void) {
    errno = 0;
  }

  void tearDown(void) {} 

protected:
  void you_can_add_watches() {
    const char *the_file = "example_file";

    touch(the_file);

    int notifier = inotify_init();
    int watch = inotify_add_watch(notifier, the_file, IN_MODIFY | IN_CREATE);

    close(notifier);

    CPPUNIT_ASSERT(watch > 0);

    unlink(the_file);
  }

  void you_cannot_watch_a_file_that_does_not_exist() {
    const char *any_file_that_does_not_exist = "xxx_does_not_exist_xxx";
    
    unlink(any_file_that_does_not_exist);

    int notifier = inotify_init();
    int watch = inotify_add_watch(notifier, any_file_that_does_not_exist, IN_MODIFY | IN_CREATE);

    close(notifier);

    CPPUNIT_ASSERT_EQUAL(-1, watch);
    CPPUNIT_ASSERT_EQUAL(ENOENT, errno);
  }

  void do_not_close_watches_you_will_find_you_kill_stdout() { }

  void you_can_monitor_a_file_for_changes_using_inotify() {
    const char *the_file = "selectable_file_example";
    
    touch(the_file);
    
    pid_t pid = fork();    

    bool i_am_a_child = pid == 0;

    if (i_am_a_child) {
      int notifier = inotify_init();
      int watch = inotify_add_watch(notifier, the_file, IN_MODIFY | IN_CREATE | IN_DELETE);

      if (notifier < 0) {
	printf("\n\nError getting notifier.\n\n");
	exit(1);
      }

      if (watch < 0) {
	printf("\n\nFailed to get watch, ensure the file exists\n\n");
	exit(1);
      }

      char buffer[1024];
      int result = read(notifier, &buffer, sizeof(inotify_event));

      CPPUNIT_ASSERT_MESSAGE("The result of the read was zero, i.e., nothing detected.", result);

      int exit_status = result ? 0 : 1;
      exit(exit_status);
    }

    sleep(2);

    touch(the_file);

    int child_exit_status;

    int wait_result = waitpid(pid, &child_exit_status, 0);

    unlink(the_file);

    CPPUNIT_ASSERT_MESSAGE("Expected the child process to've exited okay", WIFEXITED(child_exit_status));
  }

  void you_can_tell_select_has_timed_out_by_examining_the_timeval_supplied() { }

  void touch(const char *the_file) {
    int fd = open(the_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  
    lseek (fd, 0, SEEK_SET);
    write (fd, "xxx\n", 3);
    close(fd);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_io);
