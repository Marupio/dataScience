#include<unistd.h>
#include<osRelated.h>

// ****** Global Functions ****** //

void ds::sleep(int milliseconds) {
    usleep(milliseconds*1000);
}

// ****** END ****** //
