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
	Channel mD0Channel;
	Channel mD1Channel;
	Channel mEnChannel;
	U32 mPreamble;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mRefClkChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mD0ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mD1ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mEnChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mPreambleInterface;
};

#endif //RMII_ANALYZER_SETTINGS
