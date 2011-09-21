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

using std::string;

class About_io : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_io);
  CPPUNIT_TEST(you_can_monitor_a_file_for_changes_using_inotify_and_select);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void) {}
  void tearDown(void) {} 

protected:
  // TODO: blocks forever seemingly every other time this is run
  void you_can_monitor_a_file_for_changes_using_inotify_and_select() {
    const char *the_file = "selectable_file_example";

    int notifier = inotify_init();
    int watch = inotify_add_watch(notifier, the_file, IN_MODIFY | IN_CREATE | IN_DELETE);
    
    fd_set file_descriptors;
    FD_ZERO(&file_descriptors);
    FD_SET(notifier, &file_descriptors);
    FD_SET(watch, &file_descriptors);

    touch(the_file);

    int result = select(sizeof(file_descriptors)+1, &file_descriptors, NULL, NULL, NULL);

    char buffer[1024];

    read(notifier, &buffer, sizeof(inotify_event));

    CPPUNIT_ASSERT(result);

    unlink(the_file);
  }

  void touch(const char *the_file) {
    pid_t pid;    

    if ((pid = fork()) == 0) {
      sleep(2);

      int fd = open(the_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  

      lseek (fd, 0, SEEK_SET);
      write (fd, "any text", 128);

      close(fd);
      exit(0);
    }
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_io);
