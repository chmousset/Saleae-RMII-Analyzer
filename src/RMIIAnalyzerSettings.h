#ifndef RMII_ANALYZER_SETTINGS
#define RMII_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class RMIIAnalyzerSettings : public AnalyzerSettings
{
public:
	RMIIAnalyzerSettings();
	virtual ~RMIIAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	Channel mRefClkChannel;
	Channel mRx0Channel;
	Channel mRx1Channel;
	Channel mCrsDvChannel;
	// Channel mTx0Channel;
	// Channel mTx1Channel;
	// Channel mTxEnChannel;
	U32 mPreamble;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mRefClkChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mRx0ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mRx1ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mCrsDvChannelInterface;
	// std::auto_ptr< AnalyzerSettingInterfaceChannel >	mTx0ChannelInterface;
	// std::auto_ptr< AnalyzerSettingInterfaceChannel >	mTx1ChannelInterface;
	// std::auto_ptr< AnalyzerSettingInterfaceChannel >	mTxEnChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mPreambleInterface;
};

#endif //RMII_ANALYZER_SETTINGS
