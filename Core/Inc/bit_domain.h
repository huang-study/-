#ifndef __BIT_DOMAIN_H
#define __BIT_DOMAIN_H

#include "stdint.h"



union BitDomain_64
{
	uint64_t word64;
	uint32_t word32[2];
	uint16_t word16[4];
	uint8_t word8[8];
	struct 
	{
		uint64_t bit0:1;
		uint64_t bit1:1;
		uint64_t bit2:1;
		uint64_t bit3:1;
		uint64_t bit4:1;
		uint64_t bit5:1;
		uint64_t bit6:1;
		uint64_t bit7:1;
		uint64_t bit8:1;
		uint64_t bit9:1;
		uint64_t bit10:1;
		uint64_t bit11:1;
		uint64_t bit12:1;
		uint64_t bit13:1;
		uint64_t bit14:1;
		uint64_t bit15:1;
		uint64_t bit16:1;
		uint64_t bit17:1;
		uint64_t bit18:1;
		uint64_t bit19:1;
		uint64_t bit20:1;
		uint64_t bit21:1;
		uint64_t bit22:1;
		uint64_t bit23:1;
		uint64_t bit24:1;
		uint64_t bit25:1;
		uint64_t bit26:1;
		uint64_t bit27:1;
		uint64_t bit28:1;
		uint64_t bit29:1;
		uint64_t bit30:1;
		uint64_t bit31:1;
		uint64_t bit32:1;
		uint64_t bit33:1;
		uint64_t bit34:1;
		uint64_t bit35:1;
		uint64_t bit36:1;
		uint64_t bit37:1;
		uint64_t bit38:1;
		uint64_t bit39:1;
		uint64_t bit40:1;
		uint64_t bit41:1;
		uint64_t bit42:1;
		uint64_t bit43:1;
		uint64_t bit44:1;
		uint64_t bit45:1;
		uint64_t bit46:1;
		uint64_t bit47:1;
		uint64_t bit48:1;
		uint64_t bit49:1;
		uint64_t bit50:1;
		uint64_t bit51:1;
		uint64_t bit52:1;
		uint64_t bit53:1;
		uint64_t bit54:1;
		uint64_t bit55:1;
		uint64_t bit56:1;
		uint64_t bit57:1;
		uint64_t bit58:1;
		uint64_t bit59:1;
		uint64_t bit60:1;
		uint64_t bit61:1;
		uint64_t bit62:1;
		uint64_t bit63:1;
	}bit;
};

#endif /* __BIT_DOMAIN_H */
