# + TCPIP module START
#----------------------------------------------------------------------------#
#                                DAPS COMMON                                 #
#----------------------------------------------------------------------------#
MINCPATH      = DAPS/export/inc

ifeq ($(strip $(PLATFORM)), UIX8910)
MINCPATH   += chip_drv/export/inc
endif


ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)

MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT   += -DIPV6_SUPPORT

#----------------------------------------------------------------------------#
#                               TCPIP6                                        #
#----------------------------------------------------------------------------#
ifeq ($(strip $(TCPIP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS
MCFLAG_OPT   += -DINCLUDE_TCP -DNB_CONNECT -DFULL_ICMP -DIP_FRAGMENTS \
                -DIP_ROUTING -DMULTI_HOMED -DDNS_CLIENT  \
                -DPUTC_UART -DRAM_BUFFER -DASYNC_SUPPORT \
                -DPKTOUTPUT_ENABLE -DBUFF_OPTIMIZE -DWTCP_OPTIMIZE \
                -DINCLUDE_ARP -DPING_APP -DEAPOL_ENABLE

ifeq ($(strip $(VM_VLX)), TRUE)
MCFLAG_OPT   += -DVETHER_ENABLE
endif

ifeq ($(strip $(MUX_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_MUX_ENABLE_
endif

ifeq ($(strip $(TCPIP_MODEM_ONLY_SUPPORT)), TRUE)
MCFLAG_OPT   += -DMODEM_ONLY
endif


ifeq ($(strip $(PLATFORM)), SC6530)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC6531EFM)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC6600R)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
endif

ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
else
ifeq ($(strip $(PLATFORM)), SC8501C)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
else
ifeq ($(strip $(PLATFORM)), SC7702)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
endif
ifeq ($(strip $(PLATFORM)), SC7703A)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
endif
endif
endif

# 7701 dusing hardware PPP encode/decode module
ifeq ($(strip $(HWPPP_USB)), TRUE)
MCFLAG_OPT   += -DPPP_HARD_SUPPORT
endif
# 7702 data card using hardware PPP encode/decode module
ifeq ($(strip $(USE_NEW_USB_FRAMEWORK)), TRUE)
MCFLAG_OPT   += -DPPP_HARD_SUPPORT
endif

# here just adaptive to 09a platform
ifneq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT   += -DNEW_USBPORT
endif
endif

MINCPATH += $(ATC_INCPATH)
MINCPATH += $(L4_INCPATH)
MINCPATH += $(SIM_INCPATH)

ifeq ($(strip $(PRODUCT_DM)), TRUE)
MINCPATH     += atc_3g/export/inc
else
MINCPATH     += BASE/atc/export/inc
endif

MINCPATH     += DAPS/source/tcpip6/inc \
                DAPS/source/tcpip6/src/h \
                DAPS/source/tcpip6/src/armport \
                DAPS/source/tcpip6/src/autoip4 \
				DAPS/source/tcpip6/src/crypt/rsa \
				DAPS/source/tcpip6/src/crypt/rc5 \
                DAPS/source/tcpip6/src/crypt/bf \
                DAPS/source/tcpip6/src/crypt \
                DAPS/source/tcpip6/src/dhcpv6 \
                DAPS/source/tcpip6/src/ftp \
                DAPS/source/tcpip6/src/ip \
                DAPS/source/tcpip6/src/ipsec \
                DAPS/source/tcpip6/src/ipv6 \
                DAPS/source/tcpip6/src/misclib \
                DAPS/source/tcpip6/src/modem \
                DAPS/source/tcpip6/src/mschap \
                DAPS/source/tcpip6/src/natrt \
                DAPS/source/tcpip6/src/ftp \
                DAPS/source/tcpip6/src/net \
                DAPS/source/tcpip6/src/ppp \
                DAPS/source/tcpip6/src/snmp \
                DAPS/source/tcpip6/src/tcp \
                DAPS/source/tcpip6/src/telnet \
                DAPS/source/tcpip6/src/tftp \
				DAPS/source/tcpip/src/test \
                BASE/l4/export/inc \
                BASE/sim/export/inc \
                mux/include \
                MS_Customize/export/inc

ifeq ($(strip $(VM_VLX)), TRUE)
MINCPATH     += vlx_plugin/export/nk
endif
                
MINCPATH     += chip_drv/export/inc/outdated

MINCPATH     +=$(PS_INCPATH)

MSRCPATH     += DAPS/source/tcpip6/src \
                DAPS/source/tcpip6/src/allports \
                DAPS/source/tcpip6/src/armport \
                DAPS/source/tcpip6/src/autoip4 \
                DAPS/source/tcpip6/src/cli \
				DAPS/source/tcpip6/src/crypt/rsa \
				DAPS/source/tcpip6/src/crypt/rc5 \
                DAPS/source/tcpip6/src/crypt/bf \
                DAPS/source/tcpip6/src/crypt \
                DAPS/source/tcpip6/src/crypteng \
                DAPS/source/tcpip6/src/dhcpv6 \
                DAPS/source/tcpip6/src/ftp \
                DAPS/source/tcpip6/src/igmp3 \
                DAPS/source/tcpip6/src/ip \
                DAPS/source/tcpip6/src/ipmc \
                DAPS/source/tcpip6/src/ipsec \
                DAPS/source/tcpip6/src/ipv6 \
                DAPS/source/tcpip6/src/misclib \
                DAPS/source/tcpip6/src/mld \
                DAPS/source/tcpip6/src/modem \
                DAPS/source/tcpip6/src/mschap \
                DAPS/source/tcpip6/src/natrt \
                DAPS/source/tcpip6/src/net \
                DAPS/source/tcpip6/src/ppp \
                DAPS/source/tcpip6/src/snmp \
                DAPS/source/tcpip6/src/tcp \
                DAPS/source/tcpip6/src/telnet \
                DAPS/source/tcpip6/src/tftp \
				DAPS/source/tcpip6/src/test

ifeq ($(strip $(PLATFORM)), UIX8910)
MSRCPATH     += DAPS/source/tcpip_ipcBear/src
endif
				
SOURCES      += allports.c timeouts.c \
                md4.c md5.c  \
                ininit.c inmain.c intimer.c inport.c mem_check.c userdata.c uart_mux.c uartutil.c         \
                osport.c tcpip_bearer.c tcpip_ps.c xip_sec.c autoip.c upnp.c cli.c cli_group_nt.c         \
				cli_mod.c cli_nt.c rsa_chk.c rsa_eay.c rsa_err.c rsa_gen.c rsa_lib.c rsa_none.c           \
				rsa_null.c rsa_oaep.c rsa_pk1.c rsa_ssl.c rc5_ecb.c rc5_enc.c rc5_skey.c rc5cfb64.c       \
				rc5ofb64.c                  \
		        bf_cfb64.c bf_ecb.c bf_enc.c bf_ofb64.c bf_skey.c     \
                a_bitstr.c a_bmp.c a_bool.c a_bytes.c a_d2i_fp.c a_digest.c a_dup.c a_enum.c              \
                a_gentm.c a_hdr.c a_i2d_fp.c a_int.c a_mbstr.c a_meth.c a_null.c a_object.c               \
                a_octet.c a_print.c a_set.c a_sign.c a_strex.c a_strnid.c a_time.c a_type.c               \
                a_utctm.c a_utf8.c a_verify.c a_vis.c aes_cbc.c aes_cfb.c aes_core.c aes_ctr.c            \
                asn1_err.c asn1_lib.c asn1_par.c asn_pack.c b_dump.c b_print.c b_sock.c bf_buff.c         \
                bf_lbuf.c bf_nbio.c bf_null.c bio_b64.c bio_cb.c bio_enc.c bio_err.c bio_lib.c bio_md.c   \
                bio_ok.c bn_add.c bn_asm.c bn_blind.c bn_ctx.c bn_div.c bn_err.c bn_exp.c                 \
                bn_exp2.c bn_gcd.c bn_lib.c bn_mont.c bn_mpi.c bn_mul.c bn_prime.c bn_print.c             \
                bn_rand.c bn_recp.c bn_shift.c bn_sqr.c bn_word.c bss_acpt.c bss_bio.c bss_conn.c         \
                bss_fd.c bss_file.c bss_mem.c bss_null.c bss_sock.c buf_err.c buffer.c by_file.c          \
                c_all.c c_allc.c c_alld.c c_zlib.c cbc_cksm.c cfb64ede.c cfb64enc.c cfb_enc.c             \
                cpt_err.c crypt_api.c crypt_mod.c cryptlib.c d2i_dsap.c d2i_pr.c d2i_pu.c d2i_r_pr.c      \
                d2i_r_pu.c d2i_s_pr.c d2i_s_pu.c des_enc.c dh_asn1.c dh_check.c dh_err.c dh_gen.c         \
                dh_key.c dh_lib.c digest.c dsa_asn1.c dsa_err.c dsa_gen.c dsa_key.c dsa_lib.c             \
                dsa_ossl.c dsa_sign.c dsa_vrf.c e_aes.c e_bf.c e_des.c e_des3.c e_null.c                  \
                e_rc4.c e_rc5.c e_xcbc_d.c ecb3_enc.c ecb_enc.c encode.c err.c err_all.c                  \
                err_prn.c evp_asn1.c evp_enc.c evp_err.c evp_key.c evp_lib.c evp_pbe.c evp_pkey.c         \
                ex_data.c f_enum.c f_int.c f_string.c i2d_dsap.c i2d_pr.c i2d_pu.c i2d_r_pr.c             \
                i2d_r_pu.c i2d_s_pr.c i2d_s_pu.c lhash.c m_dss.c m_dss1.c m_md5.c m_null.c                \
                m_sha1.c magic.c md4.c md5.c md_rand.c mem_dbg.c names.c ncbc_enc.c                       \
                nsseq.c o_names.c obj_dat.c obj_err.c obj_lib.c ofb64ede.c ofb64enc.c p12_add.c           \
                p12_attr.c p12_bags.c p12_crpt.c p12_crt.c p12_decr.c p12_init.c p12_key.c p12_kiss.c     \
                p12_lib.c p12_mac.c p12_mutl.c p12_npas.c p12_sbag.c p12_utl.c p5_crpt.c p5_crpt2.c       \
                p5_pbe.c p5_pbev2.c p7_dgst.c p7_enc.c p7_enc_c.c p7_evp.c p7_i_s.c p7_lib.c              \
                p7_recip.c p7_s_e.c p7_signd.c p7_signi.c p8_pkey.c p_dec.c p_enc.c p_lib.c               \
                p_open.c p_seal.c p_sign.c p_verify.c pem_all.c pem_err.c pem_info.c pem_lib.c            \
                pem_seal.c pem_sign.c pk12err.c pk7_attr.c pk7_doit.c pk7_lib.c pk7_mime.c pkcs7err.c     \
                primes.c rand_err.c rand_key.c rand_lib.c rand_win.c randfile.c rc4_enc.c rc4_skey.c      \
                rsa_saos.c rsa_sign.c set_key.c sha1_new.c sha2.c sk.c spr.c ssl_debug.c                      \
                stack.c str2key.c t_bitst.c t_crl.c t_pkey.c t_req.c t_spki.c t_x509.c                    \
                t_x509a.c tasn_dec.c tasn_enc.c tasn_fre.c tasn_new.c tasn_utl.c v3_akey.c v3_alt.c       \
                v3_bcons.c v3_bitst.c v3_crld.c v3_enum.c v3_extku.c v3_genn.c v3_ia5.c v3_info.c         \
                v3_int.c v3_lib.c v3_pku.c v3_prn.c v3_purp.c v3_skey.c v3_sxnet.c v3_utl.c               \
                v3err.c x509_att.c x509_cmp.c x509_d2.c x509_def.c x509_err.c x509_ext.c x509_lu.c        \
                x509_obj.c x509_r2x.c x509_req.c x509_set.c x509_trs.c x509_txt.c x509_v3.c x509_vfy.c    \
                x509name.c x509rset.c x509spki.c x509type.c x_algor.c x_all.c x_attrib.c x_bignum.c       \
                x_cinf.c x_crl.c x_exten.c x_info.c x_long.c x_name.c x_pkey.c x_pubkey.c                 \
                x_req.c x_sig.c x_spki.c x_val.c x_x509.c x_x509a.c xcbc_enc.c                            \
				ce_mod.c ce_nt.c ciphers.c crypteng.c cu_aes.c cu_app.c cu_bf.c cu_des.c       \
				cu_hmac.c cu_md5.c cu_niche.c cu_null.c cu_rsa.c cu_sha1.c                     \
                create_sockets.c dhcpv6_loop.c dhcpv6_port.c dhcpv6_utils.c dhcpv6_bld_msg.c \
                dhcpv6_mod.c dhcpv6_bld_send_msg.c dhcpv6_nt.c dhcpv6_process_msg.c ftp_mod.c  \
                ftp_nt.c ftpclnt.c ftpcprn.c ftpsport.c ftpssock.c          \
                igmp3.c \
                et_arp.c icmp.c iface.c ip.c ip_reasm.c ipdemux.c ipmc.c ipnet.c ipport.c      \
                ipraw.c iproute.c ipstart.c pmtu.c rtbtree.c udp.c \
                igmp.c igmp2.c igmp_cmn.c igmp_mod.c igmp_nt.c ipmc46.c ipopt.c msf.c          \
                u_mctest.c \
                callbacks.c classify.c classify_sp.c crypto.c inet.c iniche.c iniche_intf.c    \
                ipcksum.c ipsec.c ipsec_id.c ipsec_mod.c ipsec_nt.c ipsecip6.c ipsecmgmt.c     \
                ipsecmisc.c ipsecrst.c ipsecsession.c log.c mgmt.c mgmtapi.c myaddrs.c         \
                os_iniche.c packet.c pkt_iniche.c process.c replay.c route.c spd.c sync_api.c  \
                timer.c util.c valnames.c \
                icmp6.c ip6.c ip62mac.c ip6_pcb.c ip6ext.c ip6frags.c ip6mc.c ip6route.c       \
                ipv6_mod.c ipv6_nt.c nd.c tcp6.c udp6.c udpsock6.c \
                app_tcp_if.c ascii_utils.c async_gethost.c async_sock.c cb.c                   \
				cksum.c console_mod.c dhcsetup.c do_script.c  genlist.c gio.c in_utils.c       \
				nextcarg.c parseip.c profiler.c reshost.c strilib.c strlib.c syslog.c          \
				syslog_mod.c syslog_nt.c task.c tcp_echo.c tcpcksum.c tcpip_debug.c ttyio.c    \
				udp_echo.c user_nt.c userpass.c mld.c mld_cmn.c mldv2.c \
                dialer.c mdmport.c modem_mod.c modem_nt.c \
                mschap.c \
                nat_mod.c nat_nt.c natfrag.c natftp.c natport.c nattl.c natutil.c \
                app_ping.c chainbuff_nt.c dc_mod.c dc_nt.c dhcpclnt.c dhcputil.c diag_mod.c    \
                diag_nt.c dnsc_nt.c dnsclnt.c ifmap.c macloop.c netif.c netmain_mod.c          \
				netmain_nt.c ping.c pktalloc.c q.c q_txcnf.c slip.c slipif.c tx_flowctrl.c     \
				udp_open.c       \
                chap.c ifppp.c ipcp.c ipv6cp.c lcp.c ppp_dhcp.c ppp_loop.c ppp_mod.c ppp_nt.c  \
                ppp_port.c pppchar.c pppfsm.c \
                pppsys.c upap.c vjcomp.c \
                in_pcb.c nptcp.c rawsock.c sockcall.c socket.c socket2.c soselect.c tcp_in.c   \
                tcp_out.c tcp_subr.c tcp_timr.c tcp_usr.c tcp_zio.c tcpport.c       \
                tcpsack.c udpsock.c \
                comport.c rfc2217.c telerr.c telnet.c telnet_mod.c telnet_nt.c telparse.c      \
                telport.c \
                tftp_mod.c tftp_nt.c tftpcli.c tftpsrv.c tftpudp.c tftputil.c \
                tcpip_version.c \
				modem_throughup.c faketcp_throughup.c pclink_utility.c
				
ifeq ($(strip $(PLATFORM)), UIX8910)
SOURCES      += tcpip_ipcBear.c 
endif				
				
endif

else
MINCPATH   += DAPS/export/inc/tcpip
#----------------------------------------------------------------------------#
#                               TCPIP                                        #
#----------------------------------------------------------------------------#
ifeq ($(strip $(TCPIP_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_RTOS
MCFLAG_OPT   += -DINCLUDE_TCP -DNB_CONNECT -DFULL_ICMP -DIP_FRAGMENTS \
                -DIP_ROUTING -DMULTI_HOMED -DDNS_CLIENT -DMAC_LOOPBACK \
                -DPUTC_UART -DRAM_BUFFER -DASYNC_SUPPORT \
                -DPKTOUTPUT_ENABLE -DBUFF_OPTIMIZE -DWTCP_OPTIMIZE \
                -DUSE_PPP -DPPP_CHARIO -DUSE_COMPORT -DPPP_DNS \
                -DINCLUDE_ARP -DDHCP_CLIENT -DPING_APP -DEAPOL_ENABLE 

ifeq ($(strip $(VM_VLX)), TRUE)
MCFLAG_OPT   += -DVETHER_ENABLE
endif

ifeq ($(strip $(MUX_SUPPORT)), TRUE)
MCFLAG_OPT   += -D_MUX_ENABLE_
endif

ifeq ($(strip $(TCPIP_MODEM_ONLY_SUPPORT)), TRUE)
MCFLAG_OPT   += -DMODEM_ONLY
endif


ifeq ($(strip $(PLATFORM)), SC6530)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
ifneq ($(strip $(SRAM_SIZE)), 16MBIT)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif

ifeq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC6600R)
ifneq ($(strip $(WIFI_SUPPORT)), NONE)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_2
else
MCFLAG_OPT   += -DFLOWRATE_LEVEL_1
endif
endif


ifeq ($(strip $(PLATFORM)), SC8800G)
MCFLAG_OPT   += -DFLOWRATE_LEVEL_3
endif

# here just adaptive to 09a platform
ifneq ($(strip $(PLATFORM)), SC6600L)
ifneq ($(strip $(PLATFORM)), SC6600R)
MCFLAG_OPT   += -DNEW_USBPORT
endif
endif

ifeq ($(strip $(PRODUCT_DM)), TRUE)
MINCPATH     += atc_3g/export/inc
else
MINCPATH     += BASE/atc/export/inc
endif

MINCPATH     += DAPS/source/tcpip/inc \
                DAPS/source/tcpip/src/h \
                DAPS/source/tcpip/src/armport \
                DAPS/source/tcpip/src/crypt \
                DAPS/source/tcpip/src/mppp \
                DAPS/source/tcpip/src/net \
                DAPS/source/tcpip/src/tcp \
                DAPS/source/tcpip/src/tftp \
                DAPS/source/tcpip/src/test \
                BASE/l4/export/inc \
                BASE/sim/export/inc \
                mux/include \
                MS_Customize/export/inc

ifeq ($(strip $(VM_VLX)), TRUE)
MINCPATH     += vlx_plugin/export/nk
endif
                
MINCPATH     += chip_drv/export/inc/outdated

MINCPATH     +=$(PS_INCPATH)

MSRCPATH     += DAPS/source/tcpip/src \
                DAPS/source/tcpip/src/allports \
                DAPS/source/tcpip/src/armport \
                DAPS/source/tcpip/src/crypt \
                DAPS/source/tcpip/src/ip \
                DAPS/source/tcpip/src/misclib \
                DAPS/source/tcpip/src/mppp \
                DAPS/source/tcpip/src/net \
                DAPS/source/tcpip/src/tcp \
                DAPS/source/tcpip/src/tftp \
                DAPS/source/tcpip/src/test

ifeq ($(strip $(PLATFORM)), UIX8910)
MSRCPATH     += DAPS/source/tcpip_ipcBear/src
endif

SOURCES      += allports.c timeouts.c tk_misc.c \
                ccksum.c mem_check.c osport.c pclink_decode.c ppp_port.c startup_prep.c        \
                tcpip_bearer.c tcpip_pclink.c tcpip_ps.c uart_mux.c uartutil.c xip_sec.c       \
                md4.c md5.c tcpip_vether.c \
                et_arp.c icmp.c iface.c ip.c ip_reasm.c ipdemux.c ipnet.c ipraw.c iproute.c    \
                ipstart.c pmtu.c rtbtree.c udp.c \
                app_ping.c app_tcp_if.c async_sock.c bsdsock.c dhcsetup.c genlist.c in_utils.c \
                memdev.c memio.c memwrap.c menulib.c menus.c msring.c netmain.c nextcarg.c     \
                nrmenus.c nvfsio.c nvparms.c parseip.c pcycles.c profiler.c rawiptst.c         \
                reshost.c rfsim.c rttest.c soperr.c strilib.c strlib.c task.c tcp_echo.c       \
                tcpcksum.c tcpip_debug.c testmenu.c ttyio.c udp_echo.c userpass.c \
                chap.c ifppp.c ipcp.c lcp.c ppp_dhcp.c ppp_loop.c pppchar.c pppfsm.c pppmenu.c \
                pppsys.c upap.c vjcomp.c \
                dhcpclnt.c dhcputil.c dnsclnt.c eapol.c macloop.c netif.c ping.c pktalloc.c    \
                q.c q_txcnf.c tx_flowctrl.c udp_open.c \
                in_pcb.c nptcp.c rawsock.c sockcall.c socket.c socket2.c soselect.c tcp_in.c   \
                tcp_menu.c tcp_out.c tcp_subr.c tcp_timr.c tcp_usr.c tcp_zio.c tcpport.c       \
                tcpsack.c udpsock.c \
                tcpip_version.c ipconflict_chk.c async_gethost.c \
                modem_throughup.c faketcp_throughup.c pclink_utility.c
				
ifeq ($(strip $(PLATFORM)), UIX8910)
SOURCES      += tcpip_ipcBear.c 
endif				
endif

endif
# - TCPIP module END
         
