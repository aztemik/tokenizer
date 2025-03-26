
#include <exception>
#include <iostream>
#include <string>
#include <sys/resource.h>
#include <sys/types.h>

#include "headers/BinaryFileHandler.h"
#include "headers/DataBaseManager.h"
#include "headers/paths.h"
#include "headers/tokenizer.h"
#include "headers/converts.h"
// #include "headers/memoryMonitor.h"
#include "headers/build_backups.h"

using namespace std;

void createAndBackup_Files_A(
    vector<u32string> onlyPairs
){

    try{
        // CREATE FILES A
        BuildBackups::hacerCopiaSeguridad(pathRecordsFileA, all_pares);
        BuildBackups::hacerCopiaSeguridad(pathMetadataFileA, all_pares_metadata);
        cout<<"BACKUP FILES A - EXITOSAMENTE"<<endl;
        BinaryFileAHandler::createFiles_A(onlyPairs);
        cout<<"CREATE FILES A - EXITOSAMENTE"<<"\n"<<endl;
    } catch (const exception e){
        cerr<<"Error: "<<e.what()<<endl;
    }

};

void createAndBackup(
    vector<pair<DataBaseManager::KeyType, DataBaseManager::countOperationFileB>> allDataFilesA
){

    try{
        // CREATE FILES B
        BuildBackups::hacerCopiaSeguridad(pathRecordsFileB, frequency_valhalla);
        BuildBackups::hacerCopiaSeguridad(pathMetadataFileB, frequency_valhalla_metadata);
        cout<<"BACKUP FILES B - EXITOSAMENTE"<<endl;
        BinaryFileBHandler::createFiles_B(allDataFilesA);
        cout<<"CREATE FILES B - EXITOSAMENTE"<<"\n"<<endl;
        // CREATE FILES C
        BuildBackups::hacerCopiaSeguridad(pathRecordsFileC, vocabulary);
        BuildBackups::hacerCopiaSeguridad(pathMetadataFileC, vocabulary_metadata);
        cout<<"BACKUP FILES C - EXITOSAMENTE"<<endl;
        BinaryFileCHandler::createFiles_C(allDataFilesA);
        cout<<"CREATE FILES C - EXITOSAMENTE"<<"\n"<<endl;
    } catch (const exception e){
        cerr<<"Error: "<<e.what()<<endl;
    }

}

vector<u32string> getPairs (const string& path){

    int amountPrompts = tokenizerHandler::how_many_prompts(path);
    cout<<"\nPATH TO PROMTS: "<< path<<""<<endl;
    cout<<"AMOUNT OF PROMPTS: "<<amountPrompts<<endl<<"\n";

    vector<u32string> onlyPairs;
    for (int i=0; i<amountPrompts; i++){
                
        string promptUTF8 = tokenizerHandler::getPromptUtf8(i, path);

        u32string promptUTF32 = utf8_to_utf32(promptUTF8);
        
        vector<u32string> tokens = tokenizerHandler::generateTokens(promptUTF32);

        // no sobreescribir los datos en el vector
        vector<u32string> newPairs = tokenizerHandler::buildPairsWithReplacement(tokens);
        onlyPairs.insert(onlyPairs.end(), newPairs.begin(), newPairs.end()); // Agregar al final

        // --DEBUG
        // cout<<"DEBUG 1"<<endl;
        // cout<<"Numero de iteracion: "<<i<<endl;
        // cout<<"Prompt UTF-8: "<<promptUTF8<<endl;
        // param_u32string_see(promptUTF32);

    }

    return onlyPairs;
};

void repeatProccess (){
    int seguimiento;

    do{

        cout<<"1. Read Two Binary Files C\n"
        <<"2. Read Two Binary Files A\n"
        <<"3. Read Two Binary Files B\n"
        <<"4. Salir"<<endl;
        cin>>seguimiento;

        if (seguimiento == 1){
            BinaryFileCHandler::readTwoBinaryFles_C();
        }else if(seguimiento == 2){
            BinaryFileAHandler::readTwoBinaryFiles_A();
        } else if(seguimiento == 3){
            BinaryFileBHandler::readTwoBinaryFiles_B();
        } else if (seguimiento == 4){
            seguimiento = -1;
        }else{
            cout<<"Elige una opcion valida"<<endl;
        }

    } while (seguimiento > 0);

    

};

int main (){

    // Ejemplo: limitar a 6GB (dejando 4GB para el sistema)
    // const size_t SIX_GB = 6ULL * 1024 * 1024 * 1024;
        
    // if (!MemoryMonitor::setMemoryLimit(SIX_GB)) {
    //     std::cerr << "No se pudo establecer el límite de memoria" << std::endl;
    //     return 1;
    // } else{
    //     cout<<"Se establecio el limite de memoria a: "<<SIX_GB<<endl;
    // }

    int opcion;
    cout<<"Opcion: "<<endl;
    cout<<"1. CREATE MERGE RULES\n"
        <<"2. List vocabulary\n"
        <<"3. List Binary/Metadata B\n"
        <<"4. List Binary/Metadata A\n"
        <<"5. Tokenizar text\n"
        <<"6. Insert especial tokens"<<endl;
    cin>>opcion; 

    if (opcion == 1){
        int pathToTraining;
        cout<<"1. schedule\n"
            <<"2. planning\n"
            <<"3. fee\n"
            <<"4. special Tokens"
            <<endl;
        cin>>pathToTraining;
        
        switch (pathToTraining) {
            case 1: {
                cout<<"\n === EMPEZANDO OPERACIONES SCHEDULE=== "<<"\n";
                vector<u32string> onlyPairs = getPairs(training_schedule);

                createAndBackup_Files_A(onlyPairs);

                vector<pair<DataBaseManager::KeyType, DataBaseManager::countOperationFileB>> allDataFilesA;

                try {
                    allDataFilesA =  BinaryFileBHandler::loadTwoFiles_A();
                    cout<<"TWO FILES A LOADED INTO RAM\n"<<endl;
                } catch (const exception e) {
                    cerr<<"Error: "<<e.what()<<endl;
                }

                createAndBackup(allDataFilesA);
                break;
            }
            
            case 2: {
                
                break;
            };

            case 3: {
                
                break;
            };        
        }

    } else if (opcion == 2){
        cout<<"\n";
        BinaryFileCHandler::readTwoBinaryFles_C();
    } else if (opcion == 3){
        cout<<"\n\n";
        BinaryFileBHandler::readTwoBinaryFiles_B();
    } else if (opcion == 4){
        BinaryFileAHandler::readTwoBinaryFiles_A();
    } else if (opcion == 5){
        string texto = "Transporte público cercano que me lleve a la estación de tren Buenavista";

        vector<uint64_t> texto_tokenizado = tokenizerHandler::tokenizer(texto);

    } else if(opcion == 6){
        SpecialTokens::insertSpecialTokensInFileC();
    }

    return 0;
}

