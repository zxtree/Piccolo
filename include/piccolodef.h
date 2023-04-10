// piccolo definitions
// author: zxtree @2023-04-08
#pragma once

#define PCL_NAMESPACE_OPEN \
namespace Piccolo {
#define PCL_NAMESPACE_CLOSE \
} /* piccolo namespace close */
#define PCL_NAMESPACE_USE \
using namespace Piccolo;

#define PCL_GAME_NS_OPEN \
namespace Piccolo {\
namespace Game {
#define PCL_GAME_NS_CLOSE \
} /* game namespace close */ \
} /* piccolo namespace close */
#define PCL_GAME_NS_USE \
using namespace Piccolo::Game;

#define PCL_ENGINE_NS_OPEN \
namespace Piccolo {\
namespace Engine {
#define PCL_ENGINE_NS_CLOSE \
} /* engine namespace close */ \
} /* piccolo namespace close */
#define PCL_ENGINE_NS_USE \
using namespace Piccolo::Engine;

#define PCL_EDITOR_NS_OPEN \
namespace Piccolo {\
namespace Editor {
#define PCL_EDITOR_NS_CLOSE \
} /* editor namespace close */ \
} /* piccolo namespace close */
#define PCL_EDITOR_NS_USE \
using namespace Piccolo::Editor;