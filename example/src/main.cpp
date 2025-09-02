#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <ScriptNode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		Build(this).schedule([this](float dt) {
			auto file = std::filesystem::path(__FILE__).parent_path() / "example.tiny";

			if (!std::filesystem::exists(file))
				return;

			auto last_write = std::filesystem::last_write_time(file);
			static decltype(last_write) last_check;

			if (last_check != last_write) {
				last_check = last_write;

				std::string code = utils::file::readString(file).unwrap();

				if (auto node = uiscripter::ScriptNode::create(code)) {
					this->removeChildByID("script-node");
					node->setID("script-node");
					node->setZOrder(99999);
					Build(node).parent(this).center();
				}
			}
		});

		return true;
	}
};
