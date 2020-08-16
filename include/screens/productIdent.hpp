#ifndef _ROCKET_PHOTO_SHOOT_PRODUCTIDENT_HPP
#define _ROCKET_PHOTO_SHOOT_PRODUCTIDENT_HPP

#include "common.hpp"

class ProductIdent : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
#ifdef NDS
private:
	bool graphicLoaded = false;
#endif
};

#endif