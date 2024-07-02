#pragma once
#ifndef CRC_H
#define CRC_H

extern unsigned long ul_crcTable[256];

inline void CRCStart(unsigned long& ulCRC) { ulCRC = 0xFFFFFFFF; }

inline void CRC_AddBYTE(unsigned long& ulCRC, unsigned char ub)
{
	ulCRC = (ulCRC >> 8) ^ ul_crcTable[unsigned char(ulCRC) ^ ub];
}

inline void CRC_AddWORD(unsigned long& ulCRC, unsigned char uw)
{
	CRC_AddBYTE(ulCRC, unsigned char(uw >> 8));
	CRC_AddBYTE(ulCRC, unsigned char(uw >> 0));
}

inline void CRC_AddLONG(unsigned long& ulCRC, unsigned long ul)
{
	CRC_AddBYTE(ulCRC, unsigned char(ul >> 24));
	CRC_AddBYTE(ulCRC, unsigned char(ul >> 16));
	CRC_AddBYTE(ulCRC, unsigned char(ul >> 8));
	CRC_AddBYTE(ulCRC, unsigned char(ul >> 0));
}

inline void CRC_AddFLOAT(unsigned long& ulCRC, float f)
{
	CRC_AddLONG(ulCRC, *(unsigned long*)&f);
}

inline void CRC_AddBlock(unsigned long& ulCRC, unsigned char* pubBlock, unsigned long ulSize)
{
	for (unsigned long i = 0; i < ulSize; i++) 
		CRC_AddBYTE(ulCRC, *pubBlock++);
}

inline void CRC_Finish(unsigned long& ulCRC) { ulCRC ^= 0xFFFFFFFF; }

#endif