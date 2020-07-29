#pragma once

namespace Howlite {

	class HDrawable {
	public:
		HDrawable() = default;
		virtual ~HDrawable() = default;

		HDrawable(HDrawable&&) = delete;
		HDrawable(const HDrawable&) = delete;

		HDrawable& operator=(HDrawable&&) = delete;
		HDrawable& operator=(const HDrawable&) = delete;

		/**
		 * Get Drawable Transform Matrix
		 * @return 
		 */
		virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;

		/**
		 * Update
		 * @param Tick 
		 * @return 
		 */
		virtual void Update(HTick Tick) noexcept = 0;

		/**
		 * Draw
		 * @param GraphicSystem 
		 */
		void Draw(class HGraphicSystem& GraphicSystem) const;

	protected:
		void AddBind(SharedPointer<class HBindable> Bindable);

		template<typename T>
		T* QueryBindable() const noexcept
		{
			for(const SharedPointer<class HBindable>& bind : mBindables)
			{
				if(auto ptr = dynamic_cast<T*>(bind.get()))
				{
					return ptr;
				}
			}

			return nullptr;
		}

	private:
		class HIndexBuffer* mIndexBuffer{ nullptr };

		std::vector<SharedPointer<class HBindable>> mBindables;

	};

}
