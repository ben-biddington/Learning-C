#include <iostream>

#include <string.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <stdlib.h> 
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

using std::string;

class About_io : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_io);
  CPPUNIT_TEST(you_can_select_on_a_file_and_for_a_period_of_time);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void) {}
  void tearDown(void) {} 

protected:
  void you_can_select_on_a_file_and_for_a_period_of_time() {
    const char *file = "selectable_file_example";
    int fd = open (file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  

    fd_set file_descriptors;

    FD_ZERO(&file_descriptors);
    FD_SET(fd, &file_descriptors);

    long timeout_seconds = 0;
    long timeout_milliseconds = 500;
    timeval timeout = {timeout_seconds, timeout_milliseconds};
    
    int result = select(1, &file_descriptors, NULL, NULL, &timeout);

    close(fd);
    unlink(file);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_io);
