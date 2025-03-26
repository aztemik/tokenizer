#include <cstdint>
#include <exception>
#include <ostream>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

#include "../headers/DataBaseManager.h"
#include "../headers/GeneralBinaryFileHandler.h"
#include "../headers/BinaryFileHandler.h"
#include "../headers/paths.h"
#include "../headers/converts.h"

using namespace std;

int BinaryFileAHandler::insertRecordsToFileA(
    const u32string value, 
    const string& recordsFile,
    const string& metadataFile
    ){

    uint64_t len = GeneralBinaryFileHandler::calculateBytesOfCharacters(value.c_str());
    DataBaseManager::ValueType type = DataBaseManager::ValueType::STRING_UTF32;

    // Create/Insert only value
    try{
        ofstream dataFile(recordsFile, ios::binary | ios::app);
        dataFile.write(reinterpret_cast<const char*>(&type), sizeof(DataBaseManager::ValueType));
        dataFile.close();
    } catch (const exception& e){
        cout<<"Error: "<<e.what()<<endl;
    }

    uint64_t pos = GeneralBinaryFileHandler::getNextPosition(recordsFile);

    try{
        ofstream dataFile(recordsFile, ios::binary | ios::app);
        dataFile.write(reinterpret_cast<const char*>(value.data()), len); // sin enteder
        dataFile.close();
    } catch (const exception& e){
        cout<<"Error: "<<e.what()<<endl;
    }

    // Create/Insert Only Metadata
    try{
        DataBaseManager::struct_metadataFileA record = { len, pos};

        ofstream metadata(metadataFile, ios::binary | ios::app);
        metadata.write(reinterpret_cast<const char*>(&record), sizeof(DataBaseManager::struct_metadataFileA));
        metadata.close();
    } catch (const exception& e){
        cout<<"Error: "<<e.what()<<endl;
    }

    return 0;
}

int BinaryFileAHandler::createFiles_A (vector<u32string> onlyPares){

    for (int i=0; i<onlyPares.size(); i++){
        // insertar y/o crear binario metadataFileA and recordsFileA
        try{
            BinaryFileAHandler::insertRecordsToFileA(
                onlyPares[i],
                pathRecordsFileA, 
                pathMetadataFileA
            );
        }catch (const exception& e){
            cout<<"Error: "<<e.what()<<endl;
        }
        
    }

    //debug 
    // cout<<"DEBUG 2"<<endl;
    // for (int i=0; i<onlyPares.size(); i++){
    //     cout<<utf32ToUtf8(onlyPares[i])<<endl;
    // }

    return 0;
};


void BinaryFileAHandler::readTwoBinaryFiles_A(){

    ifstream recordsFileA(pathRecordsFileA, ios::binary);
    ifstream metadataFileA(pathMetadataFileA, ios::binary);

    if (!metadataFileA.is_open() || !recordsFileA.is_open()){
        cerr<<"Error al abrir los archivos\n";
        return;
    }

    DataBaseManager::struct_metadataFileA metadata;

    while(metadataFileA.read(reinterpret_cast<char*>(&metadata), sizeof(DataBaseManager::struct_metadataFileA))){
        // Leer type (1 byte antes de la posición del value)
        recordsFileA.seekg(metadata.pos - streamoff(1));
        uint8_t type;
        recordsFileA.read(reinterpret_cast<char*>(&type), sizeof(uint8_t));

        // Posicionarse al inicio del value
        recordsFileA.seekg(metadata.pos);

        // Leer value (UTF-32)
        vector<char32_t> value(metadata.len / sizeof(char32_t));
        recordsFileA.read(reinterpret_cast<char*>(value.data()), metadata.len);

        // Mostrar datos
        cout << "Type: " << static_cast<int>(type)<<" - Value: ";
        cout<<utf32ToUtf8(value.data());
        cout<< " - len: "<<metadata.len
            << " - Pos: " << metadata.pos<<endl;
    }
    metadataFileA.close();
    recordsFileA.close();

    cout<<"\n";
}