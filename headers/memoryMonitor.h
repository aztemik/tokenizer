#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H


#include <sys/resource.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <string>
#include <unistd.h> // Agregamos esta cabecera para sysconf y _SC_PAGESIZE
#include <vector>


class MemoryMonitor {
private:


public:
    static bool setMemoryLimit(size_t limitInBytes) {
        struct rlimit limit;
        
        // Obtener límites actuales
        if (getrlimit(RLIMIT_AS, &limit) != 0) {
            std::cerr << "Error al obtener límites actuales" << std::endl;
            return false;
        }
        
        // Establecer nuevo límite
        limit.rlim_cur = limitInBytes;
        
        if (setrlimit(RLIMIT_AS, &limit) != 0) {
            std::cerr << "Error al establecer límite de memoria" << std::endl;
            return false;
        }
        
        return true;
    }
};


#endif /* end of MEMORYMONITOR_H */