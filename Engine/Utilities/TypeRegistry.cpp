#include "TypeRegistry.h"

namespace Howlite {

	class IEntity;
	class ISystem;
	class IComponent;

	TypeId TTypeRegistry<IEntity>::mTypeCount = 0;
	TypeId TTypeRegistry<ISystem>::mTypeCount = 0;
	TypeId TTypeRegistry<IComponent>::mTypeCount = 0;

	template class TTypeRegistry<IEntity>;
	template class TTypeRegistry<ISystem>;
	template class TTypeRegistry<IComponent>;

}