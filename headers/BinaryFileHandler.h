#ifndef BINARYFILEHANDLER_H
#define BINARYFILEHANDLER_H

#include <cstdint>
#include <string>
#include "DataBaseManager.h"
#include <unordered_map>
#include <vector>



class BinaryFileAHandler {

    public:
    static int createFiles_A (std::vector<std::u32string> onlyPares);

    static int insertRecordsToFileA(
        const std::u32string value, 
        const std::string& recordsFile,
        const std::string& metadataFile
    );

    static void readTwoBinaryFiles_A ();
};


class BinaryFileBHandler {

    public:
    static std::vector<std::pair<DataBaseManager::KeyType, DataBaseManager::countOperationFileB>> loadTwoFiles_A();

    static void mostrarVector(
        std::vector<std::pair<DataBaseManager::KeyType, DataBaseManager::countOperationFileB>> VectorOrdenadoYAllData
    );

    static void createFiles_B (
        const std::vector<std::pair<DataBaseManager::KeyType, DataBaseManager::countOperationFileB>>&  allDataForFiles_B
    );

    static void readTwoBinaryFiles_B ();

};

class BinaryFileCHandler{

    public: 
    static void createFiles_C(std::vector<std::pair<DataBaseManager::KeyType, DataBaseManager::countOperationFileB>> allData);

    static void readTwoBinaryFles_C();


};

class SpecialTokens{

    public:

    static std::string getPromptUtf8(const int& posicion, const std::string& path);

    static int how_many_prompts(const std::string& path);

    static void insertInFileC(
        std::u32string value
    );

    static int insertSpecialTokensInFileC();

};



#endif /* end of BINARYFILEHANDLER_H */