#ifndef _SAVVY_MANAGER_ROCKETROBZ_HPP
#define _SAVVY_MANAGER_ROCKETROBZ_HPP

#include "common.hpp"

class RocketRobz : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	#ifdef NDS
	bool graphicLoaded = false;
	#endif
	const char* presentedText = "Presented in";
};

#endif