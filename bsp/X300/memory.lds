/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

flash = 0x20400000; /* 512K */
itim  = 0x08000000; /*  16K */
dtim  = 0x80000000; /*  64K */

sh_buff1_start = dtim + 0xA800; sh_buff1_end  = sh_buff1_start + 4K;
sh_buff2_start = dtim + 0xB800; sh_buff2_end  = sh_buff2_start + 8K;
