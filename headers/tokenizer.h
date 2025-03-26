#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include "unordered_map"
#include "DataBaseManager.h"

class tokenizerHandler{
    public:

    static int how_many_prompts(const std::string& path);

    static std::string getPromptUtf8(const int& posicion, const std::string& path);

    static std::vector<std::u32string> generateTokens(const std::u32string& prompt);

    static std::vector<std::u32string> buildOnlyPares (std::vector<std::u32string> tokens);

    static std::pair<std::vector<DataBaseManager::parYFrecuencia>, std::vector<std::pair<std::u32string, std::u32string>>> buildParesYFrecuencia (
        std::vector<std::u32string> tokens
    );

    static std::unordered_map<std::u32string, int> loadVocabularyForTokenizer();

    static std::vector<std::u32string> buildPairsWithReplacement(
        const std::vector<std::u32string>& tokens
    );

    static std::vector<uint64_t> tokenizer(
        std::string texto
    
    );

    static std::pair<std::pair<std::u32string, std::u32string>, int> maxPar(
        std::vector<DataBaseManager::parYFrecuencia> paresYFrecuencia
    );

};


#endif /* end of TOKENIZER_H */