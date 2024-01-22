#include "RMIIAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


RMIIAnalyzerSettings::RMIIAnalyzerSettings()
:	mRefClkChannel( UNDEFINED_CHANNEL ),
	mRx0Channel( UNDEFINED_CHANNEL ),
	mRx1Channel( UNDEFINED_CHANNEL ),
	mCrsDvChannel( UNDEFINED_CHANNEL ),
	// mTx0Channel( UNDEFINED_CHANNEL ),
	// mTx1Channel( UNDEFINED_CHANNEL ),
	// mTxEnChannel( UNDEFINED_CHANNEL ),
	mPreamble( 213 ) // 0xD5
{
	mRefClkChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mRx0ChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mRx1ChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	mCrsDvChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	// mTx0ChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	// mTx1ChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );
	// mTxEnChannelInterface.reset(new AnalyzerSettingInterfaceChannel() );

	mRefClkChannelInterface->SetTitleAndTooltip("REF_CLK", "50 MHz reference clock");
	mRx0ChannelInterface->SetTitleAndTooltip("RX[0]", "RX data, bit 0");
	mRx1ChannelInterface->SetTitleAndTooltip("RX[1]", "RX data, bit 1");
	mCrsDvChannelInterface->SetTitleAndTooltip("CRS_DV", "Carrier sense/data valid");
	// mTx0ChannelInterface->SetTitleAndTooltip("TX[0]", "TX data, bit 0");
	// mTx1ChannelInterface->SetTitleAndTooltip("TX[1]", "TX data, bit 1");
	// mTxEnChannelInterface->SetTitleAndTooltip("TX_EN", "Transmit Enable");

	mRefClkChannelInterface->SetChannel( mRefClkChannel );
	mRx0ChannelInterface->SetChannel( mRx0Channel );
	mRx1ChannelInterface->SetChannel( mRx1Channel );
	mCrsDvChannelInterface->SetChannel( mCrsDvChannel );
	// mTx0ChannelInterface->SetChannel( mTx0Channel );
	// mTx1ChannelInterface->SetChannel( mTx1Channel );
	// mTxEnChannelInterface->SetChannel( mTxEnChannel );

	mPreambleInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mPreambleInterface->SetTitleAndTooltip( "Preamble", "preamble value (normally 0xD5)" );
	mPreambleInterface->SetMin(0);
	mPreambleInterface->SetMax(255);

	AddInterface( mRefClkChannelInterface.get() );
	AddInterface( mRx0ChannelInterface.get() );
	AddInterface( mRx1ChannelInterface.get() );
	AddInterface( mCrsDvChannelInterface.get() );
	// AddInterface( mTx0ChannelInterface.get() );
	// AddInterface( mTx1ChannelInterface.get() );
	// AddInterface( mTxEnChannelInterface.get() );
	AddInterface( mPreambleInterface.get() );

	AddExportOption( 0, "Export packets" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mRefClkChannel, "REF_CLK" , false );
	AddChannel( mRx0Channel, "RX[" , false );
	AddChannel( mRx1Channel, "RX[" , false );
	AddChannel( mCrsDvChannel, "CRS_DV" , false );
	// AddChannel( mTx0Channel, "TX[" , false );
	// AddChannel( mTx1Channel, "TX[" , false );
	// AddChannel( mTxEnChannel, "TX_EN" , false );
}

RMIIAnalyzerSettings::~RMIIAnalyzerSettings()
{
}

bool RMIIAnalyzerSettings::SetSettingsFromInterfaces()
{
	mRefClkChannel = mRefClkChannelInterface->GetChannel();
	mRx0Channel = mRx0ChannelInterface->GetChannel();
	mRx1Channel = mRx1ChannelInterface->GetChannel();
	mCrsDvChannel = mCrsDvChannelInterface->GetChannel();
	// mTx0Channel = mTx0ChannelInterface->GetChannel();
	// mTx1Channel = mTx1ChannelInterface->GetChannel();
	// mTxEnChannel = mTxEnChannelInterface->GetChannel();
	mPreamble = mPreambleInterface->GetInteger();

	ClearChannels();
	AddChannel( mRefClkChannel, "REF_CLK" , true );
	AddChannel( mRx0Channel, "RX[" , true );
	AddChannel( mRx1Channel, "RX[" , true );
	AddChannel( mCrsDvChannel, "CRS_DV" , true );
	// AddChannel( mTx0Channel, "TX[" , true );
	// AddChannel( mTx1Channel, "TX[" , true );
	// AddChannel( mTxEnChannel, "TX_EN" , true );

	return true;
}

void RMIIAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mRefClkChannelInterface->SetChannel( mRefClkChannel );
	mRx0ChannelInterface->SetChannel( mRx0Channel );
	mRx1ChannelInterface->SetChannel( mRx1Channel );
	mCrsDvChannelInterface->SetChannel( mCrsDvChannel );
	// mTx0ChannelInterface->SetChannel( mTx0Channel );
	// mTx1ChannelInterface->SetChannel( mTx1Channel );
	// mTxEnChannelInterface->SetChannel( mTxEnChannel );
	mPreambleInterface->SetInteger( mPreamble );
}

void RMIIAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mRefClkChannel;
	text_archive >> mRx0Channel;
	text_archive >> mRx1Channel;
	text_archive >> mCrsDvChannel;
	// text_archive >> mTx0Channel;
	// text_archive >> mTx1Channel;
	// text_archive >> mTxEnChannel;
	text_archive >> mPreamble;


	ClearChannels();
	AddChannel( mRefClkChannel, "REF_CLK" , false );
	AddChannel( mRx0Channel, "RX[" , false );
	AddChannel( mRx1Channel, "RX[" , false );
	AddChannel( mCrsDvChannel, "CRS_DV" , false );
	// AddChannel( mTx0Channel, "TX[" , false );
	// AddChannel( mTx1Channel, "TX[" , false );
	// AddChannel( mTxEnChannel, "TX_EN" , false );

	UpdateInterfacesFromSettings();
}

const char* RMIIAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mRefClkChannel;
	text_archive << mRx0Channel;
	text_archive << mRx1Channel;
	text_archive << mCrsDvChannel;
	// text_archive << mTx0Channel;
	// text_archive << mTx1Channel;
	// text_archive << mTxEnChannel;
	text_archive << mPreamble;

	return SetReturnString( text_archive.GetString() );
}
