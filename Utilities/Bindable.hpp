#pragma once

struct IBindable {
    virtual int binds(void* item) = 0;
};
