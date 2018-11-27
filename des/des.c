// static unsigned char secrets[3][8];
// static unsigned char source[8];
// static unsigned char tmp[8];
// void des()
// {
// 	secrets[0][0] = 0x61;
// 	secrets[0][1] = 0x62;
// 	secrets[0][2] = 0x63;
// 	secrets[0][3] = 0x64;


// 	secrets[1][0] = 0x62;
// 	secrets[1][1] = 0x63;
// 	secrets[1][2] = 0x64;
// 	secrets[1][3] = 0x65;


// 	secrets[2][0] = 0x63;
// 	secrets[2][1] = 0x64;
// 	secrets[2][2] = 0x65;
// 	secrets[2][3] = 0x66;


// 	source[0] = 0x6d; 
// 	source[1] = 0x69; 
// 	source[2] = 0x64; 
// 	source[3] = 0x6e; 
// 	source[4] = 0x69; 
// 	source[5] = 0x67; 
// 	source[6] = 0x68; 
// 	source[7] = 0x74;

// 	int i = 0;
// 	int j = 0;

// 	/*  */
// 	printf("original text:\n");
// 	for(i = 0; i < 8 ; i++)
// 	{
// 		printf("%c", source[i]);
// 	}
// 	printf("\n\n\n");

// 	/*  */
// 	i = 0;
// 	while(1)
// 	{
// 		for(j = 0; j < 8; j++)
// 		{
// 			tmp[j] = source[j];
// 		}

// 		/* xor */
// 		for(j = 0; j < 4; j++)
// 		{
// 			tmp[j] ^= secrets[i][j];
// 		}

// 		if(i < 2)
// 		{
// 			/* swap */
// 			for(j = 0; j < 8; j++)
// 			{
// 				if(j < 4)
// 				{
// 					source[j] = tmp[4 + j];
// 				}
// 				else
// 				{
// 					source[j] = tmp[j - 4];
// 				}
// 			}

// 			i++;
// 		}
// 		else
// 		{
// 			for(j = 0; j < 8; j++)
// 			{
// 				source[j] = tmp[j];
// 			}
			
// 			/*  */
// 			printf("*****************************************\nround %d, decrypted text:\n", i);
// 			for(j = 0; j < 8 ; j++)
// 			{
// 				printf("%x ", source[j]);
// 			}
// 			printf("\n*****************************************\n\n\n");

// 			/*  */
// 			break;
// 		}

// 		/*  */
// 		printf("round %d, decrypted text:\n", i);
// 		for(j = 0; j < 8 ; j++)
// 		{
// 			printf("%x ", source[j]);
// 		}
// 		printf("\n\n\n");
// 	}

// 	/*  */
// 	i = 0;
// 	while(1)
// 	{
// 		for(j = 0; j < 8; j++)
// 		{
// 			tmp[j] = source[j];
// 		}

// 		/* xor */
// 		for(j = 0; j < 4; j++)
// 		{
// 			tmp[j] ^= secrets[2 - i][j];
// 		}

// 		if(i < 2)
// 		{
// 			/* swap */
// 			for(j = 0; j < 8; j++)
// 			{
// 				if(j < 4)
// 				{
// 					source[j] = tmp[4 + j];
// 				}
// 				else
// 				{
// 					source[j] = tmp[j - 4];
// 				}
// 			}

// 			i++;
// 		}
// 		else
// 		{
// 			for(j = 0; j < 8; j++)
// 			{
// 				source[j] = tmp[j];
// 			}

// 			/*  */
// 			printf("*****************************************\nround %d, decrypted text:\n", i);
// 			for(j = 0; j < 8 ; j++)
// 			{
// 				printf("%c", source[j]);
// 			}
// 			printf("\n*****************************************\n\n\n");

// 			/*  */
// 			break;
// 		}

// 		/*  */
// 		printf("round %d, decrypted text:\n", i);
// 		for(j = 0; j < 8 ; j++)
// 		{
// 			printf("%x ", source[j]);
// 		}
// 		printf("\n\n\n");
// 	}
// }



// // original text:
// // midnight


// // round 1, decrypted text:
// // 69 67 68 74 c b 7 a 


// // round 2, decrypted text:
// // c b 7 a b 4 c 11 


// // *****************************************
// // round 2, decrypted text:
// // 6f 6f 62 6c b 4 c 11 
// // *****************************************


// // round 1, decrypted text:
// // b 4 c 11 c b 7 a 


// // round 2, decrypted text:
// // c b 7 a 69 67 68 74