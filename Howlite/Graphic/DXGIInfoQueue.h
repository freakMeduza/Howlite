#pragma once

namespace Howlite {

	class HDXGIInfoQueue {
	public:
		HDXGIInfoQueue();
		~HDXGIInfoQueue();

		HDXGIInfoQueue(HDXGIInfoQueue&&) = delete;
		HDXGIInfoQueue(const HDXGIInfoQueue&) = delete;

		HDXGIInfoQueue& operator=(HDXGIInfoQueue&&) = delete;
		HDXGIInfoQueue& operator=(const HDXGIInfoQueue&) = delete;

		/** 
		 * Clear Message Queue
		 */
		void ClearMessageQueue();
		
		/**
		 * Get Messages From Queue
		 * @return 
		 */
		std::vector<std::string> GetMessageQueue();

	private:
		unsigned long long mNext = 0u;
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> mInfoQueue;
		
	};

}