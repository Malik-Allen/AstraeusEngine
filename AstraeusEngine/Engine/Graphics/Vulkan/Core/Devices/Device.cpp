#include "Device.h"

namespace Hephaestus
{
	Device::Device()
	{}

	Device::~Device()
	{}

	bool Device::OnCreate( const DeviceConstructor& deviceConstructor )
	{
		m_physicalDevice = deviceConstructor.physicalDevice;
		return false;
	}

	void Device::OnDestroy()
	{}

}