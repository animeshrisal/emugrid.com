.global _boot
.text

_boot:                    
    addi x1 , x0,   2000      /* 7d000093 */
    addi x2 , x1,   2000      /* 7d008113 */ 
    addi x3, x0, -500         /* e0c00193 */
    add x5, x1, x2            /* 002082b3 */

