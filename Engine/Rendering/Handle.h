#ifndef EN_HANDLE_H
#define EN_HANDLE_H

#include <iostream>

#define EN_HANDLE_INVALID_ID 0

class CHandle {
protected:
	uint32_t MId;
public:
    inline uint32_t GetId() const
    {
        return MId;
    }

    inline void SetId(uint32_t Id)
    {
        MId = Id;
    }
};

#endif

