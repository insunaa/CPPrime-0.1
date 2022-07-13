#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <ratio>
#include <string>
#include <cstring>
#include <sys/types.h>

#ifdef _WIN32
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef __int32 int32;
typedef unsigned char uchar;
#else
typedef u_int64_t uint64;
typedef u_int32_t uint32;
typedef int32_t int32;
typedef u_char uchar;
#endif

uint64 pr = 0;
uint64 vr = -1;

void getOS()
{
    std::flush(std::cout);
#ifdef _WIN32
    std::system("ver");
#else
    std::system("uname -vrs");
#endif
}

void printHead(uint64 qpf)
{
    uint64 hpr = pr / 1000000;
    char ppr   = 'M';
    if (hpr > 1024)
    {
        hpr = hpr / 1000;
        ppr = 'B';
    }
    
    std::string banner(85, '-');
    std::string spacer(37, ' ');
    std::cout << banner << std::endl
              << spacer << "CPPrime 0.1" << spacer << std::endl
              << banner << std::endl
              << std::endl;

    std::cout << "OS\t: ";
    getOS();
    std::cout << "Timer\t: " << qpf / (1000 * 1000) << " MHz" << std::endl;
    std::cout << "Prime\t: " << hpr << ppr << " - up to " << pr << std::endl;
}

void printScore(uint64 prime, bool valid, std::chrono::duration<int, std::milli> time)
{
    if (!valid)
        std::cout << "INVALID: " << prime << std::endl;
    else
    {
        std::cout << "Prime number\t\t: " << prime << " is VALID" << std::endl;
        std::cout << "Computation time\t: " << time.count() / 1000.f << " s" << std::endl;
    }
}

void printMemalloc(uint64 bytes)
{
    float size       = bytes / 1000000000.f;
    std::string name = "GB";

    if (size < 1.f)
    {
        size = bytes / 1000000.f;
        name = "MB";
        if (size < 1.f)
        {
            size = bytes / 1000.f;
            name = "KB";
        }
    }

    std::cout << "Sieve allocation: " << size << " " << name << std::endl;
}

void printStatus(int& loop, std::chrono::steady_clock::time_point& start_time)
{
    std::cout << "\t Step " << std::setw(2) << loop << " ....... " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() / 1000.f << " s"
              << std::endl;
}

uint64 calc(uchar* sieve, uint64& limit, uint64& sqrtlimit, std::chrono::steady_clock::time_point& start_time) noexcept
{
    uint64 loopstep, nextstep, x, x2, x2b3, x2b4, y, y2, n, m, o;
    int32 loop = 0;

    loopstep   = sqrtlimit / 10;
    nextstep   = loopstep;

    for (x = 1; x <= sqrtlimit; ++x)
    {
        x2   = x * x;
        x2b3 = x2 * 3;
        x2b4 = x2b3 + x2;

        for (y = 1; y <= sqrtlimit; ++y)
        {
            y2 = y * y;
            n  = x2b4 + y2;
            m  = x2b3 + y2;
            o  = x2b3 - y2;

            if (n <= limit && (n % 12 == 5 || n % 12 == 1))
                sieve[n / 8] ^= 1 << (n % 8);

            if (m <= limit && m % 12 == 7)
                sieve[m / 8] ^= 1 << (m % 8);

            if (x > y && o <= limit && o % 12 == 11)
                sieve[o / 8] ^= 1 << (o % 8);
        }

        if (x > nextstep && loop < 9)
        {
            nextstep += loopstep;
            ++loop;

            printStatus(loop, start_time);
        }
    }

    for (x = 5; x < sqrtlimit; ++x)
    {
        if (sieve[x])
        {
            x2 = x * x;

            for (y = x2; y < limit + 1; y += x2)
                sieve[y / 8] &= ~(1 << (y % 8));
        }
    }

    x = limit / 8;
    while (sieve[x] == 0)
        --x;
    
    return x;
}

void benchmark(uint64 limit, std::chrono::steady_clock::time_point& start_time, std::chrono::steady_clock::time_point& end_time)
{
    uint64 resultx, result;
    uint64 sieve_len = limit / 8;
    uint64 sqrtlimit = std::sqrt(limit);

    printMemalloc(sieve_len);

    uchar* sieve  = nullptr;
    sieve         = (uchar*)malloc((sieve_len + 1) * sizeof(uchar));
    std::memset(sieve, (uchar)0, (sieve_len + 1) * sizeof(uchar));

    std::cout << "Starting benchmark:" << std::endl << std::endl;
    start_time = std::chrono::steady_clock::now();
    resultx    = calc(sieve, limit, sqrtlimit, start_time);
    end_time   = std::chrono::steady_clock::now();
    result     = std::floor(std::log2(sieve[resultx])) + resultx * 8;

    if (sieve)
    {
        free(sieve);
        sieve = nullptr;
    }
    std::cout << "\t Sieve Scan .... " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.f << " s" << std::endl << std::endl;

    printScore(result, result == vr, std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time));
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: CPPrime.exe [1-1024M, 1-32B]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string size = argv[1];

    if (size == "1M")
    {
        pr = 1000000;
        vr = 999983;
    }
    if (size == "2M")
    {
        pr = 2000000;
        vr = 1999993;
    }
    if (size == "4M")
    {
        pr = 4000000;
        vr = 3999971;
    }
    if (size == "8M")
    {
        pr = 8000000;
        vr = 7999993;
    }
    if (size == "16M")
    {
        pr = 20000000;
        vr = 19999999;
    }
    if (size == "32M")
    {
        pr = 32000000;
        vr = 31999939;
    }
    if (size == "64M")
    {
        pr = 64000000;
        vr = 63999979;
    }
    if (size == "128M")
    {
        pr = 128000000;
        vr = 127999981;
    }
    if (size == "256M")
    {
        pr = 256000000;
        vr = 255999983;
    }
    if (size == "512M")
    {
        pr = 512000000;
        vr = 511999979;
    }
    if (size == "1024M" || size == "1B")
    {
        pr = 1024000000;
        vr = 1023999989;
    }
    if (size == "2048M" || size == "2B")
    {
        pr = 2048000000;
        vr = 2047999957;
    }
    if (size == "4096M" || size == "4B")
    {
        pr = 4096000000;
        vr = 4095999983;
    }
    if (size == "8192M" || size == "8B")
    {
        pr = 8192000000;
        vr = 8191999993;
    }
    if (size == "16384M" || size == "16B")
    {
        pr = 16384000000;
        vr = 16383999977;
    }
    if (size == "32768M" || size == "32B")
    {
        pr = 32768000000;
        vr = 32767999997;
    }

    if (vr == -1)
    {
        std::cout << "Usage: CPPrime.exe [1-1024M, 1-32B]" << std::endl;
        return EXIT_FAILURE;
    }

    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    auto freq = std::chrono::steady_clock::period();
    printHead(freq.den / freq.num);
    benchmark(pr, start_time, end_time);
    return EXIT_SUCCESS;
}
