#include "HelloWorldScene.h"
#include <time.h>
#include <string>
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

int HelloWorld::maxScore = 0;			// ��ߵ÷�
int HelloWorld::Score = 0;				// ��ǰ�÷�

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
	/* ��ʼ��ÿ�ֲ㹲�е����ݳ�Ա */
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
	// ���ű�������
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
	// ���ű�������
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music\\gameBGM.mp3");
	
	//��ʼ��
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
	//���� ���� �л� ���� ��ͣ �˵�
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

	/* ����ӵ� */
	schedule(
		schedule_selector(HelloWorld::heroShoot),
		0.1f,
		CC_REPEAT_FOREVER,	// -1
		0);

	/* ��ӽ��� */
	schedule(
		schedule_selector(HelloWorld::createProp),
		4.0f,
		CC_REPEAT_FOREVER,	// -1
		0);

	/* ��ӵл� */
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


	// ����Button
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

	// ��ʷ���
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

	// ��ǰ�÷� 
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
	// ���Ű�ť��Ч
	SimpleAudioEngine::getInstance()->playEffect("music\\button.mp3");
	// ���ݵ�ǰ����������Ӧ
	switch (myType)
	{
	case LayerType::Load:
		// 1 �����µĲ�
		// 2 �µĲ���ӵ�ԭ����   ԭ����:�ǵ�ǰ��ĸ��ڵ�
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

	// ԭ���Ĳ���Ҫ���Ƴ�
	// 1 �Ҹ��ڵ� �Ƴ��Լ�(����)
	// this->getParent()->removeChild(this);

	// 2 ֱ�ӽ��Լ��Ӹ��ڵ��Ƴ�(��ɱ)
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
	���㴥��:
	һ �����¼�������
	�� ���ü���������
	�Ƿ����ɴ���
	��ʼ �ƶ� ���� ȡ��
	�� ���¼���������ӵ��¼��ַ���
	�ӵ��ݻ�ȡ
	���
	*/

	// һ ����
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	// �� ����
	// 1 ���ɴ���
	listener->setSwallowTouches(true);
	/*
	����ʽ: & ���ò���ȫ����ֵ
	*/
	// 2 ������ʼ
	listener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		// д������ʼʱ��Ҫִ�еĲ���:�ж��Ƿ���зɻ�
		if (hero->getBoundingBox().containsPoint(touch->getLocation()))
		{
			isTouch = true;
			hero->runAction(Blink::create(0.5f, 8));
		}
		return true;		// ע�� ������ʼ��Ҫ�з���ֵ
	};
	// 3 �����ƶ�
	listener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		if (isTouch)
		{
			// �÷ɻ����Ŵ������ƶ�
			// 1 ��ȡ������
			Vec2 touchPos = touch->getLocation();
			heroOldPosX = hero->getPositionX();
			// 2 ��ɻ���λ��
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
			if (heroOldPosX > hero->getPositionX())	// ����
			{
				hero->setTexture(
					director->getTextureCache()->addImage(
					"Images//player//player1_left.png")
					);
			}
			if (heroOldPosX < hero->getPositionX())	// ����
			{
				hero->setTexture(
					director->getTextureCache()->addImage(
					"Images//player//player1_right.png")
					);
			}
		}
	};
	// 4 ��������
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
	// �� ���¼���������ӵ��¼��ַ���
	// 1 �ӵ��ݻ�ȡ
	EventDispatcher* m_eventDispatcher = director->getEventDispatcher();
	// 2 ���
	m_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::heroShoot(float date)
{
	// ���������Ч
	SimpleAudioEngine::getInstance()->playEffect("music\\heroShoot.mp3");
	Sprite* bullet = nullptr;
	if (herobulletlist_death.size() > 0)
	{
		//ʹ�����������һ���ӵ�
		bullet = herobulletlist_death.back();
		//���Ѿ�������ӵ����������Ƴ�
		this->herobulletlist_death.pop_back();
	}
	else
	{
		bullet = Sprite::create("Images\\Bullet\\bullet.png");

	}
	bullet->setPosition(hero->getPosition() + Vec2(0, hero->getContentSize().height / 2));
	this->addChild(bullet);
	// ��ǰ�ӵ���ӵ������
	this->herobulletlist_lives.push_back(bullet);
	// ��ǰ�ӵ����ü���+1
	bullet->retain();
	
}

void HelloWorld::heroShoot1(float date)
{

	// ���������Ч
	SimpleAudioEngine::getInstance()->playEffect("music\\heroShoot1.mp3");
	for (int i = -3; i < 4; i++)
	{
		Sprite* bullet = nullptr;
		if (herobulletlist_death.size()>0)	// �����ز�Ϊ��
		{
			// ʹ�������غ����һ���ӵ�
			bullet = herobulletlist_death.back();
			// ���Ѿ�������ӵ����������Ƴ�
			this->herobulletlist_death.pop_back();
		}
		else
		{
			// ������û���ܹ�������ӵ��ʹ���
			bullet = Sprite::create("Images\\Bullet\\bullet.png");
		}
		bullet->setPosition(hero->getPosition() + Vec2(0, hero->getContentSize().height / 2));
		this->addChild(bullet);
		// ��ǰ�ӵ���ӵ������
		this->herobulletlist_lives.push_back(bullet);
		// ��ǰ�ӵ����ü���+1
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
	// ������Ʒ�� �ƶ�������Ʒ
	for (Sprite* p : HelloWorld::proplist)
	{
		p->setPositionY(p->getPositionY() - 8);
		if (hero->getBoundingBox().intersectsRect(p->getBoundingBox()))
		{
			// 1 ������Ʒ��ʧ
			p->removeFromParent();		// ͼ��
			this->proplist.remove(p);	// ����
			// 2 ��������Ч��
			schedule(
				schedule_selector(HelloWorld::heroShoot1),
				0.5f,
				4,
				0.5);
			break;
		}
		if (p->getPositionY() <= 100)
		{
			// ������Ʒ��ʧ
			p->removeFromParent();
			this->proplist.remove(p);
			break;
		}
	}

	
	
	// ���л����ӵ�
	// ����ֻ��һ�� ���ǵл����ӵ����кܶ� ==> Ƕ��:������
	for (Sprite* e : HelloWorld::enemylist_lives)		// �����л�
	{
		// �ƶ��л�(�������������ٶ�)
		// 1 �������־����ٶ�
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
		// 2 �ƶ�
		e->setPositionY(e->getPositionY() - enemySpeed * date);

		for (Sprite* b : HelloWorld::herobulletlist_lives)	// �����ӵ�
		{
			// �����ײ
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




				// һ �����ӵ�:
				// 1 ��������Ƴ�
				this->herobulletlist_lives.remove(b);
				// 2 ��ӵ�������
				this->herobulletlist_death.push_back(b);
				// 3 ��ͼ�����Ƴ�����
				b->removeFromParent();
				b->setRotation(0);

				 e->runAction(createMyAnimate("Images\\Baozha\\bz%d.png", 14, 0.1f, 1));
				// �� ����л�
				this->enemylist_lives.remove(e);
				this->enemylist_death.push_back(e);
				e->removeFromParent();

				// break;	// ֻ������һ��ѭ��
				return;
			}
			// �涨�ӵ����շ�Χ
			if (b->getPositionY() >= 700)
			{
				// 1 ��������Ƴ�
				this->herobulletlist_lives.remove(b);
				// 2 ��ӵ�������
				this->herobulletlist_death.push_back(b);
				// 3 ��ͼ�����Ƴ�����
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
		// �涨�л����շ�Χ
		// �ƶ��ӵ�
			for (Sprite* b : HelloWorld::herobulletlist_lives)	// �����ӵ�
			{
				// �ƶ��ӵ�
				float x = b->getRotation();		// ��ȡ֮ǰ���õ���ת�Ƕ�
				b->setPositionX(b->getPositionX() + x / 10);
				b->setPositionY(b->getPositionY() + 8);
			}
		}
	// ��������͵л�
	for (Sprite* e : HelloWorld::enemylist_lives)
	{
		if (e->getBoundingBox().intersectsRect(hero->getBoundingBox()))
		{
			// ��Ϸ���� �� ��Ӧ���� 

			// ����������ը��Ч
			SimpleAudioEngine::getInstance()->playEffect("music\\heroBoom.mp3");

			//������������			
			Animate* anm = createMyAnimate("Images\\Baozha\\bz%d.png", 14, 0.1f, 1);
			// ��ô������ըЧ��
			CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(HelloWorld::menuCloseCallback, this));
			Sequence* seq = Sequence::create(anm, callfunc, NULL);
			// ִ�ж���
			hero->runAction(seq);

			break;
		}
	}

}

void HelloWorld::createEnemy(float date)
{
	//// 1 �����л�
	//Sprite* enemy = Sprite::create("Images//Enemy//enemy1.png");
	//// 2 ����λ��
	//enemy->setPosition(240, 600);
	//// 3 ��ӵ���
	//this->addChild(enemy);
	//// 4 ��ӵ������
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

	// 1 ����
	// 1.1 ׼��һ���л�ָ��(Sprite*)
	Sprite* enemy = nullptr;
	// 1.2 �������ѡ����ջ����½�
	if (this->enemylist_death.size() > 0)
	{
		//// ����
		enemy = enemylist_death.back();
		//this->enemylist_death.pop_back();

		// 1 ��ȡ����ǰ������
		std::string oldType = enemy->getName();

		// 2 �Ա�,�ǲ��ǵ�ǰ��Ҫ������
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
		// 2.1 ��:����
		if (oldType == newType)
		{
			enemy = enemylist_death.back();
			this->enemylist_death.pop_back();
		}
		// 2.2 ��:�޸�/���´���
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
		// ���´���
		// enemy = Sprite::create("Images//Enemy//enemy1.png");

		// �������ʹ����л�
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

	// 2 ����λ��
	float enemyWidth = enemy->getContentSize().width;
	float enemyHeight = enemy->getContentSize().height;
	enemy->setPosition(
		Vec2(
		rand() % (int)(visibleSize.width - enemyWidth) + enemyWidth / 2,
		visibleSize.height + enemyHeight / 2
		));
	// 3 ��ӵ���
	this->addChild(enemy);
	// 4 ��ӵ������
	this->enemylist_lives.push_back(enemy);
	// 5 ���ü���+1
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
	case cocos2d::ui::Widget::TouchEventType::BEGAN:	// �㵽
		CCLOG("BEGAN");
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:	// �ƶ�
		CCLOG("MOVED");
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:	// ̧��
		CCLOG("ENDED");
		button->removeFromParent();
		Director::getInstance()->stopAnimation();

		setbg = Sprite::create("set\\setbg.png");
		setbg->setPosition(visibleSize / 2);
		this->addChild(setbg);



		myItem = MenuItemImage::create(
			"set\\bgmset.png",	// û�а���ʱ��״̬
			"set\\bgmset.png",	// ��ť����ʱ��״̬
			CC_CALLBACK_1(HelloWorld::setMusic, this)	// �ص�����
			);
		myItem->setPosition(Vec2(30, 150));

		// �� Menu
		myMenu = Menu::create(myItem, NULL);
		myMenu->setPosition(Vec2(0, 0));
		setbg->addChild(myMenu, 1);


		closeSetImage = MenuItemImage::create(
			"set\\close.png",	// û�а���ʱ��״̬
			"set\\close.png",	// ��ť����ʱ��״̬
			CC_CALLBACK_1(HelloWorld::setset, this)	// �ص�����
			);
		closeSetImage->setPosition(setbg->getContentSize());

		// �� Menu
		closeSetMenu = Menu::create(closeSetImage, NULL);
		closeSetMenu->setPosition(Vec2(0, 0));
		setbg->addChild(closeSetMenu, 1);



		bgmtype = Sprite::create("set\\bgmplay.png");
		bgmtype->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		bgmtype->setPosition(160, 150);
		setbg->addChild(bgmtype);

		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:	// ȡ��
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
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();	// ��ͣ
		bgmtype->setTexture(
			Director::getInstance()->getTextureCache()->addImage("set\\bgmstop.png"));
		isBgm = false;
	}
	else
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();	// �ָ�
		bgmtype->setTexture(
			Director::getInstance()->getTextureCache()->addImage("set\\bgmplay.png"));
		isBgm = true;
	}

}


void HelloWorld::setset(cocos2d::Ref* p)
{
	Director::getInstance()->startAnimation();

	setbg->removeFromParent();

	// ����Button
	button = Button::create("set\\set1.png", "set\\set2.png");
	button->setPosition(Vec2(200, 200));
	button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::myButtonCall_2, this));
	button->setPressedActionEnabled(true);
	this->addChild(button);

}
