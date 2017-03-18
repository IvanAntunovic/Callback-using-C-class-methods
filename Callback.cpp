#include <Arduino_FreeRTOS.h> 
#include <semphr.h>
#include <avr/io.h>
#include <portable.h>

/**
**	######################################################################
**    Calling a callback function from a Class itself using this pointer
**	######################################################################
*/
class MyClass
{
  public:
    static void vTask(void* pvParameters);
    MyClass()
    {
	  // Calling a callback function from a class itself
      xTaskCreate(this->vTask, "Task 2", 100, NULL, 1, NULL);
    }
};

void MyClass::vTask(void* pvParameters)
{
  Serial.println("Task running from MyClass method");
  _delay_ms(500);
}

/**
** #######################################################################
**		Calling a callback from a main function using static function
** 		that is used to to call a non-static function 
** #######################################################################
*/
class Caller
{
  public:
    inline void _update(void* value);
    static void callbackObject(void* object, void* value);
};

inline void Caller::_update(void* value)
{
  Serial.println("Calling method from Caller class");
  _delay_ms(500);
}

static void Caller::callbackObject(void* object, void* value)
{
  Caller* This = (Caller*) object;
  This->_update(value);
}

void vTask1(void* pvParameters)
{
  const char* pcTaskName = "Task 1 is running";

  Serial.println(pcTaskName);
  _delay_ms(500);
}

void vTask2(void* pvParameters)
{
  const char* pcTaskName = "Task 2 is running";

  Serial.println(pcTaskName);
  _delay_ms(500);
}

int main(void)
{
	Caller* caller = new Caller();
	Serial.begin(9600);
	xTaskCreate(vTask1, "Task 1", 100, NULL, 1, NULL);
	xTaskCreate(vTask2, "Task 2", 100, NULL, 1, NULL);
	xTaskCreate(Caller::callbackObject, "Task 2", 100, NULL, 1, NULL);
	MyClass* myClass = new MyClass();
	vTaskStartScheduler();
	
	while(true)
	{
		
	}
}
