#include "HelloWorldScene.h"
#include <time.h>
#include <string>
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

int HelloWorld::maxScore = 0;			// 最高得分
int HelloWorld::Score = 0;				// 当前得分

HelloWorld::HelloWorld(LayerType type)
:myType(type)
{

}
Scene* HelloWorld::createScene(LayerType type)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create(); //this scene is used forever
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create(type);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
HelloWorld* HelloWorld::create(LayerType type)
{
	HelloWorld *pRet = new(std::nothrow)HelloWorld(type);
	if (pRet&& pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	/* 初始化每种层共有的数据成员 */
	director = Director::getInstance();
	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();

	switch (myType)
	{
	case LayerType::Load:
		initLoad();
		break;
	case LayerType::Game:
		initGame();
		break;
	case LayerType::End:
		initEnd();
		break;
	default:
		break;
	}
    

    
    return true;
}

void HelloWorld::initLoad()
{
	// 播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music\\loadBGM.mp3");

	Sprite* bg = Sprite::create("Images\\Loading\\bg_menu.jpg");
	bg->setPosition(visibleSize / 2);
	this->addChild(bg, 0);

	Sprite* logo = Sprite::create("Images\\Loading\\logo.png");
	logo->setPosition(visibleSize.width / 2, 650);
	this->addChild(logo, 0);

	Sprite* people = Sprite::create("Images\\Loading\\messageGirl.png");
	people->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	people->setPosition(0, 0);
	people->setScale(0.5f);
	this->addChild(people, 0);

	Sprite* itemBg = Sprite::create("Images\\Loading\\smsBtn0.png");
	itemBg->setPosition(visibleSize.width / 2, 100);
	this->addChild(itemBg, 1);

	MenuItemImage* itemImage = MenuItemImage::create(
		"Images\\Loading\\start_menu.png",
		"Images\\Loading\\start_menu.png",
		CC_CALLBACK_0(HelloWorld::menuCloseCallback, this)
		);
	itemImage->setPosition(visibleSize.width / 2, 100);
	auto menu = Menu::create(itemImage, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);


}
void HelloWorld::initGame()
{
	// 播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music\\gameBGM.mp3");
	
	//初始化
	Score = 0;
	isBgm = true;

	scoreBg = Sprite::create("button_p.png");
	scoreBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	scoreBg->setPosition(visibleSize.width - 10, visibleSize.height - scoreBg->getContentSize().height);
	this->addChild(scoreBg, 1);

	Sprite* score = Sprite::create("zhandou_score.png");
	score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	score->setPosition(10, scoreBg->getContentSize().height / 2);
	scoreBg->addChild(score);

	scoreLbinGame = Label::createWithCharMap("tiaozhanfuhuo_num.png", 14, 22, '0');
	scoreLbinGame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	scoreLbinGame->setString("0");
	scoreLbinGame->setPosition(70, scoreBg->getContentSize().height / 2);
	scoreBg->addChild(scoreLbinGame, 2);

	ScoreNumLable = Label::create("000000", "fonts/Marker Felt.ttf", 48);
	ScoreNumLable->setPosition(visibleSize / 2);
	this->addChild(ScoreNumLable, 2);

	

	MenuItemImage* itemImage = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_0(HelloWorld::menuCloseCallback, this)
		);
	itemImage->setPosition(240, 200);
	auto menu = Menu::create(itemImage, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	//背景 主机 敌机 分数 暂停 菜单
	bg_1 = Sprite::create("Images//Bg//Game_bg.jpg");
	bg_1->setPosition(visibleSize / 2);
	this->addChild(bg_1, 0);
	bg_2 = Sprite::create("Images//Bg//Game_bg.jpg");
	bg_2->setPosition(visibleSize.width / 2, visibleSize.height * 1.5f);
	this->addChild(bg_2, 0);
	//add background 

	bgSpeed = 1000;

	// add background whose speed

	hero = Sprite::create("Images//player//player1_0.png");
	hero->setPosition(visibleSize / 2);
	this->addChild(hero, 1);
	isTouch = false;
	//add hero whose image and action 
	hero->runAction(createMyAnimate("Images//player//player1_%d.png", 8, 0.15f, 1));

	touchOneByOne();

	/* 射击子弹 */
	schedule(
		schedule_selector(HelloWorld::heroShoot),
		0.1f,
		CC_REPEAT_FOREVER,	// -1
		0);

	/* 添加奖励 */
	schedule(
		schedule_selector(HelloWorld::createProp),
		4.0f,
		CC_REPEAT_FOREVER,	// -1
		0);

	/* 添加敌机 */
	schedule(
		schedule_selector(HelloWorld::createEnemy),
		0.5f,
		CC_REPEAT_FOREVER,	// -1
		0);
	//schedule(
	//	schedule_selector(HelloWorld::createEnemyS1),
	//	1.0f,
	//	CC_REPEAT_FOREVER,	// -1
	//	0);

	this->scheduleUpdate();


	// 创建Button
	button = Button::create("set\\set1.png", "set\\set2.png");
	button->setPosition(Vec2(200, 200));
	button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::myButtonCall_2, this));
	button->setPressedActionEnabled(true);
	this->addChild(button);

}


void HelloWorld::initEnd()
{
	

	Label* lableLoad = Label::create("End Layer", "fonts/Marker Felt.ttf", 48);
	lableLoad->setPosition(visibleSize / 2);
	this->addChild(lableLoad);

	MenuItemImage* itemImage = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_0(HelloWorld::menuCloseCallback, this)
		);
	itemImage->setPosition(340, 200);
	auto menu = Menu::create(itemImage, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	isnew = false;

	if (Score > maxScore)
	{
		maxScore = Score;
		isnew = true;
	}

	// 历史最佳
	Sprite* maxScoreBg = Sprite::create("yellow3_menu.png");
	maxScoreBg->setPosition(240, 300);
	// scoreBg->setScaleX(2.5f);
	// scoreBg->setColor(Color3B::ORANGE);
	this->addChild(maxScoreBg, 0);

	Label* maxScoreLabel = Label::createWithCharMap("shadishu.png", 19, 19, '0');
	maxScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	maxScoreLabel->setScale(1.5f);
	char maxScoreNum[12];
	sprintf_s(maxScoreNum, "%d", maxScore);
	maxScoreLabel->setString(maxScoreNum);
	maxScoreLabel->setPosition(maxScoreBg->getContentSize().width / 2 - 10, maxScoreBg->getContentSize().height / 2);
	maxScoreBg->addChild(maxScoreLabel);

	Sprite* stageClass2 = Sprite::create("stageClass2.png");
	stageClass2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	stageClass2->setPosition(40, maxScoreBg->getContentSize().height / 2);
	stageClass2->setScale(0.5f);
	maxScoreBg->addChild(stageClass2, 0);

	// 当前得分 
	Sprite* thisScoreBg = Sprite::create("button_p.png");
	thisScoreBg->setPosition(240, 500);
	thisScoreBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(thisScoreBg, 0);

	Label* thisScoreLabel = Label::createWithCharMap("tiaozhanfuhuo_num.png", 14, 22, '0');
	thisScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	char thisScoreNum[12];
	sprintf_s(thisScoreNum, "%d", Score);
	thisScoreLabel->setString(thisScoreNum);
	thisScoreLabel->setPosition(70, thisScoreBg->getContentSize().height / 2);
	thisScoreBg->addChild(thisScoreLabel);

	Sprite* score = Sprite::create("zhandou_score.png");
	score->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	score->setPosition(10, thisScoreBg->getContentSize().height / 2);
	thisScoreBg->addChild(score);

	if (isnew)
	{
		Sprite* newtopscore = Sprite::create("newtopscore.png");
		newtopscore->setPosition(Vec2::ZERO);
		thisScoreBg->addChild(newtopscore);
	}
}
void HelloWorld::menuCloseCallback()
{
	// 播放按钮音效
	SimpleAudioEngine::getInstance()->playEffect("music\\button.mp3");
	// 根据当前界面类型响应
	switch (myType)
	{
	case LayerType::Load:
		// 1 创建新的层
		// 2 新的层添加到原场景   原场景:是当前层的父节点
		this->getParent()->addChild(create(LayerType::Game));
		break;
	case LayerType::Game:
		this->getParent()->addChild(create(LayerType::End));
		break;
	case LayerType::End:
		this->getParent()->addChild(create(LayerType::Load));
		break;
	default:
		break;
	}

	// 原来的层需要被移除
	// 1 找父节点 移除自己(自首)
	// this->getParent()->removeChild(this);

	// 2 直接将自己从父节点移除(自杀)
	this->removeFromParent();
}
Animate* HelloWorld::createMyAnimate(const char* fileName, int num, float delay, unsigned int loop)
{
	Animation* m_animation = Animation::create();
	for (int i = 0; i <= num; i++)
	{
		char filename[128];
		sprintf_s(filename, fileName, i);
		m_animation->addSpriteFrameWithFile(filename);
	}
	m_animation->setDelayPerUnit(delay);
	m_animation->setLoops(loop);
	m_animation->setRestoreOriginalFrame(false);
	Animate* m_animate = Animate::create(m_animation);
	return m_animate;
}

void HelloWorld::touchOneByOne()
{
	/*
	单点触摸:
	一 创建事件监听器
	二 设置监听器属性
	是否吞噬触摸
	开始 移动 结束 取消
	三 将事件监听器添加到事件分发器
	从导演获取
	添加
	*/

	// 一 创建
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	// 二 设置
	// 1 吞噬触摸
	listener->setSwallowTouches(true);
	/*
	捕获方式: & 引用捕获全部的值
	*/
	// 2 触摸开始
	listener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		// 写触摸开始时需要执行的操作:判断是否点中飞机
		if (hero->getBoundingBox().containsPoint(touch->getLocation()))
		{
			isTouch = true;
			hero->runAction(Blink::create(0.5f, 8));
		}
		return true;		// 注意 触摸开始需要有返回值
	};
	// 3 触摸移动
	listener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (isTouch)
		{
			// 让飞机跟着触摸点移动
			// 1 获取触摸点
			Vec2 touchPos = touch->getLocation();
			heroOldPosX = hero->getPositionX();
			// 2 设飞机的位置
			hero->setPosition(touchPos);
			if (hero->getPositionX() < 0)
			{
				hero->setPositionX(0);
			}
			if (hero->getPositionY() < 0)
			{
				hero->setPositionY(0);
			}
			if (hero->getPositionX() > visibleSize.width)
			{
				hero->setPositionX(visibleSize.width);
			}
			if (hero->getPositionY() > visibleSize.height)
			{
				hero->setPositionY(visibleSize.height);
			}
			if (heroOldPosX > hero->getPositionX())	// 向左
			{
				hero->setTexture(
					director->getTextureCache()->addImage(
					"Images//player//player1_left.png")
					);
			}
			if (heroOldPosX < hero->getPositionX())	// 向右
			{
				hero->setTexture(
					director->getTextureCache()->addImage(
					"Images//player//player1_right.png")
					);
			}
		}
	};
	// 4 触摸结束
	listener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		if (isTouch)
		{
			hero->setTexture(
				director->getTextureCache()->addImage(
				"Images//player//player1_0.png")
				);
			hero->runAction(
				createMyAnimate(
				"Images//player//player1_%d.png", 8, 0.08f, 1));

		}
		isTouch = false;
	};
	// 三 将事件监听器添加到事件分发器
	// 1 从导演获取
	EventDispatcher* m_eventDispatcher = director->getEventDispatcher();
	// 2 添加
	m_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::heroShoot(float date)
{
	// 播放射击音效
	SimpleAudioEngine::getInstance()->playEffect("music\\heroShoot.mp3");
	Sprite* bullet = nullptr;
	if (herobulletlist_death.size() > 0)
	{
		//使用死亡池最后一个子弹
		bullet = herobulletlist_death.back();
		//将已经复活的子弹从死亡池移除
		this->herobulletlist_death.pop_back();
	}
	else
	{
		bullet = Sprite::create("Images\\Bullet\\bullet.png");

	}
	bullet->setPosition(hero->getPosition() + Vec2(0, hero->getContentSize().height / 2));
	this->addChild(bullet);
	// 当前子弹添加到生存池
	this->herobulletlist_lives.push_back(bullet);
	// 当前子弹引用计数+1
	bullet->retain();
	
}

void HelloWorld::heroShoot1(float date)
{

	// 播放射击音效
	SimpleAudioEngine::getInstance()->playEffect("music\\heroShoot1.mp3");
	for (int i = -3; i < 4; i++)
	{
		Sprite* bullet = nullptr;
		if (herobulletlist_death.size()>0)	// 死亡池不为空
		{
			// 使用死亡池后最后一个子弹
			bullet = herobulletlist_death.back();
			// 将已经复活的子弹从死亡池移除
			this->herobulletlist_death.pop_back();
		}
		else
		{
			// 死亡池没有能够复活的子弹就创建
			bullet = Sprite::create("Images\\Bullet\\bullet.png");
		}
		bullet->setPosition(hero->getPosition() + Vec2(0, hero->getContentSize().height / 2));
		this->addChild(bullet);
		// 当前子弹添加到生存池
		this->herobulletlist_lives.push_back(bullet);
		// 当前子弹引用计数+1
		bullet->retain();
		bullet->setRotation(i * 10);
	}
}

void HelloWorld::createProp(float date)
{
	Sprite* prop = Sprite::create("ui_fish_register_z0.png");
	prop->setPosition(
		Vec2(rand() % (int)visibleSize.width,
		visibleSize.height + prop->getContentSize().height)
		);
	this->addChild(prop, 1);
	this->proplist.push_back(prop);
	prop->runAction(createMyAnimate("ui_fish_register_z%d.png", 6, 0.15f, -1));
}

void HelloWorld::update(float date)
{
	bg_1->setPositionY(bg_1->getPositionY() - bgSpeed*date);
	if (bg_1->getPositionY() <= -visibleSize.height / 2)
	{
		// bg_1->setPositionY(visibleSize.height*1.5f);
		bg_1->setPositionY(bg_1->getPositionY() + visibleSize.height * 2);
	}

	bg_2->setPositionY(bg_2->getPositionY() - bgSpeed*date);
	if (bg_2->getPositionY() <= -visibleSize.height / 2)
	{
		bg_2->setPositionY(bg_2->getPositionY() + visibleSize.height * 2);
	}
	// 遍历物品池 移动奖励物品
	for (Sprite* p : HelloWorld::proplist)
	{
		p->setPositionY(p->getPositionY() - 8);
		if (hero->getBoundingBox().intersectsRect(p->getBoundingBox()))
		{
			// 1 奖励物品消失
			p->removeFromParent();		// 图层
			this->proplist.remove(p);	// 数据
			// 2 触发奖励效果
			schedule(
				schedule_selector(HelloWorld::heroShoot1),
				0.5f,
				4,
				0.5);
			break;
		}
		if (p->getPositionY() <= 100)
		{
			// 奖励物品消失
			p->removeFromParent();
			this->proplist.remove(p);
			break;
		}
	}

	
	
	// 检测敌机与子弹
	// 主机只有一个 但是敌机与子弹都有很多 ==> 嵌套:逐个检测
	for (Sprite* e : HelloWorld::enemylist_lives)		// 遍历敌机
	{
		// 移动敌机(根据类型设置速度)
		// 1 根据名字决定速度
		std::string type = e->getName();
		if (type == "E_Type_Small")
		{
			enemySpeed = 400;
		}
		if (type == "E_Type_Medium")
		{
			enemySpeed = 250;
		}
		if (type == "E_Type_Large")
		{
			enemySpeed = 150;
		}
		// 2 移动
		e->setPositionY(e->getPositionY() - enemySpeed * date);

		for (Sprite* b : HelloWorld::herobulletlist_lives)	// 遍历子弹
		{
			// 检测碰撞
			if (e->getBoundingBox().intersectsRect(b->getBoundingBox()))
			{
				SimpleAudioEngine::getInstance()->playEffect("music\\jinitaimei.mp3"); 
				Score++;
				scoreLbinGame->removeFromParent();
				scoreLbinGame = Label::createWithCharMap("tiaozhanfuhuo_num.png", 14, 22, '0');
				scoreLbinGame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				char scoreNum[12];
				sprintf_s(scoreNum, "%d", Score);
				scoreLbinGame->setString(scoreNum);
				scoreLbinGame->setPosition(70, scoreBg->getContentSize().height / 2);
				scoreBg->addChild(scoreLbinGame, 2);

				ScoreNumLable->removeFromParent();
				char scoreNum1[12];
				sprintf_s(scoreNum1, "Score: %d", Score);
				ScoreNumLable = Label::create(scoreNum1, "fonts/Marker Felt.ttf", 48);
				ScoreNumLable->setPosition(visibleSize / 2);
				this->addChild(ScoreNumLable);




				// 一 处理子弹:
				// 1 从生存池移除
				this->herobulletlist_lives.remove(b);
				// 2 添加到死亡池
				this->herobulletlist_death.push_back(b);
				// 3 从图层上移除精灵
				b->removeFromParent();
				b->setRotation(0);

				 e->runAction(createMyAnimate("Images\\Baozha\\bz%d.png", 14, 0.1f, 1));
				// 二 处理敌机
				this->enemylist_lives.remove(e);
				this->enemylist_death.push_back(e);
				e->removeFromParent();

				// break;	// 只能跳出一层循环
				return;
			}
			// 规定子弹回收范围
			if (b->getPositionY() >= 700)
			{
				// 1 从生存池移除
				this->herobulletlist_lives.remove(b);
				// 2 添加到死亡池
				this->herobulletlist_death.push_back(b);
				// 3 从图层上移除精灵
				b->removeFromParent();
				b->setRotation(0);
				break;
			}
			
		}
		if (e->getPositionY() <= 200)
		{
			this->enemylist_lives.remove(e);
			this->enemylist_death.push_back(e);
			e->removeFromParent();
			break;
		}
		// 规定敌机回收范围
		// 移动子弹
			for (Sprite* b : HelloWorld::herobulletlist_lives)	// 遍历子弹
			{
				// 移动子弹
				float x = b->getRotation();		// 获取之前设置的旋转角度
				b->setPositionX(b->getPositionX() + x / 10);
				b->setPositionY(b->getPositionY() + 8);
			}
		}
	// 检测主机和敌机
	for (Sprite* e : HelloWorld::enemylist_lives)
	{
		if (e->getBoundingBox().intersectsRect(hero->getBoundingBox()))
		{
			// 游戏结束 或 相应操作 

			// 播放主机爆炸音效
			SimpleAudioEngine::getInstance()->playEffect("music\\heroBoom.mp3");

			//主机死亡画面			
			Animate* anm = createMyAnimate("Images\\Baozha\\bz%d.png", 14, 0.1f, 1);
			// 怎么看到爆炸效果
			CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(HelloWorld::menuCloseCallback, this));
			Sequence* seq = Sequence::create(anm, callfunc, NULL);
			// 执行动画
			hero->runAction(seq);

			break;
		}
	}

}

void HelloWorld::createEnemy(float date)
{
	//// 1 创建敌机
	//Sprite* enemy = Sprite::create("Images//Enemy//enemy1.png");
	//// 2 设置位置
	//enemy->setPosition(240, 600);
	//// 3 添加到层
	//this->addChild(enemy);
	//// 4 添加到生存池
	//this->enemylist_lives.push_back(enemy);

	int num = rand() % 100;
	if (num < 50)
	{
		enemyType = E_Type_Small;
	}
	else if (num < 80)
	{
		enemyType = E_Type_Medium;
	}
	else
	{
		enemyType = E_Type_Large;
	}

	// 1 创建
	// 1.1 准备一个敌机指针(Sprite*)
	Sprite* enemy = nullptr;
	// 1.2 根据情况选择回收还是新建
	if (this->enemylist_death.size() > 0)
	{
		//// 复活
		enemy = enemylist_death.back();
		//this->enemylist_death.pop_back();

		// 1 获取复活前的类型
		std::string oldType = enemy->getName();

		// 2 对比,是不是当前需要的类型
		std::string newType;
		switch (enemyType)
		{
		case E_Type_Small:
			newType = "E_Type_Small";
			break;
		case E_Type_Medium:
			newType = "E_Type_Medium";
			break;
		case E_Type_Large:
			newType = "E_Type_Large";
			break;
		default:
			break;
		}
		// 2.1 是:复活
		if (oldType == newType)
		{
			enemy = enemylist_death.back();
			this->enemylist_death.pop_back();
		}
		// 2.2 否:修改/重新创建
		else
		{
			switch (enemyType)
			{
			case E_Type_Small:
				enemy = Sprite::create("Images//Enemy//enemy1.png");
				enemy->setName("E_Type_Small");
				break;
			case E_Type_Medium:
				enemy = Sprite::create("Images//Enemy//enemy2.png");
				enemy->setName("E_Type_Medium");
				break;
			case E_Type_Large:
				enemy = Sprite::create("Images//Enemy//enemy3.png");
				enemy->setName("E_Type_Large");
				break;
			default:
				break;
			}
		}

	}
	else
	{
		// 重新创建
		// enemy = Sprite::create("Images//Enemy//enemy1.png");

		// 根据类型创建敌机
		switch (enemyType)
		{
		case E_Type_Small:
			enemy = Sprite::create("Images//Enemy//enemy1.png");
			enemy->setName("E_Type_Small");
			break;
		case E_Type_Medium:
			enemy = Sprite::create("Images//Enemy//enemy2.png");
			enemy->setName("E_Type_Medium");
			break;
		case E_Type_Large:
			enemy = Sprite::create("Images//Enemy//enemy3.png");
			enemy->setName("E_Type_Large");
			break;
		default:
			break;
		}
	}


	MoveBy* moveBy = MoveBy::create(2.0, Vec2(100, 0));

	if (enemyType == E_Type_Large)
	{
		enemy->runAction(moveBy);
	}

	// 2 设置位置
	float enemyWidth = enemy->getContentSize().width;
	float enemyHeight = enemy->getContentSize().height;
	enemy->setPosition(
		Vec2(
		rand() % (int)(visibleSize.width - enemyWidth) + enemyWidth / 2,
		visibleSize.height + enemyHeight / 2
		));
	// 3 添加到层
	this->addChild(enemy);
	// 4 添加到生存池
	this->enemylist_lives.push_back(enemy);
	// 5 引用计数+1
	enemy->retain();
}
void HelloWorld::createEnemyS1(float date)
{
	for (int i = -2; i < 3; i++)
	{
		Sprite* enemy = Sprite::create("Images//Enemy//enemy1.png");
		this->addChild(enemy);
		enemy->setPosition(Vec2(
			visibleSize.width / 2 + 100 * i,
			visibleSize.height + enemy->getContentSize().height / 2
			));
		this->enemylist_lives.push_back(enemy);
		enemy->retain();
	}
}

void HelloWorld::myButtonCall_2(Ref* pSender, Widget::TouchEventType type)
{
	CCLOG("HelloWorld::myButtonCall_2");
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:	// 点到
		CCLOG("BEGAN");
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:	// 移动
		CCLOG("MOVED");
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:	// 抬起
		CCLOG("ENDED");
		button->removeFromParent();
		Director::getInstance()->stopAnimation();

		setbg = Sprite::create("set\\setbg.png");
		setbg->setPosition(visibleSize / 2);
		this->addChild(setbg);



		myItem = MenuItemImage::create(
			"set\\bgmset.png",	// 没有按下时的状态
			"set\\bgmset.png",	// 按钮按下时的状态
			CC_CALLBACK_1(HelloWorld::setMusic, this)	// 回调函数
			);
		myItem->setPosition(Vec2(30, 150));

		// 二 Menu
		myMenu = Menu::create(myItem, NULL);
		myMenu->setPosition(Vec2(0, 0));
		setbg->addChild(myMenu, 1);


		closeSetImage = MenuItemImage::create(
			"set\\close.png",	// 没有按下时的状态
			"set\\close.png",	// 按钮按下时的状态
			CC_CALLBACK_1(HelloWorld::setset, this)	// 回调函数
			);
		closeSetImage->setPosition(setbg->getContentSize());

		// 二 Menu
		closeSetMenu = Menu::create(closeSetImage, NULL);
		closeSetMenu->setPosition(Vec2(0, 0));
		setbg->addChild(closeSetMenu, 1);



		bgmtype = Sprite::create("set\\bgmplay.png");
		bgmtype->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		bgmtype->setPosition(160, 150);
		setbg->addChild(bgmtype);

		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:	// 取消
		CCLOG("CANCELED");
		break;
	default:
		break;
	}
}

void HelloWorld::setMusic(cocos2d::Ref* p)
{
	if (isBgm)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();	// 暂停
		bgmtype->setTexture(
			Director::getInstance()->getTextureCache()->addImage("set\\bgmstop.png"));
		isBgm = false;
	}
	else
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();	// 恢复
		bgmtype->setTexture(
			Director::getInstance()->getTextureCache()->addImage("set\\bgmplay.png"));
		isBgm = true;
	}

}


void HelloWorld::setset(cocos2d::Ref* p)
{
	Director::getInstance()->startAnimation();

	setbg->removeFromParent();

	// 创建Button
	button = Button::create("set\\set1.png", "set\\set2.png");
	button->setPosition(Vec2(200, 200));
	button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::myButtonCall_2, this));
	button->setPressedActionEnabled(true);
	this->addChild(button);

}
