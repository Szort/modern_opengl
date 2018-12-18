
#include "Armadillo.h"
#include <iostream>


int main()
{
	ViewRender* viewport_main = new ViewRender();
	viewport_main->Init(640, 480);
	viewport_main->Render();

	return 0;
}