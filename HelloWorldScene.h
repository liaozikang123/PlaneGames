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
		unsigned int loop);	// ����һ��֡����
	void menuCloseCallback();

	void touchOneByOne();
    // a selector callback
     //�л�������Ӧ����
	void update(float) override;	//Ĭ�ϵ������ص�
	void heroShoot(float date);		//��������ӵ�
	void heroShoot1(float date);		//��������ӵ�
	void createProp(float date);	//����������Ʒ
	void createEnemy(float date);
	void createEnemyS1(float date);
public:

private:
	LayerType myType;		// ��ǰ�������
	Director* director;		// ����
	Size visibleSize;		// ���������С
	Vec2 origin;			// ��ǰ�������
	Sprite* bg_1;			// ����һ
	Sprite* bg_2;			// ������
	float bgSpeed;

	Sprite* hero;			// ����
	bool isTouch;			// �Ƿ�ѡ������
	float heroOldPosX;		// �ɻ���һ֡������
	std::list<Sprite*> herobulletlist_lives;	// �����ӵ������
	std::list<Sprite*> herobulletlist_death;	// �����ӵ�������

	/* ������� */
	std::list<Sprite*> proplist;

	std::list<Sprite*> enemylist_lives;		// �л������
	std::list<Sprite*> enemylist_death;		// �л�������
	EnemyType enemyType;
	float enemySpeed;

	/* ����ͳ�� */
	static int maxScore;			// ��ߵ÷�
	static int Score;				// ��ǰ�÷�
	Sprite* scoreBg;				// ��Ϸ����÷ֱ���
	Label* scoreLbinGame;			// ��Ϸ������ʾ�÷�
	bool isnew;
	Label* ScoreNumLable;

	/* ���ý��� */
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
