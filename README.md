# S-AES
S-AES (Simplified AES) was developed by Professor Edward Schaefer of Santa Clara University and several of his students. It is an educational rather than a secure encryption algorithm. It has similar properties and structure to AES with much smaller parameters. It is intended to be used as a teaching tool for students to learn about AES and block ciphers in general. It is not intended to be used for any other purpose.

### Difference between S-AES and AES
S-AES is a simplified version of AES. It has a smaller key size and a smaller number of rounds. It is not secure and should not be used for any purpose other than educational purposes. It is not a replacement for AES. 

### S-AES Parameters : 
* Block Size: 64 bits
* Key Size: 64 bits
* Number of Rounds: 4
* Number of Subkeys: 4
* Number of S-Boxes: 4
* Number of S-Box Bits: 4
* Number of S-Box Inputs: 16
* Number of S-Box Outputs: 16

#### S-AES Algorithm
The encryption algorithm takes a 16-bit block of plaintext as input and a 16-bit key and produces a 16-bit block of ciphertext as output. The S-AES decryption algorithm takes a 16-bit block of ciphertext and the same 16-bit key used to produce that ciphertext as input and produces the original 16-bit block of plaintext as output. The encryption algorithm involves the use of four different functions, or transformations: add key, nibble substitution (NS), shift row (SR), and mix column (MC), whose operation is explained subsequently. We can concisely express the encryption algorithm as a composition of functions:
![image](https://user-images.githubusercontent.com/72398218/208376847-6a5eaa28-50f7-490f-89ca-dc0c3bc8d7fb.png)
Each function operates on a 16-bit state, treated as a matrix of nibbles, where one nibble is a 4-bit number. The initial state matrix is the 16-bit plaintext, State is modified by each subsequent function in the encryption process, producing after the last function the 16-bit ciphertext.

We now look at each of the four functions in turn.
#### Add Key
The add key function consists of the bitwise XOR of the state matrix with the 16-bit round key. The key is a 16-bit number, and the state matrix is a 16-bit number. The XOR operation is performed on each bit of the state matrix with the corresponding bit of the key. The result is the new state matrix.
The inverse of the add key function is the same as the add key function because the XOR operation is its own inverse.
![image](https://user-images.githubusercontent.com/72398218/208418854-0c0546b9-2b2b-4924-93a8-c3ddf7e59b4f.png)

#### Nibble Substitution
The nibble substituiton function is a simple table look-up. AES defines a 4x4 matrix of nibbble values, called an S-box, that contains a permutation of all possible 4-bit values. Each individual nibble of State is mapped into a new nibble in the following way: The leftmost 2 bits of the nibble are used as a row value, and the rightmost 2 bits are used as a common value. These row and column values serve as indexes into the S-box to select a unique 4-bit output value. For example, the hexadecimal value A refernces row 2, column 2 of the S-box, which contains the value 0. Accordingly, the value of A is mapped into the value 0. The inverse nibble substitution function makes use of the inverse S-box. Note, for example, that the input 0 produces the output A, and the input A to the S-box produces 0. Here is an example:
![image](https://user-images.githubusercontent.com/72398218/208585935-bb24140a-64bb-4a29-8fee-b90d122d69c0.png)

#### Shift Row
The Shift row function performs a one-nibble circular shift of the second row of State the first row is not altered. The inverse shift row function is identical to the shift row function, because it shifts the second row back to its original position. Here is an example:
![image](https://user-images.githubusercontent.com/72398218/208586434-9358e933-3a15-4228-be94-5594b8a565c3.png)

#### Mix Column
The mix column function operates on each column individually. Each nibble of a column is mapped into a new value that is a function of both nibbles in that column. The transformation can be defined by the following matrix multiplication on State
![image](https://user-images.githubusercontent.com/72398218/208586929-4d9906d4-cb51-41fb-ac5d-385472e3bb23.png)

#### Key Expansion
For key expansion, the 16 bits of the initial key are grouped into a row of two 8-bit words. Below figure shows the expansion into sex words, by the calculation of four new words from the initial two words. The algorithm is
![image](https://user-images.githubusercontent.com/72398218/208587160-1b9233a9-1c71-41c2-8825-ff2f00cbc2af.png)
Rcon is a round constant, defined as follows: RC[i] = xi+2, so that RC[1] = x3 = 1000 and RC[2] = x4mod(x4 + x + 1) = x + 1 = 0011. RC[i] forms the left-most nibble of a byte, with the right-most nibble being all zeroes. Thus, Rcon(1) = 10000000 and Rcon(2) = 00110000. For example, suppose the key is 2D55 = 0010 1101 0101 0101 = w0w1. Then 

w2 = 00101101 ⊕ 10000000 ⊕ SubNib(01010101)

= 00101101 ⊕ 10000000 ⊕ 00010001 = 10111100

w3 = 10111100 ⊕ 01010101 = 11101001

w4 = 10111100 ⊕ 00110000 ⊕ SubNib(10011110)

= 10111100 ⊕ 00110000 ⊕ 00101111 = 10100011

w5 = 10100011 ⊕ 11101001 = 01001010

![image](https://user-images.githubusercontent.com/72398218/208587289-9c85ec49-ae0c-4290-8b01-3e2e9ef6ab6a.png)

#### SAES Transformations
![image](https://user-images.githubusercontent.com/72398218/208587530-c0e785e8-c582-4e47-b354-194b622d9ef7.png)
