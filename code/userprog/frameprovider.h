// frameprovider.h
//

#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "machine.h"
#include "bitmap.h"

class FrameProvider
{
  public:
	FrameProvider();
	~FrameProvider();

	int GetEmptyFrame();	//renvoie adresse une frame libre
				// le mapping se passe ici

	bool ReleaseFrame(int virtualAddr); //T si on peut libérer le frame
				// F si ce n'est pas possible

	int NumAvailFrame(); //return nb de frame libre

private:
	BitMap* bm;

};

#endif //FRAMEPROVIDER_H