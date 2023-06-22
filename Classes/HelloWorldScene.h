#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

#include "ui\CocosGUI.h"
using namespace cocos2d::ui;

enum LayerType
{
	Load,
	Game,
	End
};
enum EnemyType
{
	E_Type_Small = 10,
	E_Type_Medium = 20,
	E_Type_Large = 30
};



class HelloWorld : public cocos2d::Layer
{
public:
	HelloWorld(LayerType type);
	static HelloWorld* create(LayerType type);
	static Scene* crateScene(LayerType type);
    static cocos2d::Scene* createScene(LayerType type);
private:
    virtual bool init();		
	void initLoad();
	void initGame();
	void initEnd();
	Animate* createMyAnimate(
		const char* fileName,
		int num, 
		float delay,
		unsigned int loop);	// 创建一个帧动画
	void menuCloseCallback();

	void touchOneByOne();
    // a selector callback
     //切换界面相应函数
	void update(float) override;	//默认调度器回调
	void heroShoot(float date);		//主机射击子弹
	void heroShoot1(float date);		//主机设计子弹
	void createProp(float date);	//创建奖励物品
	void createEnemy(float date);
	void createEnemyS1(float date);
public:

private:
	LayerType myType;		// 当前层的类型
	Director* director;		// 导演
	Size visibleSize;		// 可视区域大小
	Vec2 origin;			// 当前坐标起点
	Sprite* bg_1;			// 背景一
	Sprite* bg_2;			// 背景二
	float bgSpeed;

	Sprite* hero;			// 主机
	bool isTouch;			// 是否选中主机
	float heroOldPosX;		// 飞机上一帧的坐标
	std::list<Sprite*> herobulletlist_lives;	// 主机子弹生存池
	std::list<Sprite*> herobulletlist_death;	// 主机子弹死亡池

	/* 奖励相关 */
	std::list<Sprite*> proplist;

	std::list<Sprite*> enemylist_lives;		// 敌机生存池
	std::list<Sprite*> enemylist_death;		// 敌机死亡池
	EnemyType enemyType;
	float enemySpeed;

	/* 分数统计 */
	static int maxScore;			// 最高得分
	static int Score;				// 当前得分
	Sprite* scoreBg;				// 游戏界面得分背景
	Label* scoreLbinGame;			// 游戏界面显示得分
	bool isnew;
	Label* ScoreNumLable;

	/* 设置界面 */
	void myButtonCall_2(Ref* pSender, Widget::TouchEventType type);
	Button* button;
	Sprite* setbg;
	Sprite* bgmset;
	Sprite* bgmtype;
	void setMusic(cocos2d::Ref* p);
	bool isBgm;
	MenuItemImage* myItem;
	Menu* myMenu;
	MenuItemImage* closeSetImage;
	Menu* closeSetMenu;
	void setset(cocos2d::Ref* p);

public:    
    // implement the "static create()" method manually
	
};

#endif // __HELLOWORLD_SCENE_H__
