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
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class About_sockets : public CPPUNIT_NS::TestCase {
  CPPUNIT_TEST_SUITE(About_sockets);
  CPPUNIT_TEST(you_can_create_one_connect_to_a_url_send_a_get_and_get_the_response);
  CPPUNIT_TEST_SUITE_END();

private:
  int x;

public:
  void setUp(void) {
    errno = 0;
  }

  void tearDown(void) {} 

protected:
  void you_can_create_one_connect_to_a_url_send_a_get_and_get_the_response() {
    struct addrinfo hints, *url_info;

    int url_info_result = getaddrinfo("www.google.com", "http", NULL, &url_info);

    CPPUNIT_ASSERT_MESSAGE("Failed to create address info", url_info_result >= 0);

    int fd = socket(url_info->ai_family, url_info->ai_socktype, url_info->ai_protocol);

    int connect_result = connect(fd, url_info->ai_addr, url_info->ai_addrlen);

    CPPUNIT_ASSERT_MESSAGE("Expected connect to return > -1 on success. Something went wrong", connect_result > -1);

    const char *msg = "GET /\n\n";
    int len = strlen(msg);
    int flags = 0;
    int result = send(fd, msg, len, flags);

    CPPUNIT_ASSERT_MESSAGE("Expected the message to've been sent", result == len);

    char response_buffer[1024 * 50];

    int recv_result = recv(fd, &response_buffer, sizeof(response_buffer), 0);

    CPPUNIT_ASSERT_MESSAGE("Expected more than zero bytes to've been returned", recv_result > 0);

    close(fd);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(About_sockets);
