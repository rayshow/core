#include"engine/application.h"
#include<xnamath.h>
class SampleApplication :public Aurora3D::Application
{
public:
	virtual void Initialize() override{}
	virtual void Config() override {}
	virtual void Update() override {}
	virtual void Exit() override{}
	virtual ~SampleApplication() override {}
};


//AURORA3D_MAIN_APPLICATION(SampleApplication)

int main(int argc, char** argv) { 

	OpenConsoleAndDetechMemoryLeak();
	//SetBreakPointAtMemoryLeak(185);
	Aurora3D::Application *app = new SampleApplication{};
	Aurora3D::Engine engine(app); 
	engine.ProcessCmd(argc, argv);
	engine.Run(); 
	return engine.Exit();
} 


