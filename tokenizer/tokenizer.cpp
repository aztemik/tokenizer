
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <utility>
#include <vector>
#include <map>
#include <fstream>
#include "../libraries/library_nlohmann/include/nlohmann/json.hpp"
#include "../headers/tokenizer.h"
#include "../headers/paths.h"
#include "../headers/converts.h"

using namespace std;
using json = nlohmann::json;

string tokenizerHandler::getPromptUtf8(const int& posicion, const string& path){
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
    if (j[posicion].contains("prompt") && j[posicion]["prompt"].is_string()) {
        return j[posicion]["prompt"];
    } else {
        cerr << "El campo 'prompt' no se encuentra en la posición especificada o no es una cadena de texto." << endl;
        return "";
    }
};

vector<u32string> tokenizerHandler::generateTokens(const u32string& prompt) {
    vector<u32string> tokens;
    for (size_t i = 0; i < prompt.size(); i++) {
        tokens.push_back(u32string(1, prompt[i]));
    }
    return tokens;
};


pair<vector<DataBaseManager::parYFrecuencia>, vector<pair<u32string, u32string>>> tokenizerHandler::buildParesYFrecuencia (
    vector<u32string> tokens
){

    vector<DataBaseManager::parYFrecuencia> paresYFrecuencia;
    map<pair<u32string, u32string>, int> not_all_pares_freq;
    vector<pair<u32string, u32string>> orden;
    vector<pair<u32string, u32string>> all_pares_not_freq;
    for (int i=0; i<tokens.size() -1; i++){
        pair<u32string, u32string> par = {tokens[i], tokens[i+1]};
        all_pares_not_freq.push_back(par);
        if (not_all_pares_freq[par] == 0) {
            orden.push_back(par);
        }
        not_all_pares_freq[par]++;
    }
    for (const auto& par : orden) {
        paresYFrecuencia.emplace_back(par.first, par.second, not_all_pares_freq[par]);
    }
    
    return make_pair(paresYFrecuencia,all_pares_not_freq);
   
};

vector<u32string> tokenizerHandler::buildOnlyPares (vector<u32string> tokens){
    vector<u32string> pares;
    for (int i=0; i<tokens.size() -1; i++){
        u32string par = tokens[i] + tokens [i+1];
        pares.push_back(par);
    }
    return pares;
}

vector<u32string> tokenizerHandler::buildPairsWithReplacement(
    const vector<u32string>& tokens
) {
    char32_t replacement_char = U'\uE000';
    vector<u32string> pairs;
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        u32string combined = tokens[i] + tokens[i + 1];
        
        // Reemplazar todos los espacios (0x20) en el par combinado
        for (auto& c : combined) {
            if (c == U'\u0020') { // Verificar si es espacio (0x20)
                c = replacement_char; // Reemplazar
            }
        }
        
        pairs.push_back(combined);
    }
    return pairs;
}


pair<pair<u32string, u32string>, int> tokenizerHandler::maxPar(
    vector<DataBaseManager::parYFrecuencia> paresYFrecuencia
){

    pair<pair<u32string, u32string>, int> parMasFrecuente = {{U"",U"" }, 0};

    for (const auto& c: paresYFrecuencia){
        pair<u32string, u32string> par = {c.par1, c.par2};
        if (c.frecuencia > parMasFrecuente.second){
            parMasFrecuente.first = par;
            parMasFrecuente.second = c.frecuencia;            
        }
    }
    
    return parMasFrecuente;
};


string encontrar_merge_rule (const string& path, const string& merge_rule){
    string elemento_encontrado;
    ifstream archivo(path); // Abrir archivo JSON

    // Verifica que el archivo se abrió correctamente
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo JSON" << endl;
        return ""; // Salir de la función si no se puede abrir el archivo
    }

    // Leer el JSON del archivo
    json data;
    archivo >> data;  // Carga el contenido del archivo en el objeto JSON
    archivo.close();

    // Verifica que el JSON contiene el array "vocab"
    if (!data.contains("vocab") || !data["vocab"].is_array()) {
        cerr << "El JSON no contiene un array llamado 'vocab'" << endl;
    }

    // bool encontrado = false;
    for (const auto& elemento : data["vocab"]) {
        if (elemento.is_string()) {
            if (elemento == merge_rule) {
                cout << "Se encontró el elemento: " << elemento << endl;
                elemento_encontrado = merge_rule;
                // encontrado = true;
                break; // Salir del bucle una vez que encuentras la coincidencia
            }
        }
    }

    return elemento_encontrado;

};

int tokenizerHandler::how_many_prompts(const string& path){
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
        if (objeto.contains("prompt") && objeto["prompt"].is_string()){
            if(!objeto["prompt"].get<string>().empty()){
                contador_prompts ++;
            }
        }
    }


    return contador_prompts;
};


unordered_map<u32string, int> tokenizerHandler::loadVocabularyForTokenizer(

) {

    vector<u32string> vocab_vector;
    unordered_map<u32string, int> value_to_tokenID;
    
    ifstream recordsFileC(pathRecordsFileC, ios::binary);
    ifstream metadataFileC(pathMetadataFileC, ios::binary);

    if (!metadataFileC.is_open() || !recordsFileC.is_open()) {
        cerr << "Error al abrir los archivos\n";
    }

    DataBaseManager::struct_metadataFileC metadata;
    while (metadataFileC.read(reinterpret_cast<char*>(&metadata), sizeof(DataBaseManager::struct_metadataFileC))) {

        // Posicionarse al inicio del value
        recordsFileC.seekg(metadata.pos);

        // Leer value (UTF-32)
        vector<char32_t> value(metadata.len / sizeof(char32_t));
        recordsFileC.read(reinterpret_cast<char*>(value.data()), metadata.len);

        // insertar merge rule en vector
        vocab_vector.push_back(value.data());
        // insertar idRecord y relacionar con la posicion del value en el vector
        value_to_tokenID[value.data()] = vocab_vector.size() - 1;


        // Debug
        // param_vectorChar32_see(value);
        // cout<<"\n";

    }
    
    metadataFileC.close();
    recordsFileC.close();

    return value_to_tokenID;
};

vector<uint64_t> tokenizerHandler::tokenizer(string texto) {

    // Cargar vocabulario
    unordered_map<u32string, int> value_to_tokenID = tokenizerHandler::loadVocabularyForTokenizer();

    // Obtener el ID del token "UNK"
    u32string unk_token = U"<UNK>"; // Ajusta esto según tu vocabulario
    auto unk_it = value_to_tokenID.find(unk_token);
    if (unk_it == value_to_tokenID.end()) {
        throw runtime_error("Token UNK no encontrado en el vocabulario");
    }
    uint64_t unk_id = unk_it->second;
    
    vector<uint64_t> texto_tokenizado;
    u32string promptUTF32 = utf8_to_utf32(texto);
    vector<u32string> tokens = tokenizerHandler::generateTokens(promptUTF32);
    vector<u32string> pairs = tokenizerHandler::buildPairsWithReplacement(tokens);
    
    // Procesar cada par
    for (const auto& pair : pairs) { // Itera directamente sobre 'pairs'
        auto it = value_to_tokenID.find(pair);
        if (it != value_to_tokenID.end()) {
            texto_tokenizado.push_back(it->second);

            // Debug
            cout<<"Token: "<<utf32ToUtf8(pair)<<" - ID: "<<it->second<<endl;

        } else {
            // Reemplazar par desconocido por UNK
            texto_tokenizado.push_back(unk_id);
            cout<<"Token: "<<utf32ToUtf8(pair)<<" - ID: "<<unk_id<<" - ";
            string tokenUtf8 = utf32ToUtf8(unk_token); 
            cout<<tokenUtf8;cout<<"\n";
        }
    }
   
    return texto_tokenizado;
}




