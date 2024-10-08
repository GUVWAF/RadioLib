#if !defined(_RADIOLIB_FEC_H)
#define _RADIOLIB_FEC_H

#include "../TypeDef.h"
#include "../Module.h"
#if defined(RADIOLIB_BUILD_ARDUINO)
#include "../ArduinoHal.h"
#endif

// BCH(31, 21) code constants
#define RADIOLIB_PAGER_BCH_N                                    (31)
#define RADIOLIB_PAGER_BCH_K                                    (21)
#define RADIOLIB_PAGER_BCH_PRIMITIVE_POLY                       (0x25)

#if RADIOLIB_STATIC_ONLY
#define RADIOLIB_BCH_MAX_N                                      (63)
#define RADIOLIB_BCH_MAX_K                                      (31)
#endif

/*!
  \class RadioLibBCH
  \brief Class to calculate Bose–Chaudhuri–Hocquenghem (BCH) class of forward error correction codes.
  In theory, this should be able to calculate an arbitrary BCH(N, K) code,
  but so far it was only tested for BCH(31, 21).
*/
class RadioLibBCH {
  public:
    /*!
      \brief Default constructor.
    */
    RadioLibBCH();

    /*!
      \brief Default detructor.
    */
    ~RadioLibBCH();

    /*!
      \brief Initialization method.
      \param n Code word length in bits, up to 32.
      \param k Data portion length in bits, up to "n".
      \param poly Powers of the irreducible polynomial.
    */
    void begin(uint8_t n, uint8_t k, uint32_t poly);

    /*!
      \brief Encoding method - encodes one data word (without check bits) into a code word (with check bits).
      \param dataword Data word without check bits. The caller is responsible to make sure the data is
      on the correct bit positions!
      \returns Code word with error check bits.
    */
    uint32_t encode(uint32_t dataword);

  private:
    uint8_t n = 0;
    uint8_t k = 0;
    uint32_t poly = 0;
    uint8_t m = 0;
    
    #if RADIOLIB_STATIC_ONLY
      int32_t alphaTo[RADIOLIB_BCH_MAX_N + 1] = { 0 };
      int32_t indexOf[RADIOLIB_BCH_MAX_N + 1] = { 0 };
      int32_t generator[RADIOLIB_BCH_MAX_N - RADIOLIB_BCH_MAX_K + 1] = { 0 };
    #else
      int32_t* alphaTo = nullptr;
      int32_t* indexOf = nullptr;
      int32_t* generator = nullptr;
    #endif
};

// the global singleton
extern RadioLibBCH RadioLibBCHInstance;

// macros to access bits in byte array, from http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
#define SET_BIT_IN_ARRAY_MSB(A, k)                              ( A[((k)/8)] |= (1 << ((k)%8)) )
#define CLEAR_BIT_IN_ARRAY_MSB(A, k)                            ( A[((k)/8)] &= ~(1 << ((k)%8)) )
#define TEST_BIT_IN_ARRAY_MSB(A, k)                             ( A[((k)/8)] & (1 << ((k)%8)) )
#define GET_BIT_IN_ARRAY_MSB(A, k)                              ( (A[((k)/8)] & (1 << ((k)%8))) ? 1 : 0 )


#endif
