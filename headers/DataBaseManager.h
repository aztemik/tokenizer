#ifndef DataBaseManager_H
#define DataBaseManager_H

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iosfwd>
#include <map>
#include <stdexcept>
#include <sys/types.h>
#include <type_traits> // Para std::is_same_v
#include <string>
#include <variant> // Para std::variant
#include <vector>

namespace DataBaseManager{

    // Identificado para el incio de cada value
    enum class ValueType : uint8_t {
        STRING_UTF32 = 1,
        UINT64 = 2,
        // Espacio para futuros tipos
    };
    
    struct struct_metadataFileA{
        uint64_t len;
        uint64_t pos;     // 64 bits en mi arquitectura 8 bytes
    };

    struct operationsFileB {
        uint8_t type;
        std::u32string value;
        uint64_t len;
    };

    // Estructura para recordsFileB (Type, Value, Amount)
    struct struct_recordFileB {
        uint8_t type;
        std::u32string value;
        uint64_t amount;
    };

    
    struct metadataFileB{
        uint64_t len;
        uint64_t pos;
    };

    struct countOperationFileB {
        operationsFileB operation;
        uint64_t amount;
    };

    // Clave única para el conteo
    using KeyType = std::pair<uint8_t, std::u32string>; 

    struct struct_metadataFileC{
        uint64_t len;
        uint64_t amount;
        uint64_t pos;
    };

    struct struct_recordFileC{
        uint8_t type;
        uint64_t value;
    };

    struct operationsFileC{
        struct_recordFileC recordFileC;
        struct_metadataFileC operation;
    };

    inline std::uint32_t seed = 2654435769;

    struct parYFrecuencia{
        std::u32string par1;
        std::u32string par2;
        int frecuencia;
        std::vector<std::u32string> generateTokens(const std::u32string& prompt) {
            std::vector<std::u32string> tokens;
            for (size_t i = 0; i < prompt.size(); i++) {
                tokens.push_back(std::u32string(1, prompt[i]));
            }
            return tokens;
        };
    
    
        // constructor 
        parYFrecuencia(std::u32string p1, std::u32string p2, int freq): par1(p1), par2(p2), frecuencia(freq) {}
    };
   
}

#endif /* end of DataBaseManager_H */