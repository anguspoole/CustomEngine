#include "cBasicTextureManager.h"

#include <sstream>

#include <iostream>

CRITICAL_SECTION tm_CS_textureNameMap;

//void cBasicTextureManager::SetBasePath(std::string basepath)
//{
//	this->m_basePath = basepath;
//	return;
//}

cBasicTextureManager::cBasicTextureManager()
{
	InitializeCriticalSection( &tm_CS_textureNameMap );

	return;
}

cBasicTextureManager::~cBasicTextureManager()
{
	DeleteCriticalSection( &tm_CS_textureNameMap );

	return;
}
//bool cBasicTextureManager::Create2DTextureFromBMPFile_ASYNC( std::string textureFileName, bool bGenerateMIPMap )
bool cBasicTextureManager::Create2DTextureFromBMPFile_ASYNC( s2DTextureLoadParams textureLoadParams, bool bGenerateMIPMap )
{
	std::string fileToLoadFullPath = textureLoadParams.basePath + "/" + textureLoadParams.filename;

	// Assume that the texture will be loaded...
	CTextureFromBMP* pTempTexture = new CTextureFromBMP();

	pTempTexture->LoadBMP2_Async( fileToLoadFullPath );
	
	this->m_LockTextureNameMap();
	this->m_map_TexNameToTexture[ textureLoadParams.filename ] = pTempTexture;
	this->m_UnlockTextureNameMap();


//	theTextureLoadStatus.pTexture->CreateNewTextureFromBMPFile_LoadFromDisk( fileToLoadFullPath );
//	// Load it into the GPU
//	theTextureLoadStatus.pTexture->CreateNewTextureFromBMPFile_CopyToGPU( bGenerateMIPMap );
//
//	theTextureLoadStatus.textureLoadStatus = cBasicTextureManager::IS_IN_GPU;


	return true;
}


void cBasicTextureManager::m_LockTextureNameMap(void)
{
	EnterCriticalSection( &tm_CS_textureNameMap );
	return;
}

void cBasicTextureManager::m_UnlockTextureNameMap(void)
{
	LeaveCriticalSection( &tm_CS_textureNameMap );
	return;
}

//bool cBasicTextureManager::Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap )
bool cBasicTextureManager::Create2DTextureFromBMPFile( s2DTextureLoadParams textureLoadParams, bool bGenerateMIPMap )
{
	std::string fileToLoadFullPath = textureLoadParams.basePath + "/" + textureLoadParams.filename;

	// Assume that the texture will be loaded...
	CTextureFromBMP* pTempTexture = new CTextureFromBMP();


	if ( ! pTempTexture->CreateNewTextureFromBMPFile2( textureLoadParams.filename, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( fileToLoadFullPath );
		this->m_appendErrorString( "\n" );
		return false;
	}

	pTempTexture->CreateNewTextureFromBMPFile_LoadFromDisk( fileToLoadFullPath );
	// Load it into the GPU
	pTempTexture->CreateNewTextureFromBMPFile_CopyToGPU( bGenerateMIPMap );



	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	
	this->m_LockTextureNameMap();
	this->m_map_TexNameToTexture[ textureLoadParams.filename ] = pTempTexture;
	this->m_UnlockTextureNameMap();
	//this->m_map_TexNameToTexture[ textureFileName ] = theTextureLoadStatus;

	return true;
}


void cBasicTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

bool cBasicTextureManager::isTexture2D(std::string textureFileName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture
		= this->m_map_TexNameToTexture.find( textureFileName );

	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return false;
	}

	return itTexture->second->getIs2DTexture();
}

bool cBasicTextureManager::isTextureCubeMap(std::string textureFileName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture
		= this->m_map_TexNameToTexture.find( textureFileName );

	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return false;
	}

	return itTexture->second->getIsCubeMap();
}


GLuint cBasicTextureManager::getTextureIDFromName( std::string textureFileName )
{	
	std::map< std::string, CTextureFromBMP* >::iterator itTexture
		= this->m_map_TexNameToTexture.find( textureFileName );

	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return 0;
	}

	//HACK: Cube maps are NOT loaded with async...
	if ( itTexture->second->getIsCubeMap() )
	{
		return itTexture->second->getTextureNumber();
	}

	// Has the texture actually been loaded into the GPU
	if ( itTexture->second->textureLoadStatus == CTextureFromBMP::IS_READY_TO_BE_COPIED_TO_GPU )
	{
		// Call the copy the data over call
		itTexture->second->CreateNewTextureFromBMPFile_CopyToGPU( true );
		// When this returns the texture has been loaded into the GPU, so we can return the ID
	}



	// Has the texture actually been loaded into the GPU
	if ( itTexture->second->textureLoadStatus != CTextureFromBMP::IS_IN_GPU )
	{
		return 0;
	}

	// Reutrn texture number (from OpenGL genTexture)
	//return itTexture->second->getTextureNumber();
	return itTexture->second->getTextureNumber();
}


void cBasicTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}


// Picks a random texture from the textures loaded
std::string cBasicTextureManager::PickRandomTexture(void)
{
	if ( this->m_map_TexNameToTexture.empty() )
	{
		// There are no textures loaded, yet.
		return "";
	}

	int textureIndexRand = rand() % (this->m_map_TexNameToTexture.size() + 1);
	if ( textureIndexRand >= this->m_map_TexNameToTexture.size() )
	{
		textureIndexRand = 0;
	}

	std::map< std::string, CTextureFromBMP* >::iterator itTex = this->m_map_TexNameToTexture.begin();

	for ( unsigned int count = 0; count != textureIndexRand; count++, itTex++ ) 
	{}

	return itTex->second->getTextureName();
}


//bool cBasicTextureManager::CreateCubeTextureFromBMPFiles( 
//                                    std::string cubeMapName, 
//		                            std::string posX_fileName, std::string negX_fileName, 
//		                            std::string posY_fileName, std::string negY_fileName, 
//									std::string posZ_fileName, std::string negZ_fileName, 
//									bool bIsSeamless, std::string &errorString )
bool cBasicTextureManager::CreateCubeTextureFromBMPFiles( sCubeMapTextureLoadParams loadParams, std::string &errorString )
{
	std::string posX_fileName_FullPath = loadParams.basePath + "/" + loadParams.posX_fileName;
	std::string negX_fileName_FullPath = loadParams.basePath + "/" + loadParams.negX_fileName;
	std::string posY_fileName_FullPath = loadParams.basePath + "/" + loadParams.posY_fileName;
	std::string negY_fileName_FullPath = loadParams.basePath + "/" + loadParams.negY_fileName;
	std::string posZ_fileName_FullPath = loadParams.basePath + "/" + loadParams.posZ_fileName;
	std::string negZ_fileName_FullPath = loadParams.basePath + "/" + loadParams.negZ_fileName;

	GLenum errorEnum;
	std::string errorDetails;

	CTextureFromBMP* pTempTexture = new CTextureFromBMP();

	if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles( 
				loadParams.cubeMapName, 
				posX_fileName_FullPath, negX_fileName_FullPath, 
	            posY_fileName_FullPath, negY_fileName_FullPath, 
	            posZ_fileName_FullPath, negZ_fileName_FullPath, 
	            loadParams.bIsSeamless, errorEnum, errorString, errorDetails ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( loadParams.cubeMapName );
		this->m_appendErrorString( " because:\n" );
		this->m_appendErrorString( errorString );
		this->m_appendErrorString( "\n" );
		this->m_appendErrorString( errorDetails );
		errorString += ("\n" + errorDetails);
		return false;
	}//if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles()

	pTempTexture->textureLoadStatus = CTextureFromBMP::IS_IN_GPU;

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	
	this->m_LockTextureNameMap();
	this->m_map_TexNameToTexture[ loadParams.cubeMapName ] = pTempTexture;
	this->m_UnlockTextureNameMap();

	return true;
}



// The async loading part
bool cBasicTextureManager::CheckForPendingTextureGPULoads(void)
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture;
	
	
	for ( itTexture = this->m_map_TexNameToTexture.begin();
		  itTexture != this->m_map_TexNameToTexture.end();
		  itTexture++ )
	{
		// Is this one ready to load?
		if ( itTexture->second->textureLoadStatus == CTextureFromBMP::IS_READY_TO_BE_COPIED_TO_GPU )
		{
			// Copy it into the GPU

			this->m_LoadPendingTextureIntoGPU( itTexture );

			// Then exit, so I only load ONE texture at a time
		}
	}

	// Only gets here if there is nothing to do...
	return true;
}


// This does the actual loading of the textures
bool cBasicTextureManager::m_LoadPendingTextureIntoGPU(
	std::map< std::string, 
	CTextureFromBMP* >::iterator itTextureToLoad )
{

	itTextureToLoad->second->CreateNewTextureFromBMPFile_CopyToGPU(true);


	return true;
}

	
