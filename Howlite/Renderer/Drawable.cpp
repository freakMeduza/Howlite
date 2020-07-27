#include "HowlitePCH.h"
#include "Drawable.h"

#include "IndexBuffer.h"

namespace Howlite {

	void HDrawable::Draw(HGraphicSystem& GraphicSystem) const
	{
		H_ASSERT(mIndexBuffer != nullptr, "Failed to draw 'mIndexBuffer == nullptr'.");
		for(const SharedPointer<HBindable>& bind : mBindables)
		{
			bind->Bind(GraphicSystem);
		}

		GraphicSystem.DrawIndexed(mIndexBuffer->GetIndicesCount());
	}

	void HDrawable::AddBind(SharedPointer<HBindable> Bindable)
	{
		if(HIndexBuffer* indexBuffer = dynamic_cast<HIndexBuffer*>(Bindable.get()))
		{
			H_ASSERT(mIndexBuffer == nullptr, "Attempting to add second index buffer. 'mIndexBuffer != nullptr'.")
			mIndexBuffer = indexBuffer;
		}

		mBindables.push_back(Bindable);
	}

}