#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET
#define TEXTE_1 "TextKey"
#define I2CPORT_16 16
#include <Blinker.h>
#include <M5Stack.h>
#include <malloc.h>

char auth[] = "eab3bfe3da5b";
char ssid[] = "orionstar-ZK-test";
char pswd[] = "ZK12345678";

static int count = 0;
#define BUTTON_1 "ButtonKey"

BlinkerButton Button1(BUTTON_1);
bool oState = false;
// 链式队列的实现(不带头结点)

typedef String ElemType;
typedef struct LinkNode{
	ElemType data;
	struct LinkNode* next;
}LinkNode;

typedef struct{
	LinkNode* front;
	LinkNode* rear;
	int length;
}LinkQueue;

LinkQueue link_t;
// 初始化队列
void InitQueue(LinkQueue& q)
{
	//初始时 front，rear都指向NULL
	q.front = NULL;
	q.rear = NULL;
}

// 判断队列是否为空
bool IsEmpty(LinkQueue& q)
{
	if (q.front == NULL)
		return true;
	else
		return false;
}

// 入队
void EnQueue(LinkQueue& q, ElemType x)
{
	LinkNode* s = (LinkNode*)malloc(sizeof(LinkNode));
	s->data = x;
	s->next = NULL;

	if (q.front == NULL)	//在空队列中插入第一个元素
	{
		q.front = s;
		q.rear = s;
	}
	else
	{ 
		q.rear->next = s;	//	新节点插入到rear节点之后
		q.rear = s;		    // 修改rear指针
	}
	q.length++;
}

// 出队
bool DeQueue(LinkQueue& q, ElemType& x)
{
	if (q.front == NULL)
		return false;	//队空则报错

	LinkNode* p = q.front;    //p指向此次出队的节点
	x = p->data;			  //用变量x返回队头指针

	q.front = p->next;		  //修改front指针
	if (q.rear == p)		  //此次是最后一个节点出队	
	{
		q.front = NULL;		  //front指向NULL
		q.rear = NULL;		  //rear指向NULL
	}
	free(p);

	q.length--;
	return true;
}

// 求队列长度
int LengthQueue(LinkQueue& q)
{
	int count = 0;
	LinkNode* p = q.front;
	while (p != NULL)
	{
		++count;
		p = p->next;
	}
	return count;
}


void roll_display()
{
    ElemType x1,x2,x3,x4,x5;
    M5.Lcd.setCursor(0,0);
    DeQueue(link_t,x1);
    M5.Lcd.println(x1);
    M5.Lcd.setCursor(0,4);
    DeQueue(link_t,x2);
    M5.Lcd.println(x2);
    M5.Lcd.setCursor(0,8);
    DeQueue(link_t,x3);
    M5.Lcd.println(x3);
    M5.Lcd.setCursor(0,12);
    DeQueue(link_t,x4);
    M5.Lcd.println(x4);
    M5.Lcd.setCursor(0,16);
    DeQueue(link_t,x5);
    M5.Lcd.println(x5);
}
void button1_callback(const String & state)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("get button state: ", state);

    if (state == BLINKER_CMD_BUTTON_TAP) {
        BLINKER_LOG("Button tap!");

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        Button1.print();
        M5.Lcd.println(">>>>>>>>>>>>>>>>");
    }
    else if (state == BLINKER_CMD_BUTTON_PRESSED) {
        BLINKER_LOG("Button pressed!");

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        Button1.print();
        M5.Lcd.println("11111111111");
    }
    else if (state == BLINKER_CMD_BUTTON_RELEASED) {
        BLINKER_LOG("Button released!");

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        // Button1.text("Your button name", "describe");
        Button1.print();
        M5.Lcd.println("222222222222");
    }
    else if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        // Button1.text("Your button name", "describe");
        Button1.print("on");
        M5.Lcd.println("3333333333333333");
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        // Button1.text("Your button name", "describe");
        Button1.print("off");
        M5.Lcd.println("44444444444");
    }
    else {
        BLINKER_LOG("Get user setting: ", state);

        Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        // Button1.text("Your button name", "describe");
        Button1.print();
    }
}

void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(I2CPORT_16, HIGH);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(I2CPORT_16, LOW);
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

        oState = false;
    }
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}

void dataRead(const String & data)
{
    count++;
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
    if(count%2)
    {
         digitalWrite(I2CPORT_16, LOW);
         //M5.Lcd.println("OFF");
         EnQueue(link_t,"off");
    }
    else
    {
         digitalWrite(I2CPORT_16, HIGH);
        // M5.Lcd.println("ON");
         EnQueue(link_t,"on");
    }
}

void setup()
{
    M5.begin();
    M5.Power.begin();
    Serial.begin(115200);
    pinMode(I2CPORT_16, OUTPUT);  //Set pin 26 to output mode.  设置26号引脚为输出模式
    digitalWrite(I2CPORT_16, LOW);
    BLINKER_DEBUG.stream(Serial);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Button1.attach(button1_callback);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    InitQueue(link_t);
}

void loop()
{
    roll_display();
    Blinker.run();
}