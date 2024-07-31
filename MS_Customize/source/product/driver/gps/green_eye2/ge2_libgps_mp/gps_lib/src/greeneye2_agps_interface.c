/******************************************************************************
 ** File Name:   greeneye2_agps_interface.c                                                *
 ** Author:                                                     *
 ** DATE:                                                         *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:          use for qx agps             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                                                                           *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "sci_api.h"
#include "greeneye2_agnss_type.h"
#include "greeneye2_common.h"
#include "greeneye2_agps_type.h"
#include "qx_supl_sdk.h"
#include "greeneye2_engine.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct TIMEPOS
{
    int timeflag;
    int locflag;
    TCgRxNTime RxTime;
    TGe2GpsPosition RxLocation;
}AGNSSTIMEPOS;
 AGNSSTIMEPOS timepos_0;
 TCgAgpsEphemerisCollection_t gnsseph;
 GE2_GLONASS_EPHEMERIS glonass_eph[28];
 WLLH_TYPE loc_time;
static TCmdData_t Pak;
static GE2_IONO_PARAM iono;


void transBdsEph(PBDS_EPHEMERIS pBdsEph,Cli_BdsNavModel_t *pBdsGnssEph)
{
    GREENEYE2_LOGD(("[GE2]transbdseph is enter"));
	pBdsEph->bdsURAI = pBdsGnssEph->navParams.bdsURAI_r12;
	pBdsEph->bdsToe = pBdsGnssEph->navParams.bdsToe_r12;  
	pBdsEph->bdsAPowerHalf = pBdsGnssEph->navParams.bdsAPowerHalf_r12; 
	pBdsEph->bdsE = pBdsGnssEph->navParams.bdsE_r12; 
	pBdsEph->bdsW = pBdsGnssEph->navParams.bdsW_r12; 
	pBdsEph->bdsDeltaN = pBdsGnssEph->navParams.bdsDeltaN_r12; 
	pBdsEph->bdsM0 = pBdsGnssEph->navParams.bdsM0_r12;  
	pBdsEph->bdsOmega0 = pBdsGnssEph->navParams.bdsOmega0_r12; 
	pBdsEph->bdsOmegaDot = pBdsGnssEph->navParams.bdsOmegaDot_r12;
	pBdsEph->bdsI0 = pBdsGnssEph->navParams.bdsI0_r12;  
	pBdsEph->bdsIDot = pBdsGnssEph->navParams.bdsIDot_r12; 
	pBdsEph->bdsCuc = pBdsGnssEph->navParams.bdsCuc_r12; 
	pBdsEph->bdsCus = pBdsGnssEph->navParams.bdsCus_r12; 
	pBdsEph->bdsCrc = pBdsGnssEph->navParams.bdsCrc_r12;
	pBdsEph->bdsCrs = pBdsGnssEph->navParams.bdsCrs_r12;
	pBdsEph->bdsCic = pBdsGnssEph->navParams.bdsCic_r12;
	pBdsEph->bdsCis = pBdsGnssEph->navParams.bdsCis_r12;
}
void transBdsClock(PBDS_CLOCK_EPHEMERIS pBdsClock,Cli_BdsNavModel_t *pBdsGnssClock)
{
    GREENEYE2_LOGD(("[GE2]transBdsClock is enter"));
    pBdsClock->bdsToc = pBdsGnssClock->navParams.bdsToc_r12;
    pBdsClock->bdsA0 = pBdsGnssClock->navParams.bdsA0_r12;
	pBdsClock->bdsA1 = pBdsGnssClock->navParams.bdsA1_r12;
	pBdsClock->bdsA2 = pBdsGnssClock->navParams.bdsA2_r12;
	pBdsClock->bdsTgd1 = pBdsGnssClock->navParams.bdsTgd1_r12; 
}
int BeidouAssData(Cli_BdsAssistData_t *pbds)
{
    Cli_BdsNavModel_t *pNav;
    static GE2_GPS_EPHEMERIS_PACKED BdsPacked;
    static BDS_EPHEMERIS BdsEph;
    static BDS_CLOCK_EPHEMERIS BdsClockEph;
    int i = 0;
    unsigned int agpsmode = 1;

    memset(&Pak,0,sizeof(TCmdData_t)); 
    //GREENEYE2_LOGD("beidou num %d",pbds->numEph);
    
    Pak.length = 4;
    memcpy(Pak.buff,&agpsmode,sizeof(int));   //zxw

    if(pbds->numEph > 14)// the firmware max 14  number  
    {  
        GREENEYE2_LOGD(("[GE2]beidou num rejust the num 14"));
        pbds->numEph = 14;
    }

    for(i = 0; i < pbds->numEph; i++)
    {    
        pNav = &pbds->bdsEph[i];
       GREENEYE2_LOGD(("[GE2]beidou svid is %d",pNav->svID));
        memset(&BdsPacked,0,sizeof(GE2_GPS_EPHEMERIS_PACKED));
        memset(&BdsEph,0,sizeof(BDS_EPHEMERIS));
        memset(&BdsClockEph,0,sizeof(BDS_CLOCK_EPHEMERIS));
        transBdsEph(&BdsEph,pNav);
        transBdsClock(&BdsClockEph,pNav);
        
        if(pNav->svID > 5)
        {
            EncodeBD2EphemerisD1(&BdsEph, &BdsClockEph, &BdsPacked);
        }
        else
        {
            EncodeBD2EphemerisD2(&BdsEph, &BdsClockEph, &BdsPacked);
        }
        BdsPacked.flag = 1;
        BdsPacked.svid = pNav->svID;
        memcpy(Pak.buff+4+i*sizeof(GE2_GPS_EPHEMERIS_PACKED),&BdsPacked,sizeof(GE2_GPS_EPHEMERIS_PACKED));  //zxw +4
        Pak.length = Pak.length + sizeof(GE2_GPS_EPHEMERIS_PACKED);
        #if 0
        D("beidou:svid=%d,clockModel present=%d,orbitpresent=%d",
                pNav->svID,pNav->clockModel.present,pNav->orbitModel.present);   
        D("bdsToc=%ld,bdsA0=%ld,bdsA1=%ld,bdsA2=%ld,bdsTgd1=%ld",
        pNav->clockModel.choice.bdsClockModel.bdsToc,pNav->clockModel.choice.bdsClockModel.bdsA0,
        pNav->clockModel.choice.bdsClockModel.bdsA1,pNav->clockModel.choice.bdsClockModel.bdsA2,
        pNav->clockModel.choice.bdsClockModel.bdsTgd1);
        D("bdsURAI=%ld,bdsToe=%ld,bdsAPowerHalf=%ld,bdsE=%ld,bdsW=%ld,bdsDeltaN=%ld,bdsM0=%ld,bdsOmega0=%ld",
        pNav->orbitModel.choice.bdsECEF.bdsURAI,pNav->orbitModel.choice.bdsECEF.bdsToe,
        pNav->orbitModel.choice.bdsECEF.bdsAPowerHalf,pNav->orbitModel.choice.bdsECEF.bdsE,
        pNav->orbitModel.choice.bdsECEF.bdsW,pNav->orbitModel.choice.bdsECEF.bdsDeltaN,
        pNav->orbitModel.choice.bdsECEF.bdsM0,pNav->orbitModel.choice.bdsECEF.bdsOmega0);
        D("bdsOmegaDot=%ld,bdsI0=%ld,bdsIDot=%ld,bdsCuc=%ld,bdsCus=%ld,bdsCrc=%ld,bdsCrs=%ld,bdsCic=%ld,bdsCis=%ld,",
        pNav->orbitModel.choice.bdsECEF.bdsOmegaDot,pNav->orbitModel.choice.bdsECEF.bdsI0,
        pNav->orbitModel.choice.bdsECEF.bdsIDot,pNav->orbitModel.choice.bdsECEF.bdsCuc,
        pNav->orbitModel.choice.bdsECEF.bdsCus,pNav->orbitModel.choice.bdsECEF.bdsCrc,
        pNav->orbitModel.choice.bdsECEF.bdsCrs,pNav->orbitModel.choice.bdsECEF.bdsCic,
        pNav->orbitModel.choice.bdsECEF.bdsCis);
        #endif
    }   

    GREENEYE2_LOGD(("[GE2]send bds data,lenth is %d",Pak.length));
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_BDSEPHEMERIS_SUBTYPE;    
    gps_adingData(&Pak);
    return 0;
}
// pEph point the aglonass eph; pTimePose : point the server time pos
static int GlonassEphIsValid(GE2_GLONASS_EPHEMERIS* pEph,AGNSSTIMEPOS* pTimePose)
{
    GE2_LLH llh_pos;
    GE2_KINEMATIC_INFO ecef_pos;
    GE2_KINEMATIC_INFO eph_pos;
    DOUBLE n;
    DOUBLE dx,dy,dz,p;
    DOUBLE Distance,R,S;
    DOUBLE SinE1;
    FLOAT el;
    int ret = 0;

    llh_pos.lat = pTimePose->RxLocation.latitude*PI/180;
    llh_pos.lon = pTimePose->RxLocation.longitude*PI/180;
    llh_pos.hae = pTimePose->RxLocation.altitude;
    n = WGS_AXIS_A/sqrt(1.0 -WGS_E1_SQR*sin(llh_pos.lat)*sin(llh_pos.lat));
    ecef_pos.x = (n + llh_pos.hae)*cos(llh_pos.lat)*cos(llh_pos.lon);
    ecef_pos.y = (n + llh_pos.hae)*cos(llh_pos.lat)*sin(llh_pos.lon);
    ecef_pos.z = (n*(1.0 - WGS_E1_SQR)+ llh_pos.hae)*sin(llh_pos.lat);

    //eph pos   
    dx = ecef_pos.x - pEph->x;
    dy = ecef_pos.y - pEph->y;
    dz = ecef_pos.z - pEph->z;
    p = pEph->x*pEph->x + pEph->y*pEph->y;
    Distance = sqrt(dx*dx + dy*dy + dz*dz);
    R = sqrt(p + ecef_pos.z *ecef_pos.z );
    S = ecef_pos.x*dx + (FLOAT)ecef_pos.y*dy;
    SinE1 = (-S - (FLOAT)ecef_pos.z*dz)/R/Distance;
    if(R < 1e-10 || ABS(SinE1) > 1.0)
    {
        el = (FLOAT)(PI/2);
        GREENEYE2_LOGD(("[GE2]GlonassEphIsValid el1 = %f",el));
    }
    else
    {
        el = asin(SinE1);
        GREENEYE2_LOGD(("[GE2]GlonassEphIsValid el2 = %f",el));
    }

    if(el > 0)
    {
        ret = 1;
    } 

   return ret;     
}

void CG_SendGlonassEph(int count)
{
    int ephLen = sizeof(GE2_GLONASS_EPHEMERIS);
    int i,j = 0;
    int packcount = 0;
    int ret = 0;
    GpsState*  s = _gps_state;
    unsigned int agpsmode = 1;    
    for(i = 0; i< count; i++)
    {
        if(!glonass_eph[i].flag)
        {
            GREENEYE2_LOGD(("[GE2]glonass_eph count: %d",i));
            break;
        }
        
        if(j%7 == 0)
        {
            memset(&Pak,0,sizeof(TCmdData_t));  
            Pak.type = GNSS_LIBGPS_FLASH_TYPE;
            Pak.subType = GNSS_LIBGPS_FLASH_GLO_EPH1_SUBTYPE + packcount;
        }
        if(s->cmcc == 0)
        {
            ret = GlonassEphIsValid(&glonass_eph[i],&timepos_0);
        }
        else
        {
            ret = 1;
        }
        if(ret)
        {
            memcpy((Pak.buff+4+ephLen*j),&glonass_eph[i],ephLen);
            Pak.length += ephLen;
            j++;
            
            if(j%7 == 0)
            {
                GREENEYE2_LOGD(("[GE2] send GLONASS EPH packet:%d, type = %d,subType = %d",
                packcount,Pak.type, Pak.subType));
		Pak.length = Pak.length+4;
		memcpy(Pak.buff,&agpsmode,4);  //for glonass agpsmode
                gps_adingData(&Pak); 
                SCI_Sleep(200);
                packcount++;
                Pak.length = 0;                        
                j = 0;
            }
        }
    }

    if(Pak.length > 0) //the last packet 
    {
        Pak.length = 7*ephLen+4;
		memcpy(Pak.buff,&agpsmode,4);
        gps_adingData(&Pak); 
        SCI_Sleep(200);
        GREENEYE2_LOGD(("[GE2] send GLONASS packet:%d,subType = %d ,ephcount =%d",packcount,Pak.subType,j));
    }
    
}
int CG_deliverGNSSAssData(QX_agnssdata_client_t *pAssData)
{
    int i = 0,gcount = 0,ephnum = 0;
    GpsState*  s = _gps_state;
    char rfchnl[24] = {1,-4,5,6,1,-4,5,6,-6,-7,0,-1,-2,-7,0,-1,4,-3,3,2,4,-3,3,2};
    double uncertain;
    Cli_NavModelOfSatellite_t *pNav;
    Cli_GlnNavModel_t *pGloNav;
	
   // extern int gps_adingData(TCmdData_t *pPak);
    GREENEYE2_LOGD(("[GE2]agps data received"));

    if(pAssData == NULL)
    {
        GREENEYE2_LOGD(("[GE2]pAssData is null"));
        return 1;
    }
    if(pAssData->statusCode != 0)
    {
    	GREENEYE2_LOGD(("[GE2]statusCode error: %d",pAssData->statusCode));
        return 1;
    }
    loc_time.weekNo = pAssData->reftime.gpsWeek;
    loc_time.second = (unsigned int)pAssData->reftime.gpsTOW23b*0.08;  //second
    loc_time.secondFrac = ((double)pAssData->reftime.gpsTOW23b*0.08-loc_time.second)*1000000000;
    loc_time.lat = pAssData->location.lat;
    loc_time.lon = pAssData->location.lon;	
    loc_time.hae = pAssData->location.height;    
    //time get
    uncertain = 10*(pow(1.1,pAssData->location.unc)-1);
    if(uncertain > 100000)
    {
	GREENEYE2_LOGD(("[GE2]uncertain is too large:%lf",uncertain));
        return 1;
    }
    GREENEYE2_LOGD(("[GE2]time week:%d, lat:%d,lon:%d",loc_time.weekNo,(int)loc_time.lat,(int)loc_time.lon));

    Pak.length = sizeof(WLLH_TYPE);
    Pak.type = GNSS_LIBGPS_AIDING_DATA_TYPE;
    Pak.subType = GNSS_LIBGPS_AIDING_POSVELTIME_SUBTYPE;
   GREENEYE2_LOGD(("[GE2]send wllh data,length is %d",Pak.length));
    memcpy(Pak.buff,&loc_time,sizeof(WLLH_TYPE));
    gps_adingData(&Pak);   //send time to firmware

    memset(&gnsseph,0,sizeof(TCgAgpsEphemerisCollection_t));
        //D("[GE2]genericAssDataList gnssnum=%d,satnum=%d",pAssData->genericAssDataList->gnssNum,gpsAssist->navModel->satNum);
    gnsseph.count = pAssData->gpsData.numEph;
    for(i = 0; i < gnsseph.count; i++)
    {
        pNav = &pAssData->gpsData.gpsEph[i];
       // D("[GE2]SVID:%d,clockModel present:%d,orbitpresent:%d",pNav->svID,pNav->clockModel.present,pNav->orbitModel.present);
        gnsseph.satellite[i].SatID = pNav->prn;
        gnsseph.satellite[i].SatelliteStatus = 0;    //test
        gnsseph.satellite[i].week_no = loc_time.weekNo;
        gnsseph.satellite[i].IODE = pNav->ephParameter.ephemIODC;
        gnsseph.satellite[i].t_oc = pNav->ephParameter.ephemToc;
        gnsseph.satellite[i].af0 = pNav->ephParameter.ephemAF0;
        gnsseph.satellite[i].af1 = pNav->ephParameter.ephemAF1;
        gnsseph.satellite[i].af2 = pNav->ephParameter.ephemAF2;
        gnsseph.satellite[i].tgd = pNav->ephParameter.ephemTgd;
        gnsseph.satellite[i].C_rs = pNav->ephParameter.ephemCrs;
        gnsseph.satellite[i].C_is = pNav->ephParameter.ephemCis;
        gnsseph.satellite[i].C_us = pNav->ephParameter.ephemCus;
        gnsseph.satellite[i].C_rc = pNav->ephParameter.ephemCrc;
        gnsseph.satellite[i].C_ic = pNav->ephParameter.ephemCic;
        gnsseph.satellite[i].C_uc = pNav->ephParameter.ephemCuc;
        gnsseph.satellite[i].OMEGA_0 = pNav->ephParameter.ephemOmegaA0;
        gnsseph.satellite[i].I_0 = pNav->ephParameter.ephemI0;
        gnsseph.satellite[i].SquareA = pNav->ephParameter.ephemAPowerHalf;
        gnsseph.satellite[i].Idot = pNav->ephParameter.ephemIDot;
        gnsseph.satellite[i].e = pNav->ephParameter.ephemE;
        gnsseph.satellite[i].OMEGA_dot = pNav->ephParameter.ephemOmegaADot;
        gnsseph.satellite[i].M_0 = pNav->ephParameter.ephemM0;
        gnsseph.satellite[i].Del_n = pNav->ephParameter.ephemDeltaN;
        gnsseph.satellite[i].omega = pNav->ephParameter.ephemW;
        gnsseph.satellite[i].toe = pNav->ephParameter.ephemToe;
        //gnsseph.satellite[i]. = pNav->orbitModel.choice.navKeplerianSet.navFitFlag;
        //gnsseph.satellite[i]. = pNav->orbitModel.choice.navKeplerianSet.navURA;
    }
    gps_sendData(EPHEMERIS,(const char *)&gnsseph); 
SCI_Sleep(200);
    gcount = pAssData->glnData.numEph;
//glonass
    if( gcount > 0)
    {
        for(i = 0; i < gcount; i++)
        {
            pGloNav = &pAssData->glnData.glnEph[i];
            GREENEYE2_LOGD(("[GE2]glonass:svid=%d",pGloNav->svID));
            glonass_eph[i].flag = 1;
            
            if(pGloNav->svID < 25)
            {
               GREENEYE2_LOGD(("[GE2]glonass only,not include auxiliaryInfo data"));
                glonass_eph[i].freq = rfchnl[pGloNav->svID-1];
                
            }
            else
            {
                GREENEYE2_LOGD(("[GE2]error:glonass svid is over 25"));
            }
            
            //glonass_eph[i].freq = tempauxi->auxInfo[i].gloChannelnumber;
           GREENEYE2_LOGD(("[GE2]glonass freq=%d",glonass_eph[i].freq));
            glonass_eph[i].M = pGloNav->navParams.gloM;
            glonass_eph[i].Ft = pGloNav->Ft;
            glonass_eph[i].n  = pGloNav->svID;
            glonass_eph[i].Bn = pGloNav->Bn;
            glonass_eph[i].En = pGloNav->navParams.gloEn;
            glonass_eph[i].tb = pGloNav->tb*60;
            //D("m=%d,ft=%d,bn=%d,en=%d,tb=%d",glonass_eph[i].M,glonass_eph[i].Ft,glonass_eph[i].Bn,glonass_eph[i].En,glonass_eph[i].tb);
            glonass_eph[i].gamma = (double)pGloNav->navParams.gloGamma/(double)((long long)1<<40);
            //D("gamma is %f",glonass_eph[i].gamma);
            glonass_eph[i].tn = (double)pGloNav->navParams.gloTau/(double)(1<<30);
            glonass_eph[i].dtn = (double)pGloNav->navParams.gloDeltaTau/(double)(1<<30);
            glonass_eph[i].x = (double)(pGloNav->navParams.gloX)/(double)(1<<11);
            glonass_eph[i].x = glonass_eph[i].x*1000.0;
            //D("glox %ld,x value is %f",pNav->orbitModel.choice.glonassECEF.gloX,glonass_eph[i].x);
            glonass_eph[i].y = (double)(pGloNav->navParams.gloY)/(double)(1<<11);
            glonass_eph[i].y = glonass_eph[i].y*1000.0;
            //D("gloy %ld,y value is %f",pNav->orbitModel.choice.glonassECEF.gloY,glonass_eph[i].y);
            glonass_eph[i].z = (double)(pGloNav->navParams.gloZ)/(double)(1<<11);
            glonass_eph[i].z = glonass_eph[i].z*1000.0;
            //D("gloz %ld,z value is %f",pNav->orbitModel.choice.glonassECEF.gloZ,glonass_eph[i].z);
            glonass_eph[i].vx = (double)(pGloNav->navParams.gloXdot)/(double)(1<<20);
            glonass_eph[i].vx = glonass_eph[i].vx * 1000.0;
            glonass_eph[i].vy = (double)(pGloNav->navParams.gloYdot)/(double)(1<<20);
            glonass_eph[i].vy = glonass_eph[i].vy * 1000.0;
            glonass_eph[i].vz = (double)(pGloNav->navParams.gloZdot)/(double)(1<<20);
            glonass_eph[i].vz = glonass_eph[i].vz * 1000.0;
            glonass_eph[i].ax = (double)(pGloNav->navParams.gloXdotdot*1000)/(double)(1<<30);
            glonass_eph[i].ay = (double)(pGloNav->navParams.gloYdotdot*1000)/(double)(1<<30);
            glonass_eph[i].az = (double)(pGloNav->navParams.gloZdotdot*1000)/(double)(1<<30);
            
        }
        CG_SendGlonassEph(gcount);
    }
    GREENEYE2_LOGD(("[GE2]bds data number is %d",pAssData->bdsData.numEph));
    if(pAssData->bdsData.numEph > 0){
        GREENEYE2_LOGD(("[GE2]begin send beidou assist data"));
        BeidouAssData(&pAssData->bdsData);
    }
    return 0;
}

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


