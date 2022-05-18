#pragma once

#include "../../Core/Renderer.h"

#include <vulkan/vulkan_core.h>
#include <memory>
#include <unordered_map>

namespace Hephaestus
{
	class Instance;
	class Device;
	class PhysicalDevice;

	/*
	* 
	*/
	class Renderer : public IRenderer
	{
	public:
		Renderer();
		~Renderer();

		virtual bool OnCreate( const RendererInfo& rendererInfo ) override final;
		virtual void OnDestroy() override final;
		virtual void RenderScene( IScene* scene ) override final;

		/*
		* @brief Add a device extension
		* @param extension The extension name
		* @param optional (Optional) Whether the extension is optional
		*/
		void AddDeviceExtension( const char* extension, bool optional = false );

		/*
		* @brief Add a instance extension
		* @param extension The extension name
		* @param optional (Optional) Whether the extension is optional
		*/
		void AddInstanceExtension( const char* extension, bool optional = false );

		/*
		* @brief Get device extensions.
		* @return Map of device extensions and whether or not they are optional. Default is empty map.
		*/
		const std::unordered_map<const char*, bool> GetDeviceExtensions();

		/*
		* @brief Get instance extensions.
		* @return Map of instance extensions and whether or not they are optional. Default is empty map.
		*/
		const std::unordered_map<const char*, bool> GetInstanceExtensions();

	private:
		/* @brief	The Vulkan Instance*/
		std::unique_ptr<Instance> m_instance;

		/* @brief	The Vulkan Surface*/
		VkSurfaceKHR m_surface;

		/* @brief	The Vulkan Device*/
		std::unique_ptr<Device> m_device;


		/** @brief Set of device extensions to be enabled for this example and whether they are optional (must be set in the derived constructor) */
		std::unordered_map<const char*, bool> m_deviceExtensions;

		/** @brief Set of instance extensions to be enabled for this example and whether they are optional (must be set in the derived constructor) */
		std::unordered_map<const char*, bool> m_instanceExtensions;


		// ECS::ISystsem::Update()
		virtual void Update( const float deltaTime ) override final;
		// DEV-NOTE: Will be called inside of ECS::ISystsem::Update() and will work with the components inside of the signature to Render MeshComponents
		virtual void Render( MeshComponent* mesh, TransformComponent* transform, Camera* camera ) override final;
	};
};


