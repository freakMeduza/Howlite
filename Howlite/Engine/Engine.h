#pragma once

namespace Howlite {

	class HEngine {
	public:
		HEngine(HINSTANCE Instance);
		~HEngine();

		HEngine(HEngine&&) = delete;
		HEngine(const HEngine&) = delete;

		HEngine& operator=(HEngine&&) = delete;
		HEngine& operator=(const HEngine&) = delete;

		/**
		 * Get Engine Instance
		 * @return 
		 */
		static HEngine& GetInstance();

		/**
		 * Run Engine
		 * @return 
		 */
		int Run();

		/**
		 * Get Window Instance
		 * @return 
		 */
		class HWindow& GetWindowInstance();

		/** 
		 * Get Graphic System Instance
		 */
		class HGraphicSystem& GetGraphicSystemInstance();

		/**
		 * Get Input System Instance
		 * @return 
		 */
		class HInputSystem& GetInputSystemInstance();

		/** 
		 * Get UI System Instance
		 * @return 
		 */
		class HUISystem& GetUISystemInstance();

	private:
		bool OnKeyPressed(class HKeyPressedEvent& Event);
		bool OnWindowClosed(class HWindowClosedEvent& Event);
		bool OnWindowResized(class HWindowResizedEvent& Event);
		
		bool mIsRun{ false };
		bool mVSyncIsEnabled{ false };
		
		ScopedPointer<class HWindow> mWindow{ nullptr };
		
		static HEngine* mEngineInstance;
	};

}
