
#include "Armadillo.h"
#include <iostream>
#include <stdlib.h>


int main()
{
	ViewRender* viewport_main = new ViewRender();
	if (viewport_main->Init(640, 480))
	{
		viewport_main->PrepResources();
		viewport_main->Render();
	}

	return 0;
}