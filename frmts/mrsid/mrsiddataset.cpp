/******************************************************************************
 * $Id$
 *
 * Project:  Multi-resolution Seamless Image Database (MrSID)
 * Purpose:  Read/write LizardTech's MrSID file format - Version 4+ SDK.
 * Author:   Andrey Kiselev, dron@remotesensing.org
 *
 ******************************************************************************
 * Copyright (c) 2003, Andrey Kiselev <dron@remotesensing.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log$
 * Revision 1.57  2006/05/19 04:53:41  fwarmerdam
 * Update to debug output.
 *
 * Revision 1.56  2006/05/19 04:32:43  fwarmerdam
 * moved the mrsidcommon.h stuff out of the MRSID_ESDK ifdef.
 *
 * Revision 1.55  2006/05/19 02:56:29  fwarmerdam
 * added temporary debugging stuff.
 *
 * Revision 1.54  2006/05/19 02:50:58  fwarmerdam
 * moved mrsidcommon.h back in here
 *
 * Revision 1.53  2006/04/28 18:56:21  fwarmerdam
 * fixed up v5/v6 compatibility
 *
 * Revision 1.52  2006/03/03 04:30:13  fwarmerdam
 * added XMLPROFILE creation option
 *
 * Revision 1.51  2006/02/28 19:30:55  fwarmerdam
 * Use LTI_COLORSPACE_MULTISPECTRAL for anything other than 1 or 3 bands.
 *
 * Revision 1.50  2006/02/21 22:42:11  fwarmerdam
 * Added ESDK 6 support.
 *
 * Revision 1.49  2006/02/19 03:01:24  fwarmerdam
 * Improved error checking in writer code.
 *
 * Revision 1.48  2006/01/31 17:44:46  fwarmerdam
 * jpc_header used even when j2k support disabled.
 *
 * Revision 1.47  2006/01/09 18:59:33  fwarmerdam
 * ensure jp2mrsid driver support jpc (non-jP2 JPEG2000 files)
 *
 * Revision 1.46  2005/11/10 21:05:01  fwarmerdam
 * Applied Rostics fix for slight overview size inaccuracies
 *
 * Revision 1.45  2005/10/17 19:30:47  fwarmerdam
 * Fixed serious bug in MrSID overview access.
 *
 * Revision 1.44  2005/09/16 22:09:18  fwarmerdam
 * Ensure SetDescription() is called before OpenZoom.
 *
 * Revision 1.43  2005/09/15 02:17:34  fwarmerdam
 * note MRSID_HAVE_GETWKT macro.
 *
 * Revision 1.42  2005/09/15 00:46:21  fwarmerdam
 * Fixed help topic link.
 *
 * Revision 1.41  2005/09/15 00:31:27  fwarmerdam
 * added JP2MrSID driver, fixedup ESDK 5.x support
 *
 * Revision 1.40  2005/09/09 02:27:28  fwarmerdam
 * Fallback to PAM if projection string is empty.
 *
 * Revision 1.39  2005/08/26 14:48:33  fwarmerdam
 * Do not return nodata on bands since MrSID NODATA semantics are
 * across all bands.   Note that IMAGE__TRANSPARENT_DATA_VALUE metadata
 * on the dataset holds the set of values for application use if needed.
 *
 * Revision 1.38  2005/07/22 15:50:10  fwarmerdam
 * Use blocked io for very small request buffers as well as small windows.
 *
 * Revision 1.37  2005/07/21 17:30:13  fwarmerdam
 * Added preliminary worldfile support.
 *
 * Revision 1.36  2005/07/14 23:40:31  fwarmerdam
 * getNumLevels() is the number of overviews
 *
 * Revision 1.35  2005/06/24 15:54:30  dron
 * Properly fetch NODATA taking in account data type of the value.
 *
 * Revision 1.34  2005/05/13 01:12:25  fwarmerdam
 * Fixed bug with block oriented access to 16bit data.
 *
 * Revision 1.33  2005/05/05 15:54:49  fwarmerdam
 * PAM Enabled
 *
 * Revision 1.32  2005/05/04 17:21:00  fwarmerdam
 * Avoid 32bit overflow when testing xsize*ysize.
 *
 * Revision 1.31  2005/04/21 16:43:26  fwarmerdam
 * Fixed bug with RasterIO() at overview levels higher than is available
 * in the file.  Now the code limits itself to available overview levels.
 *
 * Revision 1.30  2005/04/11 17:03:07  fwarmerdam
 * Improved initial open checking to ensure it works properly with no header
 * data at all.
 *
 * Revision 1.29  2005/04/11 14:41:14  fwarmerdam
 * Only call getWKT() if HAVE_MRSID_GETWKT is defined.
 *
 * Revision 1.28  2005/04/06 22:03:17  kmelero
 * Updated for CPLStrdup and mod to pointers for large file support per
 * LizardTech Mrsid Team.  (kmelero@sanz.com)
 *
 * Revision 1.27  2005/04/02 19:26:08  kmelero
 * Added read support for WKT.  (kmelero@sanz.com)
 *
 * Revision 1.26  2005/04/02 19:04:00  kmelero
 * Added setting WKT in MrSID file.  (kmelero@sanz.com)
 *
 * Revision 1.25  2005/03/26 19:29:21  kmelero
 * Added flag to set 64-bit fwrite in support of large MrSID files.  (kmelero@sanz.com)
 *
 * Revision 1.24  2005/03/25 15:02:22  kmelero
 * Removed misplaced < character.  Was causing compiler error. (kmelero@sanz.com)
 *
 * Revision 1.23  2005/02/17 22:17:36  fwarmerdam
 * some adjustments to deciding whether to use direct io or not
 *
 * Revision 1.22  2005/02/16 22:08:21  fwarmerdam
 * split out v3 driver, added dataset::IRasterIO
 *
 * Revision 1.21  2005/02/12 17:52:28  kmelero
 * Rewrote CreateCopy to support v2 and v3 of MrSID.  (kmelero@sanz.com)
 *
 * Revision 1.20  2005/02/06 18:40:05  dron
 * Fixes in colorspace handling when the new file creating; use WORLDFILE option.
 *
 * Revision 1.19  2005/02/02 14:58:46  fwarmerdam
 * Added a few debug statements.
 *
 * Revision 1.18  2004/11/14 21:18:22  dron
 * Initial support for MrSID Encoding SDK.
 */

#include "gdal_pam.h"
#include "ogr_spatialref.h"
#include "cpl_string.h"
#include <string>

#include <geo_normalize.h>
#include <geovalues.h>

CPL_CVSID("$Id$");

CPL_C_START
double GTIFAngleToDD( double dfAngle, int nUOMAngle );
CPL_C_END

// Key Macros from Makefile:
//   MRSID_ESDK: Means we have the encoding SDK (version 5 or newer required)
//   MRSID_J2K: Means we are enabling MrSID SDK JPEG2000 support. 

#include "lt_types.h"
#include "lt_base.h"
#include "lt_fileSpec.h"
#include "lt_ioFileStream.h"
#include "lt_utilStatusStrings.h"
#include "lti_geoCoord.h"
#include "lti_pixel.h"
#include "lti_navigator.h"
#include "lti_sceneBuffer.h"
#include "lti_metadataDatabase.h"
#include "lti_metadataRecord.h"
#include "lti_utils.h"
#include "lt_utilStatus.h"
#include "MrSIDImageReader.h"

#ifdef MRSID_J2K
#  include "J2KImageReader.h"
#endif

// It seems that LT_STS_UTIL_TimeUnknown was added in version 6, also
// the first version with lti_version.h
#ifdef LT_STS_UTIL_TimeUnknown
#  include "lti_version.h"
#endif

// Are we using version 6 or newer?
#if defined(LTI_SDK_MAJOR) && LTI_SDK_MAJOR >= 6
#  define MRSID_POST5
#endif

#ifdef MRSID_ESDK
# include "MG3ImageWriter.h"
# include "MG3WriterParams.h"
# include "MG2ImageWriter.h"
# include "MG2WriterParams.h"
# ifdef MRSID_J2K
#   ifdef MRSID_POST5
#     include "JP2WriterManager.h"
#     include "JPCWriterParams.h"
#   else
#     include "J2KImageWriter.h"
#     include "J2KWriterParams.h"
#   endif
# endif
#endif /* MRSID_ESDK */

#ifdef MRSID_POST5
#  define MRSID_HAVE_GETWKT
#endif

LT_USE_NAMESPACE(LizardTech)

/************************************************************************/
/* ==================================================================== */
/*                              MrSIDDataset                            */
/* ==================================================================== */
/************************************************************************/

class MrSIDDataset : public GDALPamDataset
{
    friend class MrSIDRasterBand;

    LTIImageReader      *poImageReader;

#ifdef MRSID_ESDK
    LTIGeoFileImageWriter *poImageWriter;
#endif

    LTINavigator        *poLTINav;
    LTIMetadataDatabase *poMetadata;
    const LTIPixel      *poNDPixel;

    LTISceneBuffer      *poBuffer;
    int                 nBlockXSize, nBlockYSize;
    int                 bPrevBlockRead;
    int                 nPrevBlockXOff, nPrevBlockYOff;

    LTIDataType         eSampleType;
    GDALDataType        eDataType;
    LTIColorSpace       eColorSpace;

    double              dfCurrentMag;

    int                 bHasGeoTransform;
    double              adfGeoTransform[6];
    char                *pszProjection;
    GTIFDefn            *psDefn;

    MrSIDDataset       *poParentDS;
    int                 bIsOverview;
    int                 nOverviewCount;
    MrSIDDataset        **papoOverviewDS;

    CPLErr              OpenZoomLevel( lt_int32 iZoom );
    char                *SerializeMetadataRec( const LTIMetadataRecord* );
    int                 GetMetadataElement( const char *, void *, int=0 );
    void                FetchProjParms();
    void                GetGTIFDefn();
    char                *GetOGISDefn( GTIFDefn * );

    virtual CPLErr      IRasterIO( GDALRWFlag, int, int, int, int, void *,
                                   int, int, GDALDataType, int, int *,int,
                                   int, int );

  public:
                MrSIDDataset();
                ~MrSIDDataset();

    static GDALDataset  *Open( GDALOpenInfo * );
    virtual CPLErr      GetGeoTransform( double * padfTransform );
    const char          *GetProjectionRef();

#ifdef MRSID_ESDK
    static GDALDataset  *Create( const char * pszFilename,
                                 int nXSize, int nYSize, int nBands,
                                 GDALDataType eType, char ** papszParmList );
    virtual void        FlushCache( void );
#endif
};

/************************************************************************/
/* ==================================================================== */
/*                           MrSIDRasterBand                            */
/* ==================================================================== */
/************************************************************************/

class MrSIDRasterBand : public GDALPamRasterBand
{
    friend class MrSIDDataset;

    LTIPixel        *poPixel;

    int             nBlockSize;

    int             bNoDataSet;
    double          dfNoDataValue;

  public:

                MrSIDRasterBand( MrSIDDataset *, int );
                ~MrSIDRasterBand();

    virtual CPLErr IRasterIO( GDALRWFlag, int, int, int, int,
                              void *, int, int, GDALDataType,
                              int, int );

    virtual CPLErr          IReadBlock( int, int, void * );
    virtual GDALColorInterp GetColorInterpretation();
    virtual double	    GetNoDataValue( int * );
    virtual int             GetOverviewCount();
    virtual GDALRasterBand  *GetOverview( int );

#ifdef MRSID_ESDK
    virtual CPLErr          IWriteBlock( int, int, void * );
#endif
};

/************************************************************************/
/*                           MrSIDRasterBand()                          */
/************************************************************************/

MrSIDRasterBand::MrSIDRasterBand( MrSIDDataset *poDS, int nBand )
{
    this->poDS = poDS;
    this->nBand = nBand;
    this->eDataType = poDS->eDataType;

/* -------------------------------------------------------------------- */
/*      Set the block sizes and buffer parameters.                      */
/* -------------------------------------------------------------------- */
    nBlockXSize = poDS->nBlockXSize;
    nBlockYSize = poDS->nBlockYSize;
//#ifdef notdef
    if( poDS->GetRasterXSize() > 2048 )
        nBlockXSize = 1024;
    if( poDS->GetRasterYSize() > 128 )
        nBlockYSize = 128;
    else
        nBlockYSize = poDS->GetRasterYSize();
//#endif

    nBlockSize = nBlockXSize * nBlockYSize;
    poPixel = new LTIPixel( poDS->eColorSpace, poDS->nBands,
                            poDS->eSampleType );


/* -------------------------------------------------------------------- */
/*      Set NoData values.                                              */
/*                                                                      */
/*      This logic is disabled for now since the MrSID nodata           */
/*      semantics are different than GDAL.  In MrSID all bands must     */
/*      match the nodata value for that band in order for the pixel     */
/*      to be considered nodata, otherwise all values are valid.        */
/* -------------------------------------------------------------------- */
#ifdef notdef
     if ( poDS->poNDPixel )
     {
         switch( poDS->eSampleType )
         {
             case LTI_DATATYPE_UINT8:
             case LTI_DATATYPE_SINT8:
                 dfNoDataValue = (double)
                     poDS->poNDPixel->getSampleValueUint8( nBand - 1 );
                 break;
             case LTI_DATATYPE_UINT16:
                 dfNoDataValue = (double)
                     poDS->poNDPixel->getSampleValueUint16( nBand - 1 );
                 break;
             case LTI_DATATYPE_FLOAT32:
                 dfNoDataValue =
                     poDS->poNDPixel->getSampleValueFloat32( nBand - 1 );
                 break;
             case LTI_DATATYPE_SINT16:
                 dfNoDataValue = (double)
                     *(GInt16 *)poDS->poNDPixel->getSampleValueAddr( nBand - 1 );
                 break;
             case LTI_DATATYPE_UINT32:
                 dfNoDataValue = (double)
                     *(GUInt32 *)poDS->poNDPixel->getSampleValueAddr( nBand - 1 );
                 break;
             case LTI_DATATYPE_SINT32:
                 dfNoDataValue = (double)
                     *(GInt32 *)poDS->poNDPixel->getSampleValueAddr( nBand - 1 );
                 break;
             case LTI_DATATYPE_FLOAT64:
                 dfNoDataValue =
                     *(double *)poDS->poNDPixel->getSampleValueAddr( nBand - 1 );
                 break;

             case LTI_DATATYPE_INVALID:
                 CPLAssert( FALSE );
                 break;
         }
	 bNoDataSet = TRUE;
     }
     else
#endif
     {
	dfNoDataValue = 0.0;
        bNoDataSet = FALSE;
     }
}

/************************************************************************/
/*                            ~MrSIDRasterBand()                        */
/************************************************************************/

MrSIDRasterBand::~MrSIDRasterBand()
{
    if ( poPixel )
        delete poPixel;
}

/************************************************************************/
/*                             IReadBlock()                             */
/************************************************************************/

CPLErr MrSIDRasterBand::IReadBlock( int nBlockXOff, int nBlockYOff,
                                    void * pImage )
{
    MrSIDDataset    *poGDS = (MrSIDDataset *)poDS;

#ifdef MRSID_ESDK
    if( poGDS->eAccess == GA_Update )
    {
        CPLDebug( "MrSID", 
                  "IReadBlock() - DSDK - read on updatable file fails." );
        memset( pImage, 0, nBlockSize * GDALGetDataTypeSize(eDataType) / 8 );
        return CE_None;
    }
#endif /* MRSID_ESDK */

    CPLDebug( "MrSID", "IReadBlock(%d,%d)", nBlockXOff, nBlockYOff );

    if ( !poGDS->bPrevBlockRead
         || poGDS->nPrevBlockXOff != nBlockXOff
         || poGDS->nPrevBlockYOff != nBlockYOff )
    {
        GInt32      nLine = nBlockYOff * nBlockYSize;
        GInt32      nCol = nBlockXOff * nBlockXSize;

        // XXX: The scene, passed to LTIImageStage::read() call must be
        // inside the image boundaries. So we should detect the last strip and
        // form the scene properly.
        CPLDebug( "MrSID", 
                  "IReadBlock - read() %dx%d block at %d,%d.", 
                  nBlockXSize, nBlockYSize, nCol, nLine );
                  
        if(!LT_SUCCESS( poGDS->poLTINav->setSceneAsULWH(
                            nCol, nLine,
                            (nCol+nBlockXSize>poGDS->GetRasterXSize())?
                            (poGDS->GetRasterXSize()-nCol):nBlockXSize,
                            (nLine+nBlockYSize>poGDS->GetRasterYSize())?
                            (poGDS->GetRasterYSize()-nLine):nBlockYSize,
                            poGDS->dfCurrentMag) ))
            
        {
            CPLError( CE_Failure, CPLE_AppDefined,
            "MrSIDRasterBand::IReadBlock(): Failed to set scene position." );
            return CE_Failure;
        }

        if ( !poGDS->poBuffer )
        {
            poGDS->poBuffer =
                new LTISceneBuffer( *poPixel, nBlockXSize, nBlockYSize, NULL );
        }

        if(!LT_SUCCESS(poGDS->poImageReader->read(poGDS->poLTINav->getScene(),
                                                  *poGDS->poBuffer)))
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "MrSIDRasterBand::IReadBlock(): Failed to load image." );
            return CE_Failure;
        }

        poGDS->bPrevBlockRead = TRUE;
        poGDS->nPrevBlockXOff = nBlockXOff;
        poGDS->nPrevBlockYOff = nBlockYOff;
    }

    memcpy( pImage, poGDS->poBuffer->getTotalBandData(nBand - 1), 
            nBlockSize * (GDALGetDataTypeSize(poGDS->eDataType) / 8) );

    return CE_None;
}

#ifdef MRSID_ESDK

/************************************************************************/
/*                            IWriteBlock()                             */
/************************************************************************/

CPLErr MrSIDRasterBand::IWriteBlock( int nBlockXOff, int nBlockYOff,
                                     void * pImage )
{
    MrSIDDataset    *poGDS = (MrSIDDataset *)poDS;

    CPLAssert( poGDS != NULL
               && nBlockXOff >= 0
               && nBlockYOff >= 0
               && pImage != NULL );

#if DEBUG
    CPLDebug( "MrSID", "IWriteBlock(): nBlockXOff=%d, nBlockYOff=%d",
              nBlockXOff, nBlockYOff );
#endif

    LTIScene        oScene( nBlockXOff * nBlockXSize,
                            nBlockYOff * nBlockYSize,
                            nBlockXSize, nBlockYSize, 1.0);
    LTISceneBuffer  oSceneBuf( *poPixel, poGDS->nBlockXSize,
                               poGDS->nBlockYSize, &pImage );

    if( !LT_SUCCESS(poGDS->poImageWriter->writeBegin(oScene)) )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDRasterBand::IWriteBlock(): writeBegin failed." );
        return CE_Failure;
    }

    if( !LT_SUCCESS(poGDS->poImageWriter->writeStrip(oSceneBuf, oScene)) )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDRasterBand::IWriteBlock(): writeStrip failed." );
        return CE_Failure;
    }

    if( !LT_SUCCESS(poGDS->poImageWriter->writeEnd()) )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDRasterBand::IWriteBlock(): writeEnd failed." );
        return CE_Failure;
    }

    return CE_None;
}

#endif /* MRSID_ESDK */

/************************************************************************/
/*                             IRasterIO()                              */
/************************************************************************/

CPLErr MrSIDRasterBand::IRasterIO( GDALRWFlag eRWFlag,
                                   int nXOff, int nYOff, int nXSize, int nYSize,
                                   void * pData, int nBufXSize, int nBufYSize,
                                   GDALDataType eBufType,
                                   int nPixelSpace, int nLineSpace )
    
{
    MrSIDDataset *poGDS = (MrSIDDataset *) poDS;

/* -------------------------------------------------------------------- */
/*      Fallback to default implementation if the whole scanline        */
/*      without subsampling requested.                                  */
/* -------------------------------------------------------------------- */
    if ( nXSize == poGDS->GetRasterXSize()
         && nXSize == nBufXSize
         && nYSize == nBufYSize )
    {
        return GDALRasterBand::IRasterIO( eRWFlag, nXOff, nYOff,
                                          nXSize, nYSize, pData,
                                          nBufXSize, nBufYSize, eBufType,
                                          nPixelSpace, nLineSpace );
    }

/* -------------------------------------------------------------------- */
/*      Handle via the dataset level IRasterIO()                        */
/* -------------------------------------------------------------------- */
    return poGDS->IRasterIO( eRWFlag, nXOff, nYOff, nXSize, nYSize, pData, 
                             nBufXSize, nBufYSize, eBufType, 
                             1, &nBand, nPixelSpace, nLineSpace, 0 );
}

/************************************************************************/
/*                       GetColorInterpretation()                       */
/************************************************************************/

GDALColorInterp MrSIDRasterBand::GetColorInterpretation()
{
    MrSIDDataset      *poGDS = (MrSIDDataset *) poDS;

    switch( poGDS->eColorSpace )
    {
        case LTI_COLORSPACE_RGB:
            if( nBand == 1 )
                return GCI_RedBand;
            else if( nBand == 2 )
                return GCI_GreenBand;
            else if( nBand == 3 )
                return GCI_BlueBand;
            else
                return GCI_Undefined;
        case LTI_COLORSPACE_RGBK:
            if( nBand == 1 )
                return GCI_RedBand;
            else if( nBand == 2 )
                return GCI_GreenBand;
            else if( nBand == 3 )
                return GCI_BlueBand;
            else if( nBand == 4 )
                return GCI_AlphaBand;
            else
                return GCI_Undefined;
        case LTI_COLORSPACE_CMYK:
            if( nBand == 1 )
                return GCI_CyanBand;
            else if( nBand == 2 )
                return GCI_MagentaBand;
            else if( nBand == 3 )
                return GCI_YellowBand;
            else if( nBand == 4 )
                return GCI_BlackBand;
            else
                return GCI_Undefined;
        case LTI_COLORSPACE_GRAYSCALE:
            return GCI_GrayIndex;
        default:
            return GCI_Undefined;
    }

}

/************************************************************************/
/*                           GetNoDataValue()                           */
/************************************************************************/

double MrSIDRasterBand::GetNoDataValue( int * pbSuccess )

{
    if( pbSuccess )
        *pbSuccess = bNoDataSet;

    return dfNoDataValue;
}

/************************************************************************/
/*                          GetOverviewCount()                          */
/************************************************************************/

int MrSIDRasterBand::GetOverviewCount()

{
    MrSIDDataset        *poGDS = (MrSIDDataset *) poDS;

    return poGDS->nOverviewCount;
}

/************************************************************************/
/*                            GetOverview()                             */
/************************************************************************/

GDALRasterBand *MrSIDRasterBand::GetOverview( int i )

{
    MrSIDDataset        *poGDS = (MrSIDDataset *) poDS;

    if( i < 0 || i >= poGDS->nOverviewCount )
        return NULL;
    else
        return poGDS->papoOverviewDS[i]->GetRasterBand( nBand );
}

/************************************************************************/
/*                           MrSIDDataset()                             */
/************************************************************************/

MrSIDDataset::MrSIDDataset()
{
    poImageReader = NULL;
#ifdef MRSID_ESDK
    poImageWriter = NULL;
#endif
    poLTINav = NULL;
    poMetadata = NULL;
    poNDPixel = NULL;
    eSampleType = LTI_DATATYPE_UINT8;
    nBands = 0;
    eDataType = GDT_Byte;
    
    poBuffer = NULL;
    bPrevBlockRead = FALSE;
    nPrevBlockXOff = 0;
    nPrevBlockYOff = 0;
    
    pszProjection = CPLStrdup( "" );
    bHasGeoTransform = FALSE;
    adfGeoTransform[0] = 0.0;
    adfGeoTransform[1] = 1.0;
    adfGeoTransform[2] = 0.0;
    adfGeoTransform[3] = 0.0;
    adfGeoTransform[4] = 0.0;
    adfGeoTransform[5] = 1.0;
    psDefn = NULL;
    
    dfCurrentMag = 1.0;
    bIsOverview = FALSE;
    poParentDS = this;
    nOverviewCount = 0;
    papoOverviewDS = NULL;
}

/************************************************************************/
/*                            ~MrSIDDataset()                           */
/************************************************************************/

MrSIDDataset::~MrSIDDataset()
{
    FlushCache();

#ifdef MRSID_ESDK
    if ( poImageWriter )
        delete poImageWriter;
#endif

    if ( poImageReader && !bIsOverview )
        delete poImageReader;
    if ( poLTINav )
        delete poLTINav;
    if ( poBuffer )
        delete poBuffer;
    if ( poMetadata )
        delete poMetadata;
    if ( pszProjection )
        CPLFree( pszProjection );
    if ( psDefn )
        delete psDefn;
    if ( papoOverviewDS )
    {
        for( int i = 0; i < nOverviewCount; i++ )
            delete papoOverviewDS[i];
        CPLFree( papoOverviewDS );
    }
}

/************************************************************************/
/*                             IRasterIO()                              */
/************************************************************************/

CPLErr MrSIDDataset::IRasterIO( GDALRWFlag eRWFlag,
                                int nXOff, int nYOff, int nXSize, int nYSize,
                                void * pData, int nBufXSize, int nBufYSize,
                                GDALDataType eBufType, 
                                int nBandCount, int *panBandMap,
                                int nPixelSpace, int nLineSpace, int nBandSpace )

{
/* -------------------------------------------------------------------- */
/*      We need various criteria to skip out to block based methods.    */
/* -------------------------------------------------------------------- */
    int bUseBlockedIO = bForceCachedIO;

    if( nYSize == 1 || nXSize * ((double) nYSize) < 100.0 )
        bUseBlockedIO = TRUE;

    if( nBufYSize == 1 || nBufXSize * ((double) nBufYSize) < 100.0 )
        bUseBlockedIO = TRUE;

    if( CSLTestBoolean( CPLGetConfigOption( "GDAL_ONE_BIG_READ", "NO") ) )
        bUseBlockedIO = FALSE;

    if( bUseBlockedIO )
        return GDALDataset::BlockBasedRasterIO( 
            eRWFlag, nXOff, nYOff, nXSize, nYSize,
            pData, nBufXSize, nBufYSize, eBufType, 
            nBandCount, panBandMap, nPixelSpace, nLineSpace, nBandSpace );
    CPLDebug( "MrSID", "RasterIO() - using optimized dataset level IO." );
    
/* -------------------------------------------------------------------- */
/*      What is our requested window relative to the base dataset.      */
/*      We want to operate from here on as if we were operating on      */
/*      the full res band.                                              */
/* -------------------------------------------------------------------- */
    int nZoomMag = (int) ((1/dfCurrentMag) * 1.0000001);

    nXOff *= nZoomMag;
    nYOff *= nZoomMag;
    nXSize *= nZoomMag;
    nYSize *= nZoomMag;

/* -------------------------------------------------------------------- */
/*      We need to figure out the best zoom level to use for this       */
/*      request.  We apply a small fudge factor to make sure that       */
/*      request just very, very slightly larger than a zoom level do    */
/*      not force us to the next level.                                 */
/* -------------------------------------------------------------------- */
    int iOverview = 0;
    double dfZoomMag = MIN((nXSize / (double)nBufXSize), 
                           (nYSize / (double)nBufYSize));

    for( nZoomMag = 1; 
         nZoomMag * 2 < (dfZoomMag + 0.1) 
             && iOverview < poParentDS->nOverviewCount; 
         nZoomMag *= 2, iOverview++ ) {}

/* -------------------------------------------------------------------- */
/*      Work out the size of the temporary buffer and allocate it.      */
/*      The temporary buffer will generally be at a moderately          */
/*      higher resolution than the buffer of data requested.            */
/* -------------------------------------------------------------------- */
    int  nTmpPixelSize;
    LTIPixel       oPixel( eColorSpace, nBands, eSampleType );
    
    LT_STATUS eLTStatus;
    unsigned int maxWidth;
    unsigned int maxHeight;

    eLTStatus = poImageReader->getDimsAtMag(1.0/nZoomMag,maxWidth,maxHeight);

    if( !LT_SUCCESS(eLTStatus)) {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDDataset::IRasterIO(): Failed to get zoomed image dimensions.\n%s",
                  getLastStatusString( eLTStatus ) );
        return CE_Failure;
    }

    int maxWidthAtL0 = bIsOverview?poParentDS->GetRasterXSize():this->GetRasterXSize();
    int maxHeightAtL0 = bIsOverview?poParentDS->GetRasterYSize():this->GetRasterYSize();

    int sceneUlXOff = nXOff / nZoomMag;
    int sceneUlYOff = nYOff / nZoomMag;
    int sceneWidth  = (int)(nXSize * maxWidth / (double)maxWidthAtL0 + 0.99);
    int sceneHeight = (int)(nYSize * maxHeight / (double)maxHeightAtL0 + 0.99);

    if( (sceneUlXOff + sceneWidth) > (int) maxWidth )
        sceneWidth = maxWidth - sceneUlXOff;

    if( (sceneUlYOff + sceneHeight) > (int) maxHeight )
        sceneHeight = maxHeight - sceneUlYOff;

    LTISceneBuffer oLTIBuffer( oPixel, sceneWidth, sceneHeight, NULL );

    nTmpPixelSize = GDALGetDataTypeSize( eDataType ) / 8;

/* -------------------------------------------------------------------- */
/*      Create navigator, and move to the requested scene area.         */
/* -------------------------------------------------------------------- */
    LTINavigator oNav( *poImageReader );
    
    if( !LT_SUCCESS(oNav.setSceneAsULWH( sceneUlXOff, sceneUlYOff, 
                                         sceneWidth, sceneHeight, 
                                         1.0 / nZoomMag )) )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDDataset::IRasterIO(): Failed to set scene position." );

        return CE_Failure;
    }

    CPLDebug( "MrSID", 
              "Dataset:IRasterIO(%d,%d %dx%d -> %dx%d -> %dx%d, zoom=%d)",
              nXOff, nYOff, nXSize, nYSize, 
              sceneWidth, sceneHeight,
              nBufXSize, nBufYSize, 
              nZoomMag );

    if( !oNav.isSceneValid() )
        CPLDebug( "MrSID", "LTINavigator in invalid state." );

#ifdef notdef
    {
        lt_uint32 iWidth, iHeight;
        
        poImageReader->getDimsAtMag( dfCurrentMag, iWidth, iHeight );
    }
#endif

/* -------------------------------------------------------------------- */
/*      Read into the buffer.                                           */
/* -------------------------------------------------------------------- */

    eLTStatus = poImageReader->read(oNav.getScene(),oLTIBuffer);
    if(!LT_SUCCESS(eLTStatus) )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDRasterBand::IRasterIO(): Failed to load image.\n%s",
                  getLastStatusString( eLTStatus ) );
        return CE_Failure;
    }
    
/* -------------------------------------------------------------------- */
/*      Manually resample to our target buffer.                         */
/* -------------------------------------------------------------------- */
    int         iBufLine, iBufPixel;

    for( iBufLine = 0; iBufLine < nBufYSize; iBufLine++ )
    {
        int iTmpLine = (int) floor(((iBufLine+0.5) / nBufYSize) * sceneHeight);

        for( iBufPixel = 0; iBufPixel < nBufXSize; iBufPixel++ )
        {
            int iTmpPixel = (int) 
                floor(((iBufPixel+0.5) / nBufXSize) * sceneWidth);

            for( int iBand = 0; iBand < nBandCount; iBand++ )
            {
                GByte *pabySrc, *pabyDst;

                pabyDst = ((GByte *) pData) 
                    + nPixelSpace * iBufPixel
                    + nLineSpace * iBufLine
                    + nBandSpace * iBand;

                pabySrc = (GByte *) oLTIBuffer.getTotalBandData( 
                    panBandMap[iBand] - 1 );
                pabySrc += (iTmpLine * sceneWidth + iTmpPixel) * nTmpPixelSize;

                if( eDataType == eBufType )
                    memcpy( pabyDst, pabySrc, nTmpPixelSize );
                else
                    GDALCopyWords( pabySrc, eDataType, 0, 
                                   pabyDst, eBufType, 0, 1 );
            }
        }
    }

    return CE_None;
}

/************************************************************************/
/*                          GetGeoTransform()                           */
/************************************************************************/

CPLErr MrSIDDataset::GetGeoTransform( double * padfTransform )
{
    if( bHasGeoTransform )
    {
        memcpy( padfTransform, adfGeoTransform, sizeof(adfGeoTransform[0]) * 6 );
        return CE_None;
    }
    else
        return GDALPamDataset::GetGeoTransform( padfTransform );
}

/************************************************************************/
/*                          GetProjectionRef()                          */
/************************************************************************/

const char *MrSIDDataset::GetProjectionRef()
{
    if( pszProjection && strlen(pszProjection) > 0 )
        return pszProjection;
    else
        return GDALPamDataset::GetProjectionRef();
}

/************************************************************************/
/*                        SerializeMetadataRec()                        */
/************************************************************************/

char *MrSIDDataset::SerializeMetadataRec( const LTIMetadataRecord *poMetadataRec )
{
    GUInt32  iNumDims = 0;
    const GUInt32  *paiDims = NULL;
    const void     *pData = poMetadataRec->getArrayData( iNumDims, paiDims );
    GUInt32        i, j, k = 0, iLength;
    const char     *pszTemp = NULL;
    char           *pszMetadata = CPLStrdup( "" );

    for ( i = 0; i < iNumDims; i++ )
        for ( j = 0; j < paiDims[i]; j++ )
        {
            switch( poMetadataRec->getDataType() )
            {
                case LTI_METADATA_DATATYPE_UINT8:
                case LTI_METADATA_DATATYPE_SINT8:
                    pszTemp = CPLSPrintf( "%d", ((GByte *)pData)[k++] );
                    break;
                case LTI_METADATA_DATATYPE_UINT16:
                    pszTemp = CPLSPrintf( "%u", ((GUInt16 *)pData)[k++] );
                    break;
                case LTI_METADATA_DATATYPE_SINT16:
                    pszTemp = CPLSPrintf( "%d", ((GInt16 *)pData)[k++] );
                    break;
                case LTI_METADATA_DATATYPE_UINT32:
                    pszTemp = CPLSPrintf( "%lu", ((unsigned long *)pData)[k++] );
                    break;
                case LTI_METADATA_DATATYPE_SINT32:
                    pszTemp = CPLSPrintf( "%ld", ((long *)pData)[k++] );
                    break;
                case LTI_METADATA_DATATYPE_FLOAT32:
                    pszTemp = CPLSPrintf( "%f", ((float *)pData)[k++] );
                    break;
                case LTI_METADATA_DATATYPE_FLOAT64:
                    pszTemp = CPLSPrintf( "%lf", ((double *)pData)[k++] );
                    break;
                case LTI_METADATA_DATATYPE_ASCII:
                    pszTemp = ((const char **)pData)[k++];
                    break;
                default:
                    pszTemp = "";
                    break;
            }

            iLength = strlen(pszMetadata) + strlen(pszTemp) + 2;

            pszMetadata = (char *)CPLRealloc( pszMetadata, iLength );
            if ( !EQUAL( pszMetadata, "" ) )
                strncat( pszMetadata, ",", 1 );
            strncat( pszMetadata, pszTemp, iLength );
        }

    return pszMetadata;
}

/************************************************************************/
/*                          GetMetadataElement()                        */
/************************************************************************/

int MrSIDDataset::GetMetadataElement( const char *pszKey, void *pValue,
                                      int iLength )
{
    if( strstr(pszKey,"::308") != NULL )
    {
        printf( "Trying %s.\n", pszKey );
    }

    if ( !poMetadata->has( pszKey ) )
        return FALSE;

    const LTIMetadataRecord *poMetadataRec = NULL;
    poMetadata->get( pszKey, poMetadataRec );

    if ( !poMetadataRec->isScalar() )
	return FALSE;

    if( strstr(pszKey,"::308") != NULL )
    {
        printf( "%s is scalar!\n", pszKey );
    }

    // XXX: return FALSE if we have more than one element in metadata record
    int iSize;
    switch( poMetadataRec->getDataType() )
    {
         case LTI_METADATA_DATATYPE_UINT8:
         case LTI_METADATA_DATATYPE_SINT8:
	     iSize = 1;
	     break;
         case LTI_METADATA_DATATYPE_UINT16:
         case LTI_METADATA_DATATYPE_SINT16:
             iSize = 2;
             break;
         case LTI_METADATA_DATATYPE_UINT32:
         case LTI_METADATA_DATATYPE_SINT32:
         case LTI_METADATA_DATATYPE_FLOAT32:
             iSize = 4;
             break;
         case LTI_METADATA_DATATYPE_FLOAT64:
             iSize = 8;
             break;
	 case LTI_METADATA_DATATYPE_ASCII:
	     iSize = iLength;
	     break;
         default:
             iSize = 0;
             break;
    }

    if ( poMetadataRec->getDataType() == LTI_METADATA_DATATYPE_ASCII )
    {
	strncpy( (char *)pValue,
		 ((const char**)poMetadataRec->getScalarData())[0], iSize );
        ((char *)pValue)[iSize - 1] = '\0';
    }
    else
        memcpy( pValue, poMetadataRec->getScalarData(), iSize );

    return TRUE;
}

/************************************************************************/
/*                             OpenZoomLevel()                          */
/************************************************************************/

CPLErr MrSIDDataset::OpenZoomLevel( lt_int32 iZoom )
{
/* -------------------------------------------------------------------- */
/*      Get image geometry.                                            */
/* -------------------------------------------------------------------- */
    if ( iZoom != 0 )
    {
        lt_uint32 iWidth, iHeight;
        dfCurrentMag = LTIUtils::levelToMag( iZoom );
        poImageReader->getDimsAtMag( dfCurrentMag, iWidth, iHeight );
	nRasterXSize = iWidth;
	nRasterYSize = iHeight;
    }
    else
    {
        dfCurrentMag = 1.0;
        nRasterXSize = poImageReader->getWidth();
        nRasterYSize = poImageReader->getHeight();
    }

    nBands = poImageReader->getNumBands();
    nBlockXSize = nRasterXSize;
    nBlockYSize = poImageReader->getStripHeight();

    CPLDebug( "MrSID", "Opened zoom level %d with size %dx%d.\n",
              iZoom, nRasterXSize, nRasterYSize );

    try
    {
        poLTINav = new LTINavigator( *poImageReader );
    }
    catch ( ... )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDDataset::OpenZoomLevel(): "
                  "Failed to create LTINavigator object." );
        return CE_Failure;
    }

/* -------------------------------------------------------------------- */
/*  Handle sample type and color space.                                 */
/* -------------------------------------------------------------------- */
    eColorSpace = poImageReader->getColorSpace();
    eSampleType = poImageReader->getDataType();
    switch ( eSampleType )
    {
      case LTI_DATATYPE_UINT16:
        eDataType = GDT_UInt16;
        break;
      case LTI_DATATYPE_SINT16:
        eDataType = GDT_Int16;
        break;
      case LTI_DATATYPE_UINT32:
        eDataType = GDT_UInt32;
        break;
      case LTI_DATATYPE_SINT32:
        eDataType = GDT_Int32;
        break;
      case LTI_DATATYPE_FLOAT32:
        eDataType = GDT_Float32;
        break;
      case LTI_DATATYPE_FLOAT64:
        eDataType = GDT_Float64;
        break;
      case LTI_DATATYPE_UINT8:
      case LTI_DATATYPE_SINT8:
      default:
        eDataType = GDT_Byte;
        break;
    }

/* -------------------------------------------------------------------- */
/*      Read georeferencing.                                            */
/* -------------------------------------------------------------------- */
    if ( !poImageReader->isGeoCoordImplicit() )
    {
        const LTIGeoCoord& oGeo = poImageReader->getGeoCoord();
        oGeo.get( adfGeoTransform[0], adfGeoTransform[3],
	          adfGeoTransform[1], adfGeoTransform[5],
	          adfGeoTransform[2], adfGeoTransform[4] );
        
        adfGeoTransform[0] = adfGeoTransform[0] - adfGeoTransform[1] / 2;
        adfGeoTransform[3] = adfGeoTransform[3] - adfGeoTransform[5] / 2;
	bHasGeoTransform = TRUE;
    }
    else if( iZoom == 0 )
    {
        bHasGeoTransform = 
            GDALReadWorldFile( GetDescription(), ".sdw",  
                               adfGeoTransform )
            || GDALReadWorldFile( GetDescription(), ".sidw", 
                                  adfGeoTransform )
            || GDALReadWorldFile( GetDescription(), ".wld", 
                                  adfGeoTransform );
    }
    
/* -------------------------------------------------------------------- */
/*      Read wkt.                                                       */
/* -------------------------------------------------------------------- */
#ifdef MRSID_HAVE_GETWKT
    if( !poImageReader->isGeoCoordImplicit() )
    {
	const LTIGeoCoord& oGeo = poImageReader->getGeoCoord();
	
	if( oGeo.getWKT() )
            pszProjection =  CPLStrdup( oGeo.getWKT() );
    }
#endif // HAVE_MRSID_GETWKT

/* -------------------------------------------------------------------- */
/*      Read NoData value.                                              */
/* -------------------------------------------------------------------- */
    poNDPixel = poImageReader->getNoDataPixel();

/* -------------------------------------------------------------------- */
/*      Create band information objects.                                */
/* -------------------------------------------------------------------- */
    int             iBand;

    for( iBand = 1; iBand <= nBands; iBand++ )
        SetBand( iBand, new MrSIDRasterBand( this, iBand ) );

    return CE_None;
}

/************************************************************************/
/*                             MrSIDOpen()                              */
/*                                                                      */
/*      This is just a jacket to verify that the file is MrSID.         */
/************************************************************************/

static GDALDataset *MrSIDOpen( GDALOpenInfo * poOpenInfo )
{
    if( poOpenInfo->nHeaderBytes < 32 )
        return NULL;

    if ( !EQUALN((const char *) poOpenInfo->pabyHeader, "msid", 4) )
        return NULL;

    return MrSIDDataset::Open( poOpenInfo );
}

/************************************************************************/
/*                              JP2Open()                               */
/*                                                                      */
/*      This is just a jacket to verify that the file is JPEG2000.      */
/************************************************************************/

static unsigned char jpc_header[] = 
{0xff,0x4f};

#ifdef MRSID_J2K

static GDALDataset *JP2Open( GDALOpenInfo *poOpenInfo )

{
    if( poOpenInfo->nHeaderBytes < 32 )
        return NULL;

    if( memcmp( poOpenInfo->pabyHeader, jpc_header, sizeof(jpc_header) ) == 0 )
    {
        const char *pszExtension;

        pszExtension = CPLGetExtension( poOpenInfo->pszFilename );
        
        if( !EQUAL(pszExtension,"jpc") && !EQUAL(pszExtension,"j2k") 
            && !EQUAL(pszExtension,"jp2") && !EQUAL(pszExtension,"jpx") 
            && !EQUAL(pszExtension,"j2c") )
            return NULL;
    }
    else if( !EQUALN((const char *) poOpenInfo->pabyHeader + 4, "jP  ", 4) )
        return NULL;

    return MrSIDDataset::Open( poOpenInfo );
}
#endif /* def MRSID_J2K */

/************************************************************************/
/*                                Open()                                */
/************************************************************************/

GDALDataset *MrSIDDataset::Open( GDALOpenInfo * poOpenInfo )
{
    int     bIsJP2 = FALSE;

/* -------------------------------------------------------------------- */
/*      Is this a mrsid or jpeg 2000 file?                              */
/* -------------------------------------------------------------------- */
    if( poOpenInfo->nHeaderBytes < 32 )
        return NULL;

    if( EQUALN((const char *) poOpenInfo->pabyHeader + 4, "jP  ", 4) )
        bIsJP2 = TRUE;

    else if( memcmp( poOpenInfo->pabyHeader, jpc_header, 
                     sizeof(jpc_header) ) == 0 )
        bIsJP2 = TRUE;

    else if ( !EQUALN((const char *) poOpenInfo->pabyHeader, "msid", 4) )
        return NULL;

    if(poOpenInfo->fp)
    {
        VSIFClose( poOpenInfo->fp );
        poOpenInfo->fp = NULL;
    }

/* -------------------------------------------------------------------- */
/*      Create a corresponding GDALDataset.                             */
/* -------------------------------------------------------------------- */
    MrSIDDataset        *poDS;
    const LTFileSpec    oFileSpec( poOpenInfo->pszFilename );

    poDS = new MrSIDDataset();
#ifdef MRSID_J2K
    if ( bIsJP2 )
        poDS->poImageReader = new J2KImageReader( oFileSpec, true );
    else
#endif
        poDS->poImageReader = new MrSIDImageReader( oFileSpec );

    if ( !LT_SUCCESS( poDS->poImageReader->initialize() ) )
    {
        delete poDS;
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDDataset::Open(): Failed to open file %s",
                  poOpenInfo->pszFilename );
        return NULL;
    }

/* -------------------------------------------------------------------- */
/*      Read metadata.                                                  */
/* -------------------------------------------------------------------- */
    poDS->poMetadata =
        new LTIMetadataDatabase( poDS->poImageReader->getMetadata() );
    const GUInt32       iNumRecs = poDS->poMetadata->getIndexCount();
    GUInt32             i;

    for ( i = 0; i < iNumRecs; i++ )
    {
        const LTIMetadataRecord *poMetadataRec = NULL;
        if ( LT_SUCCESS(poDS->poMetadata->getDataByIndex(i, poMetadataRec)) )
	{
            char    *pszElement = poDS->SerializeMetadataRec( poMetadataRec );
            char    *pszKey = CPLStrdup( poMetadataRec->getTagName() );
            char    *pszTemp = pszKey;

            // GDAL metadata keys should not contain ':' and '=' characters.
            // We will replace them with '_'.
            do
            {
                if ( *pszTemp == ':' || *pszTemp == '=' )
                    *pszTemp = '_';
            }
            while ( *++pszTemp );

            poDS->SetMetadataItem( pszKey, pszElement );

            CPLFree( pszElement );
            CPLFree( pszKey );
	}
    }

    poDS->GetGTIFDefn();
    
/* -------------------------------------------------------------------- */
/*      Get number of resolution levels (we will use them as overviews).*/
/* -------------------------------------------------------------------- */
#ifdef MRSID_J2K
    if( bIsJP2 )
        poDS->nOverviewCount
            = ((J2KImageReader *) (poDS->poImageReader))->getNumLevels();
    else
#endif
        poDS->nOverviewCount
            = ((MrSIDImageReader *) (poDS->poImageReader))->getNumLevels();

    if ( poDS->nOverviewCount > 0 )
    {
        lt_int32        i;

        poDS->papoOverviewDS = (MrSIDDataset **)
            CPLMalloc( poDS->nOverviewCount * (sizeof(void*)) );

        for ( i = 0; i < poDS->nOverviewCount; i++ )
        {
            poDS->papoOverviewDS[i] = new MrSIDDataset();
            poDS->papoOverviewDS[i]->poImageReader = poDS->poImageReader;
            poDS->papoOverviewDS[i]->OpenZoomLevel( i + 1 );
            poDS->papoOverviewDS[i]->bIsOverview = TRUE;
            poDS->papoOverviewDS[i]->poParentDS = poDS;
        }
    }

/* -------------------------------------------------------------------- */
/*      Create object for the whole image.                              */
/* -------------------------------------------------------------------- */
    poDS->SetDescription( poOpenInfo->pszFilename );
    poDS->OpenZoomLevel( 0 );

    CPLDebug( "MrSID",
              "Opened image: width %d, height %d, bands %d",
              poDS->nRasterXSize, poDS->nRasterYSize, poDS->nBands );

/* -------------------------------------------------------------------- */
/*      Initialize any PAM information.                                 */
/* -------------------------------------------------------------------- */
    poDS->TryLoadXML();

    return( poDS );
}

/************************************************************************/
/*                    EPSGProjMethodToCTProjMethod()                    */
/*                                                                      */
/*      Convert between the EPSG enumeration for projection methods,    */
/*      and the GeoTIFF CT codes.                                       */
/*      Explicitly copied from geo_normalize.c of the GeoTIFF package   */
/************************************************************************/

static int EPSGProjMethodToCTProjMethod( int nEPSG )

{
    /* see trf_method.csv for list of EPSG codes */
    
    switch( nEPSG )
    {
      case 9801:
        return( CT_LambertConfConic_1SP );

      case 9802:
        return( CT_LambertConfConic_2SP );

      case 9803:
        return( CT_LambertConfConic_2SP ); /* Belgian variant not supported */

      case 9804:
        return( CT_Mercator );  /* 1SP and 2SP not differentiated */

      case 9805:
        return( CT_Mercator );  /* 1SP and 2SP not differentiated */

      case 9806:
        return( CT_CassiniSoldner );

      case 9807:
        return( CT_TransverseMercator );

      case 9808:
        return( CT_TransvMercator_SouthOriented );

      case 9809:
        return( CT_ObliqueStereographic );

      case 9810:
        return( CT_PolarStereographic );

      case 9811:
        return( CT_NewZealandMapGrid );

      case 9812:
        return( CT_ObliqueMercator ); /* is hotine actually different? */

      case 9813:
        return( CT_ObliqueMercator_Laborde );

      case 9814:
        return( CT_ObliqueMercator_Rosenmund ); /* swiss  */

      case 9815:
        return( CT_ObliqueMercator );

      case 9816: /* tunesia mining grid has no counterpart */
        return( KvUserDefined );
    }

    return( KvUserDefined );
}

/* EPSG Codes for projection parameters.  Unfortunately, these bear no
   relationship to the GeoTIFF codes even though the names are so similar. */

#define EPSGNatOriginLat         8801
#define EPSGNatOriginLong        8802
#define EPSGNatOriginScaleFactor 8805
#define EPSGFalseEasting         8806
#define EPSGFalseNorthing        8807
#define EPSGProjCenterLat        8811
#define EPSGProjCenterLong       8812
#define EPSGAzimuth              8813
#define EPSGAngleRectifiedToSkewedGrid 8814
#define EPSGInitialLineScaleFactor 8815
#define EPSGProjCenterEasting    8816
#define EPSGProjCenterNorthing   8817
#define EPSGPseudoStdParallelLat 8818
#define EPSGPseudoStdParallelScaleFactor 8819
#define EPSGFalseOriginLat       8821
#define EPSGFalseOriginLong      8822
#define EPSGStdParallel1Lat      8823
#define EPSGStdParallel2Lat      8824
#define EPSGFalseOriginEasting   8826
#define EPSGFalseOriginNorthing  8827
#define EPSGSphericalOriginLat   8828
#define EPSGSphericalOriginLong  8829
#define EPSGInitialLongitude     8830
#define EPSGZoneWidth            8831

/************************************************************************/
/*                            SetGTParmIds()                            */
/*                                                                      */
/*      This is hardcoded logic to set the GeoTIFF parameter            */
/*      identifiers for all the EPSG supported projections.  As the     */
/*      trf_method.csv table grows with new projections, this code      */
/*      will need to be updated.                                        */
/*      Explicitly copied from geo_normalize.c of the GeoTIFF package.  */
/************************************************************************/

static int SetGTParmIds( int nCTProjection, 
                         int *panProjParmId, 
                         int *panEPSGCodes )

{
    int anWorkingDummy[7];

    if( panEPSGCodes == NULL )
        panEPSGCodes = anWorkingDummy;
    if( panProjParmId == NULL )
        panProjParmId = anWorkingDummy;

    memset( panEPSGCodes, 0, sizeof(int) * 7 );

    /* psDefn->nParms = 7; */
    
    switch( nCTProjection )
    {
      case CT_CassiniSoldner:
      case CT_NewZealandMapGrid:
        panProjParmId[0] = ProjNatOriginLatGeoKey;
        panProjParmId[1] = ProjNatOriginLongGeoKey;
        panProjParmId[5] = ProjFalseEastingGeoKey;
        panProjParmId[6] = ProjFalseNorthingGeoKey;

        panEPSGCodes[0] = EPSGNatOriginLat;
        panEPSGCodes[1] = EPSGNatOriginLong;
        panEPSGCodes[5] = EPSGFalseEasting;
        panEPSGCodes[6] = EPSGFalseNorthing;
        return TRUE;

      case CT_ObliqueMercator:
        panProjParmId[0] = ProjCenterLatGeoKey;
        panProjParmId[1] = ProjCenterLongGeoKey;
        panProjParmId[2] = ProjAzimuthAngleGeoKey;
        panProjParmId[3] = ProjRectifiedGridAngleGeoKey;
        panProjParmId[4] = ProjScaleAtCenterGeoKey;
        panProjParmId[5] = ProjFalseEastingGeoKey;
        panProjParmId[6] = ProjFalseNorthingGeoKey;

        panEPSGCodes[0] = EPSGProjCenterLat;
        panEPSGCodes[1] = EPSGProjCenterLong;
        panEPSGCodes[2] = EPSGAzimuth;
        panEPSGCodes[3] = EPSGAngleRectifiedToSkewedGrid;
        panEPSGCodes[4] = EPSGInitialLineScaleFactor;
        panEPSGCodes[5] = EPSGProjCenterEasting;
        panEPSGCodes[6] = EPSGProjCenterNorthing;
        return TRUE;

      case CT_ObliqueMercator_Laborde:
        panProjParmId[0] = ProjCenterLatGeoKey;
        panProjParmId[1] = ProjCenterLongGeoKey;
        panProjParmId[2] = ProjAzimuthAngleGeoKey;
        panProjParmId[4] = ProjScaleAtCenterGeoKey;
        panProjParmId[5] = ProjFalseEastingGeoKey;
        panProjParmId[6] = ProjFalseNorthingGeoKey;

        panEPSGCodes[0] = EPSGProjCenterLat;
        panEPSGCodes[1] = EPSGProjCenterLong;
        panEPSGCodes[2] = EPSGAzimuth;
        panEPSGCodes[4] = EPSGInitialLineScaleFactor;
        panEPSGCodes[5] = EPSGProjCenterEasting;
        panEPSGCodes[6] = EPSGProjCenterNorthing;
        return TRUE;
        
      case CT_LambertConfConic_1SP:
      case CT_Mercator:
      case CT_ObliqueStereographic:
      case CT_PolarStereographic:
      case CT_TransverseMercator:
      case CT_TransvMercator_SouthOriented:
        panProjParmId[0] = ProjNatOriginLatGeoKey;
        panProjParmId[1] = ProjNatOriginLongGeoKey;
        panProjParmId[4] = ProjScaleAtNatOriginGeoKey;
        panProjParmId[5] = ProjFalseEastingGeoKey;
        panProjParmId[6] = ProjFalseNorthingGeoKey;

        panEPSGCodes[0] = EPSGNatOriginLat;
        panEPSGCodes[1] = EPSGNatOriginLong;
        panEPSGCodes[4] = EPSGNatOriginScaleFactor;
        panEPSGCodes[5] = EPSGFalseEasting;
        panEPSGCodes[6] = EPSGFalseNorthing;
        return TRUE;

      case CT_LambertConfConic_2SP:
        panProjParmId[0] = ProjFalseOriginLatGeoKey;
        panProjParmId[1] = ProjFalseOriginLongGeoKey;
        panProjParmId[2] = ProjStdParallel1GeoKey;
        panProjParmId[3] = ProjStdParallel2GeoKey;
        panProjParmId[5] = ProjFalseEastingGeoKey;
        panProjParmId[6] = ProjFalseNorthingGeoKey;

        panEPSGCodes[0] = EPSGFalseOriginLat;
        panEPSGCodes[1] = EPSGFalseOriginLong;
        panEPSGCodes[2] = EPSGStdParallel1Lat;
        panEPSGCodes[3] = EPSGStdParallel2Lat;
        panEPSGCodes[5] = EPSGFalseOriginEasting;
        panEPSGCodes[6] = EPSGFalseOriginNorthing;
        return TRUE;

      case CT_SwissObliqueCylindrical:
        panProjParmId[0] = ProjCenterLatGeoKey;
        panProjParmId[1] = ProjCenterLongGeoKey;
        panProjParmId[5] = ProjFalseEastingGeoKey;
        panProjParmId[6] = ProjFalseNorthingGeoKey;

        /* EPSG codes? */
        return TRUE;

      default:
        return( FALSE );
    }
}

static char *papszDatumEquiv[] =
{
    "Militar_Geographische_Institut",
    "Militar_Geographische_Institute",
    "World_Geodetic_System_1984",
    "WGS_1984",
    "WGS_72_Transit_Broadcast_Ephemeris",
    "WGS_1972_Transit_Broadcast_Ephemeris",
    "World_Geodetic_System_1972",
    "WGS_1972",
    "European_Terrestrial_Reference_System_89",
    "European_Reference_System_1989",
    NULL
};

/************************************************************************/
/*                          WKTMassageDatum()                           */
/*                                                                      */
/*      Massage an EPSG datum name into WMT format.  Also transform     */
/*      specific exception cases into WKT versions.                     */
/*      Explicitly copied from the gt_wkt_srs.cpp.                      */
/************************************************************************/

static void WKTMassageDatum( char ** ppszDatum )

{
    int		i, j;
    char	*pszDatum;

/* -------------------------------------------------------------------- */
/*      First copy string and allocate with our CPLStrdup() to so we    */
/*      know when we are done this function we will have a CPL          */
/*      string, not a GTIF one.                                         */
/* -------------------------------------------------------------------- */
    pszDatum = CPLStrdup(*ppszDatum);
    GTIFFreeMemory( *ppszDatum );
    *ppszDatum = pszDatum;

/* -------------------------------------------------------------------- */
/*      Translate non-alphanumeric values to underscores.               */
/* -------------------------------------------------------------------- */
    for( i = 0; pszDatum[i] != '\0'; i++ )
    {
        if( !(pszDatum[i] >= 'A' && pszDatum[i] <= 'Z')
            && !(pszDatum[i] >= 'a' && pszDatum[i] <= 'z')
            && !(pszDatum[i] >= '0' && pszDatum[i] <= '9') )
        {
            pszDatum[i] = '_';
        }
    }

/* -------------------------------------------------------------------- */
/*      Remove repeated and trailing underscores.                       */
/* -------------------------------------------------------------------- */
    for( i = 1, j = 0; pszDatum[i] != '\0'; i++ )
    {
        if( pszDatum[j] == '_' && pszDatum[i] == '_' )
            continue;

        pszDatum[++j] = pszDatum[i];
    }
    if( pszDatum[j] == '_' )
        pszDatum[j] = '\0';
    else
        pszDatum[j+1] = '\0';
    
/* -------------------------------------------------------------------- */
/*      Search for datum equivelences.  Specific massaged names get     */
/*      mapped to OpenGIS specified names.                              */
/* -------------------------------------------------------------------- */
    for( i = 0; papszDatumEquiv[i] != NULL; i += 2 )
    {
        if( EQUAL(*ppszDatum,papszDatumEquiv[i]) )
        {
            CPLFree( *ppszDatum );
            *ppszDatum = CPLStrdup( papszDatumEquiv[i+1] );
            return;
        }
    }
}

/************************************************************************/
/*                           FetchProjParms()                           */
/*                                                                      */
/*      Fetch the projection parameters for a particular projection     */
/*      from MrSID metadata, and fill the GTIFDefn structure out        */
/*      with them.                                                      */
/*      Copied from geo_normalize.c of the GeoTIFF package.             */
/************************************************************************/

void MrSIDDataset::FetchProjParms()
{
    double dfNatOriginLong = 0.0, dfNatOriginLat = 0.0, dfRectGridAngle = 0.0;
    double dfFalseEasting = 0.0, dfFalseNorthing = 0.0, dfNatOriginScale = 1.0;
    double dfStdParallel1 = 0.0, dfStdParallel2 = 0.0, dfAzimuth = 0.0;

/* -------------------------------------------------------------------- */
/*      Get the false easting, and northing if available.               */
/* -------------------------------------------------------------------- */
    if( !GetMetadataElement( "GEOTIFF_NUM::3082::ProjFalseEastingGeoKey",
                             &dfFalseEasting )
        && !GetMetadataElement( "GEOTIFF_NUM::3090:ProjCenterEastingGeoKey",
                                &dfFalseEasting ) )
        dfFalseEasting = 0.0;
        
    if( !GetMetadataElement( "GEOTIFF_NUM::3083::ProjFalseNorthingGeoKey",
                             &dfFalseNorthing )
        && !GetMetadataElement( "GEOTIFF_NUM::3091::ProjCenterNorthingGeoKey",
                                &dfFalseNorthing ) )
        dfFalseNorthing = 0.0;

    switch( psDefn->CTProjection )
    {
/* -------------------------------------------------------------------- */
      case CT_Stereographic:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey",
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3092::ProjScaleAtNatOriginGeoKey",
                                &dfNatOriginScale ) == 0 )
            dfNatOriginScale = 1.0;
            
        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfNatOriginLat;
        psDefn->ProjParmId[0] = ProjCenterLatGeoKey;
        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjCenterLongGeoKey;
        psDefn->ProjParm[4] = dfNatOriginScale;
        psDefn->ProjParmId[4] = ProjScaleAtNatOriginGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_LambertConfConic_1SP:
      case CT_Mercator:
      case CT_ObliqueStereographic:
      case CT_TransverseMercator:
      case CT_TransvMercator_SouthOriented:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey", 
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3092::ProjScaleAtNatOriginGeoKey",
                                &dfNatOriginScale ) == 0 )
            dfNatOriginScale = 1.0;

        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfNatOriginLat;
        psDefn->ProjParmId[0] = ProjNatOriginLatGeoKey;
        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjNatOriginLongGeoKey;
        psDefn->ProjParm[4] = dfNatOriginScale;
        psDefn->ProjParmId[4] = ProjScaleAtNatOriginGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_ObliqueMercator: /* hotine */
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey", 
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3094::ProjAzimuthAngleGeoKey",
                                &dfAzimuth ) == 0 )
            dfAzimuth = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3096::ProjRectifiedGridAngleGeoKey",
                                &dfRectGridAngle ) == 0 )
            dfRectGridAngle = 90.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3092::ProjScaleAtNatOriginGeoKey",
                                &dfNatOriginScale ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3093::ProjScaleAtCenterGeoKey",
                                   &dfNatOriginScale ) == 0 )
            dfNatOriginScale = 1.0;
            
        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfNatOriginLat;
        psDefn->ProjParmId[0] = ProjCenterLatGeoKey;
        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjCenterLongGeoKey;
        psDefn->ProjParm[2] = dfAzimuth;
        psDefn->ProjParmId[2] = ProjAzimuthAngleGeoKey;
        psDefn->ProjParm[3] = dfRectGridAngle;
        psDefn->ProjParmId[3] = ProjRectifiedGridAngleGeoKey;
        psDefn->ProjParm[4] = dfNatOriginScale;
        psDefn->ProjParmId[4] = ProjScaleAtCenterGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_CassiniSoldner:
      case CT_Polyconic:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey", 
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;


        if( GetMetadataElement( "GEOTIFF_NUM::3092::ProjScaleAtNatOriginGeoKey",
                                &dfNatOriginScale ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3093::ProjScaleAtCenterGeoKey",
                                   &dfNatOriginScale ) == 0 )
            dfNatOriginScale = 1.0;
            
        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfNatOriginLat;
        psDefn->ProjParmId[0] = ProjNatOriginLatGeoKey;
        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjNatOriginLongGeoKey;
        psDefn->ProjParm[4] = dfNatOriginScale;
        psDefn->ProjParmId[4] = ProjScaleAtNatOriginGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_AzimuthalEquidistant:
      case CT_MillerCylindrical:
      case CT_Equirectangular:
      case CT_Gnomonic:
      case CT_LambertAzimEqualArea:
      case CT_Orthographic:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey", 
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;

        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfNatOriginLat;
        psDefn->ProjParmId[0] = ProjCenterLatGeoKey;
        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjCenterLongGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_Robinson:
      case CT_Sinusoidal:
      case CT_VanDerGrinten:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey", 
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjCenterLongGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_PolarStereographic:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3095::ProjStraightVertPoleLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey",
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3092::ProjScaleAtNatOriginGeoKey",
                                &dfNatOriginScale ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3093::ProjScaleAtCenterGeoKey",
                                   &dfNatOriginScale ) == 0 )
            dfNatOriginScale = 1.0;
            
        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfNatOriginLat;
        psDefn->ProjParmId[0] = ProjNatOriginLatGeoKey;;
        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjStraightVertPoleLongGeoKey;
        psDefn->ProjParm[4] = dfNatOriginScale;
        psDefn->ProjParmId[4] = ProjScaleAtNatOriginGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_LambertConfConic_2SP:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3078::ProjStdParallel1GeoKey",
                                &dfStdParallel1 ) == 0 )
            dfStdParallel1 = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3079::ProjStdParallel2GeoKey",
                                &dfStdParallel2 ) == 0 )
            dfStdParallel1 = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey", 
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;

        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfNatOriginLat;
        psDefn->ProjParmId[0] = ProjFalseOriginLatGeoKey;
        psDefn->ProjParm[1] = dfNatOriginLong;
        psDefn->ProjParmId[1] = ProjFalseOriginLongGeoKey;
        psDefn->ProjParm[2] = dfStdParallel1;
        psDefn->ProjParmId[2] = ProjStdParallel1GeoKey;
        psDefn->ProjParm[3] = dfStdParallel2;
        psDefn->ProjParmId[3] = ProjStdParallel2GeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;

/* -------------------------------------------------------------------- */
      case CT_AlbersEqualArea:
      case CT_EquidistantConic:
/* -------------------------------------------------------------------- */
        if( GetMetadataElement( "GEOTIFF_NUM::3078::ProjStdParallel1GeoKey",
                                &dfStdParallel1 ) == 0 )
            dfStdParallel1 = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3079::ProjStdParallel2GeoKey",
                                &dfStdParallel2 ) == 0 )
            dfStdParallel1 = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3080::ProjNatOriginLongGeoKey",
                                &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3084::ProjFalseOriginLongGeoKey",
                                   &dfNatOriginLong ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3088::ProjCenterLongGeoKey", 
                                   &dfNatOriginLong ) == 0 )
            dfNatOriginLong = 0.0;

        if( GetMetadataElement( "GEOTIFF_NUM::3081::ProjNatOriginLatGeoKey",
                                &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3085::ProjFalseOriginLatGeoKey",
                                   &dfNatOriginLat ) == 0
            && GetMetadataElement( "GEOTIFF_NUM::3089::ProjCenterLatGeoKey",
                                   &dfNatOriginLat ) == 0 )
            dfNatOriginLat = 0.0;

        /* notdef: should transform to decimal degrees at this point */

        psDefn->ProjParm[0] = dfStdParallel1;
        psDefn->ProjParmId[0] = ProjStdParallel1GeoKey;
        psDefn->ProjParm[1] = dfStdParallel2;
        psDefn->ProjParmId[1] = ProjStdParallel2GeoKey;
        psDefn->ProjParm[2] = dfNatOriginLat;
        psDefn->ProjParmId[2] = ProjNatOriginLatGeoKey;
        psDefn->ProjParm[3] = dfNatOriginLong;
        psDefn->ProjParmId[3] = ProjNatOriginLongGeoKey;
        psDefn->ProjParm[5] = dfFalseEasting;
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[6] = dfFalseNorthing;
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        psDefn->nParms = 7;
        break;
    }
}

/************************************************************************/
/*                            GetGTIFDefn()                             */
/*      This function borrowed from the GTIFGetDefn() function.         */
/*      See geo_normalize.c from the GeoTIFF package.                   */
/************************************************************************/

void MrSIDDataset::GetGTIFDefn()
{
    double	dfInvFlattening;

/* -------------------------------------------------------------------- */
/*      Initially we default all the information we can.                */
/* -------------------------------------------------------------------- */
    psDefn = new( GTIFDefn );
    psDefn->Model = KvUserDefined;
    psDefn->PCS = KvUserDefined;
    psDefn->GCS = KvUserDefined;
    psDefn->UOMLength = KvUserDefined;
    psDefn->UOMLengthInMeters = 1.0;
    psDefn->UOMAngle = KvUserDefined;
    psDefn->UOMAngleInDegrees = 1.0;
    psDefn->Datum = KvUserDefined;
    psDefn->Ellipsoid = KvUserDefined;
    psDefn->SemiMajor = 0.0;
    psDefn->SemiMinor = 0.0;
    psDefn->PM = KvUserDefined;
    psDefn->PMLongToGreenwich = 0.0;

    psDefn->ProjCode = KvUserDefined;
    psDefn->Projection = KvUserDefined;
    psDefn->CTProjection = KvUserDefined;

    psDefn->nParms = 0;
    for( int i = 0; i < MAX_GTIF_PROJPARMS; i++ )
    {
        psDefn->ProjParm[i] = 0.0;
        psDefn->ProjParmId[i] = 0;
    }

    psDefn->MapSys = KvUserDefined;
    psDefn->Zone = 0;

/* -------------------------------------------------------------------- */
/*	Try to get the overall model type.				*/
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::1024::GTModelTypeGeoKey",
                        &(psDefn->Model) );

/* -------------------------------------------------------------------- */
/*      Try to get a PCS.                                               */
/* -------------------------------------------------------------------- */
    if( GetMetadataElement( "GEOTIFF_NUM::3072::ProjectedCSTypeGeoKey",
                            &(psDefn->PCS) )
        && psDefn->PCS != KvUserDefined )
    {
        /*
         * Translate this into useful information.
         */
        GTIFGetPCSInfo( psDefn->PCS, NULL, &(psDefn->ProjCode),
                        &(psDefn->UOMLength), &(psDefn->GCS) );
    }

/* -------------------------------------------------------------------- */
/*       If we have the PCS code, but didn't find it in the CSV files   */
/*      (likely because we can't find them) we will try some ``jiffy    */
/*      rules'' for UTM and state plane.                                */
/* -------------------------------------------------------------------- */
    if( psDefn->PCS != KvUserDefined && psDefn->ProjCode == KvUserDefined )
    {
        int	nMapSys, nZone;
        int	nGCS = psDefn->GCS;

        nMapSys = GTIFPCSToMapSys( psDefn->PCS, &nGCS, &nZone );
        if( nMapSys != KvUserDefined )
        {
            psDefn->ProjCode = (short) GTIFMapSysToProj( nMapSys, nZone );
            psDefn->GCS = (short) nGCS;
        }
    }
   
/* -------------------------------------------------------------------- */
/*      If the Proj_ code is specified directly, use that.              */
/* -------------------------------------------------------------------- */
    if( psDefn->ProjCode == KvUserDefined )
        GetMetadataElement( "GEOTIFF_NUM::3074::ProjectionGeoKey",
                            &(psDefn->ProjCode) );
    
    if( psDefn->ProjCode != KvUserDefined )
    {
        /*
         * We have an underlying projection transformation value.  Look
         * this up.  For a PCS of ``WGS 84 / UTM 11'' the transformation
         * would be Transverse Mercator, with a particular set of options.
         * The nProjTRFCode itself would correspond to the name
         * ``UTM zone 11N'', and doesn't include datum info.
         */
        GTIFGetProjTRFInfo( psDefn->ProjCode, NULL, &(psDefn->Projection),
                            psDefn->ProjParm );
        
        /*
         * Set the GeoTIFF identity of the parameters.
         */
        psDefn->CTProjection = (short)
            EPSGProjMethodToCTProjMethod( psDefn->Projection );

        SetGTParmIds( psDefn->CTProjection, psDefn->ProjParmId, NULL);
        psDefn->nParms = 7;
    }

/* -------------------------------------------------------------------- */
/*      Try to get a GCS.  If found, it will override any implied by    */
/*      the PCS.                                                        */
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::2048::GeographicTypeGeoKey",
                        &(psDefn->GCS) );

/* -------------------------------------------------------------------- */
/*      Derive the datum, and prime meridian from the GCS.              */
/* -------------------------------------------------------------------- */
    if( psDefn->GCS != KvUserDefined )
    {
        GTIFGetGCSInfo( psDefn->GCS, NULL, &(psDefn->Datum), &(psDefn->PM),
                        &(psDefn->UOMAngle) );
    }
    
/* -------------------------------------------------------------------- */
/*      Handle the GCS angular units.  GeogAngularUnitsGeoKey           */
/*      overrides the GCS or PCS setting.                               */
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::2054::GeogAngularUnitsGeoKey",
                        &(psDefn->UOMAngle) );
    if( psDefn->UOMAngle != KvUserDefined )
    {
        GTIFGetUOMAngleInfo( psDefn->UOMAngle, NULL,
                             &(psDefn->UOMAngleInDegrees) );
    }

/* -------------------------------------------------------------------- */
/*      Check for a datum setting, and then use the datum to derive     */
/*      an ellipsoid.                                                   */
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::2050::GeogGeodeticDatumGeoKey",
                        &(psDefn->Datum) );

    if( psDefn->Datum != KvUserDefined )
    {
        GTIFGetDatumInfo( psDefn->Datum, NULL, &(psDefn->Ellipsoid) );
    }

/* -------------------------------------------------------------------- */
/*      Check for an explicit ellipsoid.  Use the ellipsoid to          */
/*      derive the ellipsoid characteristics, if possible.              */
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::2056::GeogEllipsoidGeoKey",
                        &(psDefn->Ellipsoid) );

    if( psDefn->Ellipsoid != KvUserDefined )
    {
        GTIFGetEllipsoidInfo( psDefn->Ellipsoid, NULL,
                              &(psDefn->SemiMajor), &(psDefn->SemiMinor) );
    }

/* -------------------------------------------------------------------- */
/*      Check for overridden ellipsoid parameters.  It would be nice    */
/*      to warn if they conflict with provided information, but for     */
/*      now we just override.                                           */
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::2057::GeogSemiMajorAxisGeoKey",
                        &(psDefn->SemiMajor) );
    GetMetadataElement( "GEOTIFF_NUM::2058::GeogSemiMinorAxisGeoKey",
                        &(psDefn->SemiMinor) );
    
    if( GetMetadataElement( "GEOTIFF_NUM::2059::GeogInvFlatteningGeoKey",
                            &dfInvFlattening ) == 1 )
    {
        if( dfInvFlattening != 0.0 )
            psDefn->SemiMinor = 
                psDefn->SemiMajor * (1 - 1.0/dfInvFlattening);
    }

/* -------------------------------------------------------------------- */
/*      Get the prime meridian info.                                    */
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::2051::GeogPrimeMeridianGeoKey",
                        &(psDefn->PM) );

    if( psDefn->PM != KvUserDefined )
    {
        GTIFGetPMInfo( psDefn->PM, NULL, &(psDefn->PMLongToGreenwich) );
    }
    else
    {
        GetMetadataElement( "GEOTIFF_NUM::2061::GeogPrimeMeridianLongGeoKey",
                            &(psDefn->PMLongToGreenwich) );

        psDefn->PMLongToGreenwich =
            GTIFAngleToDD( psDefn->PMLongToGreenwich,
                           psDefn->UOMAngle );
    }

/* -------------------------------------------------------------------- */
/*      Have the projection units of measure been overridden?  We       */
/*      should likely be doing something about angular units too,       */
/*      but these are very rarely not decimal degrees for actual        */
/*      file coordinates.                                               */
/* -------------------------------------------------------------------- */
    GetMetadataElement( "GEOTIFF_NUM::3076::ProjLinearUnitsGeoKey",
                        &(psDefn->UOMLength) );

    if( psDefn->UOMLength != KvUserDefined )
    {
        GTIFGetUOMLengthInfo( psDefn->UOMLength, NULL,
                              &(psDefn->UOMLengthInMeters) );
    }

/* -------------------------------------------------------------------- */
/*      Handle a variety of user defined transform types.               */
/* -------------------------------------------------------------------- */
    if( GetMetadataElement( "GEOTIFF_NUM::3075::ProjCoordTransGeoKey",
                            &(psDefn->CTProjection) ) )
    {
        FetchProjParms();
    }

/* -------------------------------------------------------------------- */
/*      Try to set the zoned map system information.                    */
/* -------------------------------------------------------------------- */
    psDefn->MapSys = GTIFProjToMapSys( psDefn->ProjCode, &(psDefn->Zone) );

/* -------------------------------------------------------------------- */
/*      If this is UTM, and we were unable to extract the projection    */
/*      parameters from the CSV file, just set them directly now,       */
/*      since it's pretty easy, and a common case.                      */
/* -------------------------------------------------------------------- */
    if( (psDefn->MapSys == MapSys_UTM_North
         || psDefn->MapSys == MapSys_UTM_South)
        && psDefn->CTProjection == KvUserDefined )
    {
        psDefn->CTProjection = CT_TransverseMercator;
        psDefn->nParms = 7;
        psDefn->ProjParmId[0] = ProjNatOriginLatGeoKey;
        psDefn->ProjParm[0] = 0.0;
            
        psDefn->ProjParmId[1] = ProjNatOriginLongGeoKey;
        psDefn->ProjParm[1] = psDefn->Zone*6 - 183.0;
        
        psDefn->ProjParmId[4] = ProjScaleAtNatOriginGeoKey;
        psDefn->ProjParm[4] = 0.9996;
        
        psDefn->ProjParmId[5] = ProjFalseEastingGeoKey;
        psDefn->ProjParm[5] = 500000.0;
        
        psDefn->ProjParmId[6] = ProjFalseNorthingGeoKey;

        if( psDefn->MapSys == MapSys_UTM_North )
            psDefn->ProjParm[6] = 0.0;
        else
            psDefn->ProjParm[6] = 10000000.0;
    }

    if ( pszProjection )
        CPLFree( pszProjection );
    pszProjection = GetOGISDefn( psDefn );
}

/************************************************************************/
/*                          GetOGISDefn()                               */
/*  Copied from the gt_wkt_srs.cpp.                                     */
/************************************************************************/

char *MrSIDDataset::GetOGISDefn( GTIFDefn *psDefn )
{
    OGRSpatialReference	oSRS;

    if( psDefn->Model != ModelTypeProjected 
        && psDefn->Model != ModelTypeGeographic )
        return CPLStrdup("");
    
/* -------------------------------------------------------------------- */
/*      If this is a projected SRS we set the PROJCS keyword first      */
/*      to ensure that the GEOGCS will be a child.                      */
/* -------------------------------------------------------------------- */
    if( psDefn->Model == ModelTypeProjected )
    {
        char	*pszPCSName = "unnamed";
        int         bNeedFree = FALSE;

        if( psDefn->PCS != KvUserDefined )
        {

            if( GTIFGetPCSInfo( psDefn->PCS, &pszPCSName, NULL, NULL, NULL ) )
                bNeedFree = TRUE;
            
            oSRS.SetNode( "PROJCS", pszPCSName );
            if( bNeedFree )
                GTIFFreeMemory( pszPCSName );

            oSRS.SetAuthority( "PROJCS", "EPSG", psDefn->PCS );
        }
        else
        {
            char szPCSName[200];
            strcpy( szPCSName, "unnamed" );
            if ( GetMetadataElement( "GEOTIFF_NUM::1026::GTCitationGeoKey",
                                     szPCSName, sizeof(szPCSName) ) )
                oSRS.SetNode( "PROJCS", szPCSName );
        }
    }
    
/* ==================================================================== */
/*      Setup the GeogCS                                                */
/* ==================================================================== */
    char	*pszGeogName = NULL;
    char	*pszDatumName = NULL;
    char	*pszPMName = NULL;
    char	*pszSpheroidName = NULL;
    char	*pszAngularUnits = NULL;
    double	dfInvFlattening, dfSemiMajor;
    char        szGCSName[200];
    
    if( GetMetadataElement( "GEOTIFF_NUM::2049::GeogCitationGeoKey",
                            szGCSName, sizeof(szGCSName) ) )
        pszGeogName = CPLStrdup(szGCSName);
    GTIFGetGCSInfo( psDefn->GCS, &pszGeogName, NULL, NULL, NULL );
    GTIFGetDatumInfo( psDefn->Datum, &pszDatumName, NULL );
    GTIFGetPMInfo( psDefn->PM, &pszPMName, NULL );
    GTIFGetEllipsoidInfo( psDefn->Ellipsoid, &pszSpheroidName, NULL, NULL );
    
    GTIFGetUOMAngleInfo( psDefn->UOMAngle, &pszAngularUnits, NULL );
    if( pszAngularUnits == NULL )
        pszAngularUnits = CPLStrdup("unknown");

    if( pszDatumName != NULL )
        WKTMassageDatum( &pszDatumName );

    dfSemiMajor = psDefn->SemiMajor;
    if( psDefn->SemiMajor == 0.0 )
    {
        pszSpheroidName = CPLStrdup("unretrievable - using WGS84");
        dfSemiMajor = SRS_WGS84_SEMIMAJOR;
        dfInvFlattening = SRS_WGS84_INVFLATTENING;
    }
    else if( (psDefn->SemiMinor / psDefn->SemiMajor) < 0.99999999999999999
             || (psDefn->SemiMinor / psDefn->SemiMajor) > 1.00000000000000001 )
        dfInvFlattening = -1.0 / (psDefn->SemiMinor/psDefn->SemiMajor - 1.0);
    else
        dfInvFlattening = 0.0; /* special flag for infinity */

    oSRS.SetGeogCS( pszGeogName, pszDatumName, 
                    pszSpheroidName, dfSemiMajor, dfInvFlattening,
                    pszPMName,
                    psDefn->PMLongToGreenwich / psDefn->UOMAngleInDegrees,
                    pszAngularUnits,
                    psDefn->UOMAngleInDegrees * 0.0174532925199433 );

    if( psDefn->GCS != KvUserDefined )
        oSRS.SetAuthority( "GEOGCS", "EPSG", psDefn->GCS );

    if( psDefn->Datum != KvUserDefined )
        oSRS.SetAuthority( "DATUM", "EPSG", psDefn->Datum );

    if( psDefn->Ellipsoid != KvUserDefined )
        oSRS.SetAuthority( "SPHEROID", "EPSG", psDefn->Ellipsoid );

    GTIFFreeMemory( pszGeogName );
    CPLFree( pszDatumName );
    GTIFFreeMemory( pszPMName );
    GTIFFreeMemory( pszSpheroidName );
    GTIFFreeMemory( pszAngularUnits );
        
/* ==================================================================== */
/*      Handle projection parameters.                                   */
/* ==================================================================== */
    if( psDefn->Model == ModelTypeProjected )
    {
/* -------------------------------------------------------------------- */
/*      Make a local copy of parms, and convert back into the           */
/*      angular units of the GEOGCS and the linear units of the         */
/*      projection.                                                     */
/* -------------------------------------------------------------------- */
        double		adfParm[10];
        int		i;

        for( i = 0; i < MIN(10,psDefn->nParms); i++ )
            adfParm[i] = psDefn->ProjParm[i];

        adfParm[0] /= psDefn->UOMAngleInDegrees;
        adfParm[1] /= psDefn->UOMAngleInDegrees;
        adfParm[2] /= psDefn->UOMAngleInDegrees;
        adfParm[3] /= psDefn->UOMAngleInDegrees;
        
        adfParm[5] /= psDefn->UOMLengthInMeters;
        adfParm[6] /= psDefn->UOMLengthInMeters;
        
/* -------------------------------------------------------------------- */
/*      Translation the fundamental projection.                         */
/* -------------------------------------------------------------------- */
        switch( psDefn->CTProjection )
        {
          case CT_TransverseMercator:
            oSRS.SetTM( adfParm[0], adfParm[1],
                        adfParm[4],
                        adfParm[5], adfParm[6] );
            break;

          case CT_TransvMercator_SouthOriented:
            oSRS.SetTMSO( adfParm[0], adfParm[1],
                          adfParm[4],
                          adfParm[5], adfParm[6] );
            break;

          case CT_Mercator:
            oSRS.SetMercator( adfParm[0], adfParm[1],
                              adfParm[4],
                              adfParm[5], adfParm[6] );
            break;

          case CT_ObliqueStereographic:
            oSRS.SetOS( adfParm[0], adfParm[1],
                        adfParm[4],
                        adfParm[5], adfParm[6] );
            break;

          case CT_Stereographic:
            oSRS.SetOS( adfParm[0], adfParm[1],
                        adfParm[4],
                        adfParm[5], adfParm[6] );
            break;

          case CT_ObliqueMercator: /* hotine */
            oSRS.SetHOM( adfParm[0], adfParm[1],
                         adfParm[2], adfParm[3],
                         adfParm[4],
                         adfParm[5], adfParm[6] );
            break;
        
          case CT_EquidistantConic: 
            oSRS.SetEC( adfParm[0], adfParm[1],
                        adfParm[2], adfParm[3],
                        adfParm[5], adfParm[6] );
            break;
        
          case CT_CassiniSoldner:
            oSRS.SetCS( adfParm[0], adfParm[1],
                        adfParm[5], adfParm[6] );
            break;
        
          case CT_Polyconic:
            oSRS.SetPolyconic( adfParm[0], adfParm[1],
                               adfParm[5], adfParm[6] );
            break;

          case CT_AzimuthalEquidistant:
            oSRS.SetAE( adfParm[0], adfParm[1],
                        adfParm[5], adfParm[6] );
            break;
        
          case CT_MillerCylindrical:
            oSRS.SetMC( adfParm[0], adfParm[1],
                        adfParm[5], adfParm[6] );
            break;
        
          case CT_Equirectangular:
            oSRS.SetEquirectangular( adfParm[0], adfParm[1],
                                     adfParm[5], adfParm[6] );
            break;
        
          case CT_Gnomonic:
            oSRS.SetGnomonic( adfParm[0], adfParm[1],
                              adfParm[5], adfParm[6] );
            break;
        
          case CT_LambertAzimEqualArea:
            oSRS.SetLAEA( adfParm[0], adfParm[1],
                          adfParm[5], adfParm[6] );
            break;
        
          case CT_Orthographic:
            oSRS.SetOrthographic( adfParm[0], adfParm[1],
                                  adfParm[5], adfParm[6] );
            break;
        
          case CT_Robinson:
            oSRS.SetRobinson( adfParm[1],
                              adfParm[5], adfParm[6] );
            break;
        
          case CT_Sinusoidal:
            oSRS.SetSinusoidal( adfParm[1],
                                adfParm[5], adfParm[6] );
            break;
        
          case CT_VanDerGrinten:
            oSRS.SetVDG( adfParm[1],
                         adfParm[5], adfParm[6] );
            break;

          case CT_PolarStereographic:
            oSRS.SetPS( adfParm[0], adfParm[1],
                        adfParm[4],
                        adfParm[5], adfParm[6] );
            break;
        
          case CT_LambertConfConic_2SP:
            oSRS.SetLCC( adfParm[2], adfParm[3],
                         adfParm[0], adfParm[1],
                         adfParm[5], adfParm[6] );
            break;

          case CT_LambertConfConic_1SP:
            oSRS.SetLCC1SP( adfParm[0], adfParm[1],
                            adfParm[4],
                            adfParm[5], adfParm[6] );
            break;
        
          case CT_AlbersEqualArea:
            oSRS.SetACEA( adfParm[0], adfParm[1],
                          adfParm[2], adfParm[3],
                          adfParm[5], adfParm[6] );
            break;

          case CT_NewZealandMapGrid:
            oSRS.SetNZMG( adfParm[0], adfParm[1],
                          adfParm[5], adfParm[6] );
            break;
        }

/* -------------------------------------------------------------------- */
/*      Set projection units.                                           */
/* -------------------------------------------------------------------- */
        char	*pszUnitsName = NULL;
        
        GTIFGetUOMLengthInfo( psDefn->UOMLength, &pszUnitsName, NULL );

        if( pszUnitsName != NULL && psDefn->UOMLength != KvUserDefined )
        {
            oSRS.SetLinearUnits( pszUnitsName, psDefn->UOMLengthInMeters );
            oSRS.SetAuthority( "PROJCS|UNIT", "EPSG", psDefn->UOMLength );
        }
        else
            oSRS.SetLinearUnits( "unknown", psDefn->UOMLengthInMeters );

        GTIFFreeMemory( pszUnitsName );
    }
    
/* -------------------------------------------------------------------- */
/*      Return the WKT serialization of the object.                     */
/* -------------------------------------------------------------------- */
    char	*pszWKT;

    oSRS.FixupOrdering();

    if( oSRS.exportToWkt( &pszWKT ) == OGRERR_NONE )
        return pszWKT;
    else
        return NULL;
}

#ifdef MRSID_ESDK

/************************************************************************/
/* ==================================================================== */
/*                        MrSIDDummyImageReader                         */
/*                                                                      */
/*  This is a helper class to wrap GDAL calls in MrSID interface.       */
/* ==================================================================== */
/************************************************************************/

class MrSIDDummyImageReader : public LTIImageReader
{
  public:

    MrSIDDummyImageReader( GDALDataset *poSrcDS );
    ~MrSIDDummyImageReader();
    LT_STATUS           initialize();
    lt_int64            getPhysicalFileSize(void) const { return 0; };

  private:
    GDALDataset         *poDS;
    GDALDataType        eDataType;
    LTIDataType         eSampleType;
    const LTIPixel      *poPixel;
    
    double              adfGeoTransform[6];

    virtual LT_STATUS   decodeStrip( LTISceneBuffer& stripBuffer,
                                     const LTIScene& stripScene );
    virtual LT_STATUS   decodeBegin( const LTIScene& scene )
                            { return LT_STS_Success; };
    virtual LT_STATUS   decodeEnd() { return LT_STS_Success; };
};

/************************************************************************/
/*                        MrSIDDummyImageReader()                       */
/************************************************************************/

MrSIDDummyImageReader::MrSIDDummyImageReader( GDALDataset *poSrcDS ) :
                                            LTIImageReader(), poDS(poSrcDS)
{
    poPixel = NULL;
}

/************************************************************************/
/*                        ~MrSIDDummyImageReader()                      */
/************************************************************************/

MrSIDDummyImageReader::~MrSIDDummyImageReader()
{
    if ( poPixel )
        delete poPixel;
}

/************************************************************************/
/*                             initialize()                             */
/************************************************************************/

LT_STATUS MrSIDDummyImageReader::initialize()
{
    if ( !LT_SUCCESS(LTIImageReader::initialize()) )
        return LT_STS_Failure;
    
    lt_uint16 nBands = (lt_uint16)poDS->GetRasterCount();
    LTIColorSpace eColorSpace = LTI_COLORSPACE_RGB;
    switch ( nBands )
    {
        case 1:
            eColorSpace = LTI_COLORSPACE_GRAYSCALE;
            break;
        case 3:
            eColorSpace = LTI_COLORSPACE_RGB;
            break;
        default:
            eColorSpace = LTI_COLORSPACE_MULTISPECTRAL;
            break;
    }
    
    eDataType = poDS->GetRasterBand(1)->GetRasterDataType();
    switch ( eDataType )
    {
        case GDT_UInt16:
            eSampleType = LTI_DATATYPE_UINT16;
            break;
        case GDT_Int16:
            eSampleType = LTI_DATATYPE_SINT16;
            break;
        case GDT_UInt32:
            eSampleType = LTI_DATATYPE_UINT32;
            break;
        case GDT_Int32:
            eSampleType = LTI_DATATYPE_SINT32;
            break;
        case GDT_Float32:
            eSampleType = LTI_DATATYPE_FLOAT32;
            break;
        case GDT_Float64:
            eSampleType = LTI_DATATYPE_FLOAT64;
            break;
        case GDT_Byte:
        default:
            eSampleType = LTI_DATATYPE_UINT8;
            break;
    }

    poPixel = new LTIPixel( eColorSpace, nBands, eSampleType );
    if ( !LT_SUCCESS(setPixelProps(*poPixel)) )
        return LT_STS_Failure;

    if ( !LT_SUCCESS(setDimensions(poDS->GetRasterXSize(),
                                   poDS->GetRasterYSize())) )
        return LT_STS_Failure;

    if ( poDS->GetGeoTransform( adfGeoTransform ) == CE_None )
    {
#ifdef MRSID_SDK_40
        LTIGeoCoord oGeo( adfGeoTransform[0] + adfGeoTransform[1] / 2,
                          adfGeoTransform[3] + adfGeoTransform[5] / 2,
                          adfGeoTransform[1], adfGeoTransform[5],
                          adfGeoTransform[2], adfGeoTransform[4], NULL,
                          poDS->GetProjectionRef() );
#else
        LTIGeoCoord oGeo( adfGeoTransform[0] + adfGeoTransform[1] / 2,
                          adfGeoTransform[3] + adfGeoTransform[5] / 2,
                          adfGeoTransform[1], adfGeoTransform[5],
                          adfGeoTransform[2], adfGeoTransform[4], 
                          poDS->GetProjectionRef() );
#endif
        if ( !LT_SUCCESS(setGeoCoord( oGeo )) )
            return LT_STS_Failure;
    }

    /*int     bSuccess;
    double  dfNoDataValue = poDS->GetNoDataValue( &bSuccess );
    if ( bSuccess )
    {
        LTIPixel    oNoDataPixel( *poPixel );
        lt_uint16   iBand;

        for (iBand = 0; iBand < (lt_uint16)poDS->GetRasterCount(); iBand++)
            oNoDataPixel.setSampleValueFloat32( iBand, dfNoDataValue );
        if ( !LT_SUCCESS(setNoDataPixel( &oNoDataPixel )) )
            return LT_STS_Failure;
    }*/

    setDefaultDynamicRange();
    setClassicalMetadata();

    return LT_STS_Success;
}

/************************************************************************/
/*                             decodeStrip()                            */
/************************************************************************/

LT_STATUS MrSIDDummyImageReader::decodeStrip(LTISceneBuffer& stripData,
                                             const LTIScene& stripScene)
{
    const lt_int32  nXOff = stripScene.getUpperLeftCol();
    const lt_int32  nYOff = stripScene.getUpperLeftRow();
    const lt_int32  nBufXSize = stripScene.getNumCols();
    const lt_int32  nBufYSize = stripScene.getNumRows();
    const lt_uint16 nBands = poPixel->getNumBands();

    void    *pData = CPLMalloc(nBufXSize * nBufYSize * poPixel->getNumBytes());
    if ( !pData )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDDummyImageReader::decodeStrip(): "
                  "Cannot allocate enough space for scene buffer" );
        return LT_STS_Failure;
    }

    poDS->RasterIO( GF_Read, nXOff, nYOff, nBufXSize, nBufYSize, 
                    pData, nBufXSize, nBufYSize, eDataType, nBands, NULL, 
                    0, 0, 0 );
    stripData.importDataBSQ( pData );

    CPLFree( pData );

    return LT_STS_Success;
}


/************************************************************************/
/*                             FlushCache()                             */
/************************************************************************/

void MrSIDDataset::FlushCache()

{
    GDALDataset::FlushCache();
}

/************************************************************************/
/*                          MrSIDCreateCopy()                           */
/************************************************************************/

static GDALDataset *
MrSIDCreateCopy( const char * pszFilename, GDALDataset *poSrcDS,
                 int bStrict, char ** papszOptions, 
                 GDALProgressFunc pfnProgress, void * pProgressData )

{ 
    const char* pszVersion = CSLFetchNameValue(papszOptions, "VERSION");
    LT_STATUS eStat;

#ifdef DEBUG
    bool bMeter = false;
#else
    bool bMeter = true;
#endif    

    // Output Mrsid Version 2 file.
    if( pszVersion && atoi(pszVersion) == 2 )
    {
        int nXSize = poSrcDS->GetRasterXSize();
        int nYSize = poSrcDS->GetRasterYSize();
      
        if( !pfnProgress( 0.0, NULL, pProgressData ) )
            return NULL;
      
        // Create the file.                                               
        MrSIDDummyImageReader oImageReader( poSrcDS );
        eStat = oImageReader.initialize();
        if( eStat != LT_STS_Success )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "MrSIDDummyImageReader.Initialize failed.\n%s",
                      getLastStatusString( eStat ) );
            return NULL;
        }
      
        MG2ImageWriter oImageWriter(&oImageReader);
        eStat = oImageWriter.initialize();
        if( eStat != LT_STS_Success )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "MG3ImageWriter.initialize() failed.\n%s",
                      getLastStatusString( eStat ) );
            return NULL;
        }

        oImageWriter.setUsageMeterEnabled(bMeter);
   
        // set output filename
        oImageWriter.setOutputFileSpec( pszFilename );

        // Set defaults
        oImageWriter.params().setBlockSize(oImageWriter.params().getBlockSize());
        oImageWriter.setStripHeight(oImageWriter.getStripHeight());

        // check for compression option
        const char* pszValue = CSLFetchNameValue(papszOptions, "COMPRESSION");
        if( pszValue != NULL )
            oImageWriter.params().setCompressionRatio( atof(pszValue) );

        // set MrSID world file
        if( CSLFetchNameValue(papszOptions, "WORLDFILE") != NULL )
            oImageWriter.setWorldFileSupport( true );
      
        // write the scene
        const LTIScene oScene( 0, 0, nXSize, nYSize, 1.0 );
        eStat = oImageWriter.write( oScene );
        if( eStat != LT_STS_Success )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "MG2ImageWriter.write() failed.\n%s",
                      getLastStatusString( eStat ) );
            return NULL;
        }
    }
    // Output Mrsid Version 3 file.
    else
    {
        int nXSize = poSrcDS->GetRasterXSize();
        int nYSize = poSrcDS->GetRasterYSize();
      
        if( !pfnProgress( 0.0, NULL, pProgressData ) )
            return NULL;
      
        // Create the file.   
        MrSIDDummyImageReader oImageReader( poSrcDS );
        eStat = oImageReader.initialize();
        if( eStat != LT_STS_Success )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "MrSIDDummyImageReader.Initialize failed.\n%s",
                      getLastStatusString( eStat ) );
            return NULL;
        }
      
        MG3ImageWriter oImageWriter(&oImageReader);
        eStat = oImageWriter.initialize();
        if( eStat != LT_STS_Success )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "MG3ImageWriter.initialize() failed.\n%s",
                      getLastStatusString( eStat ) );
            return NULL;
        }
      
        // Set 64-bit Interface for large files.
        oImageWriter.setFileStream64(true);

        oImageWriter.setUsageMeterEnabled(bMeter);
      
        // set output filename
        oImageWriter.setOutputFileSpec( pszFilename );

        // Set defaults
        oImageWriter.setStripHeight(oImageWriter.getStripHeight());

        // set 2 pass optimizer option
        if( CSLFetchNameValue(papszOptions, "TWOPASS") != NULL )
            oImageWriter.params().setTwoPassOptimizer( true );

        // set MrSID world file
        if( CSLFetchNameValue(papszOptions, "WORLDFILE") != NULL )
            oImageWriter.setWorldFileSupport( true );
      
        const char* pszValue;
      
        // set filesize in KB
        pszValue = CSLFetchNameValue(papszOptions, "FILESIZE");
        if( pszValue != NULL )
            oImageWriter.params().setTargetFilesize( atoi(pszValue) );
	
        // write the scene
        const LTIScene oScene( 0, 0, nXSize, nYSize, 1.0 );
        eStat = oImageWriter.write( oScene );
        if( eStat != LT_STS_Success )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "MG2ImageWriter.write() failed.\n%s",
                      getLastStatusString( eStat ) );
            return NULL;
        }
    }
  
/* -------------------------------------------------------------------- */
/*      Re-open dataset, and copy any auxilary pam information.         */
/* -------------------------------------------------------------------- */
    GDALPamDataset *poDS = (GDALPamDataset *) 
        GDALOpen( pszFilename, GA_ReadOnly );

    if( poDS )
        poDS->CloneInfo( poSrcDS, GCIF_PAM_DEFAULT );

    return poDS;
}

#ifdef MRSID_J2K
/************************************************************************/
/*                           JP2CreateCopy()                            */
/************************************************************************/

static GDALDataset *
JP2CreateCopy( const char * pszFilename, GDALDataset *poSrcDS,
                 int bStrict, char ** papszOptions, 
                 GDALProgressFunc pfnProgress, void * pProgressData )

{ 
#ifdef DEBUG
    bool bMeter = false;
#else
    bool bMeter = true;
#endif    

    int nXSize = poSrcDS->GetRasterXSize();
    int nYSize = poSrcDS->GetRasterYSize();
    LT_STATUS  eStat;
      
    if( !pfnProgress( 0.0, NULL, pProgressData ) )
        return NULL;
      
    // Create the file.   
    MrSIDDummyImageReader oImageReader( poSrcDS );
    eStat = oImageReader.initialize();
    if( eStat != LT_STS_Success )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "MrSIDDummyImageReader.Initialize failed.\n%s",
                  getLastStatusString( eStat ) );
        return NULL;
    }
      
#ifdef MRSID_POST5
    JP2WriterManager oImageWriter(&oImageReader);
#else
    J2KImageWriter oImageWriter(&oImageReader);
#endif
    eStat = oImageWriter.initialize();
    if( eStat != LT_STS_Success )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "J2KImageWriter.Initialize failed.\n%s",
                  getLastStatusString( eStat ) );
        return NULL;
    }
      
    // Set 64-bit Interface for large files.
    oImageWriter.setFileStream64(true);

    oImageWriter.setUsageMeterEnabled(bMeter);
      
    // set output filename
    oImageWriter.setOutputFileSpec( pszFilename );

    // Set defaults
    //oImageWriter.setStripHeight(oImageWriter.getStripHeight());

    // set MrSID world file
    if( CSLFetchNameValue(papszOptions, "WORLDFILE") != NULL )
        oImageWriter.setWorldFileSupport( true );
      
    // check for compression option
    const char* pszValue = CSLFetchNameValue(papszOptions, "COMPRESSION");
    if( pszValue != NULL )
        oImageWriter.params().setCompressionRatio( atof(pszValue) );
	
    pszValue = CSLFetchNameValue(papszOptions, "XMLPROFILE");
    if( pszValue != NULL )
    {
        LTFileSpec xmlprofile(pszValue);
        eStat = oImageWriter.params().readProfile(xmlprofile);
        if( eStat != LT_STS_Success )
        {
            CPLError( CE_Failure, CPLE_AppDefined,
                      "JPCWriterParams.readProfile failed.\n%s",
                      getLastStatusString( eStat ) );
            return NULL;
        }
    }

    // write the scene
    const LTIScene oScene( 0, 0, nXSize, nYSize, 1.0 );
    eStat = oImageWriter.write( oScene );
    if( eStat != LT_STS_Success )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "J2KImageWriter.write() failed.\n%s",
                  getLastStatusString( eStat ) );
        return NULL;
    }
  
/* -------------------------------------------------------------------- */
/*      Re-open dataset, and copy any auxilary pam information.         */
/* -------------------------------------------------------------------- */
    GDALPamDataset *poDS = (GDALPamDataset *) 
        GDALOpen( pszFilename, GA_ReadOnly );

    if( poDS )
        poDS->CloneInfo( poSrcDS, GCIF_PAM_DEFAULT );

    return poDS;
}
#endif /* MRSID_J2K */
#endif /* MRSID_ESDK */

/************************************************************************/
/*                        GDALRegister_MrSID()                          */
/************************************************************************/

void GDALRegister_MrSID()

{
    GDALDriver  *poDriver;

/* -------------------------------------------------------------------- */
/*      MrSID driver.                                                   */
/* -------------------------------------------------------------------- */
    if( GDALGetDriverByName( "MrSID" ) == NULL )
    {
        poDriver = new GDALDriver();

        poDriver->SetDescription( "MrSID" );
        poDriver->SetMetadataItem( GDAL_DMD_LONGNAME,
                        "Multi-resolution Seamless Image Database (MrSID)" );
        poDriver->SetMetadataItem( GDAL_DMD_HELPTOPIC, "frmt_mrsid.html" );
        poDriver->SetMetadataItem( GDAL_DMD_EXTENSION, "sid" );

#ifdef MRSID_ESDK
        poDriver->SetMetadataItem( GDAL_DMD_CREATIONDATATYPES, 
                                   "Byte Int16 UInt16 Int32 UInt32 Float32 Float64" );
        poDriver->SetMetadataItem( GDAL_DMD_CREATIONOPTIONLIST,
"<CreationOptionList>"
// Version 2 Options
"   <Option name='COMPRESSION' type='double' description='Set compression ratio (0.0 default is meant to be lossless)'/>"
// Version 3 Options
"   <Option name='TWOPASS' type='int' description='Use twopass optimizer algorithm'/>"
"   <Option name='FILESIZE' type='int' description='Set target file size (0 implies lossless compression)'/>"
// Version 2 and 3 Option
"   <Option name='WORLDFILE' type='boolean' description='Write out world file'/>"
// Version Type
"   <Option name='VERSION' type='int' description='Valid versions are 2 and 3, default = 3'/>"
"</CreationOptionList>" );

        poDriver->pfnCreateCopy = MrSIDCreateCopy;
#endif

        poDriver->pfnOpen = MrSIDOpen;

        GetGDALDriverManager()->RegisterDriver( poDriver );
    }

/* -------------------------------------------------------------------- */
/*      JP2MRSID driver.                                                */
/* -------------------------------------------------------------------- */
#ifdef MRSID_J2K
    if( GDALGetDriverByName( "JP2MrSID" ) == NULL )
    {
        poDriver = new GDALDriver();

        poDriver->SetDescription( "JP2MrSID" );
        poDriver->SetMetadataItem( GDAL_DMD_LONGNAME,
                        "MrSID JPEG2000" );
        poDriver->SetMetadataItem( GDAL_DMD_HELPTOPIC, "frmt_jp2mrsid.html" );
        poDriver->SetMetadataItem( GDAL_DMD_EXTENSION, "jp2" );

#ifdef MRSID_ESDK
        poDriver->SetMetadataItem( GDAL_DMD_CREATIONDATATYPES, 
                                   "Byte Int16 UInt16" );
        poDriver->SetMetadataItem( GDAL_DMD_CREATIONOPTIONLIST,
"<CreationOptionList>"
"   <Option name='COMPRESSION' type='double' description='Set compression ratio (0.0 default is meant to be lossless)'/>"
"   <Option name='WORLDFILE' type='boolean' description='Write out world file'/>"
"   <Option name='XMLPROFILE' type='string' description='Use named xml profile file'/>"
"</CreationOptionList>" );

        poDriver->pfnCreateCopy = JP2CreateCopy;
#endif
        poDriver->pfnOpen = JP2Open;

        GetGDALDriverManager()->RegisterDriver( poDriver );
    }
#endif /* def MRSID_J2K */
}
