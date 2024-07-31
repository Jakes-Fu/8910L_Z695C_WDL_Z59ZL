/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#ifdef JAVA_SUPPORT_IA
#include "jktypes.h"
#include "jkmediamididrv.h"

JKT_Media_MIDIControl const* JkMediaMIDIDrvGetMIDIController( void )
{
    return (JKT_Media_MIDIControl const*)JK_NULL;
}

JKSint32 JkMediaMIDIDrvCreate( void )
{
    return (JKSint32)0;
}

JKSint32 JkMediaMIDIDrvDestroy( JKSint32 id )
{
    return (JKSint32)0;
}

JKSint32 JkMediaMIDIDrvOpen( JKSint32 id )
{
    return (JKSint32)0;
}

JKSint32 JkMediaMIDIDrvClose( JKSint32 id )
{
    return (JKSint32)0;
}

JKSint32 JkMediaMIDIDrvSetMasterVolume( JKSint32 id, JKSint32 volume )
{
    return (JKSint32)0;
}

JKSint32 JkMediaMIDIDrvGetMasterVolume( JKSint32 id )
{
    return (JKSint32)0;
}
#endif
