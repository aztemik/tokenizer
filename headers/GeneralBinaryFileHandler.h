#ifndef GENERALBINARYFILEHANDLER_H
#define GENERALBINARYFILEHANDLER_H

#include "DataBaseManager.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class GeneralBinaryFileHandler {
    
    public:

    static uint64_t calculateExactBytesInNumbers(uint64_t number);

    static uint64_t calculateBytesOfCharacters(const char32_t* value);

    static uint64_t getNextPosition(const std::string& filepath);

    using DatoConvertido = std::variant<uint64_t, std::u32string>; // Puede ser número o texto
    static DatoConvertido queTipoDeValorEs(const char* value);

    static std::string valueTypeToString(DataBaseManager::ValueType type);

    static void primerosBytes(const std::string& recordsFile);

    static int insertType (const DataBaseManager::ValueType& type, const std::string& recordsFile);

};


#endif /* end of GENERALBINARYFILEHANDLER_H */
