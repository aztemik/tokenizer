#include <cstddef>
#include <cstdint>
#include <fstream>
#include <variant>

#include "../headers/converts.h"
#include "../headers/DataBaseManager.h"
#include "../headers/GeneralBinaryFileHandler.h"

using namespace std;

// Calcula el número mínimo de bytes necesarios para un número
uint64_t GeneralBinaryFileHandler::calculateExactBytesInNumbers(uint64_t number) {
    if (number == 0) return 1;
    size_t bytes = 0;
    while (number > 0) {
        bytes++;
        number >>= 8;
    }
    return bytes;
};

// Calculuint64_ta el número de bytes para caracteres UTF-32/UCS-4
uint64_t GeneralBinaryFileHandler::calculateBytesOfCharacters(const char32_t* value) {
    // Si el puntero es nulo (cadena vacía o no válida), retornamos 0.
    if (value == nullptr) return 0;

    uint64_t count = 0; // Contador para el número de caracteres en la cadena.

    // Recorremos la cadena de caracteres UTF-32 usando el puntero.
    // Continuamos iterando hasta encontrar el carácter de terminación nulo (U'\0').
    while (value[count] != U'\0') {
        count++; // Incrementamos el contador de caracteres.
    }

    // Multiplicamos el número de caracteres encontrados por 4 bytes.
    // Esto se debe a que en UTF-32, cada carácter ocupa exactamente 4 bytes.
    return count * 4; // Número total de bytes necesarios.
};

uint64_t GeneralBinaryFileHandler::getNextPosition(const string& filepath) {
    ifstream file(filepath, ios::binary | ios::ate); // ios::ate posiciona al final inmediatamente
    if (!file) {
        throw runtime_error("El archivo " + filepath + " no existe");
    }
    uint64_t pos = file.tellg();
    file.close();
    return pos;
};


using DatoConvertido = std::variant<uint64_t, std::u32string>; // Puede ser número o texto
DatoConvertido GeneralBinaryFileHandler::queTipoDeValorEs(const char* value) {
    // Intentar convertir a uint64_t
    char* endptr;
    uint64_t numericValue = strtoull(value, &endptr, 10);

    if (*endptr == '\0') {
        // Si se convierte completamente, devolver como número
        return numericValue;
    } else {
        // De lo contrario, convertir a u32string (texto)
        return utf8_to_utf32(value);
    }
};


// Función para convertir ValueType a una cadena
string GeneralBinaryFileHandler::valueTypeToString(DataBaseManager::ValueType type) {
    
    switch (type) {
        case DataBaseManager::ValueType::STRING_UTF32: return "STRING_UTF32";
        case DataBaseManager::ValueType::UINT64: return "UINT64";
        default: return "UNKNOWN";
    }
};

void GeneralBinaryFileHandler::primerosBytes(const string& recordsFile) {
    ifstream file(recordsFile, ios::binary);
    if (!file) {
        cout << "No se pudo abrir el archivo\n";
        return;
    }
    
    cout << "Debug - Primeros bytes después de escribir:\n";
    // Leemos los primeros 21 bytes (1 de tipo + 20 de la cadena)
    for(int i = 0; i < 21; ++i) {
        unsigned char byte;
        if (file.read(reinterpret_cast<char*>(&byte), 1)) {
            cout << "Byte " << i << ": " << hex 
                << setw(2) << setfill('0') 
                << static_cast<int>(byte) << dec;
            if (i == 0) {
                cout << " (tipo)";
            } else {
                cout << " (pos " << (i-1) << " del string)";
                if ((i-1) % 4 == 0) {
                    cout << " <- inicio de caracter";
                }
            }
            cout << "\n";
        } else {
            cout << "No más bytes para leer\n";
            break;
        }
    }
    file.close();
};

int GeneralBinaryFileHandler::insertType (const DataBaseManager::ValueType& type, const string& recordsFile){

    ofstream dataFile (recordsFile, ios::binary | ios::app);
    if (!dataFile.is_open()){
        cout<<"Error al abrir el archivo"<<endl;
        return 1;
    }
    try{
        dataFile.write(reinterpret_cast<const char*>(&type), sizeof(DataBaseManager::ValueType));    
    } catch (const exception& e){
        cout<<"Error: "<<e.what()<<endl;
    }

    dataFile.close();       
    
    return 0;
}
