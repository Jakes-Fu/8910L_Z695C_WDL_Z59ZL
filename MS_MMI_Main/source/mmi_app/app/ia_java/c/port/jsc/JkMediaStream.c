/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#ifdef JAVA_SUPPORT_IA
#include "jkmediastream.h"
#include "jwe_log.h"

extern JKT_MediaStream fileStream;

static const JKT_MediaStream* streamFuncs[] = {
    ( JKT_MediaStream* )&fileStream,
};

/**
 * Gets the number of the protocols that support streaming playback
 * 
 *  In this sample code of streaming playbach, user data are accessed using "file:" protocol.
 *  Since in the Security Policy of MIDP2.0 specification and JTWI1.0 specification, 
 *  using file accessing function is not permitted for the MIDlet in Untrusted Domain,
 *  but streaming from "file:" scheme is enabled only for MIDlets in Trusted Domain.
 *  In this sample code, the number of supporting protocols is switched according to the
 *  protection domain of current MIDlet.
 */
JKSint32 JkMediaStreamGetCount( void )
{
    JWE_LOG(JkMediaStreamGetCount, ("enter"));
    return 1;
}

JKT_MediaStream* JkMediaStreamGetController( JKSint32 count )
{
    JWE_LOG(JkMediaStreamGetController, ("enter count=%d",count));
    //return ( JKT_MediaStream* )0;
    return ( JKT_MediaStream* )streamFuncs[ count ];
}
#endif