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

