// Sunset inc.

#pragma once
#define NO_COPY_BUT_MOVE(name) \
    name(const name&) = delete; \
    name& operator=(const name&) = delete; \
    name(name&&) noexcept; \
    name& operator=(name&&) noexcept;

