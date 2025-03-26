#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <fstream>


#include "../headers/DataBaseManager.h"
#include "../headers/paths.h"
#include "../headers/GeneralBinaryFileHandler.h"
#include "../libraries/library_nlohmann/include/nlohmann/json.hpp"
#include "../headers/converts.h"
#include "../headers/BinaryFileHandler.h"

using json = nlohmann::json;
using namespace std;


string SpecialTokens::getPromptUtf8(const int& posicion, const string& path){
    // Cargar el archivo JSON
    ifstream inputFile(path);
    if (!inputFile.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return "";
    }

    // Parsear el contenido del archivo
    json j;
    inputFile >> j;

    // Asegurarse de que el JSON sea un array
    if (!j.is_array()) {
        cerr << "El JSON no es un array." << endl;
        return "";
    }

    // Verificar que la posición esté dentro del rango del array
    if (posicion < 0 || posicion >= j.size()) {
        cerr << "Posición fuera del rango del array JSON." << endl;
        return "";
    }

    // Acceder al elemento en la posición dada y retornar el campo "prompt" si existe
    if (j[posicion].contains("special_token") && j[posicion]["special_token"].is_string()) {
        return j[posicion]["special_token"];
    } else {
        cerr << "El campo 'prompt' no se encuentra en la posición especificada o no es una cadena de texto." << endl;
        return "";
    }
};


int SpecialTokens::how_many_prompts(const string& path){
    ifstream archivo(path);// leer archivo json

    if (!archivo.is_open()){
        cerr<<"No se puede abrir el archivo json";
    }

    //leer el archivo json

    json prompts;
    archivo >> prompts; // Carga el contenido del archivo en el objeto JSON
    archivo.close();

    int contador_prompts =0;

    for (const auto& objeto: prompts){
        if (objeto.contains("special_token") && objeto["special_token"].is_string()){
            if(!objeto["special_token"].get<string>().empty()){
                contador_prompts ++;
            }
        }
    }


    return contador_prompts;
};

void SpecialTokens::insertInFileC(
    u32string value
) {

    uint64_t len = GeneralBinaryFileHandler::calculateBytesOfCharacters(value.c_str());

    // crear los archivos C
    ofstream recordsFileC(pathRecordsFileC, ios::binary | ios::app);
    ofstream metadataFileC(pathMetadataFileC, ios::binary | ios::app);

    if (!recordsFileC || !metadataFileC) {
        throw runtime_error("Error al abrir los archivos");
    }

    DataBaseManager::ValueType type = DataBaseManager::ValueType::STRING_UTF32;

    // Escribir type (1 byte)
    recordsFileC.write(reinterpret_cast<const char*>(&type), sizeof(uint8_t));

    // Obtener posición inicial del value (después del type)
    const uint64_t pos = recordsFileC.tellp();

    // Escribir value (UTF-32)
    recordsFileC.write(
        reinterpret_cast<const char*>(value.data()),
        value.size() * sizeof(char32_t)
    );

    uint64_t amount = 1;
    // Escribir metadatos 
    metadataFileC.write(reinterpret_cast<const char*>(&len), sizeof(uint64_t));
    metadataFileC.write(reinterpret_cast<const char*>(&amount), sizeof(uint64_t));
    metadataFileC.write(reinterpret_cast<const char*>(&pos), sizeof(uint64_t));

    // Debug
    cout << "Type: " << static_cast<int>(type)
        << " - Value: " << utf32ToUtf8(value)
        << " - len: "<<len
        << " - Amount: " << amount
        << " - Pos: " << pos << endl;

    recordsFileC.close();
    metadataFileC.close();
}



int SpecialTokens::insertSpecialTokensInFileC(){

    int amountPrompt = how_many_prompts(specialTokens);

    for (int i=0; i<amountPrompt; i++){
        string promptInUtf8 = getPromptUtf8(i, specialTokens);
        u32string promptInUtf32 = utf8_to_utf32(promptInUtf8);
        try{
            insertInFileC(promptInUtf32);

        } catch (const exception e){
            cout<<"Error: "<<e.what()<<endl;
        }
    }
        
    return 0;
}