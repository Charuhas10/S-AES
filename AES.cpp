#include <array>
#include <iostream>
#include <stdint.h>

constexpr uint16_t sBox[16] = {                         //Constant expression uint16_t is unsigned int 16 bit type
    0x9, 0x4, 0xA, 0xB, 0xD, 0x1, 0x8, 0x5,
    0x6, 0x2, 0x0, 0x3, 0xC, 0xE, 0xF, 0x7,
};
constexpr uint16_t sBoxI[16] = {
    0xA, 0x5, 0x9, 0xB, 0x1, 0x7, 0x8, 0xF,
    0x6, 0x0, 0x2, 0x3, 0xC, 0x4, 0xD, 0xE,
};

using state_t = std::array<uint16_t, 4>;        //state_t is an array of 4 uint16_t(unsigned int 16 bit type)

struct simplifiedAES {
  state_t preRoundKey;
  state_t round1Key;
  state_t round2Key;

  uint16_t rotWord(uint16_t word) {
    return ((word & 0xF) << 4) + ((word & 0xF0) >> 4);              //flipping first and second nibble(4bit)
  }

  uint16_t subWord(uint16_t word) {
    return (sBox[word >> 4] << 4) + sBox[word & 0xF];               //substituting first and second nibble(4bit)
  }

  state_t intToState(uint16_t n) {
    return {
        static_cast<uint16_t>(n >> 12 & 0xF),                    //static_cast is used to convert one data type to another
        static_cast<uint16_t>((n >> 4) & 0xF),
        static_cast<uint16_t>((n >> 8) & 0xF),
        static_cast<uint16_t>(n & 0xF),
    };
  }

  std::array<state_t, 3> keyExpansion(uint16_t key) {           
    uint16_t rCon1 = 0x80;
    uint16_t rCon2 = 0x30;

    uint16_t w[6];
    w[0] = (key & 0xFF00) >> 8;                                 //w[0] is the first 8 bits of the key
    w[1] = key & 0x00FF;                                        //w[1] is the last 8 bits of the key
    w[2] = w[0] ^ (subWord(rotWord(w[1])) ^ rCon1);             //w[2] is the first 8 bits of the key after the first round of key expansion
    w[3] = w[2] ^ w[1];                                         //w[3] is the last 8 bits of the key after the first round of key expansion
    w[4] = w[2] ^ (subWord(rotWord(w[3])) ^ rCon2);             //w[4] is the first 8 bits of the key after the second round of key expansion
    w[5] = w[4] ^ w[3];                                         //w[5] is the last 8 bits of the key after the second round of key expansion

    return {
        intToState((w[0] << 8) + w[1]),
        intToState((w[2] << 8) + w[3]),
        intToState((w[4] << 8) + w[5]),
    };
  }

  state_t addRoundKey(state_t key1, state_t key2) {
    return {
        static_cast<uint16_t>(key1[0] ^ key2[0]),
        static_cast<uint16_t>(key1[1] ^ key2[1]),
        static_cast<uint16_t>(key1[2] ^ key2[2]),
        static_cast<uint16_t>(key1[3] ^ key2[3]),
    };
  }

  uint16_t gfMult(uint16_t a, uint16_t b) {             //gfMult is the function for the Galois Field multiplication
    uint16_t product = 0;

    a &= 0xF;
    b &= 0xF;

    for (int i = 0; i < 4; i++) {
      if (b & 1) {
        product ^= a;
      }

      a <<= 1;
      if (a & (1 << 4)) {
        a ^= 0b10011;
      }

      b >>= 1;
    }

    return product;
  }

  state_t mixColumns(state_t state) {
    return {
        static_cast<uint16_t>(state[0] ^ gfMult(4, state[2])),
        static_cast<uint16_t>(state[1] ^ gfMult(4, state[3])),
        static_cast<uint16_t>(state[2] ^ gfMult(4, state[0])),
        static_cast<uint16_t>(state[3] ^ gfMult(4, state[1])),
    };
  }

  state_t shiftRows(state_t state) {
    return {
        state[0],
        state[1],
        state[3],
        state[2],
    };
  }

  state_t subNibbles(const uint16_t sBox[16], state_t state) {
    return {
        sBox[state[0]],
        sBox[state[1]],
        sBox[state[2]],
        sBox[state[3]],
    };
  }

  uint16_t stateToInt(state_t state) {
    return (state[0] << 12) + (state[2] << 8) + (state[1] << 4) + state[3];
  }

  state_t inverseMixColumns(state_t state) {
    return {
        static_cast<uint16_t>(gfMult(9, state[0]) ^ gfMult(2, state[2])),
        static_cast<uint16_t>(gfMult(9, state[1]) ^ gfMult(2, state[3])),
        static_cast<uint16_t>(gfMult(9, state[2]) ^ gfMult(2, state[0])),
        static_cast<uint16_t>(gfMult(9, state[3]) ^ gfMult(2, state[1])),
    };
  }

  uint16_t encrypt(uint16_t plaintext) {
    auto state = addRoundKey(preRoundKey, intToState(plaintext));       
    state = mixColumns(shiftRows(subNibbles(sBox, state)));             
    state = addRoundKey(round1Key, state);
    state = shiftRows(subNibbles(sBox, state));
    state = addRoundKey(round2Key, state);
    return stateToInt(state);
  }

  uint16_t decrypt(uint16_t ciphertext) {
    auto state = addRoundKey(round2Key, intToState(ciphertext));
    state = subNibbles(sBoxI, state);
    state = inverseMixColumns(addRoundKey(round1Key, state));
    state = subNibbles(sBoxI, shiftRows(state));
    state = addRoundKey(preRoundKey, state);
    return stateToInt(state);
  }
};

int main(void) {
  uint16_t key = 0b0011001001000010;
  uint16_t plaintext = 0b1100110011010000;

  simplifiedAES aes;
  auto keys = aes.keyExpansion(key);

  for (int i = 0; i < 4; i++) {
    aes.preRoundKey[i] = keys[0][i];
    aes.round1Key[i] = keys[1][i];
    aes.round2Key[i] = keys[2][i];
  }

  std::cout << "Input Plaintext: " << plaintext << std::endl;
  auto ciphertext = aes.encrypt(plaintext);
  std::cout << "Ciphertext: " << ciphertext << std::endl;
  auto decrypted = aes.decrypt(ciphertext);
  std::cout << "Plaintext:  " << decrypted << std::endl;

  return 0;
}
