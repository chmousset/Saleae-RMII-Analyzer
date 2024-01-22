#include "RMIIAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


RMIIAnalyzerSettings::RMIIAnalyzerSettings()
:	mRefClkChannel( UNDEFINED_CHANNEL ),
	mD0Channel( UNDEFINED_CHANNEL ),
	mD1Channel( UNDEFINED_CHANNEL ),
	mEnChannel( UNDEFINED_CHANNEL ),
	mPreamble( 213 ) // 0xD5
{
	mRefClkChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mD0ChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mD1ChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mEnChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );

	mRefClkChannelInterface->SetTitleAndTooltip("REF_CLK", "50 MHz reference clock");
	mD0ChannelInterface->SetTitleAndTooltip("D[0]", "D/TX data, bit 0");
	mD1ChannelInterface->SetTitleAndTooltip("D[1]", "D/TX data, bit 1");
	mEnChannelInterface->SetTitleAndTooltip("EN", "CRS_DV, TX_EN or D_DV");

	mRefClkChannelInterface->SetChannel( mRefClkChannel );
	mD0ChannelInterface->SetChannel( mD0Channel );
	mD1ChannelInterface->SetChannel( mD1Channel );
	mEnChannelInterface->SetChannel( mEnChannel );

	mPreambleInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mPreambleInterface->SetTitleAndTooltip( "Preamble", "preamble value (normally 0xD5)" );
	mPreambleInterface->SetMin(0);
	mPreambleInterface->SetMax(255);

	AddInterface( mRefClkChannelInterface.get() );
	AddInterface( mD0ChannelInterface.get() );
	AddInterface( mD1ChannelInterface.get() );
	AddInterface( mEnChannelInterface.get() );
	AddInterface( mPreambleInterface.get() );

	AddExportOption( 0, "Export packets" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mRefClkChannel, "REF_CLK" , false );
	AddChannel( mD0Channel, "D[" , false );
	AddChannel( mD1Channel, "D[" , false );
	AddChannel( mEnChannel, "EN" , false );
}

RMIIAnalyzerSettings::~RMIIAnalyzerSettings()
{
}

bool RMIIAnalyzerSettings::SetSettingsFromInterfaces()
{
	mRefClkChannel = mRefClkChannelInterface->GetChannel();
	mD0Channel = mD0ChannelInterface->GetChannel();
	mD1Channel = mD1ChannelInterface->GetChannel();
	mEnChannel = mEnChannelInterface->GetChannel();
	mPreamble = mPreambleInterface->GetInteger();

	ClearChannels();
	AddChannel( mRefClkChannel, "REF_CLK" , true );
	AddChannel( mD0Channel, "D[" , true );
	AddChannel( mD1Channel, "D[" , true );
	AddChannel( mEnChannel, "CRS_DV" , true );

	return true;
}

void RMIIAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mRefClkChannelInterface->SetChannel( mRefClkChannel );
	mD0ChannelInterface->SetChannel( mD0Channel );
	mD1ChannelInterface->SetChannel( mD1Channel );
	mEnChannelInterface->SetChannel( mEnChannel );
	mPreambleInterface->SetInteger( mPreamble );
}

void RMIIAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mRefClkChannel;
	text_archive >> mD0Channel;
	text_archive >> mD1Channel;
	text_archive >> mEnChannel;
	text_archive >> mPreamble;


	ClearChannels();
	AddChannel( mRefClkChannel, "REF_CLK" , false );
	AddChannel( mD0Channel, "D[" , false );
	AddChannel( mD1Channel, "D[" , false );
	AddChannel( mEnChannel, "CRS_DV" , false );

	UpdateInterfacesFromSettings();
}

const char* RMIIAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mRefClkChannel;
	text_archive << mD0Channel;
	text_archive << mD1Channel;
	text_archive << mEnChannel;
	text_archive << mPreamble;

	return SetReturnString( text_archive.GetString() );
}
