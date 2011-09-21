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

int main(int argc, char *argv[]) {
  printf("(%d) args\n", argc);
  
  int watches[argc-1];

  if (argc > 1) {
    int notifier = inotify_init();
    
    int i;

    fd_set file_descriptors;
    FD_ZERO(&file_descriptors);

    for (i = 1; i < argc; i++) { 
      char *the_file = argv[i];

      int watch = inotify_add_watch(notifier, the_file, IN_MODIFY | IN_CREATE);
    
      if (watch < 0) {
	printf("There was an error adding a watch.");
	exit(1);
      }
  
      FD_SET(watch, &file_descriptors);

      printf("Watching file: %s, watch_id=%d\n", the_file, watch);

      watches[i-1] = watch;
    }

    bool done = false;
    int read_count = 0;

    while (!done) {
      timeval timeout = {10, 0};

      int result = select(8, &file_descriptors, NULL, NULL, NULL);

      int response_size = sizeof(inotify_event);

      char buffer[response_size];

      if (result && FD_ISSET(notifier, &file_descriptors)) {
	read(notifier, buffer, response_size);
	void *ptr = &buffer;
	inotify_event *e = (inotify_event *)ptr;
	
	printf("File changed (%d), which has name (%d) and descriptor (%d)\n", ++read_count, e->len, e->wd);
      }
    }
    
    close(notifier);
  }

  return 0;
}
