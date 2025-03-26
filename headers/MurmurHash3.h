#ifndef MurmurHash3_H
#define MurmurHash3_H

#include <cstddef>
#include <cstdint>


/* El macro BIG_CONSTANT asegura que los valores hexadecimales grandes 
    se conviertan en literales de tipo unsigned long long 
    antes de asignarlos a c1 y c2. */
#define BIG_CONSTANT(x) (x##LLU)

// Define FORCE_INLINE como una macro para forzar que una función se convierta en "inline".
// inline: Sugiere que el compilador inserte el código de la función directamente donde se llama,
//          en lugar de realizar una llamada a función tradicional.
// __attribute__((always_inline)): Instrucción específica del compilador GCC/Clang que obliga al compilador
//                                 a inlined incluso si normalmente no sería elegible (por ejemplo, funciones grandes).
#define	FORCE_INLINE inline __attribute__((always_inline))


/* Fuerza al compilador a inlined esta función para 
    mejorar el rendimiento en algoritmos 
    de procesamiento intensivo. */
FORCE_INLINE uint64_t getblock64 ( const uint64_t * p, int i )
{
    /* Retorna el bloque de 64 bits ubicado 
        en la posición i del arreglo apuntado por p. */
  return p[i];
}

// Realiza una rotación a la izquierda de un valor de 64 bits (x) por r posiciones. 
inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
    // Desplaza x a la izquierda por r posiciones y llena los bits vacíos
    // con los bits superiores de x (desplazados a la derecha).
    return (x << r) | (x >> (64 - r));
}

// Define ROTL64 como un alias para la función rotl64, facilitando su uso.
#define ROTL64(x,y)	rotl64(x,y)


// Mezcla final de 64 bits para dispersar los bits uniformemente.
FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
    // Realiza una operación XOR de los bits superiores con los inferiores.
    k ^= k >> 33;

    // Multiplica por una constante grande para mezclar más los bits.
    k *= BIG_CONSTANT(0xff51afd7ed558ccd);

    // Aplica otra operación XOR para mezclar los bits superiores con los inferiores
    k ^= k >> 33;

    // Multiplica por otra constante grande para continuar mezclando.
    k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);

    // Realiza un último XOR para dispersar los bits uniformemente.
    k ^= k >> 33;

    // Retorna el valor mezclado.
    return k;
}


inline uint64_t MurmurHash3_x64_128 ( 

  // Dato que se desea hashear un arreglo de bytes, un string o cualquier bloque de memoria.
  const void * key, 

  // Es la longitud (en bytes) del dato en key. 
  const size_t len, 

  /* Es la semilla inicial para el hash. 
  Cambiar esta semilla produce diferentes valores de hash para la misma entrada. */
  const uint32_t seed

){

    // Convertimos la clave (key) en un puntero a bytes (uint8_t*)
    const uint8_t * data = (const uint8_t*)key;

    // Calcula cuántos bloques de 16 bytes hay en len.
    const size_t nblocks = len / 16;
    // cout<<"nblock: "<<nblocks<<endl;

    // Inicializamos los valores del hash con la semilla proporcionada
    uint64_t h1 = seed;
    uint64_t h2 = seed;

    // Son constantes predefinidas (valores mágicos) que se utilizan para mezclar los datos de manera uniforme.
    const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);


    //---------- Procesar el cuerpo principal ----------

    // Convertir el puntero a bloques de 64 bits
    const uint64_t * blocks = (const uint64_t *)(data);

    for(int i = 0; i < nblocks; i++){
        
        // Lee dos bloques de 64 bits consecutivos
        uint64_t k1 = getblock64(blocks,i*2+0);
        uint64_t k2 = getblock64(blocks,i*2+1);

        // Mezcla el primer bloque (k1)
        k1 *= c1;                        // Multiplicación con una constante
        k1  = ROTL64(k1,31);        // Rotación hacia la izquierda 31 bits
        k1 *= c2;                        // Multiplicación con otra constante
        h1 ^= k1;                        // XOR con el valor del hash h1

        // Mezcla h1 con h2 y agrega entropía
        h1 = ROTL64(h1,27);        // Rotación hacia la izquierda 27 bits
        h1 += h2;                       // Suma h2
        h1 = h1*5+0x52dce729;           // Multiplicación y suma de constante

        // Mezcla el segundo bloque (k2)
        k2 *= c2;                     // Multiplicación con una constante
        k2  = ROTL64(k2,33);     // Rotación hacia la izquierda 33 bits
        k2 *= c1;                     // Multiplicación con otra constante
        h2 ^= k2;                     // XOR con el valor del hash h1

        // Mezcla h2 con h1 y agrega entropía
        h2 = ROTL64(h2,31);     // Rotación hacia la izquierda 31 bits
        h2 += h1;                    // Suma h1
        h2 = h2*5+0x38495ab5;        // Multiplicación y suma de constante
    }

    //----------
    // tail

    // Procesamiento del resto (bloques menores de 16 bytes)

    // Manejar los bytes que no completan un bloque de 16
    const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    // Procesa el resto de los bytes (switch optimizado para casos pequeños)
    switch(len & 15){

        case 15: k2 ^= ((uint64_t)tail[14]) << 48;    // Procesa el byte 14
        case 14: k2 ^= ((uint64_t)tail[13]) << 40;    // Procesa el byte 13
        case 13: k2 ^= ((uint64_t)tail[12]) << 32;    // ...
        case 12: k2 ^= ((uint64_t)tail[11]) << 24;
        case 11: k2 ^= ((uint64_t)tail[10]) << 16;
        case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
        case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;     // Procesa el byte 8

    // Mezcla k2
        k2 *= c2; 
        k2  = ROTL64(k2,33); 
        k2 *= c1; 
        h2 ^= k2;    

        case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;    // Procesa el byte 7
        case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
        case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
        case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
        case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
        case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
        case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
        case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;     // Procesa el byte 0

        // Mezcla k1
        k1 *= c1; 
        k1  = ROTL64(k1,31); 
        k1 *= c2; 
        h1 ^= k1;

    };

    //----------
    // finalization

    // Mezcla final para garantizar buena dispersión de los valores

    h1 ^= len;    // XOR con la longitud total
    h2 ^= len;

    h1 += h2;     // Combina h1 y h2
    h2 += h1;

    h1 = fmix64(h1);    // Aplicación de una función de mezcla final
    h2 = fmix64(h2);

    h1 += h2;      // Combina nuevamente
    h2 += h1;

    // retornar directamente el XOR
    return h1 ^ h2;
}

#endif /* end of MurmurHash3_H */