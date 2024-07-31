/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/*****************************************************************************
** File Name:                                    *
** Author:         figo.feng                                                 *
** Date:           06/09/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe digit camera                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        figo.feng	       Create
******************************************************************************/
#include "std_header.h"

#ifdef JAVA_SUPPORT_IA
#include "jblend_config.h"
#ifdef TRUSTED_MIDLET_OVER_MINISSL
#include "kjava_sys_ext.h"
#include "ajmm.h"
#include "string.h"
#include "libaicrypto.h"
#include <jwe_log.h>
#include <acpi_sys.h> 

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

void ext_encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

void ext_decodeblock( unsigned char in[4], unsigned char out[3] )
{   
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

/**
 * To decode the data using Base64 algorithm
 *
 * @parm	inbuf		input Base64 data(in)
 * @parm    inbuf_len   length of input data(in)
 * @parm	outbuf		output data after decode, size of 
  *                     outbuf MUST > (3 * inbuf_len / 4) bytes (out)
 *
 * @return	size of data in outbuf
 */
int ext_b64_decode(unsigned char* inbuf,  int inbuf_len, unsigned char* outbuf )
{
    unsigned char in[4], out[3], v;
    int i, len;
	int anchor_inbuf = 0;
	int anchor_outbuf = 0;

    while( anchor_inbuf < inbuf_len )
	{
		len = 0;

        for( i = 0; i < 4 && anchor_inbuf < inbuf_len; i++ )
		{
            v   = 0;
            while( anchor_inbuf < inbuf_len && v == 0 )
			{
                v = inbuf[ anchor_inbuf++ ];
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v != 0 )
				{
					if ( v == '$' )
					{
						in[i] = 0;
					}
					else
					{
						len++;
						in[i] = (unsigned char) (v - 62);
					}
                }
            }
        }

        if( len > 0 )
		{
            ext_decodeblock( in, out );
            for( i = 0; i < len - 1; i++ )
			{
                outbuf[ anchor_outbuf++ ] = out[i];
            }
        }
    }

	return (anchor_outbuf);
}
int ext_verifyChain( aicrypto_Cert *cert_root[],   int num_of_ca, 
                     aicrypto_Cert *cert_chain[],  int num_of_cert )
{
	int           i, j,k;
	int           dest_ca_index   = -1;
	int           dest_cert_index = -1;

	char          *subject_name   = NULL, 
		          *issuer_name    = NULL;
	aicrypto_Key	          *public_key     = NULL;
	int           ret;
	
	//printf( "[ext_verifyChain] enter, num of ca %d, num of cert %d\n", num_of_ca, num_of_cert  );
	JWE_LOG(ext_verifyChain, ("enter,num_of_ca is%d.",num_of_ca));
       JWE_LOG(ext_verifyChain, ("enter,num_of_cert is%d.",num_of_cert));


	if ( num_of_ca <= 0 || num_of_cert <= 0 )
	{
		//  no ca list or no certificate chain
		return -1;
	}

///////////////////////////////////////////////////
for ( i = 0; i < num_of_ca; i ++ )
	{

		for ( j = 0; j < num_of_cert; j ++ )
		{
			JWE_LOG(ext_verifyChain, ("root is 0x%08x chain is 0x%08x", cert_root[j], cert_chain[i]));
			// compare ca subject name with cert issuer name
			issuer_name  = cert_chain[j]->issuer;	
			subject_name = cert_root[i]->subject;

			JWE_LOG(ext_verifyChain, ("---cert_chain issuer_name----"));
	 		jwe_log_dumpText(issuer_name, 100);   
			
			JWE_LOG(ext_verifyChain, ("---cert_root subject_name---"));
       	 	jwe_log_dumpText(subject_name, 100);

			if ( strcmp( subject_name, issuer_name ) == 0 )
			{
				dest_cert_index  = j;
				dest_ca_index    = i;
				JWE_LOG(ext_verifyChain,(" subject issuer matched!!   only subject issuer matched!! executing Verify"));
				////////
		    		aicrypto_Cert_print(cert_root[dest_ca_index]);
			    
				public_key = cert_root[dest_ca_index]->pubkey;//得到手机内置CA的公钥，用这个公钥把MIDLET开发者的公钥给解出来/或者这个公钥能解成功的话，才确性Midlet开发者的公钥是可信任的。(因为Midlet由这个对应的CA用私钥加过密)

				if ( public_key == NULL )
				{
				    //printf("public key of cert root is NULL!\n");
				    JWE_LOG(ext_verifyChain,("public key of cert root is NULL. impossiblely!! very strange"));
					return -2;
				
				}
			       //todo: use pubkey to verify signature of the certificate chain
				if ( (ret = aicrypto_ASN1_vfy_sig(public_key,AICRYPTO_ASN1_next(cert_chain[dest_cert_index]->der),cert_chain[dest_cert_index]->signature, cert_chain[dest_cert_index]->signature_algo )) != 0)
				{
					JWE_LOG(ext_verifyChain,("ret = %d.", ret));
					JWE_LOG(ext_verifyChain,("tail certificate and dest ca verification failed."));
					//return -1;
					break;					
				}
			    
				public_key = NULL;

				// verify the certification chain
				for ( k = dest_cert_index; k > 0; k -- )
				{
				    aicrypto_Cert_print(cert_chain[k]);
					public_key = cert_chain[k]->pubkey;

					if ( aicrypto_ASN1_vfy_sig( public_key,AICRYPTO_ASN1_next(cert_chain[k-1]->der),cert_chain[k-1]->signature,cert_chain[k-1]->signature_algo ) != 0 )
					{
						// chain verify failed
						//printf("chain verify failed\n");
						JWE_LOG(ext_verifyChain,("chain verify failed"));
						return -1;
					}
			        
					public_key = NULL;
				}
				JWE_LOG(ext_verifyChain, ("Use root cert %d verify certchain success! leave ",dest_ca_index));
				return dest_ca_index;
				////////

			}


		}

//验证成功的话早return了
JWE_LOG(ext_verifyChain, ("Use root cert %d verify certchain fail " , i));

		
	}

JWE_LOG(ext_verifyChain, ("All root cert verify certchain fail "));
return -1;

}

/* verify chain certificate */
extern void dumpbuf(void *buf, int len);

int kjava_ext_verifyChainCert( KJavaCaList     *pCa,
                              int              iNumOfCa,
                              KJavaCertChain  *pChain,
                              int              iNumOfCertInChain,
                              int             *iDomain )
{
    aicrypto_Cert **certChain;
    aicrypto_Cert **certRoot;
    
    int   i, root_ca_index;
    int   cert_size;
    char *cert_data = NULL;

   char *ca_data = NULL;

	
    
    //printf("[kjava_ext_verifyChainCert] enter\n");
    JWE_LOG(kjava_ext_verifyChainCert, ("enter,the iNumOfCa is %d.",iNumOfCa));
    
    certChain = (aicrypto_Cert**)ajmm_malloc(sizeof(aicrypto_Cert*) * iNumOfCertInChain );/*lint !e737*/
    
    /* convert certificate chain and fill in SSL struct */
    for ( i = 0; i < iNumOfCertInChain; i ++ )
    {
        cert_data = (char*)ajmm_malloc(pChain[i].certLen);        
        cert_size = ext_b64_decode( pChain[i].cert, pChain[i].certLen, (unsigned char*)cert_data );
	//这个cert_data malloc出来后每次循环都没有free，看上去是内存泄漏，其实
	//在aicrypto_ASN1_read_cert 内部会把传进来的in赋值给certChain[i]的der，der是malloc出来的，每个certChain[i]
	//都对应一个，以后再下面free每个certChain[i]的时候一起把cert_data free掉
	
        JWE_LOG(kjava_ext_verifyChainCert, ("cert_size is: %d.",cert_size));
        JWE_LOG(kjava_ext_verifyChainCert, ("pChain[%d].certLen is: %d.",i,pChain[i].certLen));

	 JWE_LOG(kjava_ext_verifyChainCert, ("pChain[%d] ---------------------",i));
	 dumpbuf(pChain[i].cert, 20);	
	 
        // how to construct the Cert data structrue still need more investigation.
		if ((certChain[i] = aicrypto_ASN1_read_cert((unsigned char*)cert_data)) == NULL) {
            JWE_LOG(kjava_ext_verifyChainCert, ("fatal error: ANS1 construct Cert in JAD error!"));
	        //printf("fatal error: ANS1 construct Cert in JAD error! \n");
            return -2;
	    }	
	 JWE_LOG(kjava_ext_verifyChainCert, ("certChain[%d];issuer first value is ---------------------",i,certChain[i]->issuer[0]));
	 dumpbuf(certChain[i]->issuer, 20);
	 JWE_LOG(kjava_ext_verifyChainCert, ("certChain[%d];subject first value is ---------------------",i,certChain[i]->subject[0]));
	 dumpbuf(certChain[i]->subject, 20);
	 
    }
    
    if ( iNumOfCa <= 0 )
    {
        //printf("fatal error: Only support external root-ca.\n");
        JWE_LOG(kjava_ext_verifyChainCert, ("fatal error: Only support external root-ca."));
        return -2;
    }
    
    certRoot = (aicrypto_Cert**)ajmm_malloc(sizeof(aicrypto_Cert*) * iNumOfCa );/*lint !e737*/
    
    for ( i = 0; i < iNumOfCa; i ++ )
    {

        ca_data = (char*)ajmm_malloc(pCa[i].certLen);//要增加的        
        memcpy(ca_data,pCa[i].cert,pCa[i].certLen);//要增加的     

        JWE_LOG(kjava_ext_verifyChainCert, ("pCa[%d].certLen is: %d.",i,pCa[i].certLen));
        dumpbuf(pCa[i].cert, 20);

        if((certRoot[i] = aicrypto_ASN1_read_cert((unsigned char*)ca_data)) == NULL)
        {
            JWE_LOG(kjava_ext_verifyChainCert, ("fatal error: ANS1 construct root Cert error!"));
            return -2;
        }
        dumpbuf(certRoot[i]->issuer, 20);
        dumpbuf(certRoot[i]->subject, 20);
		
    }
    root_ca_index = ext_verifyChain( certRoot, iNumOfCa, certChain, iNumOfCertInChain );

	//if (cert_data)//被交换位置
	//{   JWE_LOG(kjava_ext_verifyChainCert, ("AAAAAAAAAAAAAAAAAAA")); 
	//    ajmm_free( cert_data );
	//}

    for ( i = 0; i < iNumOfCertInChain; i ++ )
	{ 
		aicrypto_Cert_free( certChain[i] );
	}
     
	ajmm_free( certChain );
	#if 0
	if (cert_data)//交换位置
	{
	    ajmm_free( cert_data );
	}
    	#endif
	for ( i = 0; i < iNumOfCa; i ++ )
	{   
		aicrypto_Cert_free( certRoot[i] );//mem error
	}

	ajmm_free( certRoot );
	#if 0  //链式内存，不能够重复free
	if (ca_data)//增加的
	{   
	    ajmm_free( ca_data );
	}
	#endif

	if ( root_ca_index >= 0 )
	{
	    //printf("verify successful, ca id %d, domain %d\n", root_ca_index, pCa[root_ca_index].type);
	    JWE_LOG(kjava_ext_verifyChainCert, ("verify successful, ca id %d, domain %d", root_ca_index, pCa[root_ca_index].type));
	    *iDomain = pCa[root_ca_index].type;
	    return 0;
	}
	
	//printf("verify failed, ext_verifyChain err %d\n", root_ca_index);
    JWE_LOG(kjava_ext_verifyChainCert, ("verify failed, ext_verifyChain err %d", root_ca_index));
    return -1;
}

static void fillTime(struct tm *now_time)
{
    acpi_time_t acpi_t_t=0;
    struct acpi_tm *acpi_t=NULL;
	
    /* Save system time */
    acpi_t_t = acpi_time((acpi_time_t *) NULL);
    acpi_t    = acpi_localtime(&acpi_t_t);

    now_time->tm_year = acpi_t->tm_year ;
    now_time->tm_mon = acpi_t->tm_mon;
    now_time->tm_mday = acpi_t->tm_mday-1;
    now_time->tm_hour = acpi_t->tm_hour;
    now_time->tm_min = acpi_t->tm_min;
    now_time->tm_sec = acpi_t->tm_sec;
}

static int verify_time(struct tm *first,struct tm *second)
{
	int first_value = 0,second_value = 0;

       if(first->tm_year > second->tm_year)
	   	return 1;
	else if(first->tm_year < second->tm_year)
		return 0;
	       if(first->tm_mon > second->tm_mon)
	   	return 1;
	else if(first->tm_mon < second->tm_mon)
		return 0;
	first_value += first->tm_mday;
	first_value *=24;
	first_value +=first->tm_hour;
	first_value *=60;
	first_value +=first->tm_min;
	first_value *=60;
	first_value +=first->tm_sec;

	second_value += second->tm_mday;
	second_value *=24;
	second_value +=second->tm_hour;
	second_value *=60;
	second_value +=second->tm_min;
	second_value *=60;
	second_value +=second->tm_sec;

	if(first_value > second_value)
		return 1;
	return 0;
}
/* Verify signature of Jar */
int kjava_ext_verifySignature( 
                              unsigned char  *pCert,
                              int             iCertLen,
                              unsigned char  *pJar,
                              int             iJarLen,
                              unsigned char  *pSign,
                              int             iSignLen )
{
    aicrypto_Cert      *cert;

	aicrypto_Key              *public_key = NULL;
    
    int   cert_size;
    char* cert_data;
    
    int   encry_sig_size;
    char* encry_sig_data;
    
    int   jar_sig_size;
    char* jar_sig_data;
    
    int           ret;
    struct tm now_time;
    //printf("[kjava_ext_verifySignature] enter, cert len %d, jar len %d, sig len %d\n",iCertLen, iJarLen, iSignLen );
    
    cert_data = (char*)ajmm_malloc(iCertLen);
    cert_size = ext_b64_decode( pCert, iCertLen, (unsigned char*)cert_data );
    
	if ((cert = aicrypto_ASN1_read_cert((unsigned char*)cert_data)) == NULL)
	{
	    //printf("fatal error: ANS1 construct Cert for kjava_ext_verifySignature error! \n");
        return -2;   
	}
    JWE_LOG(kjava_ext_verifySignature,("notAfter %d notBefore %d %d %d %d %d",cert->time.notAfterType,cert->time.notBeforeType,
		cert->time.notBefore.tm_year,cert->time.notBefore.tm_mon,
		cert->time.notAfter.tm_year,cert->time.notAfter.tm_mon));
    fillTime(&now_time);
    if(cert->time.notAfterType != 0 && verify_time(&now_time,&cert->time.notAfter))
		return -2;
    if(cert->time.notBeforeType != 0 && verify_time(&cert->time.notBefore,&now_time))
		return -2;
    //printf("cert_sub = %s\n",cert->subject);
	//printf("cert_issuer = %s\n",cert->issuer);
	//Cert_print(cert);

	public_key = cert->pubkey;
	//printf("keyType = %d, keySize= %d\n",public_key->key_type,public_key->size);
	
	if ( public_key == NULL )
	{
		aicrypto_Cert_free( cert );
		//printf("[kjava_ext_verifySignature] failed to get public key\n");
		return -2; // WARNING!!! public_key is not "freeable"!!!
	}
		
	encry_sig_data = (char*)ajmm_malloc(1024);
	encry_sig_size = ext_b64_decode( pSign, iSignLen, (unsigned char*)encry_sig_data );
	
	//printf("encry_sig_size = %d\n",encry_sig_size);
    //printf("encry_sig_data = %s\n",encry_sig_data);
	
	
	jar_sig_data = (char*)ajmm_malloc(1024);
	memset(jar_sig_data,0,1024);
	
	//use the JAR buffer to generate the digest "jar_sig_data".
	if(aicrypto_OK_do_digest(AICRYPTO_OBJ_SIG_SHA1RSA,pJar,iJarLen,(unsigned char*)jar_sig_data,&jar_sig_size)==NULL){
       //printf("test OK_do_digest -- error!\n");
       return -2;
	}
	
	//use the jar digest and pubkey to verify signature "encry_sig_data"
	ret = aicrypto_OK_do_verify(public_key,(unsigned char*)jar_sig_data,(unsigned char*)encry_sig_data,AICRYPTO_OBJ_SIG_SHA1RSA);

	ajmm_free( cert_data );
	aicrypto_Cert_free( cert );
	
	if ( ret  == 0 )
	{
	    //printf("[kjava_ext_verifySignature] leave, successed\n");
	    return 0;
	}
	else
	{
	    //printf("[kjava_ext_verifySignature] leave, failed\n");
	    return -1;
	}
}

#else
Platform Impl
#endif
#endif
