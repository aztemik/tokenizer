#ifndef PHATS_H
#define PHATS_H

#include <string>

inline const std::string normalPath = "";

inline const std::string pathRecordsFileA = normalPath + "all_pares.bin";
inline const std::string pathMetadataFileA = normalPath + "all_pares_metadata.bin";

inline const std::string all_pares = "all_pares_";
inline const std::string all_pares_metadata = "all_pares_metadata_";

inline const std::string pathRecordsFileB = normalPath + "fequency_valhalla.bin";
inline const std::string pathMetadataFileB = normalPath + "frequency_metadata_valhalla.bin";

inline const std::string frequency_valhalla = "frequency_valhalla_";
inline const std::string frequency_valhalla_metadata = "frequency_valhalla_metadata_";

inline const std::string pathRecordsFileC = normalPath + "vocabulary.bin";
inline const std::string pathMetadataFileC = normalPath + "vocabulary_metadata.bin";

inline const std::string vocabulary = "vocabulary";
inline const std::string vocabulary_metadata = "vocabulary_metadata";



inline std::string saveAnyChange = ""; 

inline const std::string training_schedule = "";
inline const std::string training_journey = "";
inline const std::string training_fee = "";
inline const std::string training_planning = "";

inline const std::string specialTokens = "";

#endif /* end of PHATS_H */