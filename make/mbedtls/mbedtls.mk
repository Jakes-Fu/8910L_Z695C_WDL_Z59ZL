ifeq ($(strip $(MBEDTLS_SUPPORT)), TRUE)
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif


ifeq ($(strip $(MBEDTLS_VERSION)), V206)
MINCPATH += Third-party/mbedtls/V206/include/mbedtls
MINCPATH += Third-party/mbedtls/V206/include
MSRCPATH += Third-party/mbedtls/V206/src

SOURCES += aes.c
SOURCES += aesni.c
SOURCES += arc4.c
SOURCES += asn1parse.c
SOURCES += asn1write.c
SOURCES += base64.c
SOURCES += bignum.c
SOURCES += blowfish.c
SOURCES += camellia.c
SOURCES += ccm.c
SOURCES += certs.c
SOURCES += cipher.c
SOURCES += cipher_wrap.c
SOURCES += cmac.c
SOURCES += ctr_drbg.c
SOURCES += debug.c
SOURCES += des.c
SOURCES += dhm.c
SOURCES += ecdh.c
SOURCES += ecdsa.c
SOURCES += ecjpake.c
SOURCES += ecp.c
SOURCES += ecp_curves.c
SOURCES += entropy.c
SOURCES += entropy_poll.c
SOURCES += error.c
SOURCES += gcm.c
SOURCES += havege.c
SOURCES += hmac_drbg.c
SOURCES += md.c
SOURCES += md2.c
SOURCES += md4.c
SOURCES += md5.c
SOURCES += md_wrap.c
SOURCES += memory_buffer_alloc.c
SOURCES += net_sockets.c
SOURCES += oid.c
SOURCES += padlock.c
SOURCES += pem.c
SOURCES += pk.c
SOURCES += pkcs11.c
SOURCES += pkcs12.c
SOURCES += pkcs5.c
SOURCES += pkparse.c
SOURCES += pkwrite.c
SOURCES += pk_wrap.c
SOURCES += platform.c
SOURCES += ripemd160.c
SOURCES += rsa.c
SOURCES += sha1.c
SOURCES += sha256.c
SOURCES += sha512.c
SOURCES += ssl_cache.c
SOURCES += ssl_ciphersuites.c
SOURCES += ssl_cli.c
SOURCES += ssl_cookie.c
SOURCES += ssl_srv.c
SOURCES += ssl_ticket.c
SOURCES += ssl_tls.c
SOURCES += threading.c
SOURCES += timing.c
SOURCES += version.c
SOURCES += version_features.c
SOURCES += x509.c
SOURCES += x509write_crt.c
SOURCES += x509write_csr.c
SOURCES += x509_create.c
SOURCES += x509_crl.c
SOURCES += x509_crt.c
SOURCES += x509_csr.c
SOURCES += xtea.c
SOURCES += ca.c
SOURCES += HAL_UDP_threadx.c
SOURCES += HAL_TCP_threadx.c

else  
#else ifeq ($(strip $(MBEDTLS_VERSION)), V224)
MINCPATH += Third-party/mbedtls/V224/include/
MINCPATH += Third-party/mbedtls/V224/include/mbedtls
MSRCPATH += Third-party/mbedtls/V224/src

SOURCES +=  aes.c          ctr_drbg.c        md2.c                  platform.c                    ssl_cookie.c 			\
			aesni.c        debug.c           md4.c                  platform_util.c               ssl_msg.c             \
			arc4.c         des.c             md5.c                  poly1305.c                    ssl_srv.c             \
			aria.c         dhm.c             md.c                   psa_crypto.c                  ssl_ticket.c          \
			asn1parse.c    ecdh.c            memory_buffer_alloc.c  psa_crypto_se.c               ssl_tls.c             \
			asn1write.c    ecdsa.c           net_sockets.c          psa_crypto_slot_management.c  threading.c           \
			base64.c       ecjpake.c         nist_kw.c              psa_crypto_storage.c          timing.c              \
			bignum.c       ecp.c             oid.c                  psa_its_file.c                version.c             \
			blowfish.c     ecp_curves.c      padlock.c              ripemd160.c                   version_features.c    \
			camellia.c     entropy.c         pem.c                  rsa.c                         x509.c                \
			ccm.c          entropy_poll.c    pk.c                   rsa_internal.c                x509_create.c         \
			certs.c        error.c           pkcs11.c               sha1.c                        x509_crl.c            \
			chacha20.c     gcm.c             pkcs12.c               sha256.c                      x509_crt.c            \
			chachapoly.c   havege.c          pkcs5.c                sha512.c                      x509_csr.c            \
			cipher.c       hkdf.c            pkparse.c              ssl_cache.c                   x509write_crt.c       \
			cipher_wrap.c  hmac_drbg.c       pk_wrap.c              ssl_ciphersuites.c            x509write_csr.c       \
			cmac.c         mbedtls_import.c  pkwrite.c              ssl_cli.c                     xtea.c                

endif
endif