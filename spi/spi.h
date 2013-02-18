#ifndef _SPI_H_
#define _SPI_H_

void SPI_Init();
int SPI_Send_Word(unsigned int wlen, unsigned int txWord, unsigned int *rxWord);

#endif /* _SPI_H_ */
