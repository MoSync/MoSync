# Copyright (C) 2010 MoSync AB
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to the Free
# Software Foundation, 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

LOCAL_PATH		:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := mosync
LOCAL_ARM_MODE  := arm
LOCAL_SRC_FILES := FileImpl.cpp \
				   MoSyncBridge.cpp \
				   AndroidHelpers.cpp \
				   SyscallImpl.cpp \
				   IOCtl.cpp \
				   ../../base/FileStream.cpp \
				   ../../base/MemStream.cpp \
				   ../../base/Stream.cpp \
				   ../../base/Image.cpp \
				   ../../base/Syscall.cpp \
				   ../../core/Core.cpp \
				   ../../core/disassembler.cpp \
				   ../../core/Recompiler/ArmAssembler.cpp \
				   ../../core/Recompiler/ArmRecompiler.cpp \
				   ../../../../intlibs/hashmap/hashmap.cpp \
				   ../../../../intlibs/helpers/platforms/linux/log.cpp \
				   libtommath/bn_error.c \
				   libtommath/bn_fast_mp_invmod.c \
				   libtommath/bn_fast_mp_montgomery_reduce.c \
				   libtommath/bn_fast_s_mp_mul_digs.c \
				   libtommath/bn_fast_s_mp_mul_high_digs.c \
				   libtommath/bn_fast_s_mp_sqr.c \
				   libtommath/bn_mp_2expt.c \
				   libtommath/bn_mp_abs.c \
				   libtommath/bn_mp_add.c \
				   libtommath/bn_mp_add_d.c \
				   libtommath/bn_mp_addmod.c \
				   libtommath/bn_mp_and.c \
				   libtommath/bn_mp_clamp.c \
				   libtommath/bn_mp_clear.c \
				   libtommath/bn_mp_clear_multi.c \
				   libtommath/bn_mp_cmp.c \
				   libtommath/bn_mp_cmp_d.c \
				   libtommath/bn_mp_cmp_mag.c \
				   libtommath/bn_mp_cnt_lsb.c \
				   libtommath/bn_mp_copy.c \
				   libtommath/bn_mp_count_bits.c \
				   libtommath/bn_mp_div.c \
				   libtommath/bn_mp_div_2.c \
				   libtommath/bn_mp_div_2d.c \
				   libtommath/bn_mp_div_3.c \
				   libtommath/bn_mp_div_d.c \
				   libtommath/bn_mp_dr_is_modulus.c \
				   libtommath/bn_mp_dr_reduce.c \
				   libtommath/bn_mp_dr_setup.c \
				   libtommath/bn_mp_exch.c \
				   libtommath/bn_mp_expt_d.c \
				   libtommath/bn_mp_exptmod.c \
				   libtommath/bn_mp_exptmod_fast.c \
				   libtommath/bn_mp_exteuclid.c \
				   libtommath/bn_mp_fread.c \
				   libtommath/bn_mp_fwrite.c \
				   libtommath/bn_mp_gcd.c \
				   libtommath/bn_mp_get_int.c \
				   libtommath/bn_mp_grow.c \
				   libtommath/bn_mp_init.c \
				   libtommath/bn_mp_init_copy.c \
				   libtommath/bn_mp_init_multi.c \
				   libtommath/bn_mp_init_set.c \
				   libtommath/bn_mp_init_set_int.c \
				   libtommath/bn_mp_init_size.c \
				   libtommath/bn_mp_invmod.c \
				   libtommath/bn_mp_invmod_slow.c \
				   libtommath/bn_mp_is_square.c \
				   libtommath/bn_mp_jacobi.c \
				   libtommath/bn_mp_karatsuba_mul.c \
				   libtommath/bn_mp_karatsuba_sqr.c \
				   libtommath/bn_mp_lcm.c \
				   libtommath/bn_mp_lshd.c \
				   libtommath/bn_mp_mod.c \
				   libtommath/bn_mp_mod_2d.c \
				   libtommath/bn_mp_mod_d.c \
				   libtommath/bn_mp_montgomery_calc_normalization.c \
				   libtommath/bn_mp_montgomery_reduce.c \
				   libtommath/bn_mp_montgomery_setup.c \
				   libtommath/bn_mp_mul.c \
				   libtommath/bn_mp_mul_2.c \
				   libtommath/bn_mp_mul_2d.c \
				   libtommath/bn_mp_mul_d.c \
				   libtommath/bn_mp_mulmod.c \
				   libtommath/bn_mp_n_root.c \
				   libtommath/bn_mp_neg.c \
				   libtommath/bn_mp_or.c \
				   libtommath/bn_mp_prime_fermat.c \
				   libtommath/bn_mp_prime_is_divisible.c \
				   libtommath/bn_mp_prime_is_prime.c \
				   libtommath/bn_mp_prime_miller_rabin.c \
				   libtommath/bn_mp_prime_next_prime.c \
				   libtommath/bn_mp_prime_rabin_miller_trials.c \
				   libtommath/bn_mp_prime_random_ex.c \
				   libtommath/bn_mp_radix_size.c \
				   libtommath/bn_mp_radix_smap.c \
				   libtommath/bn_mp_rand.c \
				   libtommath/bn_mp_read_radix.c \
				   libtommath/bn_mp_read_signed_bin.c \
				   libtommath/bn_mp_read_unsigned_bin.c \
				   libtommath/bn_mp_reduce.c \
				   libtommath/bn_mp_reduce_2k.c \
				   libtommath/bn_mp_reduce_2k_l.c \
				   libtommath/bn_mp_reduce_2k_setup.c \
				   libtommath/bn_mp_reduce_2k_setup_l.c \
				   libtommath/bn_mp_reduce_is_2k.c \
				   libtommath/bn_mp_reduce_is_2k_l.c \
				   libtommath/bn_mp_reduce_setup.c \
				   libtommath/bn_mp_rshd.c \
				   libtommath/bn_mp_set.c \
				   libtommath/bn_mp_set_int.c \
				   libtommath/bn_mp_shrink.c \
				   libtommath/bn_mp_signed_bin_size.c \
				   libtommath/bn_mp_sqr.c \
				   libtommath/bn_mp_sqrmod.c \
				   libtommath/bn_mp_sqrt.c \
				   libtommath/bn_mp_sub.c \
				   libtommath/bn_mp_sub_d.c \
				   libtommath/bn_mp_submod.c \
				   libtommath/bn_mp_to_signed_bin.c \
				   libtommath/bn_mp_to_signed_bin_n.c \
				   libtommath/bn_mp_to_unsigned_bin.c \
				   libtommath/bn_mp_to_unsigned_bin_n.c \
				   libtommath/bn_mp_toom_mul.c \
				   libtommath/bn_mp_toom_sqr.c \
				   libtommath/bn_mp_toradix.c \
				   libtommath/bn_mp_toradix_n.c \
				   libtommath/bn_mp_unsigned_bin_size.c \
				   libtommath/bn_mp_xor.c \
				   libtommath/bn_mp_zero.c \
				   libtommath/bn_prime_tab.c \
				   libtommath/bn_reverse.c \
				   libtommath/bn_s_mp_add.c \
				   libtommath/bn_s_mp_exptmod.c \
				   libtommath/bn_s_mp_mul_digs.c \
				   libtommath/bn_s_mp_mul_high_digs.c \
				   libtommath/bn_s_mp_sqr.c \
				   libtommath/bn_s_mp_sub.c \
				   libtommath/bncore.c \
				   libtomcrypt/src/ciphers/aes/aes.c \
				   libtomcrypt/src/misc/base64/base64_decode.c \
				   libtomcrypt/src/misc/base64/base64_encode.c \
				   libtomcrypt/src/modes/cbc/cbc_decrypt.c \
				   libtomcrypt/src/modes/cbc/cbc_done.c \
				   libtomcrypt/src/modes/cbc/cbc_encrypt.c \
				   libtomcrypt/src/modes/cbc/cbc_start.c \
				   libtomcrypt/src/misc/crypt/crypt_cipher_descriptor.c \
				   libtomcrypt/src/misc/crypt/crypt_cipher_is_valid.c \
				   libtomcrypt/src/misc/crypt/crypt_hash_descriptor.c \
				   libtomcrypt/src/misc/crypt/crypt_hash_is_valid.c \
				   libtomcrypt/src/misc/crypt/crypt_ltc_mp_descriptor.c \
				   libtomcrypt/src/misc/crypt/crypt_prng_descriptor.c \
				   libtomcrypt/src/misc/crypt/crypt_prng_is_valid.c \
				   libtomcrypt/src/misc/crypt/crypt_register_cipher.c \
				   libtomcrypt/src/misc/crypt/crypt_register_hash.c \
				   libtomcrypt/src/ciphers/des.c \
				   libtomcrypt/src/modes/ecb/ecb_decrypt.c \
				   libtomcrypt/src/modes/ecb/ecb_done.c \
				   libtomcrypt/src/modes/ecb/ecb_encrypt.c \
				   libtomcrypt/src/modes/ecb/ecb_start.c \
				   libtomcrypt/src/misc/error_to_string.c \
				   libtomcrypt/src/prngs/fortuna.c \
				   libtomcrypt/src/hashes/helper/hash_memory.c \
				   libtomcrypt/src/mac/hmac/hmac_done.c \
				   libtomcrypt/src/mac/hmac/hmac_init.c \
				   libtomcrypt/src/mac/hmac/hmac_process.c \
				   libtomcrypt/src/pk/ecc/ltc_ecc_map.c \
				   libtomcrypt/src/pk/ecc/ltc_ecc_mul2add.c \
				   libtomcrypt/src/pk/ecc/ltc_ecc_mulmod.c \
				   libtomcrypt/src/pk/ecc/ltc_ecc_points.c \
				   libtomcrypt/src/pk/ecc/ltc_ecc_projective_add_point.c \
				   libtomcrypt/src/pk/ecc/ltc_ecc_projective_dbl_point.c \
				   libtomcrypt/src/math/ltm_desc.c \
				   libtomcrypt/src/math/multi.c \
				   libtomcrypt/src/math/rand_prime.c \
				   libtomcrypt/src/pk/rsa/rsa_exptmod.c \
				   libtomcrypt/src/pk/rsa/rsa_make_key.c \
				   libtomcrypt/src/hashes/sha1.c \
				   libtomcrypt/src/hashes/sha2/sha256.c \
				   libtomcrypt/src/misc/zeromem.c
				   
LOCAL_LDLIBS	:= -L$(SYSROOT)/usr/lib -llog

LOCAL_CFLAGS	:= -DFPM_ARM

include $(BUILD_SHARED_LIBRARY)
