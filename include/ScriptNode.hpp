#include "ScriptBaseNode.hpp"
#include <UIBuilder.hpp>

namespace uiscripter {

	class ScriptNode : public ScriptBaseNode {
		CCMenu* m_menu;
		CCArray* m_nodeBinds;
	public:
		static ScriptNode* create(std::string const& code) {
			auto ret = new ScriptNode();
			if (ret && ret->init(code)) {
				ret->autorelease();
				return ret;
			}
			else {
				delete ret;
				return nullptr;
			}
		}

		inline ~ScriptNode() {
			m_nodeBinds->release();
		}

		inline void bindNode(std::string const& name, CCNode* n) {
			m_nodeBinds->addObject(n);
			bindVariable(name, "Node", n);
		}

		inline void bindConstructors() {
			bindFunction<"Node(): Node">([this]() -> auto {
				auto node = CCNode::create();
				this->addChild(node);
				return node;
			});

			bindFunction<"Label(str, str): Node">([this](std::string text, std::string font) -> auto {
				auto lab = CCLabelBMFont::create(text.c_str(), font.c_str());
				this->addChild(lab);
				return lab;
			});

			bindFunction<"Sprite(str): Node">([this](std::string name) -> auto {
				auto spr = CCSprite::createWithSpriteFrameName(name.c_str());

				// because textureldr wants to be silly
				/*auto fr = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
				log::info("{} {}", fr->m_strFrameName, name);
				if (name != fr->m_strFrameName) {
					spr = nullptr;
				}*/

				if (spr == nullptr)
					spr = CCSprite::create(name.c_str());
				if (spr == nullptr)
					spr = CCSprite::createWithSpriteFrameName("exMark_001.png");

				this->addChild(spr);

				return spr;
			});

			bindFunction<"WebSprite(str): Node">([this](std::string url) -> auto {
				auto req = utils::web::WebRequest().get(url);
				while (req.isPending()) {}

				auto spr = CCSprite::createWithSpriteFrameName("exMark_001.png");

				if (auto resp = req.getFinishedValue()) {
					auto img = new CCImage;
					img->initWithImageData(resp->data().data(), resp->data().size(), CCImage::kFmtUnKnown, 0, 0, 0);

					auto txt = new CCTexture2D;
					txt->initWithImage(img);

					spr->release();
					spr = CCSprite::createWithTexture(txt);
				}

				this->addChild(spr);
				return spr;
			});

			bindFunction<"Button(Node, str): Node">([this](CCSprite* sprite, std::string callback) -> auto {
				auto pos = sprite->getPosition();
				sprite->removeFromParent();

				auto btn = Build(sprite).intoMenuItem([=, this]() {
					this->runFunction<void>(callback);
				}).pos(pos).parent(m_menu).collect();

				return btn;
			});

			bindFunction<"ButtonSprite(str, str): Node">([this](std::string name, std::string sprite) -> auto {
				auto lbl = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
				lbl->setScale(0.7);
				auto spr = extension::CCScale9Sprite::create(sprite.c_str(), CCRectZero);
				spr->setContentSize(lbl->getContentSize());
				spr->addChild(lbl);
				lbl->setPosition(spr->getContentSize() / 2);

				return spr;
			});

			bindFunction<"Scale9Sprite(float, float, str): Node">([this](float w, float h, std::string name) -> auto {
				auto spr = extension::CCScale9Sprite::create(name.c_str(), CCRectZero);
				if (spr == nullptr)
					spr = extension::CCScale9Sprite::create("GJ_square01.png", CCRectZero);
				spr->setContentSize({w, h});
				this->addChild(spr);

				return spr;
			});

			bindFunction<"AxisLayout(): Layout">([this]() -> auto {
				return AxisLayout::create();
			});
			bindFunction<"RowLayout(): Layout">([this]() -> auto {
				return RowLayout::create();
			});
			bindFunction<"ColumnLayout(): Layout">([this]() -> auto {
				return ColumnLayout::create();
			});
			bindFunction<"AnchorLayout(): Layout">([this]() -> auto {
				return AnchorLayout::create();
			});
			bindFunction<"AxisLayoutOptions: LayoutOptions">([this]() -> auto {
				return AxisLayoutOptions::create();
			});
			bindFunction<"AnchorLayoutOptions: LayoutOptions">([this]() -> auto {
				return AnchorLayoutOptions::create();
			});
		}

		inline void bindNodeFns() {
			bindFunction<"tag(Node, int): Node">([this](CCNode* node, int tag) -> CCNode* {
				node->setTag(tag);
				return node;
			});
			bindFunction<"fetch(Node, int): Node">([this](CCNode* node, int tag) -> CCNode* {
				return node->getChildByTag(tag);
			});

			bindFunction<"id(Node, str): Node">([this](CCNode* node, std::string id) -> CCNode* {
				node->setID(id);
				return node;
			});

			bindFunction<"visible(Node, bool): Node">([this](CCNode* node, bool vis) -> CCNode* {
				node->setVisible(vis);
				return node;
			});

			bindFunction<"pos(Node, float, float): Node">([this](CCNode* node, float x, float y) -> CCNode* {
				node->setPosition({x, y});
				return node;
			});
			bindFunction<"xpos(Node, float): Node">([this](CCNode* node, float x) -> CCNode* {
				node->setPositionX(x);
				return node;
			});	
			bindFunction<"ypos(Node, float): Node">([this](CCNode* node, float y) -> CCNode* {
				node->setPositionY(y);
				return node;
			});
			bindFunction<"getX(Node): float">([this](CCNode* node) -> float {
				return node->getPositionX();
			});
			bindFunction<"getY(Node): float">([this](CCNode* node) -> float {
				return node->getPositionY();
			});
			bindFunction<"move(Node, float, float): Node">([this](CCNode* node, float x, float y) -> CCNode* {
				node->setPosition(node->getPosition() + ccp(x, y));
				return node;
			});

			bindFunction<"size(Node, float, float): Node">([this](CCNode* node, float w, float h) -> CCNode* {
				node->setContentSize({w, h});
				return node;
			});
			bindFunction<"width(Node, float): Node">([this](CCNode* node, float w) -> CCNode* {
				node->setContentSize({w, node->getContentSize().height});
				return node;
			});
			bindFunction<"height(Node, float): Node">([this](CCNode* node, float h) -> CCNode* {
				node->setContentSize({node->getContentSize().width, h});
				return node;
			});
			bindFunction<"getWidth(Node): float">([this](CCNode* node) -> float {
				return node->getContentSize().width;
			});
			bindFunction<"getHeight(Node): float">([this](CCNode* node) -> float {
				return node->getContentSize().height;
			});

			bindFunction<"anchorPoint(Node, float, float): Node">([this](CCNode* node, float x, float y) -> CCNode* {
				node->setAnchorPoint({x, y});
				return node;
			});

			bindFunction<"scale(Node, float): Node">([this](CCNode* node, float scale) -> CCNode* {
				node->setScale(scale);
				return node;
			});
			bindFunction<"scaleX(Node, float): Node">([this](CCNode* node, float scale) -> CCNode* {
				node->setScaleX(scale);
				return node;
			});
			bindFunction<"scaleY(Node, float): Node">([this](CCNode* node, float scale) -> CCNode* {
				node->setScaleY(scale);
				return node;
			});
			bindFunction<"scaleBy(Node, float): Node">([this](CCNode* node, float scale) -> CCNode* {
				node->setScale(node->getScale() * scale);
				return node;
			});

			bindFunction<"rotate(Node, float): Node">([this](CCNode* node, float angle) -> CCNode* {
				node->setRotation(angle);
				return node;
			});
			bindFunction<"rotateBy(Node, float): Node">([this](CCNode* node, float angle) -> CCNode* {
				node->setRotation(node->getRotation() + angle);
				return node;
			});

			bindFunction<"z(Node, int): Node">([this](CCNode* node, int z) -> CCNode* {
				node->setZOrder(z);
				return node;
			});
			bindFunction<"getZ(Node): int">([this](CCNode* node) -> int {
				return node->getZOrder();
			});

			bindFunction<"child(Node, Node): Node">([this](CCNode* parent, CCNode* child) -> CCNode* {
				child->removeFromParent();
				parent->addChild(child);
				return parent;
			});

			bindFunction<"runAction(Node, Action): Node">([this](CCNode* node, CCAction* action) -> CCNode* {
				node->runAction(action);
				return node;
			});

			bindFunction<"color(Node, int, int, int): Node">([this](CCNode* node, uint8_t r, uint8_t g, uint8_t b) -> CCNode* {
				if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(node))
					rgba->setColor(ccc3(r, g, b));
				else
					log::error("Node does not support color");

				return node;
			});
			bindFunction<"opacity(Node, int): Node">([this](CCNode* node, uint8_t op) -> CCNode* {
				if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(node))
					rgba->setOpacity(op);
				else
					log::error("Node does not support opacity");

				return node;
			});
			bindFunction<"text(Node, str): Node">([this](CCNode* node, std::string text) -> CCNode* {
				if (auto lab = typeinfo_cast<CCLabelBMFont*>(node))
					lab->setString(text.c_str());
				else
					log::error("Node does not support text");

				return node;
			});
			bindFunction<"font(Node, str): Node">([this](CCNode* node, std::string font) -> CCNode* {
				auto fu = CCFileUtils::sharedFileUtils();

				if (!fu->isFileExist(fu->fullPathForFilename(font.c_str(), false))) {
					log::error("Invalid font file {}", font);
					font = "bigFont.fnt";
				}

				if (auto lab = typeinfo_cast<CCLabelBMFont*>(node))
					lab->setFntFile(font.c_str());
				else
					log::error("Node does not support font");

				return node;
			});

			bindFunction<"updateLayout(Node): Node">([this](CCNode* node) -> CCNode* {
				node->updateLayout();
				return node;
			});
			bindFunction<"layout(Node, Layout): Node">([this](CCNode* node, Layout* layout) -> CCNode* {
				node->setLayout(layout);
				return node;
			});
		}

		inline void bindLayoutFns() {
			bindFunction<"axis(Layout, int): Layout">([this](Layout* layout, int axis) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setAxis(static_cast<Axis>(axis));
				else
					log::error("Layout does not support axis");
				return layout;
			});
			bindFunction<"align(Layout, int): Layout">([this](Layout* layout, int align) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setAxisAlignment(static_cast<AxisAlignment>(align));
				else
					log::error("Layout does not support alignment");
				return layout;
			});
			bindFunction<"crossAlign(Layout, int): Layout">([this](Layout* layout, int align) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setCrossAxisAlignment(static_cast<AxisAlignment>(align));
				else
					log::error("Layout does not support cross alignment");
				return layout;
			});
			bindFunction<"lineAlign(Layout, int): Layout">([this](Layout* layout, int align) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setCrossAxisLineAlignment(static_cast<AxisAlignment>(align));
				else
					log::error("Layout does not support line alignment");
				return layout;
			});
			bindFunction<"gap(Layout, float): Layout">([this](Layout* layout, float gap) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setGap(gap);
				else
					log::error("Layout does not support gap");
				return layout;
			});
			bindFunction<"reverse(Layout, bool): Layout">([this](Layout* layout, bool rev) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setAxisReverse(rev);
				else
					log::error("Layout does not support reverse");
				return layout;
			});
			bindFunction<"crossReverse(Layout, bool): Layout">([this](Layout* layout, bool rev) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setCrossAxisReverse(rev);
				else
					log::error("Layout does not support cross reverse");
				return layout;
			});
			bindFunction<"autoScale(Layout, bool): Layout">([this](Layout* layout, bool autoscale) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setAutoScale(autoscale);
				else
					log::error("Layout does not support auto scale");
				return layout;
			});
			bindFunction<"crossGrow(Layout, bool): Layout">([this](Layout* layout, bool crossgrow) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setGrowCrossAxis(crossgrow);
				else
					log::error("Layout does not support cross grow");
				return layout;
			});
			bindFunction<"crossOverflow(Layout, bool): Layout">([this](Layout* layout, bool overflow) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setCrossAxisOverflow(overflow);
				else
					log::error("Layout does not support cross overflow");
				return layout;
			});
			bindFunction<"autoGrow(Layout, bool, float): Layout">([this](Layout* layout, bool grows, float min) -> Layout* {
				if (auto axisLayout = typeinfo_cast<AxisLayout*>(layout))
					axisLayout->setAutoGrowAxis(grows ? std::optional<float>(min) : std::nullopt);
				else
					log::error("Layout does not support auto grow");
				return layout;
			});
			bindFunction<"ignoreInvisible(Layout, bool): Layout">([this](Layout* layout, bool ignore) -> Layout* {
				layout->ignoreInvisibleChildren(ignore);
				return layout;
			});
			bindFunction<"relativeScale(LayoutOptions, float): LayoutOptions">([this](LayoutOptions* options, float scale) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setRelativeScale(scale);
				else
					log::error("LayoutOptions does not support relative scale");
				return options;
			});
			bindFunction<"autoScaleOpt(LayoutOptions, bool, bool): LayoutOptions">([this](LayoutOptions* options, bool override, bool autoscale) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setAutoScale(override ? std::optional<bool>(autoscale) : std::nullopt);
				else
					log::error("LayoutOptions does not support auto scale");
				return options;
			});
			bindFunction<"length(LayoutOptions, bool, float): LayoutOptions">([this](LayoutOptions* options, bool override, float length) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setLength(override ? std::optional<float>(length) : std::nullopt);
				else
					log::error("LayoutOptions does not support length");
				return options;
			});
			bindFunction<"prevGap(LayoutOptions, bool, float): LayoutOptions">([this](LayoutOptions* options, bool override, float gap) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setPrevGap(override ? std::optional<float>(gap) : std::nullopt);
				else
					log::error("LayoutOptions does not support previous gap");
				return options;
			});
			bindFunction<"nextGap(LayoutOptions, bool, float): LayoutOptions">([this](LayoutOptions* options, bool override, float gap) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setNextGap(override ? std::optional<float>(gap) : std::nullopt);
				else
					log::error("LayoutOptions does not support next gap");
				return options;
			});
			bindFunction<"breakLine(LayoutOptions, bool): LayoutOptions">([this](LayoutOptions* options, bool brk) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setBreakLine(brk);
				else
					log::error("LayoutOptions does not support break line");
				return options;
			});
			bindFunction<"sameLine(LayoutOptions, bool): LayoutOptions">([this](LayoutOptions* options, bool same) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setSameLine(same);
				else
					log::error("LayoutOptions does not support same line");
				return options;
			});
			bindFunction<"scalePrio(LayoutOptions, int): LayoutOptions">([this](LayoutOptions* options, int prio) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setScalePriority(prio);
				else
					log::error("LayoutOptions does not support scale priority");
				return options;
			});
			bindFunction<"crossAlignOpt(LayoutOptions, bool, int): LayoutOptions">([this](LayoutOptions* options, bool override, int align) -> LayoutOptions* {
				if (auto axisOptions = typeinfo_cast<AxisLayoutOptions*>(options))
					axisOptions->setCrossAxisAlignment(override ? std::optional<AxisAlignment>(static_cast<AxisAlignment>(align)) : std::nullopt);
				else
					log::error("LayoutOptions does not support cross alignment");
				return options;
			});
			bindFunction<"anchor(LayoutOptions, int): LayoutOptions">([this](LayoutOptions* options, int anchor) -> LayoutOptions* {
				if (auto anchorOptions = typeinfo_cast<AnchorLayoutOptions*>(options))
					anchorOptions->setAnchor(static_cast<Anchor>(anchor));
				else
					log::error("LayoutOptions does not support anchor");
				return options;
			});
			bindFunction<"offset(LayoutOptions, float, float): LayoutOptions">([this](LayoutOptions* options, float x, float y) -> LayoutOptions* {
				if (auto anchorOptions = typeinfo_cast<AnchorLayoutOptions*>(options))
					anchorOptions->setOffset({x, y});
				else
					log::error("LayoutOptions does not support offset");
				return options;
			});
			bindFunction<"setAnchorOpts(Node, int, float, float): Node">([this](CCNode* node, int anchor, float x, float y) -> CCNode* {
				auto options = AnchorLayoutOptions::create()
					->setAnchor(static_cast<Anchor>(anchor))
					->setOffset({x, y});
				node->setLayoutOptions(options);
				return node;
			});
		}

		inline void bindActionConstructors() {
			bindFunction<"Sequence(Action, Action): Action">([this](CCFiniteTimeAction* action1, CCFiniteTimeAction* action2) -> CCAction* {
				return CCSequence::create(action1, action2);
			});

			bindFunction<"Delay(float): Action">([this](float time) -> CCAction* {
				return CCDelayTime::create(time);
			});

			bindFunction<"MoveTo(float, float, float): Action">([this](float time, float x, float y) -> CCAction* {
				return CCMoveTo::create(time, {x, y});
			});
			bindFunction<"MoveBy(float, float, float): Action">([this](float time, float x, float y) -> CCAction* {
				return CCMoveBy::create(time, {x, y});
			});

			bindFunction<"ScaleTo(float, float): Action">([this](float time, float scale) -> CCAction* {
				return CCScaleTo::create(time, scale);
			});
			bindFunction<"ScaleBy(float, float): Action">([this](float time, float scale) -> CCAction* {
				return CCScaleBy::create(time, scale);
			});

			bindFunction<"RotateTo(float, float): Action">([this](float time, float angle) -> CCAction* {
				return CCRotateTo::create(time, angle);
			});
			bindFunction<"RotateBy(float, float): Action">([this](float time, float angle) -> CCAction* {
				return CCRotateBy::create(time, angle);
			});

			bindFunction<"FadeTo(float, float): Action">([this](float time, float opacity) -> CCAction* {
				return CCFadeTo::create(time, opacity);
			});
			bindFunction<"FadeBy(float, float): Action">([this](float time, float opacity) -> CCAction* {
				return CCFadeTo::create(time, opacity);
			});
		}

		inline void bindSpecial() {
			bindFunction<"popup(str, str, str): void">([this](std::string title, std::string message, std::string button) -> void {
				FLAlertLayer::create(title.c_str(), message, button.c_str())->show();
			});

			bindFunction<"prompt(str, str, str): void">([this](std::string title, std::string placeholder, std::string callback) -> void {
				auto alert = new FLAlertLayer();
				alert->init(150);
				alert->autorelease();
				alert->m_buttonMenu = CCMenu::create();
				alert->m_buttonMenu->setZOrder(99999);
				alert->m_mainLayer->addChild(alert->m_buttonMenu);

				Build(alert->m_mainLayer)
					.intoNewChild(CCScale9Sprite::create("GJ_square01.png", CCRectZero))
						.contentSize(300, 150)
						.center()
					.intoNewSibling(CCLabelBMFont::create(title.c_str(), "bigFont.fnt"))
						.scale(0.8)
						.center().move(0, 45)
					.intoNewSibling(CCScale9Sprite::create("square02b_small.png", CCRectZero))
						.contentSize(220, 30)
						.color(0, 0, 0)
						.opacity(100)
						.center()
					.intoNewSibling(CCTextInputNode::create(400, 50, placeholder.c_str(), "Thonburi", 18, "bigFont.fnt"))
						.anchorPoint({0, 0})
						.scale(0.5)
						.placeholderColor(ccc3(200, 200, 200))
						.tag(1)
						.center();

				Build<ButtonSprite>::create("Ok")
					.intoMenuItem([=, this]() {
						this->runFunction<void>(callback, static_cast<CCTextInputNode*>(alert->m_mainLayer->getChildByTag(1))->getString());
						alert->removeFromParentAndCleanup(true);
					})
					.pos(0, -45)
					.parent(alert->m_buttonMenu);

				alert->show();
			});

			bindFunction<"terminate(): void">([this](CCNode* node) -> void {
				this->removeFromParentAndCleanup(true);
			});

			bindNode("self", this);
			bindNode("self_menu", m_menu);
		}

		inline bool init(std::string const& code) {
			ScriptBaseNode::init();
			m_menu = CCMenu::create();
			m_menu->setPosition({0, 0});
			this->addChild(m_menu);
			m_nodeBinds = CCArray::create();
			m_nodeBinds->retain();

			registerType("Node");
			registerType("Layout");
			registerType("LayoutOptions");
			registerType("Action");
			bindConstructors();
			bindActionConstructors();
			bindNodeFns();
			bindLayoutFns();
			bindSpecial();

			if (auto err = compile(code).err()) {
				log::error("Error: {}", err.value());
				return false;
			} else {
				if (hasFunction("main"))
					runFunction<void>("main");
				return true;
			}
		}
	};

	void createDebugNode(CCNode* parent, char const* parentfile, char const* filename) {
		auto file = std::filesystem::path(parentfile).parent_path() / filename;

		Build(parent).schedule([parent, file](float dt) {
			if (!std::filesystem::exists(file))
				return;

			auto last_write = std::filesystem::last_write_time(file);
			static decltype(last_write) last_check;

			if (last_check != last_write) {
				last_check = last_write;

				std::string code = utils::file::readString(file).unwrap();

				if (auto node = uiscripter::ScriptNode::create(code)) {
					parent->removeChildByID("script-node");
					node->setID("script-node");
					node->setZOrder(99999);
					Build(node).parent(parent).center();
				}
			}
		});
	}
}

using uiscripter::ScriptNode;
