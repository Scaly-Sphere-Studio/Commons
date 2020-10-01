#include "SSS/Commons/time.hpp"

SSS_BEGIN__

std::string UTF_Current_Time()
{
    using namespace std::chrono;

    // Retrieve current time
    system_clock::duration const now = system_clock::now().time_since_epoch();

    // Split time into hours, minutes, seconds and milliseconds
    int const ms(duration_cast<milliseconds>(now).count() % 1000);
    int const s(duration_cast<seconds>(now).count() % 60);
    int const m(duration_cast<minutes>(now).count() % 60);
    int const h(duration_cast<hours>(now).count() % 24);

    // Format the string via printf
    char buff[64];
    snprintf(buff, 64U, "%02d:%02d:%02d.%03d UTF", h, m, s, ms);

    return buff;
}

SSS_END__