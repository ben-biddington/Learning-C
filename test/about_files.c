#include <iostream>

#include <string.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_LENGTH 0x1000

using namespace std;

class About_files : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_files);
  CPPUNIT_TEST(you_can_memory_map_a_file);
  CPPUNIT_TEST(you_can_create_a_file_and_write_to_it);
  CPPUNIT_TEST(you_can_advance_a_file_descriptor_enlarging_the_file_on_disk);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void) {}
  void tearDown(void) {} 

protected:
  void you_can_memory_map_a_file(void) {
    int file_descriptor = open ("example_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    void* file_memory = mmap (0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, file_descriptor, 0);

    lseek (file_descriptor, FILE_LENGTH+1, SEEK_SET);
    write (file_descriptor, "xxx", 1);
    lseek (file_descriptor, 0, SEEK_SET);

    sprintf((char*) file_memory, "%d", 1337);

    munmap (file_memory, FILE_LENGTH);

    close (file_descriptor);
  }

  void you_can_create_a_file_and_write_to_it() {
    const char *filename = "another_example_file";
    const char *expected = "Jazz Kang and Old Street roundabout";

    int file_descriptor = open (filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    write (file_descriptor, expected, 1024);

    close (file_descriptor);

    int read_descriptor = open (filename, O_RDWR, S_IRUSR | S_IWUSR);
    lseek (file_descriptor, 0, SEEK_SET); 
    
    char actual[1024];
    
    read(file_descriptor, actual, 1024);

    CPPUNIT_ASSERT(string(actual) == string(expected));

    unlink(filename);
  }

  void you_can_advance_a_file_descriptor_enlarging_the_file_on_disk() {
    const char *filename = "another_example_file";

    bool file_exists = fopen(filename, "r") != 0;

    if (file_exists) {
      unlink(filename);
    }

    int fd = open (filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    lseek (fd, 32, SEEK_END);
    write (fd, "xxx", 3);
    close (fd);

    FILE * file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);

    fclose(file);
    
    unlink(filename);

    int expected_length = 35;

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Does not match", expected_length, filesize);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_files);
