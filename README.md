# VigenereCipher
Breaking the Advanced Vigenere Cipher

Step 1) Reading the File: The file was read charater by character with spaces, punctuation and numbers. 
Step 2) Processing the File: The file read is then processed to bring everything to lowercase, remove punctuations, numbers, and spaces. As these are conserved in the cipher text, they won’t be affected by the encryption process. Another Vector is used to store the processed file information. 
Step 3) Finding Key Length: 
To find the key length, the concept of Index of coincidence is used. The cipher text is taken and is tested with all possibilities of key lengths. In a Vigenere Cipher, the key length starts from 2 and goes up to the ciphertext size/2 (to make sure you have enough repetition of the key).
For each key length, there will be key length number of Caesar cipher sequences making up the cipher text as Vigenere cipher is Caesar cipher in sequence. 
So for each key length, 
•	first the different Caesar sequences will be found.
•	IC of each sequence will be calculates
•	To calculate the IC of a key, the average of the sequences(associated with that key) IC will be taken.
•	IC is calculated using its formula.
Step 4) Finding the Key
Once the key length is known, the processed cipher text is examined by dividing it into Caesar cipher sequences. (if keylength = 8 , number of Caesar cipher sequences will be 80). In a caesar cipher, the frequency properties of alphabets are maintained, so this can be exploited by using chi statistical analysis. 
For each Caesar sequence,
•	For each shift from 0 to 25
o	Chi statistic is calculated and stored
•	The minimum chi square value is likely to be the key shift used in that cipher 
•	As a start, The cipher text could be decrypted using the shifts found to have minimum chi value.
Step 5) Decryptinh the key
When the key is known, The key would keep repeating until the cipher text finishes. Every character of the unprocessed cipher text is examined. If the char is a punctuation, space, or a number, it is not encrypted and conserved. Its pushed back to the plain text as it is. If it is a an alphabet, it is decrypted using the key at that position.
Plaintext = ciphertext – key; 
Step 6) User Intervention:
This part of the program allows the user to look at the decrypted text found by the initial key and make any changes necessary. Instead of going with an automated dictionary approach to crack the cipher, I went with the human intervention through the use of past knowledge and experience approach. This basically involves asking the user if the key is okay by showing the key and the decrypted text using that key, if the user confirms, then the program ends that, and the clock stops. 
However, if the user chooses that the key is not right based on their perception, they should be able to reevaluate the key by providing alternatives for the current plain text present. For example:  decryption of ciphertext process the word aisance which the user from his judgment and knowledge should be able to tell that it makes more sense for it to be absence. So plaintext would change, bringing about changes in the key accordingly.
The user keeps doing that until the ciphertext is correct and readable. 
