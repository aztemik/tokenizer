#ifndef CONVERTS
#define CONVERTS

#include <codecvt>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <locale>
#include <string>
#include <type_traits> 
#include <vector>

inline std::u32string utf8_to_utf32(const std::string& input) {
    // Crear un convertidor de UTF-8 a UTF-32
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    
    try {
        // Realizar la conversión
        return converter.from_bytes(input);
    } catch(const std::range_error& e) {
        // Manejar errores de conversión
        return U"";
    }
    
};

inline std::string utf32ToUtf8(const std::u32string& input) {
    // Crear un convertidor de UTF-8 a UTF-32
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    
    try {
        // Realizar la conversión
        return converter.to_bytes(input);
    } catch(const std::range_error& e) {
        // Manejar errores de conversión
        return "";
    }
    
};


inline void param_vectorChar32_see(const std::vector<char32_t>& utf32Str) {
    // Configurar la localización para manejar caracteres Unicode
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    
    // Crear el convertidor de UTF-32 a UTF-8
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conversor;
    
    try {
        // Convertir y mostrar el string completo
        std::string utf8Str = conversor.to_bytes(utf32Str.data());
        std::cout << "Value utf32ToUtf32: " << utf8Str;
        
    } catch (const std::exception& e) {
        std::cerr << "Error en la conversión: " << e.what() << std::endl;
    }
}


// Función para convertir UTF-32 a UTF-8 y mostrarlo en consola
inline void mostrarUTF32_1(const std::vector<char32_t>& utf32Str) {
    // Configurar la localización para manejar caracteres Unicode
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    
    // Crear el convertidor de UTF-32 a UTF-8
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conversor;
    
    try {
        // Convertir y mostrar el string completo
        std::string utf8Str = conversor.to_bytes(utf32Str.data());
        std::cout << "Valor real: " << utf8Str << std::endl;
        
        // Mostrar también cada carácter individualmente si se desea
        std::cout << "Caracteres individuales:\n";
        for (const auto& ch : utf32Str) {
            std::string caracter = conversor.to_bytes(ch);
            std::cout << caracter << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error en la conversión: " << e.what() << std::endl;
    }
}

inline void param_u32string_see(const std::u32string& utf32Str) {
    // Configurar la localización para manejar caracteres Unicode
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    // Crear el convertidor de UTF-32 a UTF-8
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conversor;
    try {
        // Convertir y mostrar el string completo
        std::string utf8Str = conversor.to_bytes(utf32Str.data());
        std::cout << "UTF-8 to UTF-32: " << utf8Str << std::endl;

        std::cout << "Caracteres y sus valores hexadecimales:\n";
        for (const auto& ch : utf32Str) {
            std::string caracter = conversor.to_bytes(ch);
            std::cout 
                    // << caracter 
                    << " (0x" 
                    << std::hex               // Cambiar a formato hexadecimal
                    << std::uppercase         // Usar letras mayúsculas para A-F
                    //  << std::setw(8)          // Ancho fijo de 8 caracteres
                    //  << std::setfill('0')     // Rellenar con ceros a la izquierda
                    << static_cast<uint32_t>(ch)
                    << ") ";
        }

        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error en la conversión: " << e.what() << std::endl;
    }
}

inline void param_vectorU32string_see(const std::vector<std::u32string>& pares){
    // Configurar la localización para manejar caracteres Unicode
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conversor;

    for (int i=0; i<pares.size(); i++){

        try{
            std::string utf8Str = conversor.to_bytes(pares[i].data());
            std::cout<< "PAR UTF-32 to UTF-8: "<<utf8Str<<std::endl;
        } catch (const std::exception& e){
            std::cout<<"Error: "<<e.what()<<std::endl;
        }
        
    }

}


// Convierte una KeyType (pair<uint8_t, u32string>) a string UTF-8
// static std::string keyToString(const std::pair<uint8_t, std::u32string>& key) {
//     return std::to_string(key.first) + ": " + utf32ToUtf8(key.second);
// }



#endif // CONVERTS