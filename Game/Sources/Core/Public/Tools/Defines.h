// Sunset inc.

#pragma once
#define NO_COPY_BUT_MOVE(name) \
    name(const name&) = delete; \
    name& operator=(const name&) = delete; \
    name(name&&) noexcept; \
    name& operator=(name&&) noexcept;

#ifdef _DEBUG
#define ASSERT(con, ...) !!(!!con) || []()->bool{ ENGINE_LOG_ERROR(__VA_ARGS__); __debugbreak(); return false; }()
#else
#define ASSERT(con, ...)
#endif // DEBUG

#define MOUSE_SPEED 10.f

