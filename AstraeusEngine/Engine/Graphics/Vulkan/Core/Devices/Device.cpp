#include "Device.h"

#include <DebugLog.h>

namespace Hephaestus
{
	Device::Device( const Device_Constructor& deviceConstructor ) :
		m_logicaldevice( VK_NULL_HANDLE ),
		m_physicalDevice( deviceConstructor.physicalDevice ),
		m_surface( deviceConstructor.surface )
	{
		DEBUG_LOG( LOG::INFO, "Selected GPU: " + std::string( m_physicalDevice.GetProperties().deviceName ) );
		CONSOLE_LOG( LOG::INFO, "Selected GPU: " + std::string( m_physicalDevice.GetProperties().deviceName ) );
	}

	Device::~Device()
	{}

	bool Device::OnCreate()
	{

		return true;
	}

	void Device::OnDestroy()
	{}

}